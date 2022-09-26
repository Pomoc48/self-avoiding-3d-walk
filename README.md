# Self-avoiding walk
Path on a grid that visits every point, that doesn't cross itself (visits the same point twice) in a set and confined space.


## Preview
![Preview](https://raw.githubusercontent.com/Pomoc48/self-avoiding-3d-walk/master/assets/screenshot.png)
Example of a 4x4x4 3d self-avoiding walk.


## Finding the correct path
Number of possibilities for finding the correct path for every next dimension increases exponentially.


### Running single script at once:

```bash
dart calculate.dart 4 #size of the 3D cube
```

### Running the script multiple times in parallel:

```bash
./find
```

Specify the number of cpu cores you want to use and the size of the cube inside the file.


The script will exit when the correct path if found.
Results will be put in the newly created `output.txt` file.


## Play the animation
Import this repository as a [Godot Game Engine](https://godotengine.org/) project, open it and press F5.


You can play your own set of script-generated moves by changing the `_path` and `_box_dimensions` variables inside the gd scripts, or create your own script and attach it to the MeshInstance node.
