extends Node3D

var Gyro=SDLGyro.new()
var orientation=Gyro.gamepad_polling()
var actCalibration=false
var highlight_material = preload("res://button_hightlight.tres")  # Load the material
@onready var model = $Sketchfab_Scene/Sketchfab_model/root/GLTF_SceneRootNode
# Called when the node enters the scene tree for the first time.
func _ready():
  Gyro.sdl_init()
  Gyro.controller_init()
func _process(_delta):
  var rsdirection_vector=Vector3.ZERO
  var lsdirection_vector=Vector3.ZERO
  rsdirection_vector= Input.get_vector("RSTICK_LEFT","RSTICK_RIGHT","RSTICK_UP","RSTICK_DOWN")
  lsdirection_vector= Input.get_vector("LSTICK_LEFT","LSTICK_RIGHT","LSTICK_UP","LSTICK_DOWN")
  if lsdirection_vector!=Vector2.ZERO:
      model.get_node("Left_Stick/Ring/Mesh").set_surface_override_material(0,highlight_material)
  elif lsdirection_vector==Vector2.ZERO:
      model.get_node("Left_Stick/Ring/Mesh").set_surface_override_material(0,null)


  if rsdirection_vector!=Vector2.ZERO:
      model.get_node("Right_Stick/Ring/Mesh").set_surface_override_material(0,highlight_material)
  elif rsdirection_vector==Vector2.ZERO:
      model.get_node("Right_Stick/Ring/Mesh").set_surface_override_material(0,null)

  model.get_node("Right_Stick").rotation_degrees.x=-rsdirection_vector.x*30
  model.get_node("Right_Stick").rotation_degrees.z=-rsdirection_vector.y*30

  model.get_node("Left_Stick").rotation_degrees.x=-lsdirection_vector.x*30
  model.get_node("Left_Stick").rotation_degrees.z=-lsdirection_vector.y*30
  orientation=Gyro.gamepad_polling()
  $Sketchfab_Scene.quaternion=Quaternion(orientation[3],orientation[2],-orientation[1],orientation[0])

func _unhandled_input(event):
  if event.is_action_pressed("calibration"):
    if actCalibration==false:
      $Label.visible=true
      Gyro.calibrate()
      actCalibration=true
    elif actCalibration==true:
      Gyro.stop_calibrate()
      $Label.visible=false
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
      model.get_node("Right_Stick/Cap/Mesh").set_surface_override_material(0,highlight_material)
  elif event.is_action_released("R_STICK"):
      model.get_node("Right_Stick/Cap/Mesh").set_surface_override_material(0,null)


  if event.is_action_pressed("L_STICK"):
      model.get_node("Left_Stick/Cap/Mesh").set_surface_override_material(0,highlight_material)
  elif event.is_action_released("L_STICK"):
      model.get_node("Left_Stick/Cap/Mesh").set_surface_override_material(0,null)
