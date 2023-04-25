#ifndef CLIENT.H
#define CLIENT.H
#include<string>

class clientInfo {
    private:
        int id;
        int balance;
        int type;
        std::string wordToSearch;

    public:
        clientInfo();
        clientInfo(int id, int balance, int type, std::string wordToSearch);
        int getId();
        int getBalance();
        int getType();
        std::string getWordToSearch();
        void setId(int id);
        void setBalance(int balance);
        void setType(int type);
        void setWordToSearch(std::string wordToSearch);
};

#endif