#ifndef SDLGYRO_H
#define SDLGYRO_H
#include <godot_cpp/classes/object.hpp>
namespace godot {

  class SDLGyro : public Object {
    GDCLASS(SDLGyro, Object)

    protected:
      static void _bind_methods();

    public:
      SDLGyro();
      ~SDLGyro();
    void _process(double delta);
  };
}
#endif
