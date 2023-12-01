#include <iostream>
#include <iostream>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <cstring>
#include "../include/constants.hpp"

using namespace std;

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        perror("!! Bad arguments for bills proc\n");
        exit(EXIT_FAILURE);
    }

    cout << "BILLS PROGRAM RUNNING" << endl;

    int pipe = open(BILLS_PIPE, O_RDONLY);
    if (pipe == -1)
    {
        std::cerr << "Failed to open named pipe for reading requests" << std::endl;
        return 1;
    }

    // while (true)
    // {
    // Read the request
    char buffer[256];
    int bytesRead = read(pipe, buffer, sizeof(buffer));

    if (bytesRead > 0)
    {
        std::cout << "Request received: " << buffer << std::endl;

        // Process the request and prepare the response
        std::string responseData = "Here is the information you requested";

        // Close the reading end of the pipe
        close(pipe);

        // Open the named pipe for writing the response
        pipe = open(BILLS_PIPE, O_WRONLY);
        if (pipe == -1)
        {
            std::cerr << "Failed to open named pipe for writing response" << std::endl;
            return 1;
        }

        // Write the response to the named pipe
        write(pipe, responseData.c_str(), strlen(responseData.c_str()));

        // Close the pipe
        close(pipe);
    }
    else
    {
        std::cerr << "Failed to read request from named pipe" << std::endl;
    }
    // }

    return 0;
}
