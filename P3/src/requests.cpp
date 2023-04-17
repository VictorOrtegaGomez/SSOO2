#include "../include/requests.hpp"

clientRequest::clientRequest(client clientUser, pid_t processPid){
    this->clientUser = clientUser;
    this->processPid = processPid;
}
client clientRequest::getClient(){
    return this->clientUser;
}
pid_t clientRequest::getProcessPid(){
    return this->processPid;
}
void clientRequest::setClient(client clientUser){
    this->clientUser = clientUser;
}
void clientRequest::setProcessPid(pid_t processPid){
    this->processPid = processPid;
}

balanceRequest::balanceRequest(client clientUser, pid_t processPid){
    this->clientUser = clientUser;
    this->processPid = processPid;
}
client balanceRequest::getClient(){
    return this->clientUser;
}
pid_t balanceRequest::getProcessPid(){
    return this->processPid;
}
void balanceRequest::setClient(client clientUser){
    this->clientUser = clientUser;
}
void balanceRequest::setProcessPid(pid_t processPid){
    this->processPid = processPid;
}