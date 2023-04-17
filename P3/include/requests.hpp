#ifndef REQUESTS.H
#define REQUESTS.H
#include "../include/client.hpp"

class clientRequest {
    private:
        client clientUser;
        pid_t processPid;

    public:
        clientRequest(client clientUser, pid_t processPid);
        client getClient();
        pid_t getProcessPid();
        void setClient(client clientUser);
        void setProcessPid(pid_t processPid);
};

class balanceRequest {
    private:
        client clientUser;
        pid_t processPid;

    public:
        balanceRequest(client clientUser, pid_t processPid);
        client getClient();
        pid_t getProcessPid();
        void setClient(client clientUser);
        void setProcessPid(pid_t processPid);
};

#endif