#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <sys/stat.h>
#include <sys/wait.h>
#include "util.hpp"

using namespace std;


void masterproc(
    vector<string> resources,
    string biulding_name,
    vector<string> sub_dirs,
    string buildings_folder)
{



}

int main(int argc, char *argv[])
{

    if (argc != 2)
    {
        cerr << "your command should be fomatted like:\n ./main <buildings_directory>" << endl;
        return 0;
    }

    string buildings_folder = string(argv[1]);

    vector<string> input_resources = get_resources();
    string building_name = get_building_name();
    vector<string> sub_dirs = list_sub_dir(buildings_folder);

    for (auto &resource : input_resources)
    {
        cout << resource << endl;
    }
    cout << building_name << endl;
    for (auto &sub_dir : sub_dirs)
    {
        cout << sub_dir << endl;
    }

    masterproc(
        input_resources,
        building_name,
        sub_dirs,
        buildings_folder);


    return 1;
}