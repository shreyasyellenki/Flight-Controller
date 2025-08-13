#include "Telemetry.hpp"
#include "SharedData.hpp"
#include <chrono>
#include <thread>
#include <iostream>

void telemetryThread(MessageQueue<StateMessage>& telemetryStateQueue,
                     MessageQueue<ControlCommand>& commandQueue) {
    StateMessage lastState{};
    ControlCommand lastCmd{};

    while (running) {
        for (;;) { auto s = telemetryStateQueue.try_pop(); if (!s) break; lastState = *s; }
        for (;;) { auto c = commandQueue.try_pop();       if (!c) break; lastCmd   = *c; }

        std::cout << "[TEL] roll: " << lastState.roll
                  << " pitch: " << lastState.pitch
                  << " yaw: " << lastState.yaw
                  << " | pqr: (" << lastState.p << "," << lastState.q << "," << lastState.r << ")"
                  << " | tau: (" << lastCmd.tau_x << "," << lastCmd.tau_y << "," << lastCmd.tau_z << ")"
                  << std::endl;

        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }
}
