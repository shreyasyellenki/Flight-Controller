#include "Sensor.hpp"
#include "SharedData.hpp"
#include <random>
#include <thread>
#include <chrono>
#include <cmath>

bool running = true;

void sensorThread(MessageQueue<SensorMessage>& sensorQueue) {
    std::default_random_engine gen(std::random_device{}());
    std::normal_distribution<double> noise(0.0, 0.5);
    double t = 0.0;

    while (running) {
        SensorMessage msg;
        msg.pitch = 10 * std::sin(t) + noise(gen);
        msg.roll  = 5 * std::cos(t) + noise(gen);
        msg.yaw   = fmod(t * 5, 360.0) + noise(gen);

        sensorQueue.push(msg);
        t += 0.1;

        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
}
