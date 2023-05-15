/*This file will be the one that will be executed in order to run the application. It contais every search funtionality and the 
thread creation and management.*/

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

std::ofstream glbLog(LOG);
std::mutex mutexLog;
std::mutex mutexSemaphore;
std::vector <std::string> glbWordList = SEARCH_WORDS;
std::vector <std::string> glbFileList = FILES_NAMES;
std::vector <std::string> glbClientList = CLIENT_TYPES;
std::vector <std::shared_ptr<request>> searchQueue;
std::queue <std::shared_ptr<request>> balanceQueue;
std::mutex empty;
std::mutex full;
std::mutex mtxSearchQueue;
std::mutex mtxBalanceQueue;
std::condition_variable cvClients;
std::condition_variable cvRequestsManager;
std::condition_variable cvBalanceManager;
std::condition_variable cvBalanceClients;
int glbClientID = 0;

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

void searchWord(std::string fileName, threadData* data, clientInfo* client, std::mutex* mutexClient){

    std::ifstream file(fileName);
    std::string line, lineLowerCase;
    std::string word;
    std::string nextWord, previousWord;
    std::string wordToFind;
    int startLine;
    int endLine;

    /*We lock the semaphore while we're reading the data so there's no inconsistencies. After the reading it'll be unlocked*/


    wordToFind = data->getWordToFind();
    startLine = data->getStart();
    endLine = data->getEnd();

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

            /*We check that the client has enough credits to do the search. If the client doesn't have enough credits it will send a balance request*/

            if(client->getType() == 1){

                mutexClient->lock();

                if((client->getBalance() - WORD_FOUND_COST) >= 0){
                    client->setBalance(client->getBalance() - WORD_FOUND_COST);
                    mutexClient->unlock();
                }else{
                    std::cout << "\033[0;37mClient " << client->getId() << " has run out of credits. Sending request to balance manager\033[0m" << std::endl;

                    /*We create the request and push it to the balance queue*/
                    std::mutex local_mtx;
                    request balanceRequest(client, std::this_thread::get_id(), &local_mtx);

                    mtxBalanceQueue.lock();
                    std::shared_ptr<request> sharedReq = std::make_shared<request>(balanceRequest);
                    balanceQueue.push(sharedReq);
                    mtxBalanceQueue.unlock();

                    /* We advise the balance system that a request is available */
                    cvBalanceManager.notify_one();

                    /* We wait for the results */
                    sharedReq->getSemaphore()->lock();
                    mutexClient->unlock();
                    mutexLog.lock();
                    std::cout<< "\033[0;37mClient: " << client->getId() << " balance updated. Continuing search \033[0m" << std::endl;
                    glbLog << "Client: " << client->getId() << " balance updated. Continuing search" << sharedReq->getClient()->getBalance()<<std::endl;
                    mutexLog.unlock();
                }
            }
            
            /* We save de data once it is preprocesed */
            saveData(data, i, word, previousWord, nextWord);
            

            /*We keep loking for more appearances of the word in the line*/

            pos = lineLowerCase.find(wordToFind, pos+1);
        }
    }

}

/*Funtion that will print the search results*/

void printSearchResult(std::vector<threadData> threadsDataResults,  std::shared_ptr<request> sharedRequest){
    mutexLog.lock();
    std::cout << "\033[0;37mSaving results from client: " << sharedRequest->getClient()->getId() << "\033[0m" << std::endl;
    glbLog << "Saving results from client: " << sharedRequest->getClient()->getId() << std::endl;
    mutexLog.unlock();

    std::string fileName = "results/client"+std::to_string(sharedRequest->getClient()->getId());
    std::ofstream filePointer (fileName);
    if(!filePointer.is_open()){
        mutexLog.lock();
        std::cout << "\033[0;31mError creating file\033[0m" << std::endl;
        glbLog << "Error creating file" << std::endl;
        mutexLog.unlock();
        return;
    }
    filePointer << "Duration of the execution is: " << sharedRequest->getClient()->getDuration() << "ms" << std::endl;
    for (threadData& element : threadsDataResults){
        while (!element.isEmptyList()){
            struct SearchResult result;
            result = element.dequeueResult();

            filePointer<<"[Thread " << element.getId() << " Beginning: " << element.getStart() << " - end: ";
            filePointer<< element.getEnd() << "] lines "<< result.line << " :: "<< "..." << result.previousWord << " " << result.consideredWord << " " << result.nextWord << "..."<<std::endl;
        }
        
    }
    filePointer.close();
    
}

/*Function that will calculate the lower and upper limits where the thread should be searching*/

void calculateLimits(int *upperLimit, int *lowerLimit, int numThreads, int numLines, int i){
    int linesPerThread = numLines/numThreads;

    *lowerLimit = (i)*linesPerThread+1;
    *upperLimit = *lowerLimit + linesPerThread;

    if(i == 0){
        *upperLimit = linesPerThread;
        *lowerLimit = 1;
    } 
    
    if(i == numThreads-1) *upperLimit = numLines;

}

/*Piece of code that will be executed by the clients*/

void client(int id){

    /* We create the cliente with random values */
    mutexLog.lock();
    std::cout << "\033[0;37mClient created\033[0m" << std::endl;
    glbLog << "Client created" << std::endl;
    mutexLog.unlock();

    srand(time(NULL));
    clientInfo client_instance(id, rand() % MAX_CLIENT_BALANCE, rand() % NUM_CLIENT_TYPES, glbWordList[rand() % glbWordList.size()]);

    /* We create the request */
    std::mutex local_mtx;
    request req(&client_instance, std::this_thread::get_id(), &local_mtx);

    /* We initilize the unique lock for the condition variable, we ensure that the queue has a space before pushing the request*/
    std::unique_lock<std::mutex> lock(empty);
    std::shared_ptr<request> sharedReq = std::make_shared<request>(req);
    cvClients.wait(lock, []{return searchQueue.size() < SEARCH_QUEUE_SIZE});
    searchQueue.push_back(sharedReq);
    /* We advise the search system that a request is available */
    full.unlock();
    cvRequestsManager.notify_one();

    /* We wait for the results */
    sharedReq->getSemaphore()->lock();
    mutexLog.lock();
    std::cout<< "\033[0;36mThread: " << id << " execution finished - " << "Remaining balance: " << sharedReq->getClient()->getBalance() << "\033[0m" << std::endl;
    glbLog << "Thread: " << id << " execution finished - " << "Remaining balance: " << sharedReq->getClient()->getBalance()<<std::endl;
    mutexLog.unlock();
    printSearchResult(client_instance.getResult(), sharedReq);
    
}

/*Funtion that will create the threads needed for every file*/

void createFileThreads(std::vector<threadData> *threadsDataResults, std::vector<std::thread> *fileThreads, std::shared_ptr<request> sharedRequest){
    int counter_threadID = 0;
    int lowerLimit = 0, upperLimit = 0;
    for(int i = 0; i < NUM_OF_FILES; i++){
        int numLines = calculateTotalLines(glbFileList[i]);
        for(int z = 0; z < NUM_OF_THREADS_IN_FILE; z++){
            calculateLimits(&upperLimit, &lowerLimit, NUM_OF_THREADS_IN_FILE, numLines, z);
            threadsDataResults->push_back(threadData(counter_threadID, lowerLimit, upperLimit, sharedRequest->getClient()->getWordToSearch()));
            counter_threadID++;
        }
    }

    /* We create the threads with the asocited threadData*/
    counter_threadID = 0;
    auto start = std::chrono::high_resolution_clock::now();
    std::mutex mutexClient;

    for (int i = 0; i < NUM_OF_FILES; i++) for (int z = 0; z < NUM_OF_THREADS_IN_FILE; z++, counter_threadID++)
     fileThreads->push_back(std::thread(searchWord, glbFileList[i], &threadsDataResults->at(counter_threadID), sharedRequest->getClient(), &mutexClient));

    /* We wait for the threads */
    std::for_each(fileThreads->begin(), fileThreads->end(), std::mem_fn(&std::thread::join));
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    /* We asigned the duration time to the client */
    sharedRequest->getClient()->setDuration(duration);
}

/*Function that will select the type of client in a 80/20 ratio*/

int selectClientType(){

    srand(time(NULL));
    double clientType = rand() % 100;

    if(clientType < 80) return 0;
    else return 1;
}

/*Function that will return the position in the vector of the selected request*/

int selectSearchRequest(){
    
    srand(time(NULL));
    int type = selectClientType();
    bool premium;

    if(type == 0) premium = true;
    else premium = false;
    
    for(int i = 0; i < searchQueue.size(); i++){
        if(premium && (searchQueue[i]->getClient()->getType() == 0 || searchQueue[i]->getClient()->getType() == 1)) return i;
        else if(!premium && searchQueue[i]->getClient()->getType() == 1) return i;
    }

    if(searchQueue.size() == 0){
        return -1;
    }else return 0;
}

/*Function that will manage the search requests*/

void requestManager(int id){
    std::vector<std::thread>fileThreads;
    std::vector<threadData>threadsDataResults;
    int selectedRequestPosition;

    while (1)
    {
        /* We ensure that the queue has one or more elements */
        std::unique_lock<std::mutex> lock(full);
        cvRequestsManager.wait(lock, []{return searchQueue.size() > 0; });

        /* We ensure the mutual exlusion while accessing the queue*/
        mtxSearchQueue.lock();
        std::shared_ptr<request> sharedRequest = std::move(searchQueue[selectedRequestPosition]);
        searchQueue.erase(searchQueue.begin() + selectedRequestPosition);
        mtxSearchQueue.unlock();

        /* We advertise the client that the queue has a free space */
        empty.unlock();
        cvClients.notify_one();
        /* We have to create here the threads for the search*/
        mutexLog.lock();
        std::cout << "\033[0;32m[" << id << "]" << " Processing search request from client: " << sharedRequest->getClient()->getId() << "\033[0m" << std::endl;
        glbLog << "[" << id << "]" << " Processing search request from client: " << sharedRequest->getClient()->getId() << std::endl;
        mutexLog.unlock();

        createFileThreads(&threadsDataResults, &fileThreads, sharedRequest);

        mutexLog.lock();
        std::cout << "\033[0;32m[" << id << "]" << " Search request processing finished from client: " << sharedRequest->getClient()->getId() << "\033[0m" << std::endl;
        glbLog << "[" << id << "]" << " Search request processing finished from client: " << sharedRequest->getClient()->getId() << std::endl;
        mutexLog.unlock();

        /* We unlock the client who is wating for the results*/
        sharedRequest->getClient()->setResult(threadsDataResults);
        sharedRequest->getSemaphore()->unlock();
        fileThreads.clear();
        threadsDataResults.clear();
        std::this_thread::sleep_for(std::chrono::seconds(2));
        
    }
    
}

/*Funtion that will be launched by the balance service. It will manage balance requests*/

void balanceManager(){
    std::mutex mtx;

    while(true){

        /* We ensure that the queue has one or more elements*/
        std::unique_lock<std::mutex> lock(mtx);
        cvBalanceManager.wait(lock, []{return balanceQueue.size() > 0; });

        /* We ensure the mutual exlusion while accessing the queue*/
        mtxBalanceQueue.lock();
        std::shared_ptr<request> sharedRequest = std::move(balanceQueue.front());
        balanceQueue.pop();
        mtxBalanceQueue.unlock();

        mutexLog.lock();
        std::cout << "\033[0;33m[BALANCE_MANAGER]" << " Processing balance request from client: " << sharedRequest->getClient()->getId() << "\033[0m" << std::endl;
        glbLog << "[BALANCE_MANAGER]" << " Processing balance request from client: " << sharedRequest->getClient()->getId() << std::endl;
        mutexLog.unlock();

        /* We advertise the search thread that the queue has a free space */
        empty.unlock();
        cvBalanceClients.notify_one();

        /*We add credits to the client*/
        srand(time(NULL));
        sharedRequest->getClient()->setBalance(rand() % MAX_CLIENT_BALANCE);

        /* We unlock the search thread who is wating for the results*/
        sharedRequest->getSemaphore()->unlock();

        mutexLog.lock();
        std::cout << "\033[0;33m[BALANCE_MANAGER] " << sharedRequest->getClient()->getBalance() << " Credits added to client: " << sharedRequest->getClient()->getId() << "\033[0m" << std::endl;
        glbLog << "[BALANCE_MANAGER] " << sharedRequest->getClient()->getBalance() << " Credits added to client: " << sharedRequest->getClient()->getId() << std::endl;
        mutexLog.unlock();
    }

}

int main(int argc, char const *argv[]){
    std::vector<std::thread>searchThreads;
    std::vector<std::thread>clientThreads;

    /*We launch the balance manager*/
    std::thread balanceManagerThread(balanceManager);

    /*We create the threads that will be doing the search they get from the searchQueue*/

    for (int i = 0; i < CONCURRENT_SEARCH_REQUESTS; i++) searchThreads.push_back(std::thread(requestManager, i));
    //
    /*We'll be creating client threads the whole execution time and we won't wait for them to finish*/

    while(true){

        /*We generate a random number in order to create that number of clients*/

        srand(time(NULL));
        int numClients = rand() % MAX_CLIENT_CREATION;

        /*We create the client threads and then sleep*/

        for(int i = 0; i < numClients; i++, glbClientID++) clientThreads.push_back(std::thread(client, glbClientID));
        
        std::this_thread::sleep_for(std::chrono::seconds(10));
    }
    
    /*We wait for every search thread to finish*/

    std::for_each(searchThreads.begin(),searchThreads.end(),std::mem_fn(&std::thread::join));
    balanceManagerThread.join();

    return 0;
}