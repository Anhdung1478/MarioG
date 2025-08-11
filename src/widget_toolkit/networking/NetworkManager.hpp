// network/NetworkManager.hpp
#pragma once
#include <SFML/Network.hpp>
#include <thread>
#include <atomic>
#include <mutex>
#include <queue>
#include <optional>
#include <memory>

enum class NetworkRole { None, Server, Client };

// Add this struct:
struct NetworkMessage {
    enum Type { PlayerState, GameOver, PlayerWin } type;
    sf::Vector2f position;
    sf::Vector2f velocity;
};

class NetworkManager {
public:
    NetworkManager();
    ~NetworkManager();

    // Server: listen on port and accept a single client (blocking accept during start)
    bool startServer(unsigned short port);

    // Client: connect to server ip:port (blocking connect)
    bool connectToServer(const std::string& ip, unsigned short port);

    // Stop networking & join threads
    void stop();

    // Thread-safe send of a packet
    bool sendPacket(sf::Packet& p);

    // Poll an incoming packet (non-blocking). Returns true if a packet popped.
    bool pollPacket(sf::Packet& out);

    NetworkRole role() const { return role_; }
    bool isConnected() const { return connected_.load(); }

    bool sendPlayerState(const sf::Vector2f& position, const sf::Vector2f& velocity);
    std::unique_ptr<NetworkMessage> pollMessage();
    bool sendGameOver();
    bool sendPlayerWin();

private:
    void receiverLoop();

    // thread-safe queue
    std::queue<sf::Packet> incoming_;
    std::mutex incomingMutex_;

    sf::TcpListener listener_;
    sf::TcpSocket  socket_; // single socket used for either client or accepted server client
    std::thread receiverThread_;
    std::atomic<bool> running_;
    std::atomic<bool> connected_;
    NetworkRole role_;
};
