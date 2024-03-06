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
env.Append(CPPPATH=["src/"],LIBS='SDL2')
sources = Glob("src/*.cpp")

if env["platform"] == "windows" and (env['CC'] == 'gcc' or env['CXX'] == 'g++'):

    env.Append(CPPPATH=['C:\\mingw_dev_lib\\include\\SDL2'])
    env.Append(LIBPATH=['C:\\mingw_dev_lib\\lib'])

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
        "Godot_Gamepad_SDLTest/bin/sdlgyro.{}.{}.framework/sdlgyro.{}.{}".format(
            env["platform"], env["target"], env["platform"], env["target"]
        ),
        source=sources,
    )
else:
    library = env.SharedLibrary(
        "Godot_Gamepad_SDLTest/bin/sdlgyro{}{}".format(env["suffix"], env["SHLIBSUFFIX"]),
        source=sources,
    )

Default(library)
