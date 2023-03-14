#include <iostream>
#include <thread>

void saludoEn(){
    std::cout << "Hello world!" << std::endl;
}

void saludoEs(){
    std::cout << "Hola Mundo!" << std::endl;
}

void saludoD(){
    std::cout << "Hallo Welt!" << std::endl;
}

int main(){
    std::thread t1(saludoEn);
    std::thread t2(saludoEs);
    std::thread t3(saludoD);
    t1.join();
    t2.join();
    t3.join();
    return 0;
}