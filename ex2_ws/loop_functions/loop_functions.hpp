#ifndef HW2_LOOP_FUNCTIONS_H
#define HW2_LOOP_FUNCTIONS_H

#include <argos3/core/simulator/loop_functions.h>
#include <argos3/core/utility/datatypes/color.h>
#include <argos3/core/utility/math/vector3.h>
#include <vector>

namespace argos {

   class CPiPuckEntity;

   /*
    * The Exercise 2 referee. DO NOT MODIFY.
    *
    * It watches the robot and decides whether it:
    *   1. reached the target (the red light),
    *   2. then returned to where it started,
    *   3. without touching a wall or an obstacle.
    *
    * The experiment ends when the robot is back home, or when the time in
    * the .argos file runs out. The verdict is printed at the end.
    *
    * It also remembers the robot's path, so that the window can draw it
    * (see qt_user_functions.cpp).
    */
   class CHW2LoopFunctions : public CLoopFunctions {

   public:

      /* One point of the robot's path, with the LED color it had there */
      struct STrailPoint {
         CVector3 Position;
         CColor Color;
      };

      void Init(TConfigurationNode& t_tree) override;
      void Reset() override;
      void PostStep() override;
      bool IsExperimentFinished() override;
      void PostExperiment() override;

      const std::vector<STrailPoint>& GetTrail() const { return m_vecTrail; }
      const CVector3& GetStart() const { return m_cStart; }
      const CVector3& GetTarget() const { return m_cTarget; }

   private:

      /* Length of the shortest path from the target back to the start, moving
         between the centers of free grid cells. In meters, -1 if there is none. */
      Real ShortestGridPath() const;

      CPiPuckEntity* m_pcRobot = nullptr;
      CVector3 m_cStart;
      CVector3 m_cTarget;

      bool m_bReachedTarget = false;
      bool m_bReturnedHome = false;
      Real m_fTimeAtTarget = 0.0;
      Real m_fTimeAtHome = 0.0;
      UInt32 m_unCollisionTicks = 0;
      Real m_fReturnDistance = 0.0;
      CVector3 m_cLastPosition;

      std::vector<STrailPoint> m_vecTrail;
   };
}

#endif
