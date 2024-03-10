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

bool pollingEnabled=true;
bool gyroEnabled=false;
bool accelEnabled=false;
float deltaTime=0.0;

std::array<float, 3> rawGyro;
std::array<float, 3> rawAccel;
std::array<float, 4> rawOrientation;

std::array<float, 2> rawPlaySpace;
std::array<float, 2> rawWorldSpace;

std::array<float, 3> rawGravity;

static constexpr float toDegPerSec = float(180. / M_PI);
static constexpr float toGs = 1.f / 9.8f;
const float sideReductionThreshold = 0.125f;
const float yawRelaxFactor = 1.41f;
std::chrono::steady_clock::time_point oldTime;
std::chrono::steady_clock::time_point newTime;

GamepadMotion gyroSensor;

void SDLGyro::_bind_methods() {
  ClassDB::bind_method(D_METHOD("sdl_init"),&SDLGyro::sdl_init);
  ClassDB::bind_method(D_METHOD("controller_init"),&SDLGyro::controller_init);
  ClassDB::bind_method(D_METHOD("gamepadPoling"),&SDLGyro::gamepadPoling);
  ClassDB::bind_method(D_METHOD("calibrate"),&SDLGyro::calibrate);
  ClassDB::bind_method(D_METHOD("stop_calibrate"),&SDLGyro::stop_calibrate);
  ClassDB::bind_method(D_METHOD("getPlayer_space"),&SDLGyro::getPlayer_space);
  ClassDB::bind_method(D_METHOD("getWorld_space"),&SDLGyro::getPlayer_space);
  ClassDB::bind_method(D_METHOD("getGravity"),&SDLGyro::getGravity);
}

void SDLGyro::sdl_init() {
  newTime=std::chrono::steady_clock::now();
  oldTime=newTime;
  //SDL initializATION
  if((SDL_Init(SDL_INIT_GAMECONTROLLER))<0){
    //UtilityFunctions::print("could not initialize SDL \n");
  }
  else{
    //UtilityFunctions::print("SDL initialized!!!!!!!!!!! \n");
  }

}

//CALIBRATION
void SDLGyro::calibrate(){
  gyroSensor.Reset();
  pollingEnabled=false;
  gyroSensor.StartContinuousCalibration(); 
}
void SDLGyro::stop_calibrate(){
  gyroSensor.PauseContinuousCalibration();
  pollingEnabled=true;
}

//Convert To 2D
Variant SDLGyro::getPlayer_space(){
  TypedArray<float> playerSpace;
  gyroSensor.GetWorldSpaceGyro(rawPlaySpace[0],rawPlaySpace[1],sideReductionThreshold);
  playerSpace.push_back(rawPlaySpace[0]);
  playerSpace.push_back(rawPlaySpace[1]);
  return playerSpace;
}
Variant SDLGyro::getWorld_space(){
  TypedArray<float> worldSpace;
  gyroSensor.GetPlayerSpaceGyro(rawWorldSpace[0],rawWorldSpace[1],yawRelaxFactor);
  worldSpace.push_back(rawWorldSpace[0]);
  worldSpace.push_back(rawWorldSpace[1]);
  return worldSpace;
}
Variant SDLGyro::getGravity(){
  TypedArray<float> gravity;
  gyroSensor.GetGravity(rawGravity[0],rawGravity[1], rawGravity[3]);
  gravity.push_back(rawGravity[0]);
  gravity.push_back(rawGravity[2]);
  gravity.push_back(rawGravity[3]);
  return gravity;
}
void SDLGyro::controller_init(){
  SDL_GameController *test_controller =nullptr;
  bool test_gyroEnabled;
  bool test_accelEnabled;
   //controller initialization
  for (int i=0;i<SDL_NumJoysticks();i++){
    //UtilityFunctions::print(SDL_IsGameController(i),"\n");
    test_controller = SDL_GameControllerOpen(i);
    //UtilityFunctions::print(SDL_GameControllerNameForIndex(i),"\n");
    if(SDL_IsGameController(i)){
      //test gyro
      if (SDL_GameControllerHasSensor(test_controller,SDL_SENSOR_GYRO)){
        //UtilityFunctions::print("Gyro Detected\n");
        SDL_GameControllerSetSensorEnabled(test_controller,SDL_SENSOR_GYRO,SDL_TRUE);
        test_gyroEnabled=true;
      }
      else{ 
        //UtilityFunctions::print("gyro disabled\n");
        test_gyroEnabled=false;
      }
      //test accelerometer
      if (SDL_GameControllerHasSensor(test_controller,SDL_SENSOR_ACCEL)){
        //UtilityFunctions::print("accelerometer Detected\n");
        SDL_GameControllerSetSensorEnabled(controller,SDL_SENSOR_ACCEL,SDL_TRUE);
        test_accelEnabled=true;
      }
      else{
        //UtilityFunctions::print("accelerometer not Detected\n");
        test_accelEnabled=false;
      } 
    }
    if (test_accelEnabled && test_gyroEnabled){
      controller = test_controller;
      gyroEnabled=true;
      accelEnabled=true;
    }
  }
}


Variant SDLGyro::gamepadPoling(){
  TypedArray<float> orientation;
  //IMU gyro
  if (gyroEnabled && accelEnabled){
    SDL_GameControllerGetSensorData(controller,SDL_SENSOR_GYRO, &rawGyro[0], 3);
  //IMU accelerometer//
    SDL_GameControllerGetSensorData(controller,SDL_SENSOR_ACCEL, &rawAccel[0], 3);
  //Sensor Fussion//
    if (oldTime!=newTime)
      newTime=std::chrono::steady_clock::now();
    deltaTime=((float)std::chrono::duration_cast<std::chrono::microseconds>(newTime-oldTime).count()) / 1000000.0f;

    gyroSensor.ProcessMotion(rawGyro[0]*toDegPerSec, rawGyro[1]*toDegPerSec, rawGyro[2]*toDegPerSec, rawAccel[0]*toGs, rawAccel[1]*toGs, rawAccel[2]*toGs,deltaTime);
    oldTime=std::chrono::steady_clock::now();

    gyroSensor.GetOrientation(rawOrientation[0], rawOrientation[1], rawOrientation[2], rawOrientation[3]);
  }

  //event loop//
  while(SDL_PollEvent(&event)){
    switch (event.type) {
      //hot pluging//
      case SDL_CONTROLLERDEVICEADDED:
        if (!controller){
          //UtilityFunctions::print("controller conected\n");
          //SDLGyro::controller_init();//
        }
        break;
      case SDL_CONTROLLERDEVICEREMOVED:
        SDL_GameControllerClose(controller);
        controller=nullptr;
        //UtilityFunctions::print("controller removed\n");
        gyroEnabled=false;
        accelEnabled=false;
        break;
    //-------------------//
      case SDL_QUIT:
        //UtilityFunctions::print("Quiting SDL.\n");
      default:
        break;
    }
  }
  if (pollingEnabled==true){
    orientation.push_back(rawOrientation[0]);/*w*/
    orientation.push_back(rawOrientation[1]);/*x*/
    orientation.push_back(rawOrientation[2]);/*y*/
    orientation.push_back(rawOrientation[3]);/*z*/
    return orientation;
    }
  else{
    orientation.push_back(1.0);/*w*/
    orientation.push_back(0.0);/*x*/
    orientation.push_back(0.0);/*y*/
    orientation.push_back(0.0);/*z*/
    return orientation;
  }
}
