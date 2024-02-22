extends Node3D


# Called when the node enters the scene tree for the first time.
func _ready():
  var Gyro=SDLGyro.new()
  Gyro.sdl_init()
  Gyro.controller_init()


# Called every frame. 'delta' is the elapsed time since the previous frame.
