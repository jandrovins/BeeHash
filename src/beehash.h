#pragma once

class Bee {
public:
  /*Bee - construct a new Bee object
   *Parameters - x, y, and z coordinates of the bee
   *Example - Bee(-74.3445, 4.2353, 1707.44);
   */
  Bee(double bx, double by, double bz);
  /*Bee - construcs a new Bee object
   *Parameters - Reference to another bee object
   */
  Bee(const Bee& b);
  /*Instance variables
   *x - x coordinate of the bee in meters from the greenwich meridian stored as a double
   *y - y coordinate of the bee in meters from the equatorial line stored as a double
   *z - z coordinate of the bee in meters above see level stored as a double
   *previous - link to previous bee in 
   *following - link to following bee in
   */
  double x;
  double y;
  double z;
  Bee* previous = nullptr;
  Bee* following = nullptr;
};

class BeeList {
  /*concatenate
   */
  void concatenate_beelist_end(BeeList* bl);
};

class Cube {
  /*Cube - construct a new Cube obkect
   *Parameters - x, y, and z space distributions of space Cube takes up
   *Example - Cube(23, 56, 134);
   */
  Cube(int x, int y, int z);
  /*x, y, z - x, y, and z space of cube in meters stored as an ints
   */
  int x;
  int y;
  int z;
};

std::string find_cube_key(double x, double y, double x);

void parse_file(std::string input_file, std::vector<std::string>& v, boost::unordered_map<std::string, Pair>& um);

inline void find_for_unique_bee(std::string unique_bee_key, boost::unordered_map<std::string, Pair>& cubes);



