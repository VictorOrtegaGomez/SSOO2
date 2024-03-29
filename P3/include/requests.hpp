/*This file contains the definition of the Request class and all its methods*/

#ifndef REQUESTS.H
#define REQUESTS.H
#include "client.hpp"
#include <mutex>
#include <thread>

class request {
    private:
        clientInfo *clientUser;
        std::thread::id threadId;
        std::mutex *mutexSemaphore;

    public:
        request(clientInfo *clientUser, std::thread::id threadId, std::mutex *mutexSemaphore);
        clientInfo* getClient();
        std::thread::id getThreadId();
        std::mutex* getSemaphore(); 
        void setClient(clientInfo *clientUser);
        void setThreadId(std::thread::id threadId);
        void setSemaphore(std::mutex *mutexSemaphore);
};

#endif