# Godot SDL Gamepad Test
Example project to test the usage of GDextentions inspired
This program shows a 3D representation of a controller and rotates acording to the device orientation.  
It was made using the SDL library and the [GamepadMotionHelpers](https://github.com/JibbSmart/GamepadMotionHelpers) library by JibbSmart.  
This program workds on Linux only, for a windows equivalent try [3d-controller-overlay](http://www.3d-controller-overlay.org/)
## Usage
[Download](https://github.com/SagaPDev/Godot-SDL-Gamepad-Test/releases/latest/download/Godot_SDL_gamepad_test.zip) and extract both files in the same folder and execute, 
Make sure your gamepad is conected before runing, if drift is present place the gamepad in an even surface and press 'C' to start the calibration process, press 'C' again to end calibrarion
## Build Instructions
### Windows  
[Download SDL2-devel-X.XX.X-mingw.zip](https://github.com/libsdl-org/SDL/releases/latest)
uncompreess and place hte contetns of SDL2-X.XX.X\x86_64-w64-mingw32 in here -> C:\mingw_dev_lib (you can change that location in the SConstruct file)
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
In both cases the target library should be compiled here **Godot_Gamepad_SDLTest/bin**   
open the project located at **Godot_Gamepad_SDLTest/project.godot** with the godot editor and it should load the library

## Aditional Credits
Source of the [3Dmodel](https://sketchfab.com/3d-models/ps4-controller-from-3d-controller-overlay-d8569dc4e3af46a4b137f2926423f195) by [larf](https://sketchfab.com/larf)
