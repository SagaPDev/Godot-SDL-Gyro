extends Node3D

var Gyro=SDLGyro.new()
var orientation=Gyro.gamepadPoling()
var actCalibration=false
var highlight_material = preload("res://button_hightlight.tres")  # Load the material
@onready var model = $Sketchfab_Scene/Sketchfab_model/root/GLTF_SceneRootNode
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

  if event.is_action_pressed("B_SOUTH"):
      model.get_node("South_Button/Mesh").set_surface_override_material(0,highlight_material)
  elif event.is_action_released("B_SOUTH"):
      model.get_node("South_Button/Mesh").set_surface_override_material(0,null)

  if event.is_action_pressed("B_EAST"):
      model.get_node("East_Button/Mesh").set_surface_override_material(0,highlight_material)
  elif event.is_action_released("B_EAST"):
      model.get_node("East_Button/Mesh").set_surface_override_material(0,null)

  if event.is_action_pressed("B_WEST"):
      model.get_node("West_Button/Mesh").set_surface_override_material(0,highlight_material)
  elif event.is_action_released("B_WEST"):
      model.get_node("West_Button/Mesh").set_surface_override_material(0,null)

  if event.is_action_pressed("B_NORTH"):
      model.get_node("North_Button/Mesh").set_surface_override_material(0,highlight_material)
  elif event.is_action_released("B_NORTH"):
      model.get_node("North_Button/Mesh").set_surface_override_material(0,null)

  if event.is_action_pressed("D_UP"):
      model.get_node("Dpad_Up/Mesh").set_surface_override_material(0,highlight_material)
  elif event.is_action_released("D_UP"):
      model.get_node("Dpad_Up/Mesh").set_surface_override_material(0,null)

  if event.is_action_pressed("D_DOWN"):
      model.get_node("Dpad_Down/Mesh").set_surface_override_material(0,highlight_material)
  elif event.is_action_released("D_DOWN"):
      model.get_node("Dpad_Down/Mesh").set_surface_override_material(0,null)

  if event.is_action_pressed("D_LEFT"):
      model.get_node("Dpad_Left/Mesh").set_surface_override_material(0,highlight_material)
  elif event.is_action_released("D_LEFT"):
      model.get_node("Dpad_Left/Mesh").set_surface_override_material(0,null)

  if event.is_action_pressed("D_RIGHT"):
      model.get_node("Dpad_Right/Mesh").set_surface_override_material(0,highlight_material)
  elif event.is_action_released("D_RIGHT"):
      model.get_node("Dpad_Right/Mesh").set_surface_override_material(0,null)


  if event.is_action_pressed("RB"):
      model.get_node("Right_Bumper/Mesh").set_surface_override_material(0,highlight_material)
  elif event.is_action_released("RB"):
      model.get_node("Right_Bumper/Mesh").set_surface_override_material(0,null)

  if event.is_action_pressed("LB"):
      model.get_node("Left_Bumper/Mesh").set_surface_override_material(0,highlight_material)
  elif event.is_action_released("LB"):
      model.get_node("Left_Bumper/Mesh").set_surface_override_material(0,null)

  if event.is_action_pressed("RT"):
      model.get_node("Right_Trigger/Mesh").set_surface_override_material(0,highlight_material)
  elif event.is_action_released("RT"):
      model.get_node("Right_Trigger/Mesh").set_surface_override_material(0,null)

  if event.is_action_pressed("LT"):
      model.get_node("Left_Trigger/Mesh").set_surface_override_material(0,highlight_material)
  elif event.is_action_released("LT"):
      model.get_node("Left_Trigger/Mesh").set_surface_override_material(0,null)


  if event.is_action_pressed("START"):
      model.get_node("Start_Button/Mesh").set_surface_override_material(0,highlight_material)
  elif event.is_action_released("START"):
      model.get_node("Start_Button/Mesh").set_surface_override_material(0,null)

  if event.is_action_pressed("SELECT"):
      model.get_node("Select_Button/Mesh").set_surface_override_material(0,highlight_material)
  elif event.is_action_released("SELECT"):
      model.get_node("Select_Button/Mesh").set_surface_override_material(0,null)

  if event.is_action_pressed("R_STICK"):
      model.get_node("Right_Cap/Mesh").set_surface_override_material(0,highlight_material)
  elif event.is_action_released("R_STICK"):
      model.get_node("Right_Cap/Mesh").set_surface_override_material(0,null)


  if event.is_action_pressed("L_STICK"):
      model.get_node("Left_Cap/Mesh").set_surface_override_material(0,highlight_material)
  elif event.is_action_released("L_STICK"):
      model.get_node("Left_Cap/Mesh").set_surface_override_material(0,null)
    

  if event.is_action_pressed("L_STICK_M"):
      model.get_node("Left_Ring/Mesh").set_surface_override_material(0,highlight_material)
  elif event.is_action_released("L_STICK_M"):
      model.get_node("Left_Ring/Mesh").set_surface_override_material(0,null)


