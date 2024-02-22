#ifndef SDLGYRO_H
#define SDLGYRO_H

#include <godot_cpp/classes/object.hpp>

using namespace godot;

  class SDLGyro : public Object{
    GDCLASS(SDLGyro,Object);
    public:
      void sdl_init();
      void controller_init();
    protected:
      static void _bind_methods();
  };

#endif
