# Godot SDL Gyro
GDextention That enables to read Gyro from a controller in Godot 4, it utilizes SDL and JibbSmart [GamepadMotionHelpers](https://github.com/JibbSmart/GamepadMotionHelpers) libraries
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
      gamepadPoling()            //returns controller orientation(this function needs to be called so that the others can work)

      calibrate()                //starts continous calibrations
      stop_calibrate()           //stops continius calibration
     
      getPlayer_space()          //not working
      getWorld_space()           //not working
      getGravity()               //not working
  
      getCalibratedGyro();       //retutns the controller's angular velocity in degrees per second
      getProcessedAcceleration();//returns the controller's current acceleration in g-force with gravity removed
~~~
## Build Instructions
### Windows  
[Download SDL2-devel-X.XX.X-mingw.zip](https://github.com/libsdl-org/SDL/releases/latest)
uncompreess and place hte contetns of SDL2-X.XX.X\x86_64-w64-mingw32 in here -> libs/mingw_dev_lib (you can change that location in the SConstruct file)   
then:
~~~
$ git clone https://github.com/SagaPDev/Godot-SDL-Gamepad-Test.git
$ cd Godot-SDL-Gamepad-Test
$ scons use_minwg=yes
~~~
### Linux  
~~~
$ git clone https://github.com/SagaPDev/Godot-SDL-Gamepad-Test.git
$ cd Godot-SDL-Gamepad-Test
$ scons
~~~

## Aditional Credits
Source of the [3Dmodel](https://sketchfab.com/3d-models/ps4-controller-from-3d-controller-overlay-d8569dc4e3af46a4b137f2926423f195) by [larf](https://sketchfab.com/larf)
