#include <boost/algorithm/string.hpp>
#include <boost/unordered_map.hpp>
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

class Bee {
public:
    float x, y, z;
    Bee* previous = nullptr;
    Bee* following = nullptr;

    Bee(float x1, float y1, float z1)
    {
        x = x1 * 111111;
        y = y1 * 111111;
        z = z1;
    };

    Bee(const Bee& b)
    {
        x = b.x * 111111;
        y = b.y * 111111;
    };
};

class Cube {
public:
    int x, y, z;
    Cube(int x, int y, int z)
    {
        this->x = x * 111111;
        this->y = y * 111111;
        this->z = z;
    }
};

std::string find_cube_key(float x, float y, float z)
{
    // round each coordinate, concatenate them into a string and convert this string to an integer
    return std::to_string((int)std::round(x / 57.735026919d)) + " " + std::to_string((int)std::round(y / 57.735026919d)) + " " + std::to_string((int)std::round(z / 57.735026919d));
}

boost::unordered_map<std::string, std::pair<Bee*, bool>> parse_file(std::string input_file, std::vector<std::string>& v)
{
    boost::unordered_map<std::string, std::pair<Bee*, bool>> um;
    std::ifstream inpp(input_file);
    std::string line;
    std::getline(inpp, line);
    if (inpp.is_open()) {
        while (std::getline(inpp, line)) {
            std::vector<std::string> string_coordinates;
            boost::split(string_coordinates, line, boost::is_any_of(","));
            Bee* c = new Bee(std::stof(string_coordinates[0]), std::stof(string_coordinates[1]), std::stof(string_coordinates[2]));
            std::string key = find_cube_key(c->x, c->y, c->z);
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
    std::vector<std::string> keys;

    // string storing name of file that will be parsed
    std::string inFileName = "ConjuntoDeDatosCon1000abejas.txt";
    boost::unordered_map<std::string, std::pair<Bee*, bool>> cubes = parse_file(inFileName, keys);
    std::stringstream s;

    std::vector<std::string> xyz_from_key;

    for (std::string i : keys) {
        std::pair<Bee*, bool> p = cubes[i];
        Bee* tony = p.first;
        if (p.second == true) {
            do {
                // s << tony->x << "," << tony->y << "," << tony->z << std::endl;
                tony = tony->following;
            } while (tony->following != nullptr);
        } else {
            boost::split(xyz_from_key, i, boost::is_any_of(" "));

        }
    }
    std::cout << s.str();
}
