#ifndef DATATYPES.H
#define DATATYPES.H
#include<queue>
#include<string>
struct SearchResult
{
    int line;
    std::string previousWord;
    std::string nextWord;
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
        int getid();
        int getstart();
        int getend();
        std::string getWordToFind();
        std::queue<SearchResult> getQueue();
        void setid(int id);
        void setstart(int start);
        void setend(int end);
        void queueResult(SearchResult w);
        SearchResult dequeueResult();
};
#endif
