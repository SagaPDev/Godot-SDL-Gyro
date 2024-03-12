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

      void calibrate();
      void stop_calibrate();

      Variant getPlayer_space();//not working
      Variant getWorld_space();//not working

      Variant getGravity();//not working
  
      Variant getCalibratedGyro();

      Variant getProcessedAcceleration();

    protected:
      static void _bind_methods();
  };

#endif
