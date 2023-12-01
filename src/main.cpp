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

string compose_message_to_building(vector<string> target_resources)
{
    string message_to_building;

    for (int i = 0; i < target_resources.size(); i++)
    {
        message_to_building += target_resources[i];
        message_to_building = (i == target_resources.size() - 1) ? message_to_building : message_to_building + " ";
    }

    return message_to_building;
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
        to_string(pipefd_building_to_parent[1]).c_str(),     // write-to-parent end
        to_string(pipefd_parent_to_building[0]).c_str(),     // read-from-parent end
        (buildings_dir + "/" + building_name + "/").c_str(), // path to building
        building_name.c_str(),                               // building name
        NULL);

    close(pipefd_parent_to_building[0]);
    close(pipefd_building_to_parent[1]);
}

void handle_buildings_procs(
    vector<string> target_resources,
    vector<string> target_buildings,
    string buildings_dir)
{
    for (string target_building : target_buildings)
    {
        int pipefd_parent_to_building[2];
        int pipefd_building_to_parent[2];
        pid_t building_pid;

        if (pipe(pipefd_parent_to_building) == -1 || pipe(pipefd_building_to_parent) == -1)
        {
            std::cerr << "Pipe creation failed." << std::endl;
            return;
        }

        building_pid = fork();

        if (building_pid < 0)
        {
            std::cerr << "!! Building fork failed." << std::endl;
            return;
        }

        if (building_pid == 0)
        {
            exec_building(
                pipefd_parent_to_building,
                pipefd_building_to_parent,
                buildings_dir,
                target_building);

            break;
        }
        else
        {
            close(pipefd_parent_to_building[0]);
            close(pipefd_building_to_parent[1]);

            string message_to_building = compose_message_to_building(target_resources);

            write(
                pipefd_parent_to_building[1],
                message_to_building.c_str(),
                sizeof(message_to_building));

            char buffer[50];
            waitpid(building_pid, NULL, 0); ////////////////////
            read(
                pipefd_building_to_parent[0],
                buffer,
                sizeof(buffer));

            cout << "Parent received: " << buffer << endl;

            close(pipefd_parent_to_building[1]);
            close(pipefd_building_to_parent[0]);
        }
    }
}

void masterproc(
    vector<string> target_resources,
    vector<string> target_buildings,
    string buildings_dir)
{
    pid_t bills_pid;

    unlink(BILLS_PIPE);
    if (mkfifo(BILLS_PIPE, S_IRWXU | 0666) == -1)
    {
        perror("!! Failed to make Bills named pipe.");
        return;
    }

    bills_pid = fork();

    if (bills_pid < 0)
    {
        std::cerr << "!! Bills fork failed." << std::endl;
        return;
    }

    if (bills_pid == 0)
    {
        execlp(
            "./bin/out/bills.out",
            "./bin/out/bills.out",
            (buildings_dir + "/").c_str(), // path to buildings directory
            NULL);
    }
    else
    {
        handle_buildings_procs(
            target_resources,
            target_buildings,
            buildings_dir);
        waitpid(bills_pid, NULL, 0); ////////////////////
    }

    unlink(BILLS_PIPE); ////////////////////
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