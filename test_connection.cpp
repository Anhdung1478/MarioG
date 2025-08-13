#include <SFML/Network.hpp>
#include <iostream>
#include <thread>
#include <chrono>

void testServer() {
    std::cout << "Starting server on port 54000...\n";
    
    sf::TcpListener listener;
    if (listener.listen(54000, sf::IpAddress::Any) != sf::Socket::Status::Done) {
        std::cerr << "Failed to listen on port 54000\n";
        return;
    }
    
    std::cout << "Server listening... waiting for client\n";
    
    sf::TcpSocket client;
    if (listener.accept(client) != sf::Socket::Status::Done) {
        std::cerr << "Failed to accept client\n";
        return;
    }
    
    std::cout << "Client connected! Remote address: " << client.getRemoteAddress().toString() << "\n";
    
    // Test sending/receiving
    for (int i = 0; i < 10; i++) {
        sf::Packet packet;
        packet << "Hello from server " << i;
        
        if (client.send(packet) != sf::Socket::Status::Done) {
            std::cerr << "Failed to send packet " << i << "\n";
            break;
        }
        
        std::cout << "Sent packet " << i << "\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
    
    std::cout << "Server test complete\n";
}

void testClient() {
    std::cout << "Connecting to 192.168.1.40:54000...\n";
    
    sf::TcpSocket socket;
    auto status = socket.connect(sf::IpAddress("192.168.1.40"), 54000, sf::seconds(5));
    
    if (status != sf::Socket::Status::Done) {
        std::cerr << "Failed to connect: " << static_cast<int>(status) << "\n";
        return;
    }
    
    std::cout << "Connected! Local address: " << socket.getLocalAddress().toString() << "\n";
    
    // Test receiving
    socket.setBlocking(false);
    
    for (int i = 0; i < 10; i++) {
        sf::Packet packet;
        auto result = socket.receive(packet);
        
        if (result == sf::Socket::Status::Done) {
            std::string message;
            packet >> message;
            std::cout << "Received: " << message << "\n";
        } else if (result == sf::Socket::Status::NotReady) {
            std::cout << "No data available (attempt " << i << ")\n";
        } else {
            std::cerr << "Receive error: " << static_cast<int>(result) << "\n";
            break;
        }
        
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
    
    std::cout << "Client test complete\n";
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "Usage: " << argv[0] << " [server|client]\n";
        return 1;
    }
    
    std::string mode = argv[1];
    
    if (mode == "server") {
        testServer();
    } else if (mode == "client") {
        testClient();
    } else {
        std::cout << "Invalid mode. Use 'server' or 'client'\n";
        return 1;
    }
    
    return 0;
}
