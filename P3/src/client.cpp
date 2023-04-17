#include "../include/client.hpp"

client::client(){
    this->id = NULL;
    this->balance = NULL;
    this->type = NULL;
    this->wordToSearch = "";
}
client::client(int id, int balance, int type, std::string wordToSearch){
    this->id = id;
    this->balance = balance;
    this->type = type;
    this->wordToSearch = wordToSearch;
}
int client::getId(){
    return this->id;
}
int client::getBalance(){
    return this->balance;
}
int client::getType(){
    return this->type;
}
std::string client::getWordToSearch(){
    return this->wordToSearch;
}
void client::setId(int id){
    this->id = id;
}
void client::setBalance(int balance){
    this->balance = balance;
}
void client::setType(int type){
    this->type = type;
}
void client::setWordToSearch(std::string wordToSearch){
    this->wordToSearch = wordToSearch;
}
