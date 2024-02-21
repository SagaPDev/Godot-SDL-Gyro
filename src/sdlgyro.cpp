#include "sdlgyro.h"
#include <godot_cpp/core/class_db.hpp>
#include <SDL2/SDL.h>
#include <godot_cpp/variant/utility_functions.hpp>

SDL_Event event;  
SDL_GameController *controller =nullptr;

using namespace godot;

void SDLGyro::_bind_methods() {
}

SDLGyro::SDLGyro() {
	// Initialize any variables here.

}
SDLGyro::~SDLGyro() {
	// Add your cleanup here.
}

void SDLGyro::_process(double delta) {

  /*SDL initializATION*/
  SDL_SetHint(SDL_HINT_JOYSTICK_ALLOW_BACKGROUND_EVENTS, "1");
  if((SDL_Init(SDL_INIT_GAMECONTROLLER))<0){
    UtilityFunctions::print("could not initialize SDL \n");
  }
  else{
    UtilityFunctions::print("SDL initialized!!!!!!!!!!! \n");
  }

}
