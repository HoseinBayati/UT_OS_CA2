#include <iostream>
// #include <sys/stat.h>
#include <sys/wait.h>
#include <fstream>
#include "../include/util.hpp"
#include "../include/constants.hpp"

using namespace std;

void read_file(
    string file_dir,
    int (&consumption_info)[MONTHS_COUNT][DAYS_COUNT][HOURS_COUNT])
{
    fstream fin;
    string line;
    vector<string> words;
    int hour_0_column = 3;

    cout << "read from: " << file_dir << endl;

    fin.open(file_dir, ios::in);
    if (!fin.is_open())
    {
        cout << "file not open" << endl;
        return;
    }

    getline(fin, line);
    for (int i = 0; i < MONTHS_COUNT; i++)
    {
        for (int j = 0; j < DAYS_COUNT; j++)
        {
            getline(fin, line);
            words = split_string(line.c_str(), ',');
            for (int k = 0; k < HOURS_COUNT; k++)
            {
                consumption_info[i][j][k] = atoi(words[hour_0_column + k].c_str());
            }
        }
    }
}

int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        perror("didn't receive the port fd's\n");
        exit(EXIT_FAILURE);
    }

    int write_fd = atoi(argv[1]);
    int read_fd = atoi(argv[2]);
    string building_dir = argv[3];

    char buffer[50];
    read(read_fd, buffer, sizeof(buffer));

    vector<string> target_resources = split_string(buffer, ' ');

    cout << "Building received: " << building_dir << endl;

    int g_consumption[MONTHS_COUNT][DAYS_COUNT][HOURS_COUNT];
    int w_consumption[MONTHS_COUNT][DAYS_COUNT][HOURS_COUNT];
    int e_consumption[MONTHS_COUNT][DAYS_COUNT][HOURS_COUNT];

    for (auto &resource_key : target_resources)
    {
        if (resource_key == "G")
        {
            string file_dir = building_dir + GAS_FILE;
            read_file(file_dir, g_consumption);
        }
        else if (resource_key == "W")
        {
            string file_dir = building_dir + WATER_FILE;
            read_file(file_dir, w_consumption);
        }
        else if (resource_key == "E")
        {
            string file_dir = building_dir + ELECTRICITY_FILE;
            read_file(file_dir, e_consumption);
        }
        else
        {
            cout << "invalid resource key" << endl;
        }
    }

    // for (int i = 0; i < MONTHS_COUNT; i++)
    // {
    //     for (int j = 0; j < DAYS_COUNT; j++)
    //     {
    //         for (int k = 0; k < HOURS_COUNT; k++)
    //         {
    //             cout << e_consumption[i][j][k] << ",";
    //         }
    //         cout << endl;
    //     }
    // }
    

    write(
        write_fd,
        buffer,
        sizeof(buffer));
}
