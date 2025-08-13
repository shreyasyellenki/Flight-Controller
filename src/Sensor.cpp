#include "SharedData.hpp"
#include <random>
#include <thread>
#include <chrono>
#include <cmath>

SensorData sensorData{};
std::mutex dataMutex;
bool running = true;

void sensorThread() {
    std::default_random_engine gen(std::random_device{}());
    std::normal_distribution<double> noise(0.0, 0.5);
    double t = 0.0;

    while (running) {
        {
            std::lock_guard<std::mutex> lock(dataMutex);
            sensorData.pitch = 10 * std::sin(t) + noise(gen);
            sensorData.roll  = 5 * std::cos(t) + noise(gen);
            sensorData.yaw   = fmod(t * 5, 360.0) + noise(gen);
        }
        t += 0.1;
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
}
