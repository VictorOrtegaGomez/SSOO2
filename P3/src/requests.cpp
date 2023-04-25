#include "../include/requests.hpp"

clientRequest::clientRequest(client clientUser, int threadId, std::mutex *mutexSemaphore){
    this->clientUser = clientUser;
    this->threadId = threadId;
    this->mutexSemaphore = mutexSemaphore;
}
client clientRequest::getClient(){
    return this->clientUser;
}
int clientRequest::getThreadId(){
    return this->threadId;
}
std::mutex* clientRequest::getSemaphore(){
    return this->mutexSemaphore;
}
void clientRequest::setClient(client clientUser){
    this->clientUser = clientUser;
}
void clientRequest::setThreadId(int threadId){
    this->threadId = threadId;
}
void clientRequest::setSemaphore(std::mutex *mutexSemaphore){
    this->mutexSemaphore = mutexSemaphore;
}

balanceRequest::balanceRequest(client clientUser, int threadId, std::mutex *mutexSemaphore){
    this->clientUser = clientUser;
    this->threadId = threadId;
    this->mutexSemaphore = mutexSemaphore;
}
client balanceRequest::getClient(){
    return this->clientUser;
}
int balanceRequest::getThreadId(){
    return this->threadId;
}
std::mutex* balanceRequest::getSemaphore(){
    return this->mutexSemaphore;
}
void balanceRequest::setClient(client clientUser){
    this->clientUser = clientUser;
}
void balanceRequest::setThreadId(int threadId){
    this->threadId = threadId;
}
void balanceRequest::setSemaphore(std::mutex *mutexSemaphore){
    this->mutexSemaphore = mutexSemaphore;
}