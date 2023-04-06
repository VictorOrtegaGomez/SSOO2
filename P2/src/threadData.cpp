#include "../include/dataTypes.hpp"
threadData::threadData(int id, int start, int end){
    this->id = id;
    this->end = end;
    this->start = start;
}
void threadData::setid(int id){
    this->id = id;
}
void threadData::setend(int end){
    this->end = end;
}
void threadData::setstart(int start){
    this->start = start;
}
int threadData::getid(){
    return this->id;
}
int threadData::getend(){
    return this->end;
}
int threadData::getstart(){
    return this->start;
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