#include "NetworkManager.hpp"
#include <iostream>
#include <chrono>
#include <thread>

NetworkManager::NetworkManager()
    : running_(false), connected_(false), role_(NetworkRole::None) {
    socket_.setBlocking(false);
    listener_.setBlocking(false);
}

NetworkManager::~NetworkManager() {
    stop();
}

bool NetworkManager::startServer(unsigned short port) {
    if (running_) return false;

    if (listener_.listen(port, sf::IpAddress::Any) != sf::Socket::Status::Done) {
        std::cerr << "[Network] listen failed on port " << port << "\n";
        return false;
    }

    std::cout << "[Network] listening on port " << port << " (waiting for client)...\n";

    listener_.setBlocking(true);
    auto status = listener_.accept(socket_);
    if (status != sf::Socket::Status::Done) {
        std::cerr << "[Network] accept failed with status: " << static_cast<int>(status) << "\n";
        return false;
    }

    std::cout << "[Network] client connected successfully\n";
    
    // Simple connection test
    sf::Packet testPacket;
    testPacket << static_cast<uint8_t>(NetworkMessage::ConnectionTest);
    if (socket_.send(testPacket) != sf::Socket::Status::Done) {
        std::cerr << "[Network] initial test failed\n";
        socket_.disconnect();
        return false;
    }

    socket_.setBlocking(false);
    listener_.setBlocking(false);

    role_ = NetworkRole::Server;
    connected_ = true;
    running_ = true;

    receiverThread_ = std::thread(&NetworkManager::receiverLoop, this);
    std::this_thread::sleep_for(std::chrono::milliseconds(300));
    
    std::cout << "[Network] server ready for game communication\n";
    return true;
}

bool NetworkManager::connectToServer(const std::string& ip, unsigned short port) {
    if (running_) return false;

    auto addr = sf::IpAddress::resolve(ip);
    if (!addr) {
        std::cerr << "[Network] Failed to resolve IP: " << ip << "\n";
        return false;
    }

    std::cout << "[Network] attempting to connect to " << ip << ":" << port << "\n";
    
    socket_.setBlocking(true);
    auto status = socket_.connect(addr.value(), port, sf::seconds(5));
    if (status == sf::Socket::Status::Done) {
        std::cout << "[Network] TCP connection established\n";
        
        // Wait for server's connection test
        sf::Packet testPacket;
        if (socket_.receive(testPacket) == sf::Socket::Status::Done) {
            uint8_t type;
            if (testPacket >> type && type == static_cast<uint8_t>(NetworkMessage::ConnectionTest)) {
                std::cout << "[Network] connection test successful\n";
            }
        }
        
        socket_.setBlocking(false);
        
        role_ = NetworkRole::Client;
        connected_ = true;
        running_ = true;
        
        std::cout << "[Network] starting client receiver thread...\n";
        receiverThread_ = std::thread(&NetworkManager::receiverLoop, this);
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
        
        std::cout << "[Network] client ready for game communication\n";
        return true;
    } else {
        std::cerr << "[Network] connection failed with status: " << static_cast<int>(status) << "\n";
        return false;
    }
}

void NetworkManager::stop() {
    if (!running_) return;
    running_ = false;

    if (receiverThread_.joinable())
        receiverThread_.join();

    if (connected_) {
        socket_.disconnect();
        connected_ = false;
    }
    role_ = NetworkRole::None;
}

bool NetworkManager::sendPacket(sf::Packet& p) {
    if (!connected_) return false;
    auto result = socket_.send(p);
    if (result != sf::Socket::Status::Done) {
        if (result == sf::Socket::Status::Disconnected) {
            std::cerr << "[Network] disconnected while sending\n";
            connected_ = false;
        }
        return false;
    }
    return true;
}

bool NetworkManager::pollPacket(sf::Packet& out) {
    std::lock_guard<std::mutex> lock(incomingMutex_);
    if (incoming_.empty()) return false;
    out = incoming_.front();
    incoming_.pop();
    return true;
}

void NetworkManager::receiverLoop() {
    bool isServer = (role_ == NetworkRole::Server);
    std::cout << "[Network] receiver thread started (" << (isServer ? "server" : "client") << ")\n";

    while (running_ && connected_) {
        sf::Packet packet;
        auto result = socket_.receive(packet);
        
        if (result == sf::Socket::Status::Done) {
            std::lock_guard<std::mutex> lock(incomingMutex_);
            incoming_.push(packet);
        } else if (result == sf::Socket::Status::NotReady) {
            std::this_thread::sleep_for(std::chrono::milliseconds(5));
        } else if (result == sf::Socket::Status::Disconnected) {
            std::cout << "[Network] peer disconnected\n";
            connected_ = false;
            break;
        } else if (result == sf::Socket::Status::Error) {
            std::cerr << "[Network] socket error in receiver\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }
    
    std::cout << "[Network] receiver thread stopped\n";
}

bool NetworkManager::sendPlayerState(const sf::Vector2f& position, const sf::Vector2f& velocity) {
    sf::Packet packet;
    packet << static_cast<uint8_t>(NetworkMessage::PlayerState)
           << position.x << position.y
           << velocity.x << velocity.y;
    return sendPacket(packet);
}

bool NetworkManager::sendGameOver() {
    sf::Packet packet;
    packet << static_cast<uint8_t>(NetworkMessage::GameOver);
    return sendPacket(packet);
}

bool NetworkManager::sendPlayerWin() {
    sf::Packet packet;
    packet << static_cast<uint8_t>(NetworkMessage::PlayerWin);
    return sendPacket(packet);
}

bool NetworkManager::sendItemCollected(int itemId, const sf::Vector2f& position) {
    sf::Packet packet;
    packet << static_cast<uint8_t>(NetworkMessage::ItemCollected)
           << itemId << position.x << position.y;
    return sendPacket(packet);
}

bool NetworkManager::sendEnemyDefeated(int enemyId, const sf::Vector2f& position) {
    sf::Packet packet;
    packet << static_cast<uint8_t>(NetworkMessage::EnemyDefeated)
           << enemyId << position.x << position.y;
    return sendPacket(packet);
}

std::unique_ptr<NetworkMessage> NetworkManager::pollMessage() {
    sf::Packet packet;
    if (!pollPacket(packet)) {
        return nullptr;
    }

    auto msg = std::make_unique<NetworkMessage>();
    uint8_t type;
    
    if (!(packet >> type)) {
        std::cerr << "[Network] failed to extract message type\n";
        return nullptr;
    }
    
    msg->type = static_cast<NetworkMessage::Type>(type);
    
    switch (msg->type) {
        case NetworkMessage::PlayerState:
            if (!(packet >> msg->position.x >> msg->position.y >> msg->velocity.x >> msg->velocity.y)) {
                std::cerr << "[Network] failed to extract player state data\n";
                return nullptr;
            }
            break;
            
        case NetworkMessage::ItemCollected:
            if (!(packet >> msg->itemId >> msg->position.x >> msg->position.y)) {
                std::cerr << "[Network] failed to extract item collected data\n";
                return nullptr;
            }
            break;
            
        case NetworkMessage::EnemyDefeated:
            if (!(packet >> msg->enemyId >> msg->position.x >> msg->position.y)) {
                std::cerr << "[Network] failed to extract enemy defeated data\n";
                return nullptr;
            }
            break;
            
        case NetworkMessage::GameOver:
            int deadId;
            if (!(packet >> deadId)) return nullptr;
            msg->playerId = deadId;
            break;
        case NetworkMessage::PlayerWin:
        case NetworkMessage::ConnectionTest:
            // No additional data needed for these message types
            break;
            
        default:
            std::cerr << "[Network] unknown message type: " << static_cast<int>(type) << "\n";
            return nullptr;
    }
    
    return msg;
}