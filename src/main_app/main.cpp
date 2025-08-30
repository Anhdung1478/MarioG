#include <bits/stdc++.h>
#include "main-window.hpp"
#include "../widget_toolkit/networking/NetworkManager.hpp"
/*#include <windows.h>
#include <psapi.h>*/
#include "../widget_toolkit/controls/button-list.hpp"

void printMemoryUsage() {
    /*PROCESS_MEMORY_COUNTERS_EX pmc;
    if (GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc))) {
        // WorkingSetSize is the physical memory (RAM) currently in use by the process.
        // It's a good indicator of actual RAM consumption.
        double workingSetSizeMB = static_cast<double>(pmc.WorkingSetSize) / (1024 * 1024);
        
        // PrivateUsage is the amount of virtual memory that is currently committed 
        // to the process. This memory is not shared with other processes.
        double privateUsageMB = static_cast<double>(pmc.PrivateUsage) / (1024 * 1024);

        std::cout << "Current Process Memory Usage:" << std::endl;
        std::cout << "  Working Set (RAM): " << workingSetSizeMB << " MB" << std::endl;
        std::cout << "  Private Bytes (Virtual): " << privateUsageMB << " MB" << std::endl;
    } else {
        std::cerr << "Failed to get process memory info. Error code: " << GetLastError() << std::endl;
    }*/
}

void initalizeForStaticMember() {
    sf::Texture *button_texture = new sf::Texture();
    if(!button_texture->loadFromFile("asset/sprites/main_menu_button_mushroom.png")) {
        std::cerr << "Could not load texture for main menu button mushroom !!\n";
    }
    
    mario::Button::p_texture = button_texture;
}

int main(int argc, char* argv[]) {
    initalizeForStaticMember();

    NetworkManager network;

    if (argc >= 2) {
        std::string mode = argv[1];
        if (mode == "server" && argc == 3) {
            unsigned short port = static_cast<unsigned short>(std::stoi(argv[2]));
            if (!network.startServer(port)) {
                std::cerr << "Failed to start server\n";
                return 1;
            }
            std::cout << "Server started on port " << port << std::endl;
        } 
        else if (mode == "client" && argc == 4) {
            std::string ip = argv[2];
            unsigned short port = static_cast<unsigned short>(std::stoi(argv[3]));
            if (!network.connectToServer(ip, port)) {
                std::cerr << "Failed to connect to server\n";
                return 1;
            }
            std::cout << "Connected to " << ip << ":" << port << std::endl;
        } 
        else {
            std::cout << "Usage:\n";
            std::cout << "  MARIOG.exe server <port>\n";
            std::cout << "  MARIOG.exe client <ip> <port>\n";
            return 0;
        }
    }

    // Always run the game loop after setting up network
    mario::MainWindow mainWindow; 
    mainWindow.run();

    // Memory usage after window closes (resources might be deallocated)
    std::cout << "\n--- Memory Usage After Window Closes ---" << std::endl;
    printMemoryUsage();
    std::cout << "--------------------------------------" << std::endl;
    
    // Cleanup
    network.stop();

    return 0;
}