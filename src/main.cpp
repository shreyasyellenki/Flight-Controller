#include "Sensor.hpp"
#include "Control.hpp"
#include "Telemetry.hpp"
#include "SharedData.hpp"

#include <thread>
#include <chrono>

int main() {
    MessageQueue<SensorMessage> sensorQueue;
    MessageQueue<ControlCommand> commandQueue;

    std::thread t1(sensorThread, std::ref(sensorQueue));
    std::thread t2(controlThread, std::ref(sensorQueue), std::ref(commandQueue));
    std::thread t3(telemetryThread, std::ref(commandQueue));

    std::this_thread::sleep_for(std::chrono::seconds(5)); 
    running = false;

    t1.join();
    t2.join();
    t3.join();
}