/*This file contains the Client class implementation and all its methods*/

#include "../include/client.hpp"

clientInfo::clientInfo(){
    this->id = NULL;
    this->balance = NULL;
    this->type = NULL;
    this->wordToSearch = "";
    this->duration = 0.0;
}
clientInfo::clientInfo(int id, int balance, int type, std::string wordToSearch){
    this->id = id;
    this->balance = balance;
    this->type = type;
    this->wordToSearch = wordToSearch;
    this->duration = 0.0;
}
int clientInfo::getId(){
    return this->id;
}
int clientInfo::getBalance(){
    return this->balance;
}
int clientInfo::getType(){
    return this->type;
}
std::string clientInfo::getWordToSearch(){
    return this->wordToSearch;
}
std::vector<threadData> clientInfo::getResult(){
    return this->result;
}
void clientInfo::setId(int id){
    this->id = id;
}
void clientInfo::setBalance(int balance){
    this->balance = balance;
}
void clientInfo::setType(int type){
    this->type = type;
}
void clientInfo::setWordToSearch(std::string wordToSearch){
    this->wordToSearch = wordToSearch;
}
void clientInfo::setResult(std::vector<threadData> result){
    this->result = result;
}
float clientInfo::getDuration(){
    return this->duration;
}
void clientInfo::setDuration(float duration){
    this->duration = duration;
}
