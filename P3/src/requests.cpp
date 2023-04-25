#include "../include/requests.hpp"

request::request(client clientUser, int threadId, std::mutex *mutexSemaphore){
    this->clientUser = clientUser;
    this->threadId = threadId;
    this->mutexSemaphore = mutexSemaphore;
}
client request::getClient(){
    return this->clientUser;
}
int request::getThreadId(){
    return this->threadId;
}
std::mutex* request::getSemaphore(){
    return this->mutexSemaphore;
}
void request::setClient(client clientUser){
    this->clientUser = clientUser;
}
void request::setThreadId(int threadId){
    this->threadId = threadId;
}
void request::setSemaphore(std::mutex *mutexSemaphore){
    this->mutexSemaphore = mutexSemaphore;
}