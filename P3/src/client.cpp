#include "../include/client.hpp"

clientInfo::clientInfo(){
    this->id = NULL;
    this->balance = NULL;
    this->type = NULL;
    this->wordToSearch = "";
}
clientInfo::clientInfo(int id, int balance, int type, std::string wordToSearch){
    this->id = id;
    this->balance = balance;
    this->type = type;
    this->wordToSearch = wordToSearch;
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
