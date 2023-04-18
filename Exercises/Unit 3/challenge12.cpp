#include <iostream>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <queue>
#include <chrono>
#include <ctime>

struct request{
    int id;
    int balance;
    std::mutex *semaphore;
};


std::queue <request*> requests;
std::condition_variable cv;

void paymentService(){
    std::mutex mtx;
        
    while(1){
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, []{return !requests.empty();});
        request *req = requests.front();
        requests.pop();
        req->balance -= 100;
        std::cout <<  "Id Cliente: " << req->id << " Saldo: " << req->balance << std::endl;
        req->semaphore->unlock();
    }
}

void sendRequest(int id, int balance, int sleepTime){
    std::mutex mtx;
    mtx.lock();

    srand(time(NULL));

    request req;
    req.id = id;
    req.balance = balance;
    req.semaphore = &mtx;

    while(req.balance > 0){
        requests.push(&req);
        cv.notify_one();
        mtx.lock();
        std::this_thread::sleep_for(std::chrono::milliseconds(sleepTime));
    }

}

int main(){
    std::thread t_paymentService(paymentService);
    std::thread t_sendRequest1(sendRequest, 1, 1000, 3000);
    std::thread t_sendRequest2(sendRequest, 2, 2000, 2000);
    std::thread t_sendRequest3(sendRequest, 3, 3000, 1000);

    t_paymentService.join();
    t_sendRequest1.join();
    t_sendRequest2.join();
    t_sendRequest3.join();

    return 0;
}