extends Spatial


var _tween: Tween
var _scene: Spatial

var _tween_playing: bool
var _result: bool

var _path = [
	Vector3(0, 3, 1),
	Vector3(0, 2, 1),
	Vector3(0, 2, 2),
	Vector3(0, 3, 2),
	Vector3(1, 3, 2),
	Vector3(2, 3, 2),
	Vector3(2, 4, 2),
	Vector3(1, 4, 2),
	Vector3(0, 4, 2),
	Vector3(0, 4, 1),
	Vector3(1, 4, 1),
	Vector3(2, 4, 1),
	Vector3(3, 4, 1),
	Vector3(3, 3, 1),
	Vector3(2, 3, 1),
	Vector3(2, 2, 1),
	Vector3(2, 2, 2),
	Vector3(1, 2, 2),
	Vector3(1, 2, 3),
	Vector3(1, 2, 4),
	Vector3(2, 2, 4),
	Vector3(2, 2, 3),
	Vector3(2, 1, 3),
	Vector3(2, 1, 4),
	Vector3(3, 1, 4),
	Vector3(3, 1, 3),
	Vector3(3, 1, 2),
	Vector3(4, 1, 2),
	Vector3(4, 0, 2),
	Vector3(4, 0, 3),
	Vector3(4, 1, 3),
	Vector3(4, 1, 4),
	Vector3(4, 0, 4),
	Vector3(3, 0, 4),
	Vector3(3, 0, 3),
	Vector3(3, 0, 2),
	Vector3(2, 0, 2),
	Vector3(2, 1, 2),
	Vector3(1, 1, 2),
	Vector3(1, 1, 3),
	Vector3(0, 1, 3),
	Vector3(0, 2, 3),
	Vector3(0, 2, 4),
	Vector3(0, 3, 4),
	Vector3(0, 3, 3),
	Vector3(0, 4, 3),
	Vector3(0, 4, 4),
	Vector3(1, 4, 4),
	Vector3(1, 3, 4),
	Vector3(2, 3, 4),
	Vector3(2, 3, 3),
	Vector3(1, 3, 3),
	Vector3(1, 4, 3),
	Vector3(2, 4, 3),
	Vector3(2, 4, 4),
	Vector3(3, 4, 4),
	Vector3(3, 4, 3),
	Vector3(3, 3, 3),
	Vector3(4, 3, 3),
	Vector3(4, 3, 2),
	Vector3(3, 3, 2),
	Vector3(3, 4, 2),
	Vector3(4, 4, 2),
	Vector3(4, 4, 3),
	Vector3(4, 4, 4),
	Vector3(4, 3, 4),
	Vector3(3, 3, 4),
	Vector3(3, 2, 4),
	Vector3(4, 2, 4),
	Vector3(4, 2, 3),
	Vector3(3, 2, 3),
	Vector3(3, 2, 2),
	Vector3(4, 2, 2),
	Vector3(4, 2, 1),
	Vector3(4, 1, 1),
	Vector3(4, 0, 1),
	Vector3(4, 0, 0),
	Vector3(4, 1, 0),
	Vector3(4, 2, 0),
	Vector3(4, 3, 0),
	Vector3(4, 3, 1),
	Vector3(4, 4, 1),
	Vector3(4, 4, 0),
	Vector3(3, 4, 0),
	Vector3(3, 3, 0),
	Vector3(3, 2, 0),
	Vector3(3, 2, 1),
	Vector3(3, 1, 1),
	Vector3(3, 0, 1),
	Vector3(3, 0, 0),
	Vector3(3, 1, 0),
	Vector3(2, 1, 0),
	Vector3(2, 2, 0),
	Vector3(1, 2, 0),
	Vector3(1, 2, 1),
	Vector3(1, 3, 1),
	Vector3(1, 3, 0),
	Vector3(2, 3, 0),
	Vector3(2, 4, 0),
	Vector3(1, 4, 0),
	Vector3(0, 4, 0),
	Vector3(0, 3, 0),
	Vector3(0, 2, 0),
	Vector3(0, 1, 0),
	Vector3(1, 1, 0),
	Vector3(1, 1, 1),
	Vector3(1, 0, 1),
	Vector3(1, 0, 2),
	Vector3(1, 0, 3),
	Vector3(2, 0, 3),
	Vector3(2, 0, 4),
	Vector3(1, 0, 4),
	Vector3(1, 1, 4),
	Vector3(0, 1, 4),
	Vector3(0, 0, 4),
	Vector3(0, 0, 3),
	Vector3(0, 0, 2),
	Vector3(0, 1, 2),
	Vector3(0, 1, 1),
	Vector3(0, 0, 1),
	Vector3(0, 0, 0),
	Vector3(1, 0, 0),
	Vector3(2, 0, 0),
	Vector3(2, 0, 1),
	Vector3(2, 1, 1),
]

var _iteration: int = 0
var _box_dimensions: int = 5


func _ready():
	randomize()
	_tween = get_node("Tween")
	_scene = get_node("/root/World/Scene")

	_tween_playing = false
	_prepare_scene()
	translation = _path[0]
	_place_been(translation)
	print(_path[0])


func _process(_delta):
	if _tween_playing:
		return

	_tween_playing = true

	_try_animate()


func _try_animate():

	if _iteration < _path.size() - 1:
		_play_tween_animation("translation", _path[_iteration + 1], 0.2)
		_iteration += 1

		print(_path[_iteration])


func _place_dot(pos: Vector3):
	var dot_path = "res://assets/check.tscn"
	var dot_block = load(dot_path) as PackedScene
	var dot_instance = dot_block.instance() as Spatial
	dot_instance.translation = pos

	_scene.call_deferred("add_child", dot_instance)


func _place_been(pos: Vector3):
	var dot_path = "res://assets/been.tscn"
	var dot_block = load(dot_path) as PackedScene
	var dot_instance = dot_block.instance() as Spatial
	dot_instance.translation = pos

	_scene.call_deferred("add_child", dot_instance)


func _prepare_scene():
	for x in _box_dimensions:
		for z in _box_dimensions:
			for y in _box_dimensions:
				_place_dot(Vector3(x, y, z))


func _play_tween_animation(type, new_vector, time):
	_result = _tween.interpolate_property(self, type, null, new_vector, time)
	_result = _tween.start()


func _on_tween_completed():
	_tween_playing = false
	_place_been(translation)
