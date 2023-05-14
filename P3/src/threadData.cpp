/*This file contains the thredData class and all its methods*/

#include "../include/dataTypes.hpp"

threadData::threadData(int id, int start, int end, std::string wordToFind){
    this->id = id;
    this->end = end;
    this->start = start;
    this->wordToFind = wordToFind;
}
void threadData::setId(int id){
    this->id = id;
}
void threadData::setEnd(int end){
    this->end = end;
}
void threadData::setStart(int start){
    this->start = start;
}
int threadData::getId(){
    return this->id;
}
int threadData::getEnd(){
    return this->end;
}
int threadData::getStart(){
    return this->start;
}
std::string threadData::getWordToFind(){
    return this->wordToFind;
}
std::queue<SearchResult> threadData::getQueue(){
    return this->resultsQueue;
}
void threadData::queueResult(SearchResult w){
    this->resultsQueue.push(w);
}
SearchResult threadData::dequeueResult(){
    SearchResult w = this->resultsQueue.front();
    this->resultsQueue.pop();
    return w;
}
bool threadData::isEmptyList(){
    return this->resultsQueue.empty();
}