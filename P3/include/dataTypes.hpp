/*This file contains the definition of the ThreadData class and all its methods. 
It also includes de definition of the data structure of Search Result*/

#ifndef DATATYPES.H
#define DATATYPES.H
#include<queue>
#include<string>
struct SearchResult
{
    int line;
    std::string previousWord;
    std::string nextWord;
    std::string consideredWord;
    std::string fileName; //Name of the file where the word was found
};
class threadData {
    private:
        int id;
        int start;
        int end;
        std::string wordToFind;
        std::queue<SearchResult> resultsQueue;
    public:
        threadData(int id, int start, int end, std::string wordToFind);
        int getId();
        int getStart();
        int getEnd();
        std::string getWordToFind();
        std::queue<SearchResult> getQueue();
        void setId(int id);
        void setStart(int start);
        void setEnd(int end);
        void queueResult(SearchResult w);
        SearchResult dequeueResult();
        bool isEmptyList();
};
#endif
