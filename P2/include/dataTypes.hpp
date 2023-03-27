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
        std::queue<SearchResult> queue_words;
    public:
        threadData(int id, int start, int end);
        int getid();
        int getstart();
        int getend();
        std::queue<SearchResult> getQueue();
        void setid(int id);
        void setstart(int start);
        void setend(int end);
        void queueWord(SearchResult w);
        SearchResult dequeueWord();
};
#endif
