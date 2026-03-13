import socket
import matplotlib.pyplot as plt
import matplotlib.animation as animation
from collections import deque

class TelemetryVisualizer:
    def __init__(self, udp_ip="127.0.0.1", udp_port=5005, window_size=200):
        self.udp_ip = udp_ip
        self.udp_port = udp_port
        self.window_size = window_size

        # UDP socket
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        self.sock.bind((self.udp_ip, self.udp_port))
        self.sock.setblocking(False)

        # Data buffers
        self.rolls = deque([0]*window_size, maxlen=window_size)
        self.pitches = deque([0]*window_size, maxlen=window_size)
        self.yaws = deque([0]*window_size, maxlen=window_size)

        # Plot
        self.fig, self.ax = plt.subplots()
        self.line_roll, = self.ax.plot([], [], label="Roll")
        self.line_pitch, = self.ax.plot([], [], label="Pitch")
        self.line_yaw, = self.ax.plot([], [], label="Yaw")

        self.ax.set_ylim(-0.5, 0.5)
        self.ax.set_xlim(0, window_size)
        self.ax.set_xlabel("Sample")
        self.ax.set_ylabel("Angle (radians)")
        self.ax.set_title("Real-time Attitude Telemetry")
        self.ax.legend()
        self.ax.grid(True)

    def update(self, frame):
        try:
            data, _ = self.sock.recvfrom(1024)
            decoded = data.decode('utf-8')
            values = list(map(float, decoded.strip().split(',')))
            roll, pitch, yaw = values[0], values[1], values[2]

            self.rolls.append(roll)
            self.pitches.append(pitch)
            self.yaws.append(yaw)
        except BlockingIOError:
            pass

        self.line_roll.set_data(range(len(self.rolls)), self.rolls)
        self.line_pitch.set_data(range(len(self.pitches)), self.pitches)
        self.line_yaw.set_data(range(len(self.yaws)), self.yaws)

        return self.line_roll, self.line_pitch, self.line_yaw

    def run(self):
        self.ani = animation.FuncAnimation(self.fig, self.update, interval=50)
        plt.show()


if __name__ == "__main__":
    visualizer = TelemetryVisualizer()
    visualizer.run()
