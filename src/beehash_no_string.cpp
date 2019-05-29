#include <boost/algorithm/string.hpp>
#include <boost/unordered_map.hpp>
#include <chrono>
#include <cmath>
#include <fstream>
#include <iostream>
#include <malloc.h>
#include <sstream>
#include <string>
#include <vector>

class Bee {
public:
    double x, y, z;
    Bee* previous = nullptr;
    Bee* following = nullptr;
    int cube_x, cube_y, cube_z;

    Bee(double x1, double y1, double z1)
    {
        x = x1 * 111111;
        y = y1 * 111111;
        z = z1;

        cube_x = std::round(x / 57.735026919);
        cube_y = std::round(y / 57.735026919);
        cube_z = std::round(z / 57.735026919);
    };

    Bee(const Bee& b)
    {
        x = b.x;
        y = b.y;
        z = b.z;

        cube_x = b.cube_x;
        cube_y = b.cube_y;
        cube_z = b.cube_z;
    };

    bool operator==(Bee const& other) const
    {
        return x == other.x && y == other.y && z == other.z;
    }

    friend std::size_t hash_value(Bee const& p)
    {
        std::size_t seed = 0;

        boost::hash_combine(seed, p.cube_x);
        boost::hash_combine(seed, p.cube_y);
        boost::hash_combine(seed, p.cube_z);

        return seed;
    }
};

class BeeList {
public:
    Bee* first = nullptr;
    Bee* last = nullptr;
    bool visited = false;
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
        this->size = 1;
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

    void display()
    {
        Bee* bee = first;
        std::stringstream ss;
        while (bee != nullptr) {
            ss << bee->x << " " << bee->y << " " << bee->z << "\n";
            bee = bee->following;
        }
        std::cout << ss.str();
    }
};
BeeList resultant_blist;
boost::hash<Bee> bee_hasher;

inline void parse_file(std::string input_file, std::vector<std::size_t>& v, boost::unordered_map<std::size_t, BeeList>& um)
{
    std::ifstream inpp(input_file);
    std::string line;
    std::getline(inpp, line);
    if (inpp.is_open()) {
        BeeList* bl;
        while (std::getline(inpp, line)) {
            std::vector<std::string> string_coordinates;

            //The data from the file is split and stored in the previously declared vector.
            boost::split(string_coordinates, line, boost::is_any_of(","));

            //A new bee is created using the extracted coordinates.
            Bee* c = new Bee(std::stod(string_coordinates[0]), std::stod(string_coordinates[1]), std::stod(string_coordinates[2]));

            std::size_t key = bee_hasher(*c);
            bl = &um[key];
            if ((bl->size) == 0) {
                bl->add_first_bee(c);
                bl->visited = false;
                v.push_back(key);
            } else {
                bl->add_bee(c);
                bl->visited = true;
            }
        }
    }
    inpp.close();
}

inline bool compare_adjacent(std::size_t adj_key, std::size_t unique_bee_key, boost::unordered_map<std::size_t, BeeList>& cubes, double x, double y, double z)
{

    Bee* current_bee;
    BeeList* blist = &cubes[adj_key];
    if (blist->size != 0) {
        current_bee = blist->first;
        double x_distance, y_distance, z_distance;
        while (current_bee != nullptr) {
            x_distance = x - current_bee->x;
            if (x_distance >= -100 && x_distance <= 100) {
                y_distance = y - current_bee->y;
                if (y_distance >= -100 && y_distance <= 100) {
                    z_distance = z - current_bee->z;
                    if (z_distance >= -100 && z_distance <= 100) {
                        if (blist->visited == true) {
                            cubes[unique_bee_key].visited = true;
                            resultant_blist.concatenate_beelist_end(&cubes[unique_bee_key]);
                            return true;
                        } else {
                            blist->visited = true;
                            cubes[unique_bee_key].visited = true;
                            //resultant_blist.concatenate_beelist_end(blist);
                            resultant_blist.concatenate_beelist_end(&cubes[unique_bee_key]);
                            return true;
                        }
                    }
                }
            }
            current_bee = current_bee->following;
        }
    }
    return false;
}

inline void find_for_unique_bee(std::size_t unique_bee_key, boost::unordered_map<std::size_t, BeeList>& cubes)
{
    Bee* unique_bee = cubes[unique_bee_key].first;
    double x = unique_bee->x, y = unique_bee->y, z = unique_bee->z;

    Bee neighbor_bee = *unique_bee;
    size_t neighbor_key;
    //std::cout<<unique_bee->cube_x << " " << unique_bee->cube_y << " " << unique_bee -> cube_z << " " << unique_bee_key << std::endl;
    //std::cout<<neighbor_bee.cube_x << " " << neighbor_bee.cube_y << " " << neighbor_bee.cube_z << " " << unique_bee_key << std::endl;

    //std::cout << "\n\n\n\n";
    for (int i = -1; i <= 1; ++i) {
        neighbor_bee.cube_x = unique_bee->cube_x;
        neighbor_bee.cube_x += i;
        for (int j = -1; j <= 1; ++j) {
            neighbor_bee.cube_y = unique_bee->cube_y;
            neighbor_bee.cube_y += j;
            for (int k = -1; k <= 1; ++k) {
                neighbor_bee.cube_z = unique_bee->cube_z;
                neighbor_bee.cube_z += k;
                neighbor_key = bee_hasher(neighbor_bee);
                if (neighbor_key == unique_bee_key)
                    continue;
                //std::cout << neighbor_bee.cube_x << " " << neighbor_bee.cube_y << " " << neighbor_bee.cube_z<<" -> " <<  neighbor_key << std::endl;
                compare_adjacent(neighbor_key, unique_bee_key, cubes, x, y, z);
            }
        }
    }
}

BeeList* tony;
double x, y, z;

int main()
{

    boost::unordered_map<std::size_t, BeeList> cubes;

    //                  TESTS hash function
    //Bee a(1, 1, 1);
    //Bee b = a;
    //Bee c(1, 2, 1);
    //Bee d = c;
    //Bee e(1, 2, 3);
    //Bee f(2, 2, 2);
    //Bee g = f;

    //boost::hash<Bee> bee_hasher;
    //lstd::cout << bee_hasher(a)<< std::endl;
    //lstd::cout << bee_hasher(b)<< std::endl;
    //lstd::cout << bee_hasher(c)<< std::endl;
    //lstd::cout << bee_hasher(d)<< std::endl;
    //lstd::cout << bee_hasher(e)<< std::endl;
    //lstd::cout << bee_hasher(f)<< std::endl;
    //lstd::cout << bee_hasher(g)<< std::endl;
    //assert(bee_hasher(a) == bee_hasher(b));
    //assert(bee_hasher(c) == bee_hasher(d));
    //assert(bee_hasher(f) == bee_hasher(g));
    //
    //assert(bee_hasher(a) != bee_hasher(c));
    //assert(bee_hasher(c) != bee_hasher(b));
    //assert(bee_hasher(e) != bee_hasher(f));

    std::string inFileName, nbees;
    std::cout << "Enter the number of bees: ";
    std::cin >> nbees;
    inFileName = "../datasets/" + nbees + "beesSet.csv";

    std::vector<std::size_t> keys;
    keys.reserve(700000);

    parse_file(inFileName, keys, cubes);

    std::size_t key;
    BeeList* tony;
    for (int i = 0; i < keys.size(); i++) {
        key = keys[i];
        tony = &cubes[key];
        if (tony->visited == true) {
            resultant_blist.concatenate_beelist_end(tony);
        } else {
            find_for_unique_bee(key, cubes);
        }
    }
    Bee* b = resultant_blist.first;
    std::ofstream outFile(nbees + "out.csv");
    std::stringstream s;
    while (b != nullptr) {
        s << b->x / 111111 << "," << b->y / 111111 << "," << b->z << std::endl;
        b = b->following;
    }
    outFile << s.rdbuf();
    outFile.close();
}
