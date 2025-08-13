#pragma once
#include "MessageQueue.hpp"
#include "Messages.hpp"

void sensorThread(MessageQueue<SensorMessage>& sensorQueue);
