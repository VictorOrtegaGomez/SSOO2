#include <iostream>
#include <string>
#include"../include/dataTypes.hpp"
#include <thread>
#include <functional>
#include <mutex>
#include <vector>
#include <algorithm>
#include <fstream>

std::mutex mutexSemaphore;

void saveData(threadData *data, int line, std::string previousWord, std::string nextWord){
    struct SearchResult result;

    result.line = line;
    result.previousWord = previousWord;
    result.nextWord = nextWord;

    /*We lock the semaphore while we're writing the data so there's no inconsistencies. After the writing it'll be unlocked*/

    mutexSemaphore.lock();

    data->queueResult(result);

    mutexSemaphore.unlock();
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

void searchWord(std::string fileName, threadData *data){

    std::ifstream file(fileName);
    std::string line;
    std::string word;
    std::string nextWord, previousWord;
    std::string wordToFind;
    int startLine;
    int endLine;

    /*We lock the semaphore while we're reading the data so there's no inconsistencies. After the reading it'll be unlocked*/

    mutexSemaphore.lock();

    wordToFind = data->getWordToFind();
    startLine = data->getstart();
    endLine = data->getend();

    mutexSemaphore.unlock();

    /*We skip the lines until reaching the ones that the thread got assigned*/

    for(int i = 0; i < data->getstart() - 1; i++) getline(file, line);

    /*Now we get the first assigned line and we start looking for the word we need. This will go on until we've checked every single line that the thread had assigned*/

    for(int i = data->getstart(); i < data->getend(); i++){
        getline(file, line);
        size_t pos = line.find(data->getWordToFind());

        while (pos != std::string::npos){
            word = line.substr(pos, line.find(" ", pos)-pos);
            
            /*We check that the word is not the last one of the line*/

            if (pos+word.length() < line.length()){ 
                
                /*We check if it is the one before the last word, if so we'll look for the end line character \r instead of the white space character " " */

                if(line.find(" ", pos+word.length()+1) != std::string::npos) nextWord = line.substr(line.find(" ", pos)+1, line.find(" ", pos+word.length()+1)-pos-word.length());
                
                else nextWord = line.substr(line.find(" ", pos)+1, line.find("\r", pos+word.length()+1)-pos-word.length());

            } else nextWord = "null";

            size_t beginningOfPreviousWord = line.rfind(" ", pos-2);

            if (beginningOfPreviousWord != std::string::npos){
                previousWord = line.substr(beginningOfPreviousWord+1, pos-beginningOfPreviousWord-1);
            }

            /*We save the results*/
            
            saveData(data, i, previousWord, nextWord);

            /*We keep loking for more appearances of the word in the line*/

            pos = line.find(data->getWordToFind(), pos+1);
        }
    }

}

/*A SearchResult struct is created and queued in the threadData's queue results*/

/*Funtion that will print the search results*/

void printSearchResult(std::vector<threadData> threadsDataResults){
    for (threadData& element : threadsDataResults){
        while (!element.isEmptyList()){
            struct SearchResult result;
            result = element.dequeueResult();
            std::cout<<"[Hilo " << element.getid() << " inicio: " << element.getstart() << "- final: " << element.getend() << "] :: linea "<< result.line << "::"<< "..." << result.previousWord << element.getWordToFind()<< " " << result.nextWord << "..."<<std::endl;
        }
        
    }
    
}

int main(int argc, char const *argv[]){
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

    /*We create the thread data objects where the results will be saved*/

    for(int i = 0; i < numThreads; i++){
        int lowerLimit = (i-1)*linesPerThread;
        int upperLimit = i*linesPerThread;

        if(i == numThreads-1) upperLimit = numLines;
        if(i == 0) lowerLimit = 0;

        threadsDataResults.push_back(threadData(i, lowerLimit, upperLimit, wordToFind));
    }

    /*We create the threads and assign them the threadData object that belongs to them*/

    for(int i = 0; i < numThreads; i++) threads.push_back(std::thread(searchWord, fileName, &threadsDataResults[i]));
    
    /*We wait for every thread to finish their task*/

    std::for_each(threads.begin(),threads.end(),std::mem_fn(&std::thread::join));

    printSearchResult(threadsDataResults);

    return 0;
}