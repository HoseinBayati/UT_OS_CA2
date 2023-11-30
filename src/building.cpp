#include <iostream>
// #include <sys/stat.h>
#include <sys/wait.h>

using namespace std;

int main(int argc, char *argv[])
{
    cout << "hello" << endl;

    if (argc != 4)
    {
        perror("didn't receive the port fd's\n");
        exit(EXIT_FAILURE);
    }

    int write_fd = atoi(argv[1]);
    int read_fd = atoi(argv[2]);
    string building_name = argv[3];

    char buffer[50];
    read(read_fd, buffer, sizeof(buffer));
    cout << "Building received: " << buffer << endl;

    write(
        write_fd,
        argv[3],
        sizeof(argv[3]));
}
