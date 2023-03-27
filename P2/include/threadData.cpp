#include "dataTypes.hpp"
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
    return this->queue_words;
}
void threadData::queueWord(SearchResult w){
    this->queue_words.push(w);
}
SearchResult threadData::dequeueWord(){
    SearchResult w = this->queue_words.front();
    this->queue_words.pop();
    return w;
}