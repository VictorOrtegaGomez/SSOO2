#ifndef CLIENT.H
#define CLIENT.H
#include<string>

class client {
    private:
        int id;
        int balance;
        int type;
        std::string wordToSearch;

    public:
        client();
        client(int id, int balance, int type, std::string wordToSearch);
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