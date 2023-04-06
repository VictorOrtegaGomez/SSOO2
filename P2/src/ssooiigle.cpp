#include <iostream>
#include <string>
#include "../include/dataTypes.hpp"
#include <thread>
#include <mutex>
#include <vector>
#include <algorithm>
#include <fstream>

int main(int argc, char const *argv[]){
    /*
    struct SearchResult word;
    struct SearchResult word2;
    word.line = 1;
    word.nextWord = "ajkdslfjakl";
    word.previousWord = "prev";
    threadData Miclass(0, 10, 100);
    Miclass.queueWord(word);
    std::cout << "ID de la hebra : " << Miclass.getid() << "Next word de la palabra buscada :\t" << Miclass.dequeueWord().previousWord << std::endl;
    */
    int numThreads;
    int linesPerThread;
    int numLines;
    std::string fileName;
    std::string wordToFind;
    std::vector<std::thread>threads;
    std::vector<threadData>threadsDataResults;

    /*We make sure that the number of arguments is correct*/

    if(argc != 4){
        std::cout << "Error: Arguments must be <file_name> <word> <number_of_threads>" << std::endl;
        return -1;
    }

    /*We save de arguments into variables*/

    fileName = argv[1];
    wordToFind = argv[2];

    if((numThreads = std::stoi(argv[3])) < 1){
        std::cout << "Error: Number of threads must be greater than 0" << std::endl;
        return -1;
    }

    /*We calculate the number of lines that each thread will have to read*/

    if((numLines = calculateTotalLines(fileName)) < 1){
        std::cout << "Error: File could not be opened or the file is blank" << std::endl;
        return -1;
    }

    linesPerThread = numLines/numThreads;

    /*We create the threads and assign them the line they need to read and start them*/

    for(int i = 0; i < numThreads; i++){
        int lowerLimit = (i-1)*linesPerThread;
        int upperLimit = i*linesPerThread;

        if(i == numThreads-1) upperLimit = numLines;
        if(i == 0) lowerLimit = 0;

        threadsDataResults.push_back(threadData data(i, lowerLimit, upperLimit, wordToFind));
        threads.push_back(std::thread(searchWord, fileName, lowerLimit,upperLimit, wordToFind));
    }

    std::for_each(threads.begin(),threads.end(),std::mem_fn(&std::thread::join));

    return 0;
}

/*Funtion that will read and count the number of lines in a file. The return result will be that said number*/

int calculateTotalLines(std::string fileName){
    std::ifstream file;
    std::string line;
    int numLines = 0;

    file.open(fileName);

    if(file.is_open()){
        while(getline(file, line)) numLines++;
    }
    else return -1;

    file.close();

    return numLines;
}

void searchWord(std::string fileName, int lowerLimit, int upperLimit, std::string wordToFind){

}

/*A SearchResult struct is created and queued in the threadData's queue results*/

void saveData(threadData *data, int line, std::string previousWord, std::string nextWord){
    struct SearchResult result;

    result.line = line;
    result.previousWord = previousWord;
    result.nextWord = nextWord;

    data->queueResult(result);
}
