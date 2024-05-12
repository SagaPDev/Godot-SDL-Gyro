# Godot SDL Gyro
GDextention that allows to read Gyro from a controller in Godot 4, it utilizes SDL and JibbSmart [GamepadMotionHelpers](https://github.com/JibbSmart/GamepadMotionHelpers) libraries
## Usage
Download the latest build from the [Releases](https://github.com/SagaPDev/Godot-SDL-Gyro/releases/latest) tab Extract the addons folder and place it in the root of your project

in your GDscript make a new SDLGyro object
~~~
...
var Gyro=SDLGyro.new()
...
~~~
initialize SDL and The controller ready function
~~~
...
func _ready():
  Gyro.sdl_init()
  Gyro.controller_init()
...
~~~
call  any of the folloing functions in the process fuction
~~~
      gamepad_polling()            //returns controller orientation(this function needs to be called so that the others can work)

      calibrate()                //starts continous calibrations
      stop_calibrate()           //stops continius calibration
     
      get_player_space()          //not working
      get_world_space()           //not working
      get_gravity()               //not working
  
      get_calibrated_gyro();       //retutns the controller's angular velocity in degrees per second
      get_processed_acceleration();//returns the controller's current acceleration in g-force with gravity removed
~~~
## Build Instructions
### Windows  
[Download SDL2-devel-X.XX.X-mingw.zip](https://github.com/libsdl-org/SDL/releases/latest)
extract and place the contetns of SDL2-X.XX.X\x86_64-w64-mingw32 in here -> libs/mingw_dev_lib (you could change that location in the SConstruct file)   
then:
~~~
$ git clone https://github.com/SagaPDev/Godot-SDL-Gamepad-Test.git
$ cd Godot-SDL-Gamepad-Test
$ scons use_mingw=yes
~~~
### Linux  
~~~
$ git clone https://github.com/SagaPDev/Godot-SDL-Gamepad-Test.git
$ cd Godot-SDL-Gamepad-Test
$ scons
~~~
## TO DO:  
Hot Plugin   
Suport for Multiple Gamepads   
Release on Godot's Asset Library   
## Aditional Credits
Source of the [3Dmodel](https://sketchfab.com/3d-models/ps4-controller-from-3d-controller-overlay-d8569dc4e3af46a4b137f2926423f195) by [larf](https://sketchfab.com/larf)
