#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <sys/stat.h>
#include <sys/wait.h>
#include "util.hpp"

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

vector<string> get_resources()
{
    string resources;
    cout << "Enter Resources:" << endl;
    std::getline(cin, resources);
    return split_string(resources.c_str(), ' ');
}

string get_building_name()
{
    cout << "Enter The Building Name:" << endl;

    string building_name;
    cin >> building_name;
    return building_name;
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
    if (ls_pid == 0) // child process
    {
        close(STDOUT_FILENO);
        close(ls_pipe_fd[0]);
        dup(ls_pipe_fd[1]); // make stdout same as ls_pipe_fd[1]
        execlp("ls", "ls", "-q", "--file-type", path.c_str(), NULL);
        close(ls_pipe_fd[1]);
        exit(1);
    }
    else if (ls_pid > 0) // parent process
    {
        close(ls_pipe_fd[1]);
        wait(NULL); // wait for ls
        int grep_pid = fork();
        if (grep_pid == 0) // child process
        {
            close(STDIN_FILENO);
            dup(ls_pipe_fd[0]); // read from ls

            close(STDOUT_FILENO);
            dup(grep_pipe_fd[1]);   // make stdout same as grep_pipe_fd[1]
            close(grep_pipe_fd[0]); // we don't need this
            execlp("grep", "grep", "/", NULL);
            exit(1);
        }
        else if (grep_pid > 0) // parent process
        {
            close(grep_pipe_fd[1]);
            char buffer[MAX_LEN] = {0};
            read(grep_pipe_fd[0], buffer, MAX_LEN);
            return split_string(buffer, '\n');
        }
    }
    return {};
}
