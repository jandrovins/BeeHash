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
        first->previous = b;
        b->following = first;
        first = b;
        ++size;
    }

    void add_first_bee(Bee* b)
    {
        b->following = nullptr;
        b->previous = nullptr;
        first = b;
        last = b;
        ++size;
    }

    void concatenate_beelist_end(BeeList* bl)
    {
        if (first == nullptr) {
            first = bl->first;
            last = bl->last;
            return;
        }
        bl->first->previous = last;
        last->following = bl->first;
        last = bl->last;
        size += bl->size;
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
    //return std::to_string(std::round(x / 57.735026919)) + " " + std::to_string((int)std::round(y / 57.735026919)) + " " + std::to_string((int)std::round(z / 57.735026919));
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
            boost::split(string_coordinates, line, boost::is_any_of(","));
            Bee* c = new Bee(std::stod(string_coordinates[0]), std::stod(string_coordinates[1]), std::stod(string_coordinates[2]));
            std::string key = find_cube_key(c->x, c->y, c->z);
            p = &um[key];
            if ((p->first.first) == nullptr) {
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
    //#pragma omp parallel for private(current_bee, p, blist)
    for (int i = 0; i < 26; ++i) {
        p = &cubes[adj_key];
        blist = &p->first;
        if (blist->size != 0)
            //std::cout << blist->size << "\n";
            if (blist->first != nullptr) {
                current_bee = blist->first;
                double x_distance, y_distance, z_distance;
                //std::cout << current_bee->x << " " << current_bee->y << " " << current_bee->z << "\n";
                do {
                    x_distance = x - current_bee->x;
                    //std::cout << x_distance << "<-xdistance\n";
                    if (x_distance >= -100 && x_distance <= 100) {
                        y_distance = y - current_bee->y;
                        //std::cout << y_distance << "<-ydistance\n";
                        if (y_distance >= -100 && y_distance <= 100) {
                            z_distance = z - current_bee->z;
                            //std::cout << z_distance << "<-zdistance\n";
                            if (z_distance >= -100 && z_distance <= 100) {
                                if (p->second == true) {
                                    cubes[unique_bee_key].second = true;
                                    resultant_blist.concatenate_beelist_end(&cubes[unique_bee_key].first);
                                    //std::cout << "SECOND TRUEEEEEE\n";
                                    return true; //result.str();
                                } else {
                                    //std::cout << "SECOND FALSEEEE\n";
                                    p->second = true;
                                    cubes[unique_bee_key].second = true;
                                    resultant_blist.concatenate_beelist_end(blist);
                                    resultant_blist.concatenate_beelist_end(&cubes[unique_bee_key].first);
                                    return true; //result.str();
                                }
                            }
                        }
                    }
                } while (current_bee->following != nullptr && current_bee != blist->first);
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
    if(compare_adjacent(((((std::to_string(x_idx + 1) += " ") += std::to_string(y_idx + 1)) += " ") += std::to_string(z_idx - 1)),unique_bee_key,cubes,x,y,z)){
        return;
    }

    if(compare_adjacent(((((std::to_string(x_idx + 1) += " ") += std::to_string(y_idx)) += " ") += std::to_string(z_idx + 1)),unique_bee_key,cubes,x,y,z)){
        return;
    }
    if(compare_adjacent(((((std::to_string(x_idx + 1) += " ") += std::to_string(y_idx)) += " ") += std::to_string(z_idx)),unique_bee_key,cubes,x,y,z)){
        return;
    }
    if(compare_adjacent(((((std::to_string(x_idx + 1) += " ") += std::to_string(y_idx)) += " ") += std::to_string(z_idx - 1)),unique_bee_key,cubes,x,y,z)){
        return;
    }

    if(compare_adjacent(((((std::to_string(x_idx + 1) += " ") += std::to_string(y_idx - 1)) += " ") += std::to_string(z_idx + 1)),unique_bee_key,cubes,x,y,z)){
        return;
    }
    if(compare_adjacent(((((std::to_string(x_idx + 1) += " ") += std::to_string(y_idx - 1)) += " ") += std::to_string(z_idx)),unique_bee_key,cubes,x,y,z)){
        return;
    }
    if(compare_adjacent(((((std::to_string(x_idx + 1) += " ") += std::to_string(y_idx - 1)) += " ") += std::to_string(z_idx - 1)),unique_bee_key,cubes,x,y,z)){
        return;
    }

    if(compare_adjacent(((((std::to_string(x_idx) += " ") += std::to_string(y_idx + 1)) += " ") += std::to_string(z_idx + 1)),unique_bee_key,cubes,x,y,z)){
        return;
    }
    if(compare_adjacent(((((std::to_string(x_idx) += " ") += std::to_string(y_idx + 1)) += " ") += std::to_string(z_idx)),unique_bee_key,cubes,x,y,z)){
        return;
    }
    if(compare_adjacent(((((std::to_string(x_idx) += " ") += std::to_string(y_idx + 1)) += " ") += std::to_string(z_idx - 1)),unique_bee_key,cubes,x,y,z)){
        return;
    }

    if(compare_adjacent(((((std::to_string(x_idx) += " ") += std::to_string(y_idx)) += " ") += std::to_string(z_idx + 1)),unique_bee_key,cubes,x,y,z)){
        return;
    }
    if(compare_adjacent(((((std::to_string(x_idx) += " ") += std::to_string(y_idx)) += " ") += std::to_string(z_idx - 1)),unique_bee_key,cubes,x,y,z)){
        return;
    }

    if(compare_adjacent(((((std::to_string(x_idx) += " ") += std::to_string(y_idx - 1)) += " ") += std::to_string(z_idx + 1)),unique_bee_key,cubes,x,y,z)){
        return;
    }
    if(compare_adjacent(((((std::to_string(x_idx) += " ") += std::to_string(y_idx - 1)) += " ") += std::to_string(z_idx)),unique_bee_key,cubes,x,y,z)){
        return;
    }
    if(compare_adjacent(((((std::to_string(x_idx) += " ") += std::to_string(y_idx - 1)) += " ") += std::to_string(z_idx - 1)),unique_bee_key,cubes,x,y,z)){
        return;
    }

    if(compare_adjacent(((((std::to_string(x_idx - 1) += " ") += std::to_string(y_idx + 1)) += " ") += std::to_string(z_idx + 1)),unique_bee_key,cubes,x,y,z)){
        return;
    }
    if(compare_adjacent(((((std::to_string(x_idx - 1) += " ") += std::to_string(y_idx + 1)) += " ") += std::to_string(z_idx)),unique_bee_key,cubes,x,y,z)){
        return;
    }
    if(compare_adjacent(((((std::to_string(x_idx - 1) += " ") += std::to_string(y_idx + 1)) += " ") += std::to_string(z_idx - 1)),unique_bee_key,cubes,x,y,z)){
        return;
    }

    if(compare_adjacent(((((std::to_string(x_idx - 1) += " ") += std::to_string(y_idx)) += " ") += std::to_string(z_idx + 1)),unique_bee_key,cubes,x,y,z)){
        return;
    }
    if(compare_adjacent(((((std::to_string(x_idx - 1) += " ") += std::to_string(y_idx)) += " ") += std::to_string(z_idx)),unique_bee_key,cubes,x,y,z)){
        return;
    }
    if(compare_adjacent(((((std::to_string(x_idx - 1) += " ") += std::to_string(y_idx)) += " ") += std::to_string(z_idx - 1)),unique_bee_key,cubes,x,y,z)){
        return;
    }

    if(compare_adjacent(((((std::to_string(x_idx - 1) += " ") += std::to_string(y_idx - 1)) += " ") += std::to_string(z_idx + 1)),unique_bee_key,cubes,x,y,z)){
        return;
    }
    if(compare_adjacent(((((std::to_string(x_idx - 1) += " ") += std::to_string(y_idx - 1)) += " ") += std::to_string(z_idx)),unique_bee_key,cubes,x,y,z)){
        return;
    }
    if(compare_adjacent(((((std::to_string(x_idx - 1) += " ") += std::to_string(y_idx - 1)) += " ") += std::to_string(z_idx - 1)),unique_bee_key,cubes,x,y,z)){
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

    // string storing name of file that will be parsed
    std::string inFileName = "ConjuntoDeDatosCon1000000abejas.txt";

    parse_file(inFileName, keys, cubes);

    auto st = std::chrono::high_resolution_clock::now();
    clock_t start, end;
    start = std::clock();
    std::string key;
    Pair* p;
#pragma omp parallel for private(key, p, tony)
    for (int i = 0; i < keys.size(); i++) {
        key = keys[i];
        p = &cubes[key];
        tony = &p->first;
        if (p->second == true) {
            resultant_blist.concatenate_beelist_end(tony);
        } else {
            //std::cout << "UNICASSSS\n\n\n";
            find_for_unique_bee(key, cubes);
        }
    }
    end = std::clock();
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - st);

    std::cout << "CHRONO: " << duration.count() << "\n";
    //std::cout << s.str();
    double time_taken = double(end - start) / double(CLOCKS_PER_SEC);
    std::cout << "CLOCK: " << time_taken << "\n";
    //std::cout << s.str() << "\n";
}
