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

std::stringstream s;
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
        z = b.z;
    };
};

typedef std::pair<Bee*, bool> Pair;

class Cube {
public:
    int x, y, z;
    Cube(int x, int y, int z)
    {
        this->x = x;
        this->y = y;
        this->z = z;
    };
    Cube()
    {
        x = 0;
        y = 0;
        z = 0;
    };
    ~Cube() {};

    bool operator==(const Cube& rhs) const
    {
        return this->x == rhs.x && this->y == rhs.y && this->z == rhs.z;
    };
};

template <class T>
class my_allocator {
public:
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;
    typedef T* pointer;
    typedef const T* const_pointer;
    typedef T& reference;
    typedef const T& const_reference;
    typedef T value_type;

    my_allocator() {}
    ~my_allocator() {}
    my_allocator(const my_allocator&) {}

    pointer allocate(size_type n, const void* = 0)
    {
        T* t = (T*)malloc(n * sizeof(T));
        return t;
    }

    void deallocate(void* p, size_type)
    {
        if (p) {
            free(p);
        }
    }

    pointer address(reference x) const { return &x; }
    const_pointer address(const_reference x) const { return &x; }

    my_allocator<T>& operator=(const my_allocator&) { return *this; }

    void construct(pointer p, const T& val)
    {
        new ((T*)p) T(val);
    }
    void destroy(pointer p) { p->~T(); }

    size_type max_size() const { return size_t(-1); }

    template <class U>
    struct rebind {
        typedef my_allocator<U> other;
    };

    template <class U>
    my_allocator(const my_allocator<U>&) {}

    template <class U>
    my_allocator& operator=(const my_allocator<U>&) { return *this; }
};

class cube_hasher {
public:
    std::size_t operator()(const Cube& a) const
    {
        std::size_t h = 0;

        int e = a.x;
        h ^= std::hash<int> {}(e) + 0x9e3779b9 + (h << 6) + (h >> 2);

        e = a.y;
        h ^= std::hash<int> {}(e) + 0x9e3779b9 + (h << 6) + (h >> 2);

        e = a.y;
        h ^= std::hash<int> {}(e) + 0x9e3779b9 + (h << 6) + (h >> 2);

        return h;
    };
};

class cube_is_equal {
public:
    bool operator()(const Cube& a, const Cube& b) const
    {
        return a.x == b.x && a.y == b.y && a.z == b.z;
    }
};

inline Cube find_cube_key(float x, float y, float z)
{
    // round each coordinate, concatenate them into a string and convert this string to an integer
    return Cube(std::round(x / 57.735026919), std::round(y / 57.735026919), std::round(z / 57.735026919));
}

inline void split(std::string s, std::vector<std::string>& v)
{
    size_t idx_2 = 0, idx = 0;
    v.push_back("");
    v.push_back("");
    v.push_back("");
    for (char ch : s) {
        if (ch == '\n')
            return;
        if (ch == ',' || ch == ' ') {
            ++idx_2;
            continue;
        }
        v[idx_2] += ch;
    }
}

void parse_file(std::string input_file, std::vector<Cube>& v, boost::unordered_map<Cube, Pair, cube_hasher, cube_is_equal, my_allocator<Cube>>& um)
{
    std::ifstream inpp(input_file);
    std::string line; //cambiar por stringstream para ver si es más rápida la conversion a entero cuando se define el puntero a Bee unas líneas más abajo!!!!!!!!!!!!!!! line
    std::stringstream iss;
    iss << inpp.rdbuf();
    std::getline(iss, line);
    Cube key;
    Bee* c;
    while (std::getline(iss, line)) {
        std::vector<std::string> string_coordinates;
        split(line, string_coordinates);
        c = new Bee(std::stof(string_coordinates[0]), std::stof(string_coordinates[1]), std::stof(string_coordinates[2]));
        key = find_cube_key(c->x, c->y, c->z);
        if ((um[key].first) == nullptr) {
            um[key].first = c;
            um[key].second = false;
            v.push_back(key);
        } else {
            um[key].second = true;
            um[key].first->previous = c;
            c->following = um[key].first;
            um[key].first = c;
        }
    }
}

inline void find_for_unique_bee(Cube unique_bee_key, boost::unordered_map<Cube, Pair, cube_hasher, cube_is_equal, my_allocator<Cube>>& cubes)
{
    float x = cubes[unique_bee_key].first->x, y = cubes[unique_bee_key].first->y, z = cubes[unique_bee_key].first->z;

    std::vector<int[4]> keys;
    keys.reserve(32);

    keys.push_back({ unique_bee_key.x + 1, unique_bee_key.y + 1, unique_bee_key.z + 1 });
    keys.push_back({ unique_bee_key.x + 1, unique_bee_key.y + 1, unique_bee_key.z });
    keys.push_back({ unique_bee_key.x + 1, unique_bee_key.y + 1, unique_bee_key.z - 1 });

    keys.push_back({ unique_bee_key.x + 1, unique_bee_key.y, unique_bee_key.z + 1 });
    keys.push_back({ unique_bee_key.x + 1, unique_bee_key.y, unique_bee_key.z });
    keys.push_back({ unique_bee_key.x + 1, unique_bee_key.y, unique_bee_key.z - 1 });

    keys.push_back({ unique_bee_key.x + 1, unique_bee_key.y - 1, unique_bee_key.z + 1 });
    keys.push_back({ unique_bee_key.x + 1, unique_bee_key.y - 1, unique_bee_key.z });
    keys.push_back({ unique_bee_key.x + 1, unique_bee_key.y - 1, unique_bee_key.z - 1 });

    keys.push_back({ unique_bee_key.x, unique_bee_key.y + 1, unique_bee_key.z + 1 });
    keys.push_back({ unique_bee_key.x, unique_bee_key.y + 1, unique_bee_key.z });
    keys.push_back({ unique_bee_key.x, unique_bee_key.y + 1, unique_bee_key.z - 1 });

    keys.push_back({ unique_bee_key.x, unique_bee_key.y, unique_bee_key.z + 1 });
    keys.push_back({ unique_bee_key.x, unique_bee_key.y, unique_bee_key.z - 1 });

    keys.push_back({ unique_bee_key.x, unique_bee_key.y - 1, unique_bee_key.z + 1 });
    keys.push_back({ unique_bee_key.x, unique_bee_key.y - 1, unique_bee_key.z });
    keys.push_back({ unique_bee_key.x, unique_bee_key.y - 1, unique_bee_key.z - 1 });

    keys.push_back({ unique_bee_key.x - 1, unique_bee_key.y + 1, unique_bee_key.z + 1 });
    keys.push_back({ unique_bee_key.x - 1, unique_bee_key.y + 1, unique_bee_key.z });
    keys.push_back({ unique_bee_key.x - 1, unique_bee_key.y + 1, unique_bee_key.z - 1 });

    keys.push_back({ unique_bee_key.x - 1, unique_bee_key.y, unique_bee_key.z + 1 });
    keys.push_back({ unique_bee_key.x - 1, unique_bee_key.y, unique_bee_key.z });
    keys.push_back({ unique_bee_key.x - 1, unique_bee_key.y, unique_bee_key.z - 1 });

    keys.push_back({ unique_bee_key.x - 1, unique_bee_key.y - 1, unique_bee_key.z + 1 });
    keys.push_back({ unique_bee_key.x - 1, unique_bee_key.y - 1, unique_bee_key.z });
    keys.push_back({ unique_bee_key.x - 1, unique_bee_key.y - 1, unique_bee_key.z - 1 });

    Bee* current_bee;
    Pair p;
    for (int i = 0; i < 26; ++i) {
        p = cubes[Cube(keys[i][0], keys[i][1], keys[i][2])];
        if (p.first != nullptr) {
            current_bee = p.first;
            float x_distance, y_distance, z_distance;
            int cont = 0;
            do {
                x_distance = x - current_bee->x;
                y_distance = y - current_bee->y;
                z_distance = z - current_bee->z;
                ++cont;
                std::cout << cont << "\n";
                if (x_distance >= -100 && x_distance <= 100) {
                    if (y_distance >= -100 && y_distance <= 100) {
                        if (z_distance >= -100 && z_distance <= 100) {
                            if (p.second == true) {
                                //result << x << "," << y << "," << z << "\n";
                                cubes[unique_bee_key].second = true;
                                return; //result.str();
                            } else {
                                // result << current_bee->x << "," << current_bee->y << "," << current_bee->z << "\n";
                                //result << x << "," << y << "," << z << "\n";
                                p.second = true;
                                cubes[unique_bee_key].second = true;
                                return; //result.str();
                            }
                        }
                    }
                }
            } while (current_bee->following != nullptr);
        }
    }
    //return result.str();
}

Bee* tony;
float x, y, z;

int main()
{
    // map trepresent cubes
    boost::unordered_map<Cube, Pair, cube_hasher, cube_is_equal, my_allocator<Cube>> cubes;

    // vector to store keys and int representing how many bees are in each cube
    std::vector<Cube> keys;

    // string storing name of file that will be parsed
    std::string inFileName = "ConjuntoDeDatosCon1500000abejas.txt";

    parse_file(inFileName, keys, cubes);

    std::cout << keys.size();
    auto st = std::chrono::high_resolution_clock::now();
    clock_t start, end;
    start = std::clock();
    Cube key;
    Pair p;
    //#pragma omp parallel for private(key, p, tony)
    for (int i = 0; i < keys.size(); i++) {
        key = keys[i];
        p = cubes[key];
        tony = p.first;
        if (p.second == true) {
            while (tony->following != nullptr) {
               // s << tony->x << "," << tony->y << "," << tony->z << "\n";
                tony = tony->following;
            }
        } else {
            find_for_unique_bee(key, cubes);
        }
    }
    end = std::clock();
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - st);

    std::cout <<"CHRONO: " << duration.count() << "\n";
    //std::cout << s.str();
    double time_taken = double(end - start) / double(CLOCKS_PER_SEC);
    std::cout <<"CLOCK: " << time_taken << "\n";
    //std::cout << s.str() << "\n";
}
