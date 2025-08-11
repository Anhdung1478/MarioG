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

    // Bind to all interfaces to allow both localhost and LAN connections
    if (listener_.listen(port, sf::IpAddress::Any) != sf::Socket::Status::Done) {
        std::cerr << "[Network] listen failed on port " << port << "\n";
        return false;
    }

    std::cout << "[Network] listening on port " << port << " (waiting for client)...\n";

    // Block until a client connects, with timeout
    listener_.setBlocking(true);
    auto status = listener_.accept(socket_);
    if (status != sf::Socket::Status::Done) {
        std::cerr << "[Network] accept failed with status: " << static_cast<int>(status) << "\n";
        return false;
    }

    std::cout << "[Network] client connected successfully\n";
    
    // Test the connection immediately
    sf::Packet testPacket;
    testPacket << "PONG"; // Response to client's PING
    if (socket_.send(testPacket) != sf::Socket::Status::Done) {
        std::cerr << "[Network] initial response failed, connection unstable\n";
        socket_.disconnect();
        return false;
    }

    // Set socket to non-blocking for game communication
    socket_.setBlocking(false);
    listener_.setBlocking(false);

    role_ = NetworkRole::Server;
    connected_ = true;
    running_ = true;

    // Start the receiver thread
    receiverThread_ = std::thread(&NetworkManager::receiverLoop, this);
    
    // Give receiver thread time to start
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    
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
    
    // Try to connect with blocking socket first, with timeout
    socket_.setBlocking(true);
    auto status = socket_.connect(addr.value(), port, sf::seconds(5));
    if (status == sf::Socket::Status::Done) {
        std::cout << "[Network] TCP connection established\n";
        
        // Test the connection with a small ping
        sf::Packet testPacket;
        testPacket << "PING";
        if (socket_.send(testPacket) != sf::Socket::Status::Done) {
            std::cerr << "[Network] initial ping failed, connection unstable\n";
            socket_.disconnect();
            return false;
        }
        
        // Small delay to let connection stabilize
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        
        // Now set to non-blocking for game communication
        socket_.setBlocking(false);
        
        role_ = NetworkRole::Client;
        connected_ = true;
        running_ = true;
        
        std::cout << "[Network] starting client receiver thread...\n";
        
        // Start receiver thread
        receiverThread_ = std::thread(&NetworkManager::receiverLoop, this);
        
        // Give receiver thread time to start properly
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        
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

    // Give socket time to stabilize before starting receive loop
    std::this_thread::sleep_for(std::chrono::milliseconds(200));

    while (running_) {
        if (!connected_) {
            if (isServer) {
                // Server should re-listen for new connections
                std::cout << "[Network] waiting for new client connection...\n";
                listener_.setBlocking(true);
                if (listener_.accept(socket_) == sf::Socket::Status::Done) {
                    std::cout << "[Network] new client connected\n";
                    socket_.setBlocking(false);
                    connected_ = true;
                    // Give new connection time to stabilize
                    std::this_thread::sleep_for(std::chrono::milliseconds(300));
                } else {
                    std::this_thread::sleep_for(std::chrono::milliseconds(100));
                    continue;
                }
            } else {
                std::this_thread::sleep_for(std::chrono::milliseconds(50));
                continue;
            }
        }

        sf::Packet packet;
        auto result = socket_.receive(packet);
        if (result == sf::Socket::Status::Done) {
            // Handle ping/pong messages
            std::string message;
            if (packet >> message) {
                if (message == "PING" && isServer) {
                    sf::Packet pong;
                    pong << "PONG";
                    socket_.send(pong);
                    continue;
                } else if (message == "PONG" && !isServer) {
                    std::cout << "[Network] ping/pong successful\n";
                    continue;
                }
                // If not ping/pong, put the packet back for game logic
                packet.clear();
                packet << message;
            }
            
            std::cout << "[Network] received packet\n";
            std::lock_guard<std::mutex> lock(incomingMutex_);
            incoming_.push(packet);
        } else if (result == sf::Socket::Status::NotReady) {
            // No data available, sleep briefly
            std::this_thread::sleep_for(std::chrono::milliseconds(5));
        } else if (result == sf::Socket::Status::Disconnected) {
            std::cout << "[Network] peer disconnected (detected in receiver)\n";
            connected_ = false;
            socket_.disconnect();
            // Loop will go back to listening for new connections if server
        } else if (result == sf::Socket::Status::Error) {
            std::cerr << "[Network] socket error in receiver\n";
            // Don't immediately disconnect on error - might be temporary
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        } else {
            std::cerr << "[Network] receiver error: " << static_cast<int>(result) << "\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(20));
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

std::unique_ptr<NetworkMessage> NetworkManager::pollMessage() {
    sf::Packet packet;
    if (!pollPacket(packet)) {
        return nullptr;
    }

    auto msg = std::make_unique<NetworkMessage>();
    uint8_t type;
    
    // Try to extract the message type first
    if (!(packet >> type)) {
        std::cerr << "[Network] failed to extract message type\n";
        return nullptr;
    }
    
    msg->type = static_cast<NetworkMessage::Type>(type);
    
    // Only extract position/velocity for PlayerState messages
    if (msg->type == NetworkMessage::PlayerState) {
        if (!(packet >> msg->position.x >> msg->position.y >> msg->velocity.x >> msg->velocity.y)) {
            std::cerr << "[Network] failed to extract player state data\n";
            return nullptr;
        }
    }
    
    return msg;
}