#include "controller1.hpp"

namespace argos {

   /****************************************/
   /****************************************/

   void Controller1::Init(TConfigurationNode& t_tree) {
      /* Get the actuators and sensors */
      m_pcWheels = GetActuator<CCI_PiPuckDifferentialDriveActuator>("pipuck_differential_drive");
      m_pcColoredLEDs = GetActuator<CCI_PiPuckColorLEDsActuator>("pipuck_leds");
      m_pcSystem = GetSensor<CCI_PiPuckSystemSensor>("pipuck_system");
      m_pcCamera = GetSensor<CCI_ColoredBlobOmnidirectionalCameraSensor>("colored_blob_omnidirectional_camera");
      m_pcRangefinders = GetSensor<CCI_PiPuckRangefindersSensor>("pipuck_rangefinders");

      /* Your Init code here */
   }

   void Controller1::ControlStep() {
      /* Your ControlStep code here */
   }

   /****************************************/
   /****************************************/

   REGISTER_CONTROLLER(Controller1, "controller1");

}

