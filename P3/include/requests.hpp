#ifndef REQUESTS.H
#define REQUESTS.H
#include "../include/client.hpp"
#include "mutex"

class clientRequest {
    private:
        client clientUser;
        int threadId;
        std::mutex *mutexSemaphore;

    public:
        clientRequest(client clientUser, int threadId, std::mutex *mutexSemaphore);
        client getClient();
        int getThreadId();
        std::mutex* getSemaphore(); 
        void setClient(client clientUser);
        void setThreadId(int threadId);
        void setSemaphore(std::mutex *mutexSemaphore);
};

class balanceRequest {
    private:
        client clientUser;
        int threadId;
        std::mutex *mutexSemaphore;

    public:
        balanceRequest(client clientUser, int threadId, std::mutex *mutexSemaphore);
        client getClient();
        int getThreadId();
        std::mutex* getSemaphore(); 
        void setClient(client clientUser);
        void setThreadId(int threadId);
        void setSemaphore(std::mutex *mutexSemaphore);
};

#endif