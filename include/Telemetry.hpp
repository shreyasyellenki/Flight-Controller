#pragma once
#include "MessageQueue.hpp"
#include "Messages.hpp"

void telemetryThread(MessageQueue<ControlCommand>& commandQueue);
