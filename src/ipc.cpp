#include <iostream>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <cstring>
#include <unistd.h>

#define SHM_SIZE 1024

void writer() {
    int shmid = shmget(1234, SHM_SIZE, 0666 | IPC_CREAT);
    char* data = (char*)shmat(shmid, nullptr, 0);
    std::string message = "IPC Message!";
    
    strcpy(data, message.c_str());
    shmdt(data);
}

void reader() {
    int shmid = shmget(1234, SHM_SIZE, 0666);
    char* data = (char*)shmat(shmid, nullptr, 0);
    
    std::cout << "Received IPC: " << data << std::endl;
    shmdt(data);
}

int main() {
    std::thread writerThread(writer);
    std::thread readerThread(reader);

    writerThread.join();
    readerThread.join();

    return 0;
}
