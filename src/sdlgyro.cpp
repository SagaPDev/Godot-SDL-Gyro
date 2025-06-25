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

// Joy-Con izquierdo y derecho
// Estos son punteros a los controladores SDL para los Joy-Con izquierdo y derecho
// Joy-Con izquierdo
SDL_GameController *joy_con_left = nullptr;
bool pollingEnabled_L = true;
bool gyroEnabled_L = false;
bool accelEnabled_L = false;
Vector3 Gyro_L;
Vector3 Accel_L;
Vector4 Orientation_L;
GamepadMotion gyroSensor_L;

// Joy-Con derecho
SDL_GameController *joy_con_right = nullptr;
bool pollingEnabled_R = true;
bool gyroEnabled_R = false;
bool accelEnabled_R = false;
Vector3 Gyro_R;
Vector3 Accel_R;
Vector4 Orientation_R;
GamepadMotion gyroSensor_R;


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
  ClassDB::bind_method(D_METHOD("get_available_gyro_controllers"), &SDLGyro::get_available_gyro_controllers);
  ClassDB::bind_method(D_METHOD("get_gamepad_name", "controller_index"), &SDLGyro::get_gamepad_name);
  ClassDB::bind_method(D_METHOD("poll_motion_data", "controller_index"), &SDLGyro::poll_motion_data);
  ClassDB::bind_method(D_METHOD("poll_both_joycon_gyros", "controller_index"), &SDLGyro::poll_both_joycon_gyros);
  ClassDB::bind_method(D_METHOD("init_joycons"), &SDLGyro::init_joycons);
  ClassDB::bind_method(D_METHOD("joycon_polling"), &SDLGyro::joycon_polling);
  ClassDB::bind_method(D_METHOD("joycon_calibrate"), &SDLGyro::joycon_calibrate);
  ClassDB::bind_method(D_METHOD("joycon_stop_calibrate"), &SDLGyro::joycon_stop_calibrate);
  ClassDB::bind_method(D_METHOD("get_joycon_l_orientation"), &SDLGyro::get_joycon_l_orientation);
  ClassDB::bind_method(D_METHOD("get_joycon_r_orientation"), &SDLGyro::get_joycon_r_orientation);
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
  gyroSensor.GetWorldSpaceGyro(playerSpace[0],playerSpace[1]);
  return playerSpace;
}
Variant SDLGyro::getWorld_space(){
  Vector2 worldSpace;
  gyroSensor.GetPlayerSpaceGyro(worldSpace[0],worldSpace[1]);
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

// Polls motion data from a specific controller index
Array SDLGyro::get_available_gyro_controllers() {
  Array result;
  for (int i = 0; i < SDL_NumJoysticks(); i++) {
    if (SDL_IsGameController(i)) {
      SDL_GameController *temp = SDL_GameControllerOpen(i);
      if (!temp) continue;

      if (SDL_GameControllerHasSensor(temp, SDL_SENSOR_GYRO)) {
        result.append(i);
      }
      SDL_GameControllerClose(temp);
    }
  }
  return result;
}

// Polls motion data from a specific controller index
String SDLGyro::get_gamepad_name(int controller_index) {
  if (SDL_IsGameController(controller_index)) {
    const char* name = SDL_GameControllerNameForIndex(controller_index);
    if (name) return String(name);
  }
  return "[Desconocido]";
}


// Polls motion data from a specific controller index
Variant SDLGyro::poll_motion_data(int controller_index) {
  if (!SDL_IsGameController(controller_index)) {
    UtilityFunctions::print("Controlador inválido\n");
    return Variant();
  }

  SDL_GameController* temp = SDL_GameControllerOpen(controller_index);
  if (!temp) return Variant();

  if (!SDL_GameControllerHasSensor(temp, SDL_SENSOR_GYRO)) {
    SDL_GameControllerClose(temp);
    UtilityFunctions::print("Este mando no tiene giroscopio\n");
    return Variant();
  }

  float gyro[3];
  SDL_GameControllerSetSensorEnabled(temp, SDL_SENSOR_GYRO, SDL_TRUE);
  SDL_GameControllerGetSensorData(temp, SDL_SENSOR_GYRO, gyro, 3);
  SDL_GameControllerClose(temp);

  Vector3 v(gyro[0] * toDegPerSec, gyro[1] * toDegPerSec, gyro[2] * toDegPerSec);
  return v;
}

// Polls both Joy-Con gyros and returns their data in a Dictionary
Dictionary SDLGyro::poll_both_joycon_gyros(int controller_index) {
    Dictionary result;

    if (!SDL_IsGameController(controller_index)) {
        UtilityFunctions::print("Controlador inválido\n");
        return result;
    }

    SDL_GameController *gc = SDL_GameControllerOpen(controller_index);
    if (!gc) {
        UtilityFunctions::print("No se pudo abrir el controlador\n");
        return result;
    }

    float gyro_l[3] = {0}, gyro_r[3] = {0};

    // Gyro Izquierdo
    if (SDL_GameControllerHasSensor(gc, SDL_SENSOR_GYRO_L)) {
        SDL_GameControllerSetSensorEnabled(gc, SDL_SENSOR_GYRO_L, SDL_TRUE);
        SDL_GameControllerGetSensorData(gc, SDL_SENSOR_GYRO_L, gyro_l, 3);
    }

    // Gyro Derecho
    if (SDL_GameControllerHasSensor(gc, SDL_SENSOR_GYRO_R)) {
        SDL_GameControllerSetSensorEnabled(gc, SDL_SENSOR_GYRO_R, SDL_TRUE);
        SDL_GameControllerGetSensorData(gc, SDL_SENSOR_GYRO_R, gyro_r, 3);
    }

    SDL_GameControllerClose(gc);

    result["gyro_left"] = Vector3(gyro_l[0] * toDegPerSec, gyro_l[1] * toDegPerSec, gyro_l[2] * toDegPerSec);
    result["gyro_right"] = Vector3(gyro_r[0] * toDegPerSec, gyro_r[1] * toDegPerSec, gyro_r[2] * toDegPerSec);

    return result;
}

// Initializes Joy-Con controllers if both left and right sensors are available
void SDLGyro::init_joycons() {
    int total = SDL_NumJoysticks();

    for (int i = 0; i < total; i++) {
        if (!SDL_IsGameController(i)) continue;

        SDL_GameController *gc = SDL_GameControllerOpen(i);
        if (!gc) continue;

        // Revisamos si este mando tiene ambos sensores L y R
        if (SDL_GameControllerHasSensor(gc, SDL_SENSOR_GYRO_L) &&
            SDL_GameControllerHasSensor(gc, SDL_SENSOR_GYRO_R)) {
            
            SDL_GameControllerSetSensorEnabled(gc, SDL_SENSOR_GYRO_L, SDL_TRUE);
            SDL_GameControllerSetSensorEnabled(gc, SDL_SENSOR_ACCEL_L, SDL_TRUE);
            SDL_GameControllerSetSensorEnabled(gc, SDL_SENSOR_GYRO_R, SDL_TRUE);
            SDL_GameControllerSetSensorEnabled(gc, SDL_SENSOR_ACCEL_R, SDL_TRUE);

            controller = gc;

            gyroEnabled_L = accelEnabled_L = true;
            gyroEnabled_R = accelEnabled_R = true;

            UtilityFunctions::print("Joy-Con izquierdo y derecho inicializados.");
            break;
        } else {
            SDL_GameControllerClose(gc);
        }
    }

    if (!controller) {
        UtilityFunctions::print("No están conectados ambos sensores (L/R).");
    }
}



// Polls motion data from Joy-Con controllers
Dictionary SDLGyro::joycon_polling() {
    Dictionary result;

    auto now = std::chrono::steady_clock::now();
    deltaTime = ((float)std::chrono::duration_cast<std::chrono::microseconds>(now - oldTime).count()) / 1000000.0f;
    oldTime = now;

    // Joy-Con izquierdo
    if (gyroEnabled_L && accelEnabled_L && pollingEnabled_L && controller) {
        SDL_GameControllerGetSensorData(controller, SDL_SENSOR_GYRO_L, &Gyro_L[0], 3);
        SDL_GameControllerGetSensorData(controller, SDL_SENSOR_ACCEL_L, &Accel_L[0], 3);

        gyroSensor_L.ProcessMotion(
            Gyro_L[0] * toDegPerSec, Gyro_L[1] * toDegPerSec, Gyro_L[2] * toDegPerSec,
            Accel_L[0] * toGs, Accel_L[1] * toGs, Accel_L[2] * toGs,
            deltaTime
        );

        gyroSensor_L.GetOrientation(
            Orientation_L[0], Orientation_L[1], Orientation_L[2], Orientation_L[3]
        );
    }

    // Joy-Con derecho
    if (gyroEnabled_R && accelEnabled_R && pollingEnabled_R && controller) {
        SDL_GameControllerGetSensorData(controller, SDL_SENSOR_GYRO_R, &Gyro_R[0], 3);
        SDL_GameControllerGetSensorData(controller, SDL_SENSOR_ACCEL_R, &Accel_R[0], 3);

        gyroSensor_R.ProcessMotion(
            Gyro_R[0] * toDegPerSec, Gyro_R[1] * toDegPerSec, Gyro_R[2] * toDegPerSec,
            Accel_R[0] * toGs, Accel_R[1] * toGs, Accel_R[2] * toGs,
            deltaTime
        );

        gyroSensor_R.GetOrientation(
            Orientation_R[0], Orientation_R[1], Orientation_R[2], Orientation_R[3]
        );
    }

    // IMPORTANTE: procesar el event loop para mantener SDL funcionando correctamente
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_CONTROLLERDEVICEADDED:
                // Podrías volver a llamar a init_joycons() si necesitas reconectar
                break;

            case SDL_CONTROLLERDEVICEREMOVED:
                SDL_GameControllerClose(controller);
                controller = nullptr;
                gyroEnabled_L = accelEnabled_L = false;
                gyroEnabled_R = accelEnabled_R = false;
                break;

            case SDL_QUIT:
                // Si fuera necesario manejar cierre de app
                break;

            default:
                break;
        }
    }

    // Guardamos en el diccionario
    result["left"] = Orientation_L;
    result["right"] = Orientation_R;

    return result;
}


// Starts the continuous calibration for both Joy-Con gyros
void SDLGyro::joycon_calibrate() {
    if (gyroEnabled_L)
        gyroSensor_L.Reset();
    if (gyroEnabled_R)
        gyroSensor_R.Reset();

    pollingEnabled_L = false;
    pollingEnabled_R = false;

    if (gyroEnabled_L)
        gyroSensor_L.StartContinuousCalibration();
    if (gyroEnabled_R)
        gyroSensor_R.StartContinuousCalibration();
}


// Stops the continuous calibration for both Joy-Con gyros
void SDLGyro::joycon_stop_calibrate() {
    if (gyroEnabled_L)
        gyroSensor_L.PauseContinuousCalibration();
    if (gyroEnabled_R)
        gyroSensor_R.PauseContinuousCalibration();

    pollingEnabled_L = true;
    pollingEnabled_R = true;
}

// Returns the orientation of the left Joy-Con
Variant SDLGyro::get_joycon_l_orientation() {
    return Orientation_L;
}

// Returns the orientation of the right Joy-Con
Variant SDLGyro::get_joycon_r_orientation() {
    return Orientation_R;
}
