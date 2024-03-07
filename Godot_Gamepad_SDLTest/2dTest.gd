extends Node2D
var Gyro=SDLGyro.new()
var world_space=Gyro.getWorld_space()
var player_space=Gyro.getPlayer_space()

# Called when the node enters the scene tree for the first time.
func _ready():
  Gyro.sdl_init()
  Gyro.controller_init()


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(_delta):
  Gyro.gamepadPoling()
  prints(Gyro.getWorld_space())
  world_space=Gyro.getPlayer_space()
  $Sprite2D.position.x=world_space[0]
  $Sprite2D.position.y=world_space[1]
