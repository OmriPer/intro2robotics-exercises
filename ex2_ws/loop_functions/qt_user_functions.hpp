#ifndef HW2_QT_USER_FUNCTIONS_H
#define HW2_QT_USER_FUNCTIONS_H

#include <argos3/plugins/simulator/visualizations/qt-opengl/qtopengl_user_functions.h>

namespace argos {

   class CHW2LoopFunctions;

   /*
    * Draws, in the ARGoS window:
    *   - the path the robot drove, in the color its LEDs had at each point,
    *   - a black ring where the robot started,
    *   - a red ring around the target: inside it the target counts as reached.
    *
    * DO NOT MODIFY.
    */
   class CHW2QTUserFunctions : public CQTOpenGLUserFunctions {

   public:

      CHW2QTUserFunctions();

      void DrawInWorld() override;

   private:

      CHW2LoopFunctions& m_cLoopFunctions;
   };
}

#endif
