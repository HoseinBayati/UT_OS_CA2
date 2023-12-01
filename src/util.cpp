#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <sys/stat.h>
#include <sys/wait.h>
#include "../include/util.hpp"
#include "../include/constants.hpp"

using namespace std;

vector<string> split_string(
    const char *buffer,
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

vector<string> list_sub_dir(string path)
{
    int ls_pipe_fd[2];
    int grep_pipe_fd[2];
    if (pipe(ls_pipe_fd) == -1 || pipe(grep_pipe_fd) == -1)
    {
        perror("pipe error.");
    }
    int ls_pid = fork();
    if (ls_pid == 0)
    {
        close(STDOUT_FILENO);
        close(ls_pipe_fd[0]);
        dup(ls_pipe_fd[1]);
        execlp("ls", "ls", "-q", "--file-type", path.c_str(), NULL);
        close(ls_pipe_fd[1]);
        exit(1);
    }
    else if (ls_pid > 0)
    {
        close(ls_pipe_fd[1]);
        wait(NULL);
        int grep_pid = fork();
        if (grep_pid == 0)
        {
            close(STDIN_FILENO);
            dup(ls_pipe_fd[0]);

            close(STDOUT_FILENO);
            dup(grep_pipe_fd[1]);
            close(grep_pipe_fd[0]);
            execlp("grep", "grep", "/", NULL);
            exit(1);
        }
        else if (grep_pid > 0)
        {
            close(grep_pipe_fd[1]);
            char buffer[BUFFER_CAPACITY] = {0};
            read(grep_pipe_fd[0], buffer, BUFFER_CAPACITY);
            return split_string(buffer, '\n');
        }
    }
    return {};
}

void show_resource_options()
{
    cout << "Please enter resource keywords" << endl;

    for (int i = 0; i < RESOURCES_COUNT; i++)
    {
        cout << RESOURCES[i] << ": " << RESOURCE_SIGNS[i];
        if (i < RESOURCES_COUNT - 1)
        {
            cout << " | ";
        }
    }
    cout << endl;
}

vector<string> get_resources()
{
    show_resource_options();

    string resources;
    std::getline(cin, resources);
    return split_string(resources.c_str(), ' ');
}

void show_building_options(string buildings_folder)
{
    vector<string> sub_dirs = list_sub_dir(buildings_folder);
    cout << "Please enter target buildings" << endl;
    for (auto &sub_dir : sub_dirs)
    {
        sub_dir.pop_back();
        cout << sub_dir << " ";
    }
    cout << endl;
}

vector<string> get_building_names(string buildings_folder)
{
    show_building_options(buildings_folder);

    string buildings;
    std::getline(cin, buildings);
    return split_string(buildings.c_str(), ' ');
}
