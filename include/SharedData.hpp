#pragma once
#include <mutex>

struct SensorData {
    double pitch;
    double roll;
    double yaw;
};

extern SensorData sensorData;
extern std::mutex dataMutex;
extern bool running;