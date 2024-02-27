extends Node3D

var Gyro=SDLGyro.new()
var orientation=Gyro.gamepadPoling()
var actCalibration=false
# Called when the node enters the scene tree for the first time.
func _ready():
  Gyro.sdl_init()
  Gyro.controller_init()

func _process(delta):
  #Gyro.gamepadPoling()
  orientation=Gyro.gamepadPoling()
  
  $Sketchfab_Scene.quaternion=Quaternion(orientation[3],orientation[2],-orientation[1],orientation[0])
  #$Sketchfab_Scene.quaternion.w=orientation[0]
  #$Sketchfab_Scene.quaternion.z=orientation[1]
  #$Sketchfab_Scene.quaternion.y=orientation[2]
  #$Sketchfab_Scene.quaternion.x=-orientation[3]
  #$MeshInstance3D.quaternion.x=orientation[0]
  #$MeshInstance3D.quaternion.y=orientation[1]
  #$MeshInstance3D.quaternion.z=orientation[2]
func _unhandled_input(event):
  if event.is_action_pressed("calibration"):
    if actCalibration==false:
      Gyro.calibrate()
      actCalibration=true
    elif actCalibration==true:
      Gyro.stop_calibrate()
      actCalibration=false
