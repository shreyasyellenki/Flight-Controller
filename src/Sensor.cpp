#include "Sensor.hpp"
#include "SharedData.hpp"
#include <random>
#include <chrono>
#include <thread>

void sensorThread(MessageQueue<StateMessage>& stateQueue,
                  MessageQueue<SensorMessage>& sensorQueue) {
    std::default_random_engine gen(std::random_device{}());
    std::normal_distribution<double> n_angle(0.0, 0.01); // ~0.57 deg std

    while (running) {
        auto maybeState = stateQueue.pop();
        if (!maybeState) break; // queue closed

        const auto& s = *maybeState;
        SensorMessage meas{
            s.roll  + n_angle(gen),
            s.pitch + n_angle(gen),
            s.yaw   + n_angle(gen)
        };
        sensorQueue.push(meas);

        std::this_thread::sleep_for(std::chrono::milliseconds(50)); // 20 Hz sensor rate
    }
    sensorQueue.close();
}