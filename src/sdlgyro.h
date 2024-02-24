#ifndef SDLGYRO_H
#define SDLGYRO_H

#include "godot_cpp/variant/typed_array.hpp"
#include <godot_cpp/classes/object.hpp>
#include <godot_cpp/variant/variant.hpp>
using namespace godot;

  class SDLGyro : public Object{
    GDCLASS(SDLGyro,Object);
    public:
      void sdl_init();
      void controller_init();
      Variant gamepadPoling();
    protected:
      static void _bind_methods();
  };

#endif
