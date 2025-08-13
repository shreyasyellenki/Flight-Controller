#include "Sensor.hpp"
#include "Control.hpp"
#include "Telemetry.hpp"
#include "SharedData.hpp"

#include <thread>
#include <chrono>

int main() {
    std::thread t1(sensorThread);
    std::thread t2(controlThread);
    std::thread t3(telemetryThread);

    std::this_thread::sleep_for(std::chrono::seconds(5)); // Run for 5 seconds
    running = false;

    t1.join();
    t2.join();
    t3.join();

    return 0;
}