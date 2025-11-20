import tkinter as tk
import tkinter.ttk as ttk
import serial

# UI to send preset commands over serial to arduino board
class SensorSledUI:
    def __init__(self, master, serial_port):
        self.master = master
        self.serial_port = serial_port
        master.title("Sensor Sled Control")
        master.geometry("300x300")

        self.label = ttk.Label(master, text="Sensor Sled Control Panel")
        self.label.pack(pady=(0, 10))

        self.start_button = ttk.Button(master, text="Start Sensors", command=self.start_sensors)
        self.start_button.pack()
        self.sleep_button = ttk.Button(master, text="Sleep Mode", command=self.stop_sensors)
        self.sleep_button.pack()
        self.write_to_csv_button = ttk.Button(master, text="Write to CSV", command=self.sleep_mode)
        self.write_to_csv_button.pack(pady=(0, 20))

        # set sensor time to an inputted value
        self.set_time_label = ttk.Label(master, text="Set Sensor Time (HH:MM:SS):")
        self.set_time_label.pack()
        self.set_time_entry = ttk.Entry(master)
        self.set_time_entry.pack()
        self.set_time_button = ttk.Button(master, text="Set Time", command=self.set_sensor_time, )
        self.set_time_button.pack(pady=(0, 20))

        # Read period, 0 seconds for one read
        self.read_period_label = ttk.Label(master, text="Read Period (s, 0 for single read):")
        self.read_period_label.pack()
        self.read_period_entry = ttk.Entry(master)
        self.read_period_entry.pack()
        self.read_period_button = ttk.Button(master, text="Set Read Period", command=self.write_to_csv)
        self.read_period_button.pack(pady=(0, 20))

        self.quit_button = ttk.Button(master, text="Quit", command=master.quit)
        self.quit_button.pack()

    def start_sensors(self):
        command = "START\n"
        self.serial_port.write(command.encode('utf-8'))
        print("Sent command:", command.strip())

    def stop_sensors(self):
        command = "STOP\n"
        self.serial_port.write(command.encode('utf-8'))
        print("Sent command:", command.strip())

    def set_sensor_time(self):
        time_value = self.set_time_entry.get()
        command = f"SET_TIME {time_value}\n"
        self.serial_port.write(command.encode('utf-8'))
        print("Sent command:", command.strip())

    def sleep_mode(self):
        command = "SLEEP\n"
        self.serial_port.write(command.encode('utf-8'))
        print("Sent command:", command.strip())

    def write_to_csv(self):
        command = "WRITE_CSV\n"
        self.serial_port.write(command.encode('utf-8'))
        print("Sent command:", command.strip())

    def set_read_period(self):
        period_value = self.read_period_entry.get()
        command = f"SET_READ_PERIOD {period_value}\n"
        self.serial_port.write(command.encode('utf-8'))
        print("Sent command:", command.strip())

if __name__ == "__main__":
    root = tk.Tk()
    ser = serial.Serial('COM6', 9600, timeout=1)
    app = SensorSledUI(root, ser)
    root.mainloop()
    ser.close()