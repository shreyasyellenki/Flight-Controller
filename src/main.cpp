#include "Dynamics.hpp"
#include "Sensor.hpp"
#include "Control.hpp"
#include "Telemetry.hpp"
#include "SharedData.hpp"
#include <thread>
#include <chrono>

int main() {
    MessageQueue<ControlCommand> commandQueue;         // Control -> Dynamics
    MessageQueue<StateMessage>   stateForSensorQueue;  // Dynamics -> Sensor
    MessageQueue<StateMessage>   stateForTelQueue;     // Dynamics -> Telemetry
    MessageQueue<SensorMessage>  sensorQueue;          // Sensor  -> Control

    std::thread td(dynamicsThread, std::ref(commandQueue),
                   std::ref(stateForSensorQueue), std::ref(stateForTelQueue));
    std::thread ts(sensorThread, std::ref(stateForSensorQueue), std::ref(sensorQueue));
    std::thread tc(controlThread, std::ref(sensorQueue), std::ref(commandQueue));
    std::thread tt(telemetryThread, std::ref(stateForTelQueue), std::ref(commandQueue));

    std::this_thread::sleep_for(std::chrono::seconds(8)); // let it fly
    running = false;

    // Close producers to unblock pops
    commandQueue.close();
    stateForSensorQueue.close();
    stateForTelQueue.close();
    sensorQueue.close();

    td.join(); ts.join(); tc.join(); tt.join();
    return 0;
}
