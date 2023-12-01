#include <iostream>
#include <iostream>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <cstring>
#include "../include/constants.hpp"

using namespace std;

char *read_bills_file(char *buffer)
{
    return buffer;
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        perror("!! Bad arguments for bills proc\n");
        exit(EXIT_FAILURE);
    }

    cout << "BILLS PROGRAM RUNNING" << endl;

    for (int i = 0; i < 9; i++)
    {
        int pipe = open(BILLS_PIPE, O_RDONLY);
        if (pipe == -1)
        {
            std::cerr << "Failed to open named pipe for reading requests" << std::endl;
            return 1;
        }

        char buffer[BUFFER_CAPACITY];
        memset(buffer, 0, BUFFER_CAPACITY);
        int bytesRead = 0;

        bytesRead = read(pipe, buffer, sizeof(buffer));

        if (bytesRead > 0)
        {
            std::cout << "Request received: " << buffer << std::endl;

            std::string responseData = "Here is the information you requested";

            close(pipe);

            pipe = open(BILLS_PIPE, O_WRONLY);
            if (pipe == -1)
            {
                std::cerr << "Failed to open named pipe for writing response" << std::endl;
                return 1;
            }

            char *response = read_bills_file(buffer);
            write(pipe, response, sizeof(response));

            close(pipe);
        }
        else
        {
            usleep(1000);
        }
    }

    cout << "=====  out of bills while  =====" << endl;

    return 0;
}
