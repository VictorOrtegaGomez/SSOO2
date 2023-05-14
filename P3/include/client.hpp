#ifndef CLIENT.H
#define CLIENT.H
#include<string>
#include "dataTypes.hpp"

class clientInfo {
    private:
        int id;
        int balance;
        int type;
        std::string wordToSearch;
        std::vector<threadData> result;
        float duration;

    public:
        clientInfo();
        clientInfo(int id, int balance, int type, std::string wordToSearch);
        void setResult(std::vector<threadData> result);
        std::vector<threadData> getResult();
        int getId();
        int getBalance();
        int getType();
        std::string getWordToSearch();
        void setId(int id);
        void setBalance(int balance);
        void setType(int type);
        void setWordToSearch(std::string wordToSearch);
        float getDuration();
        void setDuration(float duration);
};

#endif