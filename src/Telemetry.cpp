#include "SharedData.hpp"
#include <iostream>
#include <thread>
#include <chrono>

void telemetryThread() {
    while (running) {
        SensorData dataCopy;
        {
            std::lock_guard<std::mutex> lock(dataMutex);
            dataCopy = sensorData;
        }

        std::cout << "[TEL] Pitch: " << dataCopy.pitch
                  << " Roll: " << dataCopy.roll
                  << " Yaw: " << dataCopy.yaw << "\n";

        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }
}