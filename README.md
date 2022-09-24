# Self-avoiding walk
Path on a grid that visits every point, that doesn't cross itself (visits the same point twice) in a set and confined space.


## Preview
![Preview](https://raw.githubusercontent.com/Pomoc48/self-avoiding-3d-walk/master/assets/screenshot.png)
Example of a 4x4x4 3d self-avoiding walk.


## Finding the correct path
Number of possibilities for finding the correct path for every next dimension increases exponentially.
Highest I was able to generate was a 5x5x5, which took about 45 mins with a script runing on every cpu thread.


Create an executable for finding valid paths:

```bash
g++ -o Calculate.cpp -o calc -O3
chmod +x calc
```

You can specifiy desired `const int dimensions` in the script file.

Running the script:

```bash
./find.sh
```

Specify the number of cpu cores you want to use inside the file.


The script will run multiple instances at once and exit when any of them find the correct path.
<br/>
Results will be put in the newly created output.txt file.


## Play the animation
Import this repository as a [Godot Game Engine](https://godotengine.org/) project, open it and press F5.


You can play your own set of script-generated moves by changing the `_path` and `_box_dimensions` variables inside the gd scripts, or create your own script and attach it to the MeshInstance node.
