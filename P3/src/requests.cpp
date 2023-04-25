#include "../include/requests.hpp"

request::request(clientInfo *clientUser, std::thread::id threadId, std::mutex *mutexSemaphore){
    this->clientUser = clientUser;
    this->threadId = threadId;
    mutexSemaphore->lock();
    this->mutexSemaphore = mutexSemaphore;
}
clientInfo* request::getClient(){
    return this->clientUser;
}
std::thread::id request::getThreadId(){
    return this->threadId;
}
std::mutex* request::getSemaphore(){
    return this->mutexSemaphore;
}
void request::setClient(clientInfo *clientUser){
    this->clientUser = clientUser;
}
void request::setThreadId(std::thread::id threadId){
    this->threadId = threadId;
}
void request::setSemaphore(std::mutex *mutexSemaphore){
    this->mutexSemaphore = mutexSemaphore;
}