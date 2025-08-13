#include "Telemetry.hpp"
#include "SharedData.hpp"
#include <chrono>
#include <thread>
#include <iostream>

#include <sys/socket.h>  
#include <arpa/inet.h>   
#include <unistd.h>      
#include <cstring>       


void telemetryThread(MessageQueue<StateMessage>& telemetryStateQueue,
                     MessageQueue<ControlCommand>& commandQueue) {
    // Create UDP socket
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        std::cerr << "[Telemetry] Failed to create UDP socket\n";
        return;
    }

    sockaddr_in servaddr{};
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(5005);             
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1"); 

    StateMessage lastState{};
    ControlCommand lastCmd{};

    while (running) {
        // Grab latest messages if available (non-blocking)
        for (;;) { auto s = telemetryStateQueue.try_pop(); if (!s) break; lastState = *s; }
        for (;;) { auto c = commandQueue.try_pop();       if (!c) break; lastCmd   = *c; }

        // Format data as CSV: roll,pitch,yaw,p,q,r,tau_x,tau_y,tau_z
        char buffer[256];
        snprintf(buffer, sizeof(buffer),
                 "%f,%f,%f,%f,%f,%f,%f,%f,%f",
                 lastState.roll, lastState.pitch, lastState.yaw,
                 lastState.p, lastState.q, lastState.r,
                 lastCmd.tau_x, lastCmd.tau_y, lastCmd.tau_z);

        // Send UDP packet
        ssize_t sent = sendto(sockfd, buffer, strlen(buffer), 0,
                              (const struct sockaddr*)&servaddr, sizeof(servaddr));
        if (sent < 0) {
            std::cerr << "[Telemetry] Failed to send UDP packet\n";
        }

        std::cout << "[TEL] " << buffer << std::endl;

        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }
    close(sockfd);
}
