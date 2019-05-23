### BeeHash

This repository cointains the source code of a software that given the coordinates for a set of 3D points creates a data structure that finds all points that are nearer than a X distance from any other point, with pretty good performance. The coordinates of the datasets are in latitude, longitude and heigth above sea level.

Datasets are under *datasets* directory.

### Dependencies

* GCC compiler
* Boost headers. Usually, this is available on your linux distro repos.

### Compile and run

Compile using make. This will generate 2 executables. 

*beehash_all_times* will benchmark the *Parse_file* and the *find_collisions* functions from the data structure. It will run the code 100 times for each file, and return the mean, max and min time for each function.

*beehash_single_dataset_times* will do the same but only with the dataset which name should be provided via standard input.
