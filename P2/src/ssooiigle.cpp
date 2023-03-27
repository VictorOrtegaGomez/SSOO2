#include<iostream>
#include<string>
#include "../include/dataTypes.hpp"
int main(){
    struct SearchResult word;
    struct SearchResult word2;
    word.line = 1;
    word.nextWord = "ajkdslfjakl";
    word.previousWord = "prev";
    threadData Miclass(0, 10, 100);
    Miclass.queueWord(word);
    std::cout << "ID de la hebra : " << Miclass.getid() << "Next word de la palabra buscada :\t" << Miclass.dequeueWord().previousWord << std::endl;
    return 0;
}
