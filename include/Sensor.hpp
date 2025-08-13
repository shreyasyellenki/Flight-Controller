#pragma once
#include "MessageQueue.hpp"
#include "Messages.hpp"

void sensorThread(MessageQueue<StateMessage>& stateQueue,
                  MessageQueue<SensorMessage>& sensorQueue);
