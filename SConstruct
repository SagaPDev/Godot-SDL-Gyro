#!/usr/bin/env python
import os
import sys

env = SConscript("godot-cpp/SConstruct")
# For reference:
# - CCFLAGS are compilation flags shared between C and C++
# - CFLAGS are for C-specific compilation flags
# - CXXFLAGS are for C++-specific compilation flags
# - CPPFLAGS are for pre-processor flags
# - CPPDEFINES are for pre-processor defines
# - LINKFLAGS are for linking flags
# tweak this if you want to use different folders, or more folders, to store your source code in.
print("CC ",env['CC'])
print("C++ ",env['CXX'])
print("Using mngw",env["use_mingw"])

env.Append(CPPPATH=["libs/mingw_dev_lib/include"],LIBS='SDL2')
sources = Glob("src/*.cpp")

if env["platform"] == "windows" and (env["use_mingw"]==True):
    env.Append(CPPPATH=['libs/mingw_dev_lib/include'])
    env.Append(LIBPATH=['libs/mingw_dev_lib/lib'])

    env.Append(LINKFLAGS=["-Wl,--dynamicbase","-Wl,--nxcompat"])
    env.Append(CXXFLAGS =["-mwindows"])
    env.Append(
        LIBS=[
            "SDL2",
            "SDL2main",
            "setupapi",
            "winmm",
            "user32",
            "gdi32",
            "dinput8",
            "dxguid",
            "dxerr8",
            "imm32",
            "ole32",
            "version",
            "uuid",
            "m",
            "oleaut32",
            "shell32"
        ]
    )

elif env['CC'] == 'cl':
    env.Append(
        LIBS=[
            "SDL2",
            "SDL2main",
        ]
    )

if env["platform"] == "macos":
    library = env.SharedLibrary(
        "Godot_Gamepad_SDLTest/addons/godot-sdl-gyro/sdlgyro.{}.{}.framework/sdlgyro.{}.{}".format(
            env["platform"], env["target"], env["platform"], env["target"]
        ),
        source=sources,
    )
else:
    library = env.SharedLibrary(
        "Godot_Gamepad_SDLTest/addons/godot-sdl-gyro/sdlgyro{}{}".format(env["suffix"], env["SHLIBSUFFIX"]),
        source=sources,
    )

Default(library)
