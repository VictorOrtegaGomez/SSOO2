#ifndef REQUESTS.H
#define REQUESTS.H
#include "client.hpp"
#include "mutex"

class request {
    private:
        client clientUser;
        int threadId;
        std::mutex *mutexSemaphore;

    public:
        request(client clientUser, int threadId, std::mutex *mutexSemaphore);
        client getClient();
        int getThreadId();
        std::mutex* getSemaphore(); 
        void setClient(client clientUser);
        void setThreadId(int threadId);
        void setSemaphore(std::mutex *mutexSemaphore);
};

#endif