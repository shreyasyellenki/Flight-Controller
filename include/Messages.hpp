#pragma once

struct SensorMessage {
    double pitch;
    double roll;
    double yaw;
};

struct ControlCommand {
    double pitchCmd;
    double rollCmd;
    double yawCmd;
};