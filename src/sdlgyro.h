#ifndef SDLGYRO_H
#define SDLGYRO_H

#include <godot_cpp/classes/object.hpp>
#include <godot_cpp/variant/variant.hpp>
using namespace godot;

  class SDLGyro : public Object{
    GDCLASS(SDLGyro,Object);
    public:
      void sdl_init();
      void controller_init();

      Variant gamepadPolling();

      // Modificacion Multiples controles
      Array get_available_gyro_controllers();
      String get_gamepad_name(int controller_index);
      Variant poll_motion_data(int controller_index);
      godot::Dictionary poll_both_joycon_gyros(int controller_index);
      // Funciones para Joy-Con
      void init_joycons();
      Dictionary joycon_polling();
      void joycon_calibrate();
      void joycon_stop_calibrate();
      Variant get_joycon_l_orientation();
      Variant get_joycon_r_orientation();


      void calibrate();
      void stop_calibrate();

      Variant getPlayer_space();//not working
      Variant getWorld_space();//not working

      Variant getGravity();//not working
  
      Variant getCalibratedGyro();

      Variant getProcessedAcceleration();

      void setAutoCalibration();
      bool isCalibrationSteady();
      float getCalibrationConfidence();
    protected:
      static void _bind_methods();
  };

#endif
