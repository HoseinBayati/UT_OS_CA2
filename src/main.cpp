#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include "../include/util.hpp"
#include "../include/constants.hpp"

using namespace std;

void compose_message_to_building(vector<string> target_resources)
{
    string message_to_building;

    for (auto &resource : target_resources)
    {
        message_to_building += resource;
        message_to_building += " ";
    }
}

void exec_building(
    int pipefd_parent_to_building[2],
    int pipefd_building_to_parent[2],
    string buildings_dir,
    string building_name)
{
    string building_msg = building_name;
    char buffer[50];

    close(pipefd_parent_to_building[1]);
    close(pipefd_building_to_parent[0]);

    execlp(
        "./bin/out/building.out",
        "./bin/out/building.out",
        to_string(pipefd_building_to_parent[1]).c_str(), // write-to-parent end
        to_string(pipefd_parent_to_building[0]).c_str(), // read-from-parent end
        (buildings_dir + "/" + building_name + "/").c_str(), // path to building
        NULL);

    close(pipefd_parent_to_building[0]);
    close(pipefd_building_to_parent[1]);
}

void masterproc(
    vector<string> target_resources,
    vector<string> target_buildings,
    string buildings_dir)
{
    int i = 0;
    for (string target_building : target_buildings)
    {
        int pipefd_parent_to_building[2];
        int pipefd_building_to_parent[2];
        pid_t pid;

        if (pipe(pipefd_parent_to_building) == -1 || pipe(pipefd_building_to_parent) == -1)
        {
            std::cerr << "Pipe creation failed." << std::endl;
            return;
        }

        pid = fork();

        if (pid < 0)
        {
            std::cerr << "Fork failed." << std::endl;
            return;
        }

        if (pid == 0)
        {
            // Child process
            exec_building(
                pipefd_parent_to_building,
                pipefd_building_to_parent,
                buildings_dir,
                target_building);

            break;
        }
        else
        {
            // Parent process
            close(pipefd_parent_to_building[0]);
            close(pipefd_building_to_parent[1]);

            string message_to_building;

            for (auto &resource : target_resources)
            {
                message_to_building += resource;
                message_to_building += " ";
            }

            write(
                pipefd_parent_to_building[1],
                message_to_building.c_str(),
                sizeof(message_to_building));

            char buffer[50];
            read(
                pipefd_building_to_parent[0],
                buffer,
                sizeof(buffer));

            cout << "Parent received: " << buffer << endl;

            close(pipefd_parent_to_building[1]);
            close(pipefd_building_to_parent[0]);
        }

        i++;
    }
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        cerr << "command format:\n ./main <buildings_directory>" << endl;
        return 0;
    }

    string buildings_dir = string(argv[1]);

    vector<string> target_resources = get_resources();
    vector<string> target_buildings = get_building_names(buildings_dir);

    masterproc(
        target_resources,
        target_buildings,
        buildings_dir);

    return 1;
}