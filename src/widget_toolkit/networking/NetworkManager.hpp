// network/NetworkManager.hpp
#pragma once
#include <SFML/Network.hpp>
#include <SFML/System/Vector2.hpp>
#include <thread>
#include <atomic>
#include <mutex>
#include <queue>
#include <optional>
#include <memory>

enum class NetworkRole { None, Server, Client };

// Add this struct:
struct NetworkMessage {
    enum Type : uint8_t {
        PlayerState = 1,
        GameOver = 2,
        PlayerWin = 3,
        ItemCollected = 4,
        EnemyDefeated = 5,
        ConnectionTest = 6,
        EnemyState = 7,
        EnemyAuthority = 8
    };
    
    Type type;
    sf::Vector2f position;
    sf::Vector2f velocity;
    sf::Vector2f enemyPosition;
    sf::Vector2f enemyVelocity;
    int itemId = -1;
    int enemyNetworkId = -1;
    int enemyId = -1;
    int playerId = -1;  // Add player ID for game over/win messages
    bool isAlive = true;
    bool isActive = true;
    uint64_t timestamp = 0;
    std::string spriteId;
    bool faceForward = true;
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
    bool sendItemCollected(int itemId, const sf::Vector2f& position);
    bool sendEnemyDefeated(int enemyNetworkId, const sf::Vector2f& position);
    bool sendEnemyState(int enemyId, const sf::Vector2f& pos, const sf::Vector2f& vel, bool alive,
                                    bool active, const std::string& spriteId, bool faceForward);
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
