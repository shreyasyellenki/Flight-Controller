#include "Control.hpp"
#include "SharedData.hpp"
#include <thread>
#include <chrono>
#include <iostream>

void controlThread(MessageQueue<SensorMessage>& sensorQueue,
                   MessageQueue<ControlCommand>& commandQueue) {
    const double targetPitch = 0.0, targetRoll = 0.0, targetYaw = 0.0;

    while (running) {
        SensorMessage data = sensorQueue.pop();

        // Simple proportional control
        ControlCommand cmd;
        cmd.pitchCmd = -(data.pitch - targetPitch) * 0.1;
        cmd.rollCmd  = -(data.roll  - targetRoll)  * 0.1;
        cmd.yawCmd   = -(data.yaw   - targetYaw)   * 0.1;

        commandQueue.push(cmd);

        std::cout << "[CTRL] PitchCmd: " << cmd.pitchCmd
                  << " RollCmd: " << cmd.rollCmd
                  << " YawCmd: " << cmd.yawCmd << "\n";

        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
}