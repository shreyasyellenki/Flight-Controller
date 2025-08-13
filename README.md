# Flight Control Simulation

## Overview

This project simulates a simplified aircraft's flight dynamics in **C++** and provides real-time telemetry visualization using **Python**. It models roll, pitch, and yaw dynamics, implements PID controllers with rate damping, and streams live data over UDP to a Python visualizer.

- **Dynamics Thread** – Simulates aircraft attitude based on control torques.
- **Control Thread** – Computes PID control commands to stabilize the aircraft.
- **Telemetry Thread** – Streams flight data over UDP.
- **Visualizer** – Python tool that plots real-time telemetry.

---

## Features

- Multi-threaded C++ simulation of flight dynamics
- PID controllers with proportional, integral, and derivative (rate damping) terms
- Real-time UDP telemetry streaming
- Python visualization tool with live plotting of roll, pitch, yaw, and other metrics
- Modular design using thread-safe message queues for inter-thread communication

---

## Technologies Used

- **C++17** – Multi-threaded simulation and control
- **Python 3** – Real-time telemetry visualization
- **matplotlib** – Plotting live telemetry data
- **UDP sockets** – Real-time data streaming between C++ and Python
- **Thread-safe message queues** – Synchronization between simulation, control, and telemetry