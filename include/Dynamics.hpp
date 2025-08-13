#pragma once
#include "MessageQueue.hpp"
#include "Messages.hpp"

void dynamicsThread(MessageQueue<ControlCommand>& cmdQueue,
                    MessageQueue<StateMessage>& stateForSensorQueue,
                    MessageQueue<StateMessage>& stateForTelemetryQueue);