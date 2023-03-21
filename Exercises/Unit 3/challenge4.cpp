#include <iostream>
#include <thread>
#include <fstream>
#include <string>
#include <bits/stdc++.h> 

#define NUMLINES 15

void readFile(std::string const& s){
    std::ifstream file(s.c_str());
    std::string line;
    int values[15];
    int i = 0;

    while (getline(file, line) && i < NUMLINES) {
        std::stringstream lineStream(line);
        getline(lineStream, line, ' ');
        values[i] = std::stoi(line, nullptr, 10);
        i++;
    }

    for(int j = 0; j < NUMLINES; j++){
        if(values[j]%2 == 0) std::cout << values[j] << " es par" << std::endl;
        else std::cout << values[j] << " es impar" << std::endl;
    }
}

int main(){
    std::string fileName = "estudiantes_p1.txt";
    std::thread t1(readFile, fileName);
    t1.join();
    return 0;
}