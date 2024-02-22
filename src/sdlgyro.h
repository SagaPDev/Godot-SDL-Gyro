#ifndef SDLGYRO_H
#define SDLGYRO_H

#include <godot_cpp/classes/node3d.hpp>

using namespace godot;

  class SDLGyro : public Node3D {
    GDCLASS(SDLGyro, Node3D);
    public:
      void sdl_init();
    protected:
      static void _bind_methods();
  };

#endif
