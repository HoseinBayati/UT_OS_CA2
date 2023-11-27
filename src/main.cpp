#include <iostream>
#include <string>
#include <vector>
#include <sstream>

using namespace std;

vector<string> split_string(const char *buffer, 
                            char delim)
{
    vector<string> vec;
    string s(buffer), temp;
    stringstream ss;
    ss << s;
    while (std::getline(ss, temp, delim))
    {
        vec.push_back(temp);
    }
    return vec;
}

vector<string> get_resources()
{
    string s;
    cout << "Enter resources to get stats :" << endl;
    std::getline(cin, s);
    return split_string(s.c_str(), ' ');
}

string get_building_name()
{

}

vector<string> list_sub_dir(string path)
{

}

void masterproc(vector<string> bureaus,
                string biulding_name,
                vector<string> sub_dirs,
                string buildings_folder)
{
    
}

int main(int argc, char *argv[])
{

    if (argc != 2)
    {
        cerr << "your command should be fomatted like:\n ./main <resource> <resource> ..." << endl;
        return 0;
    }

    string buildings_folder = string(argv[1]);

    masterproc(
        get_resources(),
        get_building_name(),
        list_sub_dir(buildings_folder),
        buildings_folder);

    return 1;
}