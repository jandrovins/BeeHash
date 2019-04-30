#include <boost/algorithm/string.hpp>
#include <boost/unordered_map.hpp>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

class Bee {
public:
    float x, y, z;
    Bee(float x1, float y1, float z1)
    {
        x = x1;
        y = y1;
        z = z1;
    };

    Bee(const Bee& b)
    {
        x = b.x;
        y = b.y;
    };
};

class Cube {
public:
    int x, y, z;
    Cube* previous = nullptr;
    Cube* following = nullptr;

    Cube(int x, int y, int z)
    {
        this->x = x;
        this->y = y;
        this->z = z;
    }
};

class CubeKey {
public:
    int x, y, z, counter;

    CubeKey(int x1, int y1, int z1)
    {
        x = x1;
        y = y1;
        z = z1;
        counter = 0;
    }

    CubeKey(int x1, int y1, int z1, int counter)
    {
        {
            x = x1;
            y = y1;
            z = z1;
            counter = counter;
        }
    }
};

int find_cube_key(Cube* c)
{
    // round each coordinate, concatenate them into a string anf convert this string to a integer
    return std::stoi(std::to_string(std::round(c->x / 57.735026919d)) + std::to_string(std::round(c->y / 57.735026919d)) + std::to_string(round(c->z / 57.735026919d)));
}

boost::unordered_map<int, std::pair<Cube*, bool>> parse_file(std::string input_file, std::vector<int>& v)
{
    boost::unordered_map<int, std::pair<Cube*, bool>> um;
    std::ifstream inpp(input_file);
    std::string line;
    std::getline(inpp, line);
    if (inpp.is_open()) {
        while (std::getline(inpp, line)) {
            std::vector<std::string> string_coordinates;
            boost::split(string_coordinates, line, boost::is_any_of(","));
            Cube* c = new Cube(std::stoi(string_coordinates[0]), std::stoi(string_coordinates[1]), std::stoi(string_coordinates[2]));
            int key = find_cube_key(c);
            if ((um[key].first) == nullptr) {
                um[key] = std::make_pair(c, false);
                v.push_back(key);
            } else {
                um[key].second = true;
                um[key].first->previous = c;
                c->following = um[key].first;
                um[key].first = c;
            }
        }
    }
    inpp.close();
    return um;
}

int main()
{
    // map trepresent cubes

    // vector to store keys and int representing how many bees are in each cube
    std::vector<int> keys;

    // string storing name of file that will be parsed
    std::string inFileName = "ConjuntoDeDatosCon1000abejas.txt";
    boost::unordered_map<int, std::pair<Cube*, bool>> cubes = parse_file(inFileName, keys);

    for (int i : keys) {
        Cube* tony = cubes[i].first;
        do {
            std::cout << tony->x << "  " << tony->y << "  " << tony->z << std::endl;
            tony = tony->following;
        } while (tony->following != nullptr);
    }
}
