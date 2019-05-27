### BeeHash

This repository cointains the source code of a software that given the coordinates for a set of 3D points creates a data structure that finds all points that are nearer than a X distance from any other point, with pretty good performance. The coordinates of the datasets are in latitude, longitude and heigth above sea level.

Datasets are under *datasets* directory.

### Dependencies

* GCC compiler
* Boost headers. Usually, this is available on your linux distro repos.
* Point Cloud Library if you want to visualize the results

### Compile and run

Compile using make. This will generate several executables executables. 

* *beehash_all_times* will benchmark the *Parse_file* and the *find_collisions* functions from the data structure. It will run the code 100 times for each file, and return the mean, max and min time for each function.

* *beehash_single_dataset_times* will do the same but only with the dataset which name is be provided via standard input.

* *beehash* will generate a file with the coordinates of the points that surpass the threshold (100 meters).

* *create_pcd* will generate a .pcd file from the dataset as specified.

* *open3d_visualize* will generate a graph containing the points as specified in the .pcd file. It requires open3d python library to be installed. It requires open3d python library to be installed.
