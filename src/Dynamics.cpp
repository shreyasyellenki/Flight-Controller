#include "Dynamics.hpp"
#include "SharedData.hpp"
#include <chrono>
#include <thread>
#include <algorithm>

// Simple rigid-body attitude dynamics with diagonal inertia (Ix, Iy, Iz).
// Integrator: explicit Euler (fine for small dt).
// State: Euler angles (phi, theta, psi) and body rates (p, q, r).
namespace {
struct State {
    double phi{0}, theta{0}, psi{0}; // roll, pitch, yaw
    double p{0}, q{0}, r{0};         // body rates
};

inline void clampAngles(State& s) {
    const double pi = 3.141592653589793;
    while (s.psi >  pi) s.psi -= 2*pi;
    while (s.psi < -pi) s.psi += 2*pi;
}

// Convert Euler angle rates from body rates
// [phi_dot, theta_dot, psi_dot]^T = E(angles) * [p q r]^T
inline void eulerKinematics(const State& s, double& phi_dot, double& theta_dot, double& psi_dot) {
    const double ctheta = std::max(1e-6, std::cos(s.theta));
    const double ttheta = std::tan(s.theta);
    const double sphi = std::sin(s.phi), cphi = std::cos(s.phi);

    phi_dot   = s.p + s.q * sphi * ttheta + s.r * cphi * ttheta;
    theta_dot = s.q * cphi - s.r * sphi;
    psi_dot   = s.q * sphi / ctheta + s.r * cphi / ctheta;
}
}

bool running = true;

void dynamicsThread(MessageQueue<ControlCommand>& cmdQueue,
                    MessageQueue<StateMessage>& stateForSensorQueue,
                    MessageQueue<StateMessage>& stateForTelemetryQueue) {
    // Inertia (kg·m^2)
    const double Ix = 0.02, Iy = 0.025, Iz = 0.03;

    State x; // initial state 0
    ControlCommand u{0,0,0}; // latest command

    using clock = std::chrono::steady_clock;
    auto next = clock::now();
    const std::chrono::milliseconds dt_ms(10);
    const double dt = 0.010; // 10 ms

    while (running) {
        for (;;) {
            auto maybe = cmdQueue.try_pop();
            if (!maybe) break;
            u = *maybe;
        }

        // Rigid-body rotational dynamics
        // p_dot = (tau_x - (Iz - Iy) * q * r) / Ix, etc.
        const double p_dot = (u.tau_x - (Iz - Iy) * x.q * x.r) / Ix;
        const double q_dot = (u.tau_y - (Ix - Iz) * x.p * x.r) / Iy;
        const double r_dot = (u.tau_z - (Iy - Ix) * x.p * x.q) / Iz;

        // Integrate body rates
        x.p += p_dot * dt;
        x.q += q_dot * dt;
        x.r += r_dot * dt;

        // Kinematics: integrate Euler angles
        double phi_dot, theta_dot, psi_dot;
        eulerKinematics(x, phi_dot, theta_dot, psi_dot);
        x.phi   += phi_dot   * dt;
        x.theta += theta_dot * dt;
        x.psi   += psi_dot   * dt;
        clampAngles(x);

        // Publish state to both consumers (sensor + telemetry)
        StateMessage sm{ x.phi, x.theta, x.psi, x.p, x.q, x.r };
        stateForSensorQueue.push(sm);
        stateForTelemetryQueue.push(sm);

        next += dt_ms;
        std::this_thread::sleep_until(next);
    }

    // Graceful shutdown: close queues so blocking pops exit
    stateForSensorQueue.close();
    stateForTelemetryQueue.close();
}
