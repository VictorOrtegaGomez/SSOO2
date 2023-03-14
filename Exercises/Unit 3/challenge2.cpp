#include <iostream>
#include <thread>

void saludo(){
    std::cout << "Hola Mundo!" << std::endl;
}

int main(){
    std::thread t1(saludo);
    t1.join();
    return 0;
}