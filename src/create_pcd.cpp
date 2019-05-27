#include <boost/algorithm/string.hpp>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

string GetStdoutFromCommand(string cmd)
{

    string data;
    FILE* stream;
    const int max_buffer = 256;
    char buffer[max_buffer];
    cmd.append(" 2>&1");

    stream = popen(cmd.c_str(), "r");
    if (stream) {
        while (!feof(stream))
            if (fgets(buffer, max_buffer, stream) != NULL)
                data.append(buffer);
        pclose(stream);
    }
    return data;
}

int main(int argc, char** argv)
{
    string outFile, inFile, line, nbees, mode, rgb, inFile_length;
    stringstream oss;
    if (argc != 4) {
        cerr << "You should enter 2 parameters: <nbees> <in | out> <rgb>" << endl;
        return -1;
    }
    nbees = argv[1];
    mode = argv[2];
    rgb = argv[3];

    if (mode == "in") {
        inFile = "../datasets/" + nbees + "beesSet.csv";
        outFile = nbees + "in.pcd";
    } else if (mode == "out") {
        inFile = nbees + "out.csv";
        outFile = nbees + "out.pcd";
    }

    ofstream outf(outFile);
    ifstream inf(inFile);

    if (mode == "in") {
        getline(inf, line);
        inFile_length = nbees;
    } else {
        string wc = GetStdoutFromCommand("wc -l " + inFile);
        vector<string> wc_vec;
        boost::split(wc_vec, wc, boost::is_any_of(" "));
        inFile_length = wc_vec[0];
    }

    oss << "# .PCD v.7 - Point Cloud Data file format" << endl
        << "VERSION .7" << endl
        << "FIELDS x y z rgb" << endl
        << "SIZE 4 4 4 4" << endl
        << "TYPE F F F F" << endl
        << "WIDTH " + inFile_length << endl
        << "HEIGHT 1" << endl
        << "VIEWPOINT 0 0 0 1 0 0 0" << endl
        << "POINTS " + inFile_length << endl
        << "DATA ascii" << endl;

    while (getline(inf, line)) {
        std::vector<std::string> string_coordinates;

        boost::split(string_coordinates, line, boost::is_any_of(","));

        float x = std::stof(string_coordinates[0]), y = std::stof(string_coordinates[1]), z = std::stof(string_coordinates[2]);
        oss << x * 111111 << " " << y * 111111 << " " << z << " " << rgb << endl;
    }
    outf << oss.str();
}
