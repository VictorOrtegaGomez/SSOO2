#include <iostream>
#include <thread>
#include <ctime>
#include <algorithm>
#include <functional>
#include <vector>

#define LIMIT 10000000
#define NUMTHREADS 10

int totalSum = 0;

void sum(int start, int end, std::vector<int>numberVector){
    int sum = 0;
    for(int i = start; i < end; i++){
        sum += numberVector[i];
    }
    totalSum += sum;
}

void genRandomNumbers(std::vector<int>&numberVector){
    srand(time(NULL));
    for(int i = 0; i < LIMIT; i++){
        numberVector.push_back(rand() % 100);
    }
}

int main(){
    std::vector<std::thread>threads;
    std::vector<int>numberVector;
    int result = 0;
    int division = LIMIT/NUMTHREADS;
    unsigned t0, t1;    
    double time;

    genRandomNumbers(ref(numberVector));

    sum(0,LIMIT,numberVector);

    std::cout << "The result of the operation is " << totalSum << std::endl;

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


    std::cout << "The result of the thread operation is " << totalSum << "\nExecution Time: " << time << std::endl;
}