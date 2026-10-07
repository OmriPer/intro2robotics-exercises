#include "qt_user_functions.hpp"
#include "loop_functions.hpp"
#include <argos3/core/simulator/simulator.h>
#include <argos3/core/utility/math/ray3.h>

namespace argos {

   CHW2QTUserFunctions::CHW2QTUserFunctions() :
      m_cLoopFunctions(dynamic_cast<CHW2LoopFunctions&>(
         CSimulator::GetInstance().GetLoopFunctions())) {}

   /****************************************/
   /****************************************/

   void CHW2QTUserFunctions::DrawInWorld() {
      /* Start and target */
      CVector3 cStart = m_cLoopFunctions.GetStart();
      CVector3 cTarget = m_cLoopFunctions.GetTarget();
      cStart.SetZ(0.005);
      cTarget.SetZ(0.005);
      DrawCircle(cStart, CQuaternion(), 0.05, CColor::BLACK, false);
      DrawCircle(cTarget, CQuaternion(), 0.12, CColor::RED, false);
      /* The path, as short line segments */
      const auto& vecTrail = m_cLoopFunctions.GetTrail();
      for (size_t i = 1; i < vecTrail.size(); ++i) {
         DrawRay(CRay3(vecTrail[i - 1].Position, vecTrail[i].Position), vecTrail[i].Color, 4.0);
      }
   }

   /****************************************/
   /****************************************/

   REGISTER_QTOPENGL_USER_FUNCTIONS(CHW2QTUserFunctions, "hw2_qt_user_functions")

}
