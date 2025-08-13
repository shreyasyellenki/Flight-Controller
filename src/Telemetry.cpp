#include "Telemetry.hpp"
#include "SharedData.hpp"
#include <thread>
#include <chrono>
#include <iostream>

void telemetryThread(MessageQueue<ControlCommand>& commandQueue) {
    while (running) {
        ControlCommand cmd = commandQueue.pop();

        std::cout << "[TEL] PitchCmd: " << cmd.pitchCmd
                  << " RollCmd: " << cmd.rollCmd
                  << " YawCmd: " << cmd.yawCmd << "\n";

        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }
}