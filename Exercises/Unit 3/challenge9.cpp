#include <iostream>
#include <thread>
#include <vector>
#include <shared_mutex>
#include <chrono>
#include <mutex>

std::vector<int>sharedVector;
std::shared_mutex bufferMutex;

void readVector(){
    while(1){
        std::shared_lock<std::shared_mutex>lock(bufferMutex);

        std::cout << "Thread " << std::this_thread::get_id() << ": ";

        for(int i = 0; i < sharedVector.size(); i++){
            std::cout << sharedVector[i] << " ";
        }

        std::cout << std::endl;

        lock.unlock();
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

void writeVector(){
    int i = 0;
    while(1){
        std::unique_lock<std::shared_mutex>lock(bufferMutex);

        std::cout << "Writing number in vector" << std::endl;

        sharedVector.push_back(i);

        std::this_thread::sleep_for(std::chrono::seconds(5));
        lock.unlock();

        i++;
        std::this_thread::sleep_for(std::chrono::seconds(3));
    }

}

int main(){
    std::vector<std::thread>threads;

    std::thread tW(writeVector);
    std::thread tR1(readVector);
    std::thread tR2(readVector);
    std::thread tR3(readVector);

    tW.join();
    tR1.join();
    tR2.join();
    tR3.join();

    return 0;
}