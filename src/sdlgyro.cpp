#include "sdlgyro.h"
#include <array>
#include <godot_cpp/core/class_db.hpp>
#include <SDL2/SDL.h>
#include <godot_cpp/variant/utility_functions.hpp>
#include "GamepadMotion.hpp"
#include "godot_cpp/variant/array.hpp"
#include "godot_cpp/variant/typed_array.hpp"
#include "godot_cpp/variant/variant.hpp"
#include <godot_cpp/godot.hpp>
#include <chrono>

using namespace godot;

SDL_Event event;  
SDL_GameController *controller =nullptr;

bool isRunning=true;
bool gyroEnabled=false;
bool accelEnabled=false;
float deltaTime=0.0;

std::array<float, 3> rawGyro;
std::array<float, 3> rawAccel;
std::array<float, 4> rawOrientation;

static constexpr float toDegPerSec = float(180. / M_PI);
static constexpr float toGs = 1.f / 9.8f;

std::chrono::steady_clock::time_point oldTime;
std::chrono::steady_clock::time_point newTime;

GamepadMotion gyroSensor;

void SDLGyro::_bind_methods() {
  ClassDB::bind_method(D_METHOD("sdl_init"),&SDLGyro::sdl_init);
  ClassDB::bind_method(D_METHOD("controller_init"),&SDLGyro::controller_init);
  ClassDB::bind_method(D_METHOD("gamepadPoling"),&SDLGyro::gamepadPoling);
}

void SDLGyro::sdl_init() {

  newTime=std::chrono::steady_clock::now();
  oldTime=newTime;

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


Variant SDLGyro::gamepadPoling(){
  TypedArray<float> gyro(3);
  TypedArray<float> accel(3);
  TypedArray<float> orientation(4);
  /*IMU gyro*/
  if (gyroEnabled==true){
    SDL_GameControllerGetSensorData(controller,SDL_SENSOR_GYRO, &rawGyro[0], 3);
    UtilityFunctions::print("gyro  X= ",rawGyro[0]*toDegPerSec," Y= ",rawGyro[1]*toDegPerSec," Z= ",rawGyro[2]*toDegPerSec,"\n");
    /*for (int i = 0; i < 2; i++) {
      gyro[i] = rawGyro[i]*toDegPerSec;
    }*/
  }
  /*IMU accelerometer*/
  if (accelEnabled==true){
    SDL_GameControllerGetSensorData(controller,SDL_SENSOR_ACCEL, &rawAccel[0], 3);
    UtilityFunctions::print("accel X= ",rawAccel[0]*toGs," Y= ",rawAccel[1]*toGs," Z= ",rawAccel[2]*toGs,"\n");
    /*for (int i = 0; i < 2; i++) {
      accel[i] = rawAccel[i]*toGs;
    }*/
  }

  /*Sensor Fussion*/
  if (gyroEnabled && accelEnabled){
    if (oldTime!=newTime)
      newTime=std::chrono::steady_clock::now();
    deltaTime=((float)std::chrono::duration_cast<std::chrono::microseconds>(newTime-oldTime).count()) / 1000000.0f;

    gyroSensor.ProcessMotion(rawGyro[0]*toDegPerSec, rawGyro[1]*toDegPerSec, rawGyro[2]*toDegPerSec, rawAccel[0]*toGs, rawAccel[1]*toGs, rawAccel[2]*toGs,deltaTime);
    oldTime=std::chrono::steady_clock::now();

    gyroSensor.GetOrientation(rawOrientation[0], rawOrientation[1], rawOrientation[2], rawOrientation[3]);
    UtilityFunctions::print("orien X= ",rawOrientation[1]," Y= ",rawOrientation[2]," Z= ",rawOrientation[3]," W= ",rawOrientation[0],"\n");
    UtilityFunctions::print(deltaTime,"\n");

    orientation.push_back(rawOrientation[0]); /*w*/
    orientation.push_back(rawOrientation[1]);/*x*/
    orientation.push_back(rawOrientation[2]);/*y*/
    orientation.push_back(rawOrientation[3]);/*z*/
/*
    orientation.push_back(0.0);
    orientation.push_back(0.0);
    orientation.push_back(0.0);
    orientation.push_back(0.0);
*/
    /*for (int i = 0; i < 3; i++) {
      orientation[i] = rawOrientation[i];
    }*/
  }

  /*event loop*/
  while(SDL_PollEvent(&event)){
    switch (event.type) {
      case SDL_CONTROLLERBUTTONDOWN:
        UtilityFunctions::print(SDL_GameControllerGetStringForButton(SDL_GameControllerButton(event.cbutton.button)),"\n"); 
        break;
      case SDL_CONTROLLERAXISMOTION:
        UtilityFunctions::print(SDL_GameControllerGetStringForAxis(SDL_GameControllerAxis(event.caxis.axis))," ",event.caxis.value,"\n");
        break;
      /*hot pluging*/
      case SDL_CONTROLLERDEVICEADDED:
        if (!controller){
          UtilityFunctions::print("controller conected\n");
          /*SDLGyro::controller_init();*/
        }
        break;
      case SDL_CONTROLLERDEVICEREMOVED:
        SDL_GameControllerClose(controller);
        controller=nullptr;
        UtilityFunctions::print("controller removed\n");
        gyroEnabled=false;
        accelEnabled=false;
        break;
    /*-------------------*/
      case SDL_QUIT:
        UtilityFunctions::print("Quiting SDL.\n");
        isRunning=false;
      default:
        break;
    }
  }
return orientation;
}
