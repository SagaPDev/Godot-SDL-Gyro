#include "sdlgyro.h"
#include <godot_cpp/core/class_db.hpp>
#include <SDL2/SDL.h>
#include <godot_cpp/variant/utility_functions.hpp>

SDL_Event event;  
SDL_GameController *controller =nullptr;

using namespace godot;

void SDLGyro::_bind_methods() {
  ClassDB::bind_method(D_METHOD("sdl_init"),&SDLGyro::sdl_init);
}

void SDLGyro::sdl_init() {

  /*SDL initializATION*/
  SDL_SetHint(SDL_HINT_JOYSTICK_ALLOW_BACKGROUND_EVENTS, "1");
  if((SDL_Init(SDL_INIT_GAMECONTROLLER))<0){
    UtilityFunctions::print("could not initialize SDL \n");
  }
  else{
    UtilityFunctions::print("SDL initialized!!!!!!!!!!! \n");
  }

}
