#include "sdlgyro.h"
#include <godot_cpp/core/class_db.hpp>
#include <SDL2/SDL.h>
#include <godot_cpp/variant/utility_functions.hpp>
#include "GamepadMotion.hpp"

using namespace godot;

SDL_Event event;  
SDL_GameController *controller =nullptr;

bool isRunning=true;
bool gyroEnabled=false;
bool accelEnabled=false;
float deltaTime=0.0;
Vector3 gyro;
Vector3 accel;
Vector4 orientation;
static constexpr float toDegPerSec = float(180. / M_PI);
static constexpr float toGs = 1.f / 9.8f;

GamepadMotion gyroSensor;

void SDLGyro::_bind_methods() {
  ClassDB::bind_method(D_METHOD("sdl_init"),&SDLGyro::sdl_init);
  ClassDB::bind_method(D_METHOD("controller_init"),&SDLGyro::controller_init);
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

void SDLGyro::controller_init(){
  /*controller initialization*/
  for (int i=0;i<SDL_NumJoysticks();i++){
    if(SDL_IsGameController(i)){
      UtilityFunctions::print(SDL_IsGameController(i),"\n");
      controller = SDL_GameControllerOpen(i);
      UtilityFunctions::print(SDL_GameControllerNameForIndex(i),"\n");
      }
    }
  /*test gyro*/
  if (SDL_GameControllerHasSensor(controller,SDL_SENSOR_GYRO)){
    UtilityFunctions::print("Gyro Detected\n");
    SDL_GameControllerSetSensorEnabled(controller,SDL_SENSOR_GYRO,SDL_TRUE);
  }
  else UtilityFunctions::print("Gyro not Detected\n");

  if(SDL_GameControllerIsSensorEnabled(controller, SDL_SENSOR_GYRO)){
    UtilityFunctions::print("gyro enabled\n");
    gyroEnabled=true;
  }
  else UtilityFunctions::print("gyro disabled\n");

  /*test accelerometer*/
  if (SDL_GameControllerHasSensor(controller,SDL_SENSOR_ACCEL)){
    UtilityFunctions::print("accelerometer Detected\n");
    SDL_GameControllerSetSensorEnabled(controller,SDL_SENSOR_ACCEL,SDL_TRUE);
  }
  else UtilityFunctions::print("accelerometer not Detected\n");

      if(SDL_GameControllerIsSensorEnabled(controller, SDL_SENSOR_ACCEL)){
        UtilityFunctions::print("accelerometer enabled\n");
        accelEnabled=true;
      }
      else UtilityFunctions::print("accelerometer disabled\n");
}
