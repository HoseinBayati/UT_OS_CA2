#include <iostream>
// #include <sys/stat.h>
#include <sys/wait.h>
#include "../include/constants.hpp"

using namespace std;

void read_files()
{
    
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
    string building_directory = argv[3];

    char buffer[50];
    read(read_fd, buffer, sizeof(buffer));
    cout << "Building received: " << building_directory << " " << buffer << endl;

    int gas_cons[MONTHS_COUNT][DAYS_COUNT][HOURS_COUNT];

    write(
        write_fd,
        buffer,
        sizeof(buffer));
}
