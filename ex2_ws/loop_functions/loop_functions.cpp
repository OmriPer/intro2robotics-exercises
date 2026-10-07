#include "loop_functions.hpp"
#include <argos3/core/simulator/simulator.h>
#include <argos3/core/simulator/entity/embodied_entity.h>
#include <argos3/core/simulator/physics_engine/physics_engine.h>
#include <argos3/core/utility/logging/argos_log.h>
#include <argos3/plugins/robots/pi-puck/simulator/pipuck_entity.h>
#include <argos3/plugins/simulator/entities/box_entity.h>
#include <argos3/plugins/simulator/entities/led_equipped_entity.h>
#include <argos3/plugins/simulator/entities/light_entity.h>
#include <cmath>
#include <queue>

namespace argos {

   /* The robot reached the target when it is this close to the light, in meters */
   static const Real TARGET_RADIUS = 0.12;
   /* The robot is back home when it is this close to its start, in meters */
   static const Real HOME_RADIUS = 0.05;
   /* The grid: 8 x 8 cells of 0.5 m, covering the arena from (-2,-2) to (2,2) */
   static const SInt32 GRID_SIZE = 8;
   static const Real CELL_SIZE = 0.5;
   static const Real ARENA_MIN = -2.0;

   /****************************************/
   /****************************************/

   static Real PlanarDistance(const CVector3& c_a, const CVector3& c_b) {
      return std::hypot(c_a.GetX() - c_b.GetX(), c_a.GetY() - c_b.GetY());
   }

   static SInt32 ToCellIndex(Real f_coordinate) {
      return SInt32(std::floor((f_coordinate - ARENA_MIN) / CELL_SIZE));
   }

   /****************************************/
   /****************************************/

   void CHW2LoopFunctions::Init(TConfigurationNode& t_tree) {
      /* The robot: the first (and only) Pi-puck in the arena */
      CSpace::TMapPerType& tRobots = GetSpace().GetEntitiesByType("pipuck");
      m_pcRobot = any_cast<CPiPuckEntity*>(tRobots.begin()->second);
      m_cStart = m_pcRobot->GetEmbodiedEntity().GetOriginAnchor().Position;
      m_cLastPosition = m_cStart;
      /* The target: the first (and only) light in the arena */
      CSpace::TMapPerType& tLights = GetSpace().GetEntitiesByType("light");
      m_cTarget = any_cast<CLightEntity*>(tLights.begin()->second)->GetPosition();
   }

   /****************************************/
   /****************************************/

   void CHW2LoopFunctions::Reset() {
      m_bReachedTarget = false;
      m_bReturnedHome = false;
      m_fTimeAtTarget = 0.0;
      m_fTimeAtHome = 0.0;
      m_unCollisionTicks = 0;
      m_fReturnDistance = 0.0;
      m_cLastPosition = m_cStart;
      m_vecTrail.clear();
   }

   /****************************************/
   /****************************************/

   void CHW2LoopFunctions::PostStep() {
      if (m_bReturnedHome) return;
      const CVector3& cPosition = m_pcRobot->GetEmbodiedEntity().GetOriginAnchor().Position;
      Real fTime = GetSpace().GetSimulationClock() * CPhysicsEngine::GetSimulationClockTick();

      /* 3. Collisions */
      if (m_pcRobot->GetEmbodiedEntity().IsCollidingWithSomething()) {
         ++m_unCollisionTicks;
      }

      /* 1. Target, 2. home */
      if (!m_bReachedTarget) {
         if (PlanarDistance(cPosition, m_cTarget) <= TARGET_RADIUS) {
            m_bReachedTarget = true;
            m_fTimeAtTarget = fTime;
            LOG << "[HW2] Target reached after " << fTime << " s" << std::endl;
         }
      } else {
         m_fReturnDistance += PlanarDistance(cPosition, m_cLastPosition);
         if (PlanarDistance(cPosition, m_cStart) <= HOME_RADIUS) {
            m_bReturnedHome = true;
            m_fTimeAtHome = fTime;
            LOG << "[HW2] Back home after " << fTime << " s" << std::endl;
         }
      }
      m_cLastPosition = cPosition;

      /* Remember the path, for drawing */
      if (m_vecTrail.empty() || PlanarDistance(cPosition, m_vecTrail.back().Position) > 0.01) {
         CVector3 cPoint(cPosition.GetX(), cPosition.GetY(), 0.01);
         m_vecTrail.push_back({ cPoint, m_pcRobot->GetLEDEquippedEntity().GetLED(0).GetColor() });
      }
   }

   /****************************************/
   /****************************************/

   bool CHW2LoopFunctions::IsExperimentFinished() {
      return m_bReturnedHome;
   }

   /****************************************/
   /****************************************/

   Real CHW2LoopFunctions::ShortestGridPath() const {
      /* Mark the cells covered by boxes. The four arena walls lie outside the grid. */
      bool bBlocked[GRID_SIZE][GRID_SIZE] = {};
      CSpace::TMapPerType& tBoxes = CSimulator::GetInstance().GetSpace().GetEntitiesByType("box");
      for (auto& tPair : tBoxes) {
         CBoxEntity* pcBox = any_cast<CBoxEntity*>(tPair.second);
         const CVector3& cCenter = pcBox->GetEmbodiedEntity().GetOriginAnchor().Position;
         const CVector3& cSize = pcBox->GetSize();
         for (SInt32 nCol = 0; nCol < GRID_SIZE; ++nCol) {
            for (SInt32 nRow = 0; nRow < GRID_SIZE; ++nRow) {
               Real fX = ARENA_MIN + (nCol + 0.5) * CELL_SIZE;
               Real fY = ARENA_MIN + (nRow + 0.5) * CELL_SIZE;
               if (Abs(fX - cCenter.GetX()) < cSize.GetX() / 2 && Abs(fY - cCenter.GetY()) < cSize.GetY() / 2) {
                  bBlocked[nCol][nRow] = true;
               }
            }
         }
      }
      /* Breadth-first search from the target's cell to the start's cell */
      SInt32 nSteps[GRID_SIZE][GRID_SIZE];
      for (auto& tColumn : nSteps) for (SInt32& n : tColumn) n = -1;
      std::queue<std::pair<SInt32, SInt32>> cQueue;
      SInt32 nTargetCol = ToCellIndex(m_cTarget.GetX()), nTargetRow = ToCellIndex(m_cTarget.GetY());
      SInt32 nStartCol = ToCellIndex(m_cStart.GetX()), nStartRow = ToCellIndex(m_cStart.GetY());
      nSteps[nTargetCol][nTargetRow] = 0;
      cQueue.push({ nTargetCol, nTargetRow });
      const SInt32 nDCol[4] = { 1, -1, 0, 0 }, nDRow[4] = { 0, 0, 1, -1 };
      while (!cQueue.empty()) {
         auto [nCol, nRow] = cQueue.front();
         cQueue.pop();
         for (SInt32 i = 0; i < 4; ++i) {
            SInt32 nC = nCol + nDCol[i], nR = nRow + nDRow[i];
            if (nC < 0 || nC >= GRID_SIZE || nR < 0 || nR >= GRID_SIZE) continue;
            if (bBlocked[nC][nR] || nSteps[nC][nR] != -1) continue;
            nSteps[nC][nR] = nSteps[nCol][nRow] + 1;
            cQueue.push({ nC, nR });
         }
      }
      if (nSteps[nStartCol][nStartRow] == -1) return -1.0;
      return nSteps[nStartCol][nStartRow] * CELL_SIZE;
   }

   /****************************************/
   /****************************************/

   void CHW2LoopFunctions::PostExperiment() {
      bool bPass = m_bReachedTarget && m_bReturnedHome && m_unCollisionTicks == 0;
      LOG << "[HW2] ---------------- result ----------------" << std::endl;
      LOG << "[HW2] 1. reached the target:  " << (m_bReachedTarget ? "YES" : "NO");
      if (m_bReachedTarget) LOG << "  (after " << m_fTimeAtTarget << " s)";
      LOG << std::endl;
      LOG << "[HW2] 2. returned home:       " << (m_bReturnedHome ? "YES" : "NO");
      if (m_bReturnedHome) LOG << "  (after " << m_fTimeAtHome << " s)";
      LOG << std::endl;
      LOG << "[HW2] 3. no collisions:       " << (m_unCollisionTicks == 0 ? "YES" : "NO");
      if (m_unCollisionTicks > 0) LOG << "  (touched something during " << m_unCollisionTicks << " steps)";
      LOG << std::endl;
      if (m_bReturnedHome) {
         LOG << "[HW2] way home: drove " << m_fReturnDistance << " m, the shortest grid path is "
             << ShortestGridPath() << " m" << std::endl;
      }
      LOG << "[HW2] " << (bPass ? "PASS" : "FAIL") << std::endl;
      LOG.Flush();
   }

   /****************************************/
   /****************************************/

   REGISTER_LOOP_FUNCTIONS(CHW2LoopFunctions, "hw2_loop_functions");

}
