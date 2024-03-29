#include <iostream>
#include <string>
#include "../include/dataTypes.hpp"
#include <thread>
#include <functional>
#include <mutex>
#include <vector>
#include <algorithm>
#include <fstream>

std::mutex mutexSemaphore;

/*A SearchResult struct is created and queued in the threadData's queue results*/

void saveData(threadData *data, int line, std::string word, std::string previousWord, std::string nextWord){
    struct SearchResult result;

    result.line = line;
    result.previousWord = previousWord;
    result.consideredWord = word;
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

/*With this method we ensure that the word does not have return carriage*/

void preprocessWord(std::string& previousWord, std::string& word, std::string& nextWord){

        size_t pospre, posword, posnext;
        /*We look for the return carriage*/
        pospre = previousWord.find("\r");
        posword = word.find("\r");
        posnext = nextWord.find("\r");
        if (pospre != std::string::npos){
            /*If we find a rturn carriage we eliminate it*/
            previousWord.erase(pospre, 1);
        }
        if (posword != std::string::npos){
            word.erase(posword , 1);
        }
        if (posnext != std::string::npos){
            nextWord.erase(posnext, 1);
        }
}

/*Function that will search the word in the assigned lines*/

void searchWord(std::string fileName, threadData *data){

    std::ifstream file(fileName);
    std::string line, lineLowerCase;
    std::string word;
    std::string nextWord, previousWord;
    std::string wordToFind;
    int startLine;
    int endLine;

    /*We lock the semaphore while we're reading the data so there's no inconsistencies. After the reading it'll be unlocked*/

    mutexSemaphore.lock();

    wordToFind = data->getWordToFind();
    startLine = data->getStart();
    endLine = data->getEnd();

    mutexSemaphore.unlock();

    /*We skip the lines until reaching the ones that the thread got assigned*/

    for(int i = 1; i < startLine; i++) getline(file, line);

    /*Now we get the first assigned line and we start looking for the word we need. This will go on until we've checked every single line that the thread had assigned*/

    for(int i = startLine; i <= endLine; i++){

        /*We get the line and convert its character to lower case in order to do a better search*/

        getline(file, line);
        lineLowerCase = line;
        std::transform(lineLowerCase.begin(), lineLowerCase.end(), lineLowerCase.begin(), ::tolower);

        /*We find the position of the word*/

        size_t pos = lineLowerCase.find(wordToFind);

        while (pos != std::string::npos){
            word = line.substr(pos, lineLowerCase.find(" ", pos)-pos);
            
            /*We check that the word is not the last one of the line*/

            if (pos+word.length() < lineLowerCase.length()){ 
                
                /*We check if it is the one before the last word, if so we'll look for the end line character \r instead of the white space character " " */

                if(lineLowerCase.find(" ", pos+word.length()+1) != std::string::npos) nextWord = line.substr(lineLowerCase.find(" ", pos)+1, lineLowerCase.find(" ", pos+word.length()+1)-pos-word.length()-1);
                


            } else nextWord = "";

            size_t beginningOfPreviousWord = lineLowerCase.rfind(" ", pos-2);

            if (beginningOfPreviousWord != std::string::npos){
                previousWord = line.substr(beginningOfPreviousWord+1, pos-beginningOfPreviousWord-1);
            }
            /* Here we preproces de data to eliminate carriage return*/
            preprocessWord(previousWord, word, nextWord);

            /* We save de data once it is preprocesed */
            saveData(data, i, word, previousWord, nextWord);
            

            /*We keep loking for more appearances of the word in the line*/

            pos = lineLowerCase.find(wordToFind, pos+1);
        }
    }

}

/*Funtion that will print the search results*/

void printSearchResult(std::vector<threadData> threadsDataResults){
    for (threadData& element : threadsDataResults){
        while (!element.isEmptyList()){
            struct SearchResult result;
            result = element.dequeueResult();
    
            std::cout<<"[Hilo " << element.getId() << " inicio: " << element.getStart() << " - final: ";
            std::cout << element.getEnd() << "] linea "<< result.line << " :: "<< "..." << result.previousWord << " " << result.consideredWord << " " << result.nextWord << "..."<<std::endl;
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

    for(int i = 1; i <= numThreads; i++){
        int upperLimit = (i)*linesPerThread;
        int lowerLimit = upperLimit - linesPerThread + 1;

        if(i == 1){
            upperLimit = linesPerThread;
            lowerLimit = 1;
        } 
        
        if(i == numThreads) upperLimit = numLines;

        threadsDataResults.push_back(threadData(i, lowerLimit, upperLimit, wordToFind));
    }

    /*We create the threads and assign them the threadData object that belongs to them*/

    for(int i = 0; i < numThreads; i++) threads.push_back(std::thread(searchWord, fileName, &threadsDataResults[i]));
    
    /*We wait for every thread to finish their task*/

    std::for_each(threads.begin(),threads.end(),std::mem_fn(&std::thread::join));

    printSearchResult(threadsDataResults);

    return 0;
}
