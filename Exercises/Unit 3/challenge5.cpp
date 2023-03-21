#include <iostream>
#include <thread>

void showNumber(int *num){
    while(1){
        std::cout << *num << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

void incrementNumber(int *num){
    while(1){
        *num = *num + 1;
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }
}

int main(){
    int number = 0;
    std::thread t1(showNumber, &number);
    t1.detach();
    std::thread t2(incrementNumber, &number);
    t2.detach();
    std::this_thread::sleep_for(std::chrono::seconds(15));
    return 0;
}