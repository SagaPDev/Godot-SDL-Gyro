#include "sdlgyro.h"
#include <godot_cpp/core/class_db.hpp>
#include <SDL2/SDL.h>
#include <godot_cpp/variant/utility_functions.hpp>
#include "GamepadMotion.hpp"
#include "godot_cpp/variant/array.hpp"
#include "godot_cpp/variant/vector2.hpp"
#include "godot_cpp/variant/vector3.hpp"
#include "godot_cpp/variant/vector4.hpp"
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

Vector3 Gyro;
Vector3 Accel;
Vector4 Orientation;


static constexpr float toDegPerSec = float(180. / M_PI);
static constexpr float toGs = 1.f / 9.8f;

std::chrono::steady_clock::time_point oldTime;
std::chrono::steady_clock::time_point newTime;

GamepadMotion gyroSensor;

void SDLGyro::_bind_methods() {
  ClassDB::bind_method(D_METHOD("sdl_init"),&SDLGyro::sdl_init);
  ClassDB::bind_method(D_METHOD("controller_init"),&SDLGyro::controller_init);
  ClassDB::bind_method(D_METHOD("gamepad_polling"),&SDLGyro::gamepadPolling);
  ClassDB::bind_method(D_METHOD("calibrate"),&SDLGyro::calibrate);
  ClassDB::bind_method(D_METHOD("stop_calibrate"),&SDLGyro::stop_calibrate);
  ClassDB::bind_method(D_METHOD("get_player_space"),&SDLGyro::getPlayer_space);
  ClassDB::bind_method(D_METHOD("get_world_space"),&SDLGyro::getWorld_space);
  ClassDB::bind_method(D_METHOD("get_gravity"),&SDLGyro::getGravity);
  ClassDB::bind_method(D_METHOD("get_calibrated_gyro"),&SDLGyro::getCalibratedGyro);
  ClassDB::bind_method(D_METHOD("get_processed_acceleration"),&SDLGyro::getProcessedAcceleration);
  ClassDB::bind_method(D_METHOD("set_auto_calibration"),&SDLGyro::setAutoCalibration);

  ClassDB::bind_method(D_METHOD("is_gyro_steady"),&SDLGyro::isCalibrationSteady);
  ClassDB::bind_method(D_METHOD("get_calibration_confidence"),&SDLGyro::getCalibrationConfidence);


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

bool SDLGyro::isCalibrationSteady(){
  return gyroSensor.GetAutoCalibrationIsSteady();
}

float SDLGyro::getCalibrationConfidence(){
  return gyroSensor.GetAutoCalibrationConfidence();
}

//Convert To 2D
Variant SDLGyro::getPlayer_space(){
  Vector2 playerSpace;
  gyroSensor.GetWorldSpaceGyro(playerSpace[0],playerSpace[1],sideReductionThreshold);
  return playerSpace;
}
Variant SDLGyro::getWorld_space(){
  Vector2 worldSpace;
  gyroSensor.GetPlayerSpaceGyro(worldSpace[0],worldSpace[1],yawRelaxFactor);
  return worldSpace;
}
Variant SDLGyro::getGravity(){
  Vector3 gravity;
  gyroSensor.GetGravity(gravity[0],gravity[1], gravity[2]);
  return gravity;
}

Variant SDLGyro::getCalibratedGyro(){
  Vector3 calibratedgyro;
  gyroSensor.GetCalibratedGyro(calibratedgyro[0],calibratedgyro[1], calibratedgyro[2]);
  return calibratedgyro;
}
Variant SDLGyro::getProcessedAcceleration(){
  Vector3 processedAcc;
  gyroSensor.GetCalibratedGyro(processedAcc[0],processedAcc[1], processedAcc[2]);
  return processedAcc;
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
        SDL_GameControllerSetSensorEnabled(test_controller,SDL_SENSOR_ACCEL,SDL_TRUE);
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

void SDLGyro::setAutoCalibration(){
  gyroSensor.SetCalibrationMode(GamepadMotionHelpers::Stillness | GamepadMotionHelpers::SensorFusion);
}

Variant SDLGyro::gamepadPolling(){
  Vector4 orientation;
  //TypedArray<float> orientation;
  //IMU gyro
  if (gyroEnabled && accelEnabled){
    SDL_GameControllerGetSensorData(controller,SDL_SENSOR_GYRO, &Gyro[0], 3);
  //IMU accelerometer//
    SDL_GameControllerGetSensorData(controller,SDL_SENSOR_ACCEL, &Accel[0], 3);
  //Sensor Fussion//
    if (oldTime!=newTime)
      newTime=std::chrono::steady_clock::now();
    deltaTime=((float)std::chrono::duration_cast<std::chrono::microseconds>(newTime-oldTime).count()) / 1000000.0f;

    gyroSensor.ProcessMotion(Gyro[0]*toDegPerSec, Gyro[1]*toDegPerSec, Gyro[2]*toDegPerSec, Accel[0]*toGs, Accel[1]*toGs, Accel[2]*toGs,deltaTime);
    oldTime=std::chrono::steady_clock::now();

    gyroSensor.GetOrientation(orientation[0], orientation[1], orientation[2], orientation[3]);
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
  if (pollingEnabled){
    return orientation;
    }
  else{
    orientation = Vector4(1.0,0.0,0.0,0.0);
    return orientation;
  }
}
