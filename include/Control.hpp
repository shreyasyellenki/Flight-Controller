#pragma once
#include "MessageQueue.hpp"
#include "Messages.hpp"

void controlThread(MessageQueue<SensorMessage>& sensorQueue,
                   MessageQueue<ControlCommand>& commandQueue);