#include "Control.hpp"
#include "SharedData.hpp"
#include <chrono>
#include <thread>
#include <iostream>
#include <cmath> 

// Convert degrees to radians
constexpr double deg2rad(double deg) { return deg * M_PI / 180.0; }

void controlThread(MessageQueue<SensorMessage>& sensorQueue,
                   MessageQueue<ControlCommand>& commandQueue) {

    // PID gains for roll, pitch, yaw 
    const double Kp_roll = 2.0, Ki_roll = 0.5, Kd_roll = 1.0;
    const double Kp_pitch = 2.0, Ki_pitch = 0.5, Kd_pitch = 1.0;
    const double Kp_yaw = 1.0, Ki_yaw = 0.2, Kd_yaw = 0.5;

    double integral_roll = 0.0, integral_pitch = 0.0, integral_yaw = 0.0;

    // Initial target angles (radians)
    double target_roll = 0.0;
    double target_pitch = 0.0;
    double target_yaw = 0.0;

    auto start_time = std::chrono::steady_clock::now();

    // To store previous errors for derivative calculation
    double prev_error_roll = 0.0;
    double prev_error_pitch = 0.0;
    double prev_error_yaw = 0.0;

    const double dt = 0.05; // control loop period (seconds)

    while (running) {
        auto maybeMeas = sensorQueue.pop();
        if (!maybeMeas) break; // queue closed
        const auto& z = *maybeMeas;

        // Step command: after 2 seconds, command roll = 10 degrees
        auto elapsed = std::chrono::steady_clock::now() - start_time;
        if (elapsed > std::chrono::seconds(2)) {
            target_roll = deg2rad(10.0);
        }

        // Calculate errors
        double error_roll = target_roll - z.roll;
        double error_pitch = target_pitch - z.pitch;
        double error_yaw = target_yaw - z.yaw;

        // Update integrals (simple rectangle integration)
        integral_roll += error_roll * dt;
        integral_pitch += error_pitch * dt;
        integral_yaw += error_yaw * dt;

        // Calculate derivatives (rate of error change)
        double derivative_roll = (error_roll - prev_error_roll) / dt;
        double derivative_pitch = (error_pitch - prev_error_pitch) / dt;
        double derivative_yaw = (error_yaw - prev_error_yaw) / dt;

        // PID output torque commands
        ControlCommand u{
            Kp_roll * error_roll + Ki_roll * integral_roll + Kd_roll * derivative_roll,
            Kp_pitch * error_pitch + Ki_pitch * integral_pitch + Kd_pitch * derivative_pitch,
            Kp_yaw * error_yaw + Ki_yaw * integral_yaw + Kd_yaw * derivative_yaw
        };

        commandQueue.push(u);

        // Save errors for next iteration
        prev_error_roll = error_roll;
        prev_error_pitch = error_pitch;
        prev_error_yaw = error_yaw;

        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
    commandQueue.close();
}