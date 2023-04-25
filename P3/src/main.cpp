#include <iostream>
#include <string>
#include "../include/dataTypes.hpp"
#include "../include/systemVariables.hpp"
#include "../include/client.hpp"
#include "../include/requests.hpp"
#include <condition_variable>
#include <thread>
#include <functional>
#include <mutex>
#include <vector>
#include <algorithm>
#include <memory>
#include <fstream>
#include <queue>
#include <chrono>


std::mutex mutexSemaphore;
std::vector <std::string> glbWordList = SEARCH_WORDS;
std::queue <std::shared_ptr<request>> searchQueue;
std::queue <request> balanceQueue;
std::mutex empty;
std::mutex full;
std::mutex mtxManager;
std::condition_variable conditionClients;
std::condition_variable conditionManager;

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

void searchWord(std::string fileName, threadData* data){

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

/*Piece of code that will be executed by the clients*/

void client(int id){
    /* We create the cliente with random values */
    srand(time(NULL));
    clientInfo client_instance(id, rand() % MAX_CLIENT_BALANCE, rand() % NUM_CLIENT_TYPES, glbWordList[rand() % glbWordList.size()]);

    /* We create the request */
    std::mutex local_mtx;
    request req(client_instance, std::this_thread::get_id(), &local_mtx);

    /* We initilize the unique lock for the condition variable*/
    std::unique_lock<std::mutex> lock(empty);
    std::shared_ptr<request> sharedReq = std::make_shared<request>(req);
    conditionClients.wait(lock, []{return searchQueue.size() < SEARCH_QUEUE_SIZE});
    searchQueue.push(sharedReq);

    /* We advise the search system that a request is available */
    conditionManager.notify_one();
    full.unlock();

    /* We wait for the results */
    sharedReq->getSemaphore()->lock();
    std::cout<< "Ejecucion de hilo: " << id << " Finalizada" << std::endl;
    
}
void RequestManager(int id){
    while (1)
    {
        std::unique_lock<std::mutex> lock(full);
        conditionManager.wait(lock, []{return searchQueue.size() > 0; });
        std::shared_ptr<request> sharedRequest = searchQueue.front();

        /* We have to create here the threads for the search (to implement)*/
        std::this_thread::sleep_for(std::chrono::seconds(2));
        std::cout << "Peticion procesada por hilo: " << id << "procedente del hilo: " << sharedRequest->getThreadId() << std::endl;

        /* We unlock the client */
        sharedRequest->getSemaphore()->unlock();
        searchQueue.pop();
        empty.unlock();
        conditionClients.notify_one();
    }
    
}

int main(int argc, char const *argv[]){
    std::vector<std::thread>searchThreads;
    std::vector<std::thread>clientThreads;

    /*We create the threads that will be doing the search they get from the searchQueue*/

    for(int i = 0; i <= CONCURRENT_SEARCH_REQUESTS; i++) searchThreads.push_back(std::thread(RequestManager, i));

    /*We'll be creating client threads the whole execution time and we won't wait for them to finish*/

    while(true){

        /*We generate a random number in order to create that number of clients*/

        srand(time(NULL));
        int numClients = rand() % MAX_CLIENT_CREATION;

        /*We create the client threads and then sleep*/

        for(int i = 0; i < numClients; i++) clientThreads.push_back(std::thread(client, i));
        
        std::this_thread::sleep_for(std::chrono::milliseconds(MAIN_SLEEP_TIME));
    }
    
    /*We wait for every search thread to finish*/

    std::for_each(searchThreads.begin(),searchThreads.end(),std::mem_fn(&std::thread::join));

    return 0;
}