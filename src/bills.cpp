#include <iostream>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <cstring>
#include <fstream>
#include "../include/util.hpp"
#include "../include/constants.hpp"

using namespace std;

void read_file(
    string file_dir,
    int (&prices)[MONTHS_COUNT][RESOURCES_COUNT])
{
    fstream fin;
    string line;
    vector<string> words;
    int hour_0_column = 3;

    fin.open(file_dir, ios::in);
    if (!fin.is_open())
    {
        cout << "!! couldn't open the file (invalid building name)" << endl;
        return;
    }

    getline(fin, line);
    for (int i = 0; i < MONTHS_COUNT; i++)
    {
        getline(fin, line);
        words = split_string(line.c_str(), ',');
        for (int j = 0; j < RESOURCES_COUNT; j++)
        {
            prices[i][j] = atoi(words[2 + j].c_str());
        }
    }

    fin.close();
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        perror("!! Bad arguments for bills proc\n");
        exit(EXIT_FAILURE);
    }
    string file_dir = argv[1];
    int buildings_count = atoi(argv[2]);

    // cout << "BILLS PROGRAM RUNNING " << file_dir << endl;

    int prices[MONTHS_COUNT][RESOURCES_COUNT];

    read_file(file_dir, prices);

    usleep(PIPE_WAIT_TIME);

    string response_message = "";
    for (int i = 0; i < MONTHS_COUNT; i++)
    {
        for (int j = 0; j < RESOURCES_COUNT; j++)
        {
            response_message = j ? response_message + " " : response_message;
            response_message += to_string(prices[i][j]);
        }
        response_message += ",";
    }

    const char *response = response_message.c_str();

    // cout << response << endl;

    for (int i = 0; i < buildings_count; i++)
    {
        int pipe = open(BILLS_PIPE, O_RDONLY);
        if (pipe == -1)
        {
            cerr << "Failed to open named pipe for reading requests" << endl;
            return 1;
        }

        char buffer[BUFFER_CAPACITY];
        memset(buffer, 0, BUFFER_CAPACITY);
        int bytesRead = 0;

        bytesRead = read(pipe, buffer, BUFFER_CAPACITY);

        if (bytesRead > 0)
        {
            cout << "Request received: " << buffer << endl;

            close(pipe);

            pipe = open(BILLS_PIPE, O_WRONLY);
            if (pipe == -1)
            {
                cerr << "Failed to open named pipe for writing response" << endl;
                return 1;
            }
            write(pipe, response, BUFFER_CAPACITY);

            close(pipe);
        }
        else
        {
            usleep(PIPE_WAIT_TIME);
        }
    }

    // cout << "=====  out of bills while  =====" << endl;

    return 0;
}
