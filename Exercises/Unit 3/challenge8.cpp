#include <iostream>
#include <thread>
#include <ctime>
#include <algorithm>
#include <functional>
#include <vector>
#include <mutex>

#define LIMIT 10000000
#define NUMTHREADS 12

unsigned int totalSum = 0;
std::mutex semMutex;

void sum(int start, int end, std::vector<unsigned int>numberVector){
    int sum = 0;
    for(int i = start; i < end; i++){
        sum += numberVector[i];
    }
    semMutex.lock();
    totalSum += sum;
    semMutex.unlock();
}

void genRandomNumbers(std::vector<unsigned int>&numberVector){
    srand(time(NULL));
    for(int i = 0; i < LIMIT; i++){
        numberVector.push_back(rand() % 100);
    }
}

int main(){
    std::vector<std::thread>threads;
    std::vector<unsigned int>numberVector;
    int result = 0;
    int division = LIMIT/NUMTHREADS;
    unsigned t0, t1;    
    double time;

    genRandomNumbers(ref(numberVector));

    t0 = std::clock();

    sum(0,LIMIT,numberVector);

    t1 = std::clock();
    time = (double(t1-t0)/CLOCKS_PER_SEC);

    std::cout << "Execution Time (No threads): " << time << "\nThe result of the operation is " << totalSum << std::endl;

    totalSum = 0;

    t0=std::clock();

    for(int i = 0; i < NUMTHREADS; i++){
        int lowerLimit = (i-1)*division;
        int upperLimit = i*division;
        if(i == NUMTHREADS-1) upperLimit = LIMIT;
        if(i == 0) lowerLimit = 0;
        threads.push_back(std::thread(sum,lowerLimit,upperLimit,numberVector));
    }

    std::for_each(threads.begin(),threads.end(),std::mem_fn(&std::thread::join));

    t1 = std::clock(); 
    time = (double(t1-t0)/CLOCKS_PER_SEC);


    std::cout << "Execution Time (Threads): " << time << "\nThe result of the thread operation is " << totalSum << std::endl;
}