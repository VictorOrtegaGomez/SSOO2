#include <iostream>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <queue>
#include <chrono>


int bufferSize = 5;
std::queue <int> items;
std::mutex empty;
std::mutex full;
std::condition_variable cvConsumer;
std::condition_variable cvProducer;

void producer(int numItems){
    for(int i = 0; i < numItems; i++){
        std::unique_lock<std::mutex> lock(empty);
        cvProducer.wait(lock, []{return items.size() < bufferSize;});
        items.push(i);
        std::cout << "Produced item: " << i << std::endl;
        full.unlock();
        cvConsumer.notify_one();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void consumer(int numItems){
    for(int i = 0; i < numItems; i++){
        std::unique_lock<std::mutex> lock(full);
        cvConsumer.wait(lock, []{return items.size() > 0;});
        int item = items.front();
        items.pop();
        std::cout << "Consumed item: " << item << std::endl;
        empty.unlock();
        cvProducer.notify_one();
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
}

int main(){
    int numItems = 10;

    std::thread t_producer(producer, numItems);
    std::thread t_consumer(consumer, numItems);

    t_producer.join();
    t_consumer.join();

    return 0;
}