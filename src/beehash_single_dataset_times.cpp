#include <bits/stdc++.h>
#include <boost/algorithm/string.hpp>
#include <boost/unordered_map.hpp>
#include <chrono>
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <malloc.h>

class Bee {
public:
    double x, y, z;
    Bee* previous = nullptr;
    Bee* following = nullptr;

    Bee(double x1, double y1, double z1)
    {
        x = x1 * 111111;
        y = y1 * 111111;
        z = z1;
    };

    Bee(const Bee& b)
    {
        x = b.x;
        y = b.y;
        z = b.z;
    };
};

class BeeList {
public:
    Bee* first = nullptr;
    Bee* last = nullptr;
    int size = 0;

    void add_bee(Bee* b)
    {
        b->previous = nullptr;
        b->following = this->first;
        if (this->first != nullptr) {
            this->first->previous = b;
            this->last = this->first;
        }
        this->first = b;
        this->size++;
    }

    void add_first_bee(Bee* b)
    {
        b->following = nullptr;
        b->previous = nullptr;
        this->first = b;
        this->last = b;
        this->size++;
    }

    void concatenate_beelist_end(BeeList* bl)
    {
        if (this->size == 0) {
            this->first = bl->first;
            this->last = bl->last;
            this->size = bl->size;
        } else {
            bl->first->previous = this->last;
            last->following = bl->first;
            this->last = bl->last;
            this->size += bl->size;
        }
    }
};

typedef std::pair<BeeList, bool> Pair;
BeeList resultant_blist;

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

std::string find_cube_key(double x, double y, double z)
{
    // round each coordinate, concatenate them into a string and convert this string to an integer
    return ((((std::to_string((int)std::round(x / 57.735026919)) += " ") += std::to_string((int)std::round(y / 57.735026919))) += " ") += std::to_string((int)std::round(z / 57.735026919)));
}

inline void parse_file(std::string input_file, std::vector<std::string>& v, boost::unordered_map<std::string, Pair>& um)
{
    std::ifstream inpp(input_file);
    std::string line;
    std::getline(inpp, line);
    if (inpp.is_open()) {
        Pair* p;
        while (std::getline(inpp, line)) {
            std::vector<std::string> string_coordinates;

            //The data from the file is split and stored in the previously declared vector.
            boost::split(string_coordinates, line, boost::is_any_of(","));

            //A new bee is created using the extracted coordinates.
            Bee* c = new Bee(std::stod(string_coordinates[0]), std::stod(string_coordinates[1]), std::stod(string_coordinates[2]));

            //The coordinates assigned to the generated bee are used to generate a key in order to store said bee in the unordered map.
            std::string key = find_cube_key(c->x, c->y, c->z);
            p = &um[key];
            if ((p->first.size) == 0) {
                p->first.add_first_bee(c);
                p->second = false;
                v.push_back(key);
            } else {
                p->first.add_bee(c);
                p->second = true;
            }
        }
    }
    inpp.close();
}

inline bool compare_adjacent(std::string adj_key, std::string unique_bee_key, boost::unordered_map<std::string, Pair>& cubes, double x, double y, double z)
{

    Bee* current_bee;
    Pair* p;
    BeeList* blist;
    for (int i = 0; i < 26; ++i) {
        p = &cubes[adj_key];
        blist = &p->first;
        if (blist->first != nullptr) {
            current_bee = blist->first;
            double x_distance, y_distance, z_distance;
            while (current_bee != nullptr) {
                x_distance = x - current_bee->x;
                if (x_distance >= -100 && x_distance <= 100) {
                    y_distance = y - current_bee->y;
                    if (y_distance >= -100 && y_distance <= 100) {
                        z_distance = z - current_bee->z;
                        if (z_distance >= -100 && z_distance <= 100) {
                            if (p->second == true) {
                                cubes[unique_bee_key].second = true;
                                resultant_blist.concatenate_beelist_end(&cubes[unique_bee_key].first);
                                return true;
                            } else {
                                p->second = true;
                                cubes[unique_bee_key].second = true;
                                //resultant_blist.concatenate_beelist_end(blist);
                                resultant_blist.concatenate_beelist_end(&cubes[unique_bee_key].first);
                                return true;
                            }
                        }
                    }
                }
                current_bee = current_bee->following;
            }
        }
    }
    return false;
}

void find_for_unique_bee(std::string unique_bee_key, boost::unordered_map<std::string, Pair>& cubes)
{
    Bee* unique_bee = cubes[unique_bee_key].first.first;
    double x = unique_bee->x, y = unique_bee->y, z = unique_bee->z;
    std::vector<std::string> xyz_from_key;
    boost::split(xyz_from_key, unique_bee_key, boost::is_any_of(" "));
    int x_idx = stoi(xyz_from_key[0]), y_idx = stoi(xyz_from_key[1]), z_idx = stoi(xyz_from_key[2]);
    std::vector<std::string> keys;
    keys.reserve(32);
    keys.assign(32, "");

    if (compare_adjacent(((((std::to_string(x_idx + 1) += " ") += std::to_string(y_idx + 1)) += " ") += std::to_string(z_idx + 1)), unique_bee_key, cubes, x, y, z)) {
        return;
    }
    if (compare_adjacent(((((std::to_string(x_idx + 1) += " ") += std::to_string(y_idx + 1)) += " ") += std::to_string(z_idx)), unique_bee_key, cubes, x, y, z)) {
        return;
    };
    if (compare_adjacent(((((std::to_string(x_idx + 1) += " ") += std::to_string(y_idx + 1)) += " ") += std::to_string(z_idx - 1)), unique_bee_key, cubes, x, y, z)) {
        return;
    }

    if (compare_adjacent(((((std::to_string(x_idx + 1) += " ") += std::to_string(y_idx)) += " ") += std::to_string(z_idx + 1)), unique_bee_key, cubes, x, y, z)) {
        return;
    }
    if (compare_adjacent(((((std::to_string(x_idx + 1) += " ") += std::to_string(y_idx)) += " ") += std::to_string(z_idx)), unique_bee_key, cubes, x, y, z)) {
        return;
    }
    if (compare_adjacent(((((std::to_string(x_idx + 1) += " ") += std::to_string(y_idx)) += " ") += std::to_string(z_idx - 1)), unique_bee_key, cubes, x, y, z)) {
        return;
    }

    if (compare_adjacent(((((std::to_string(x_idx + 1) += " ") += std::to_string(y_idx - 1)) += " ") += std::to_string(z_idx + 1)), unique_bee_key, cubes, x, y, z)) {
        return;
    }
    if (compare_adjacent(((((std::to_string(x_idx + 1) += " ") += std::to_string(y_idx - 1)) += " ") += std::to_string(z_idx)), unique_bee_key, cubes, x, y, z)) {
        return;
    }
    if (compare_adjacent(((((std::to_string(x_idx + 1) += " ") += std::to_string(y_idx - 1)) += " ") += std::to_string(z_idx - 1)), unique_bee_key, cubes, x, y, z)) {
        return;
    }

    if (compare_adjacent(((((std::to_string(x_idx) += " ") += std::to_string(y_idx + 1)) += " ") += std::to_string(z_idx + 1)), unique_bee_key, cubes, x, y, z)) {
        return;
    }
    if (compare_adjacent(((((std::to_string(x_idx) += " ") += std::to_string(y_idx + 1)) += " ") += std::to_string(z_idx)), unique_bee_key, cubes, x, y, z)) {
        return;
    }
    if (compare_adjacent(((((std::to_string(x_idx) += " ") += std::to_string(y_idx + 1)) += " ") += std::to_string(z_idx - 1)), unique_bee_key, cubes, x, y, z)) {
        return;
    }

    if (compare_adjacent(((((std::to_string(x_idx) += " ") += std::to_string(y_idx)) += " ") += std::to_string(z_idx + 1)), unique_bee_key, cubes, x, y, z)) {
        return;
    }
    if (compare_adjacent(((((std::to_string(x_idx) += " ") += std::to_string(y_idx)) += " ") += std::to_string(z_idx - 1)), unique_bee_key, cubes, x, y, z)) {
        return;
    }

    if (compare_adjacent(((((std::to_string(x_idx) += " ") += std::to_string(y_idx - 1)) += " ") += std::to_string(z_idx + 1)), unique_bee_key, cubes, x, y, z)) {
        return;
    }
    if (compare_adjacent(((((std::to_string(x_idx) += " ") += std::to_string(y_idx - 1)) += " ") += std::to_string(z_idx)), unique_bee_key, cubes, x, y, z)) {
        return;
    }
    if (compare_adjacent(((((std::to_string(x_idx) += " ") += std::to_string(y_idx - 1)) += " ") += std::to_string(z_idx - 1)), unique_bee_key, cubes, x, y, z)) {
        return;
    }

    if (compare_adjacent(((((std::to_string(x_idx - 1) += " ") += std::to_string(y_idx + 1)) += " ") += std::to_string(z_idx + 1)), unique_bee_key, cubes, x, y, z)) {
        return;
    }
    if (compare_adjacent(((((std::to_string(x_idx - 1) += " ") += std::to_string(y_idx + 1)) += " ") += std::to_string(z_idx)), unique_bee_key, cubes, x, y, z)) {
        return;
    }
    if (compare_adjacent(((((std::to_string(x_idx - 1) += " ") += std::to_string(y_idx + 1)) += " ") += std::to_string(z_idx - 1)), unique_bee_key, cubes, x, y, z)) {
        return;
    }

    if (compare_adjacent(((((std::to_string(x_idx - 1) += " ") += std::to_string(y_idx)) += " ") += std::to_string(z_idx + 1)), unique_bee_key, cubes, x, y, z)) {
        return;
    }
    if (compare_adjacent(((((std::to_string(x_idx - 1) += " ") += std::to_string(y_idx)) += " ") += std::to_string(z_idx)), unique_bee_key, cubes, x, y, z)) {
        return;
    }
    if (compare_adjacent(((((std::to_string(x_idx - 1) += " ") += std::to_string(y_idx)) += " ") += std::to_string(z_idx - 1)), unique_bee_key, cubes, x, y, z)) {
        return;
    }

    if (compare_adjacent(((((std::to_string(x_idx - 1) += " ") += std::to_string(y_idx - 1)) += " ") += std::to_string(z_idx + 1)), unique_bee_key, cubes, x, y, z)) {
        return;
    }
    if (compare_adjacent(((((std::to_string(x_idx - 1) += " ") += std::to_string(y_idx - 1)) += " ") += std::to_string(z_idx)), unique_bee_key, cubes, x, y, z)) {
        return;
    }
    if (compare_adjacent(((((std::to_string(x_idx - 1) += " ") += std::to_string(y_idx - 1)) += " ") += std::to_string(z_idx - 1)), unique_bee_key, cubes, x, y, z)) {
        return;
    }

    if (compare_adjacent(((((std::to_string(x_idx) += " ") += std::to_string(y_idx)) += " ") += std::to_string(z_idx + 2)), unique_bee_key, cubes, x, y, z)) {
        return;
    }
    if (compare_adjacent(((((std::to_string(x_idx) += " ") += std::to_string(y_idx)) += " ") += std::to_string(z_idx - 2)), unique_bee_key, cubes, x, y, z)) {
        return;
    }

    if (compare_adjacent(((((std::to_string(x_idx) += " ") += std::to_string(y_idx + 2)) += " ") += std::to_string(z_idx)), unique_bee_key, cubes, x, y, z)) {
        return;
    }
    if (compare_adjacent(((((std::to_string(x_idx) += " ") += std::to_string(y_idx - 2)) += " ") += std::to_string(z_idx)), unique_bee_key, cubes, x, y, z)) {
        return;
    }

    if (compare_adjacent(((((std::to_string(x_idx + 2) += " ") += std::to_string(y_idx)) += " ") += std::to_string(z_idx)), unique_bee_key, cubes, x, y, z)) {
        return;
    }
    if (compare_adjacent(((((std::to_string(x_idx - 2) += " ") += std::to_string(y_idx)) += " ") += std::to_string(z_idx)), unique_bee_key, cubes, x, y, z)) {
        return;
    }
}

BeeList* tony;
double x, y, z;

int main()
{

        // map trepresent cubes
        boost::unordered_map<std::string, Pair> cubes;

        // vector to store keys and int representing how many bees are in each cube
        std::vector<std::string> keys;
        keys.reserve(500000);

        auto duration_mean = std::chrono::system_clock::duration::zero().count();
        auto duration_max = std::chrono::system_clock::duration::min().count();
        auto duration_min = std::chrono::system_clock::duration::max().count();
        std::string inFileName;
        std::cout<<"Enter file name: ";
        std::cin>>inFileName;
	inFileName = "../datasets/" + inFileName;
            auto st = std::chrono::high_resolution_clock::now();
            parse_file(inFileName, keys, cubes);

            auto stop1 = std::chrono::high_resolution_clock::now();
            auto dif = std::chrono::duration_cast<std::chrono::microseconds>(stop1 - st).count();
        std::cout << "PARSE_FILE - " << inFileName << " "
                  << "mean: " << dif << " microseconds" << std::endl;
        std::cout << "PARSE_FILE - " << inFileName << " "
                  << "max: " << dif << " microseconds" << std::endl;
        std::cout << "PARSE_FILE - " << inFileName << " "
                  << "min: " << dif << " microseconds" << std::endl;

        duration_mean = std::chrono::system_clock::duration::zero().count();
        duration_max = std::chrono::system_clock::duration::min().count();
        duration_min = std::chrono::system_clock::duration::max().count();
            st = std::chrono::high_resolution_clock::now();
            std::string key;
            Pair* p;
            for (int i = 0; i < keys.size(); i++) {
                key = keys[i];
                p = &cubes[key];
                tony = &p->first;
                if (p->second == true) {
                    resultant_blist.concatenate_beelist_end(tony);
                } else {
                    find_for_unique_bee(key, cubes);
                }
            }
             stop1 = std::chrono::high_resolution_clock::now();
            dif = std::chrono::duration_cast<std::chrono::microseconds>(stop1 - st).count();
        std::cout << "FIND_COLLISIONS - " << inFileName << " "
                  << "mean: " << dif << " microseconds" << std::endl;
        std::cout << "FIND_COLLISIONS - " << inFileName << " "
                  << "max: " << dif << " microseconds" << std::endl;
        std::cout << "FIND_COLLISIONS - " << inFileName << " "
                  << "min: " << dif << " microseconds" << std::endl;
}
