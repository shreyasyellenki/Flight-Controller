#pragma once

struct StateMessage {
    // Euler angles (radians) in body->inertial 3-2-1 (yaw-pitch-roll) convention
    double roll;   // phi
    double pitch;  // theta
    double yaw;    // psi
    double p, q, r;
};

struct SensorMessage {
    // Measured angles (noisy)
    double roll;
    double pitch;
    double yaw;
};

struct ControlCommand {
    // Commanded body torques (N·m)
    double tau_x;
    double tau_y;
    double tau_z;
};
