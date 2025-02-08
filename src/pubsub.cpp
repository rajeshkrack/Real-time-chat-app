#include <iostream>
#include <vector>
#include <mutex>
#include <thread>

class PubSub {
private:
    std::vector<std::string> messages;
    std::mutex mtx;
public:
    void publish(const std::string& msg) {
        std::lock_guard<std::mutex> lock(mtx);
        messages.push_back(msg);
    }

    void subscribe() {
        while (true) {
            std::lock_guard<std::mutex> lock(mtx);
            if (!messages.empty()) {
                std::cout << "New Message: " << messages.back() << std::endl;
                messages.pop_back();
            }
        }
    }
};

int main() {
    PubSub pubsub;
    
    std::thread pubThread([&](){ pubsub.publish("Hello Subscribers!"); });
    std::thread subThread([&](){ pubsub.subscribe(); });

    pubThread.join();
    subThread.join();
    
    return 0;
}
