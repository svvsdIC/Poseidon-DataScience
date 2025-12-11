import tkinter as tk
import tkinter.ttk as ttk
import serial
import threading
import time

# UI to send preset commands over serial to arduino board
class SensorSledUI:
    def __init__(self, master, serial_port):
        self.master = master
        self.serial_port = serial_port
        master.title("Sensor Sled Control")
        master.geometry("900x620")
        
        # Flag to keep serial reading thread alive
        self.reading = True

        # Create main container with two columns
        main_container = ttk.Frame(master)
        main_container.pack(fill='both', expand=True, padx=10, pady=10)
        
        # Left column for controls
        left_frame = ttk.Frame(main_container)
        left_frame.pack(side='left', fill='both', expand=False, padx=(0, 10))
        
        # Right column for responses
        right_frame = ttk.Frame(main_container)
        right_frame.pack(side='right', fill='both', expand=True)

        self.label = ttk.Label(left_frame, text="Sensor Sled Control Panel")
        self.label.pack(pady=(0, 10))

        self.reset_button = ttk.Button(left_frame, text="Reset Sensor Sled", command=lambda: serial_port.write(b'RESET'))
        self.reset_button.pack()
        self.write_to_csv_button = ttk.Button(left_frame, text="Write to CSV", command=self.write_to_csv)
        self.write_to_csv_button.pack(pady=(0, 20))

        # set sensor time to an inputted value
        self.set_time_label = ttk.Label(left_frame, text="Set Sensor Time (HH:MM:SS):")
        self.set_time_label.pack()
        self.set_time_entry = ttk.Entry(left_frame)
        self.set_time_entry.pack()
        self.set_time_button = ttk.Button(left_frame, text="Set Time", command=self.set_sensor_time)
        self.set_time_button.pack(pady=(0, 20))

        # Read period, 0 seconds for one read
        self.read_period_label = ttk.Label(left_frame, text="Read Period (in s, 0 for single read):")
        self.read_period_label.pack()
        self.read_period_disclaimer = ttk.Label(left_frame, text="(There is a minimum 10s period)")
        self.read_period_disclaimer.pack()
        self.read_period_entry = ttk.Entry(left_frame)
        self.read_period_entry.pack()
        self.read_period_button = ttk.Button(left_frame, text="Set Read Period", command=self.set_read_period)
        self.read_period_button.pack(pady=(0, 20))

        # Break line
        self.separator = ttk.Separator(left_frame, orient='horizontal')
        self.separator.pack(fill='x', pady=10)

        # Entry and button to upload turbidity calibration data
        self.upload_turbidity_label = ttk.Label(left_frame, text="Upload Turbidity Calibration Data (low1 mid1 high1 low2 mid2 high2 ...):")
        self.upload_turbidity_label.pack()
        self.upload_turbidity_entry = ttk.Entry(left_frame)
        self.upload_turbidity_entry.pack()
        self.upload_turbidity_button = ttk.Button(left_frame, text="Upload Calibration Data", command=lambda: serial_port.write(f'UPLOAD_TURBIDITY_CAL {self.upload_turbidity_entry.get()}'.encode('utf-8')))
        self.upload_turbidity_button.pack(pady=(0, 0))

        # Button to get turbidity calibration data
        self.export_turbidity_button = ttk.Button(left_frame, text="Get Calibration Data", command=lambda: serial_port.write(b'EXPORT_TURBIDITY_CAL'))
        self.export_turbidity_button.pack(pady=(0, 20))

        # Entry and button to upload turbidity sample settings
        self.upload_turbidity_label = ttk.Label(left_frame, text="Upload Turbidity Settings (numSamples msBetween):")
        self.upload_turbidity_label.pack()
        self.upload_turbidity_entry = ttk.Entry(left_frame)
        self.upload_turbidity_entry.pack()
        self.upload_turbidity_button = ttk.Button(left_frame, text="Upload Settings", command=lambda: serial_port.write(f'SET_TURBIDITY_SETTINGS {self.upload_turbidity_entry.get()}'.encode('utf-8')))
        self.upload_turbidity_button.pack(pady=(0, 0))

        # Button to retrieve turbidity sample settings
        self.get_turbidity_button = ttk.Button(left_frame, text="Get Sample Settings", command=lambda: serial_port.write(b'GET_TURBIDITY_SETTINGS'))
        self.get_turbidity_button.pack(pady=(0, 20))
        
        # Quit button at bottom of left frame
        self.quit_button = ttk.Button(left_frame, text="Quit", command=self.on_closing)
        self.quit_button.pack(pady=(20, 0))

        # Response display area on the right
        self.response_label = ttk.Label(right_frame, text="Device Responses:")
        self.response_label.pack(anchor='w', pady=(0, 5))
        
        # Frame to hold text and scrollbar
        text_frame = ttk.Frame(right_frame)
        text_frame.pack(fill='both', expand=True)
        
        self.response_text = tk.Text(text_frame, height=30, width=40, state='disabled')
        self.response_text.pack(side='left', fill='both', expand=True)
        
        # Scrollbar for response text
        scrollbar = ttk.Scrollbar(text_frame, command=self.response_text.yview)
        scrollbar.pack(side='right', fill='y')
        self.response_text.config(yscrollcommand=scrollbar.set)
        
        # Clear button
        self.clear_button = ttk.Button(right_frame, text="Clear Responses", command=self.clear_responses)
        self.clear_button.pack(pady=(5, 0))

    def set_sensor_time(self):
        time_value = self.set_time_entry.get()
        command = f"SET_TIME {time_value}"
        self.serial_port.write(command.encode('utf-8'))
        print("Sent command:", command.strip())

    def write_to_csv(self):
        command = "WRITE_CSV"
        self.serial_port.write(command.encode('utf-8'))
        print("Sent command:", command.strip())

    def set_read_period(self):
        period_value = self.read_period_entry.get()
        command = f"SET_READ_PERIOD {period_value}"
        self.serial_port.write(command.encode('utf-8'))
        print("Sent command:", command.strip())

    def display_response(self, message):
        """Add message to response display"""
        self.response_text.config(state='normal')
        self.response_text.insert('end', message + '\n')
        self.response_text.see('end')  # Auto scroll to bottom
        self.response_text.config(state='disabled')

    def clear_responses(self):
        """Clear all text from response display"""
        self.response_text.config(state='normal')
        self.response_text.delete('1.0', 'end')
        self.response_text.config(state='disabled')

    def read_serial(self):
        """Read from serial port in a separate thread"""
        while self.reading:
            try:
                if self.serial_port.in_waiting:
                    response = self.serial_port.readline().decode('utf-8', errors='ignore').strip()
                    if response:
                        # Use after() to safely update UI from background thread
                        self.master.after(0, lambda msg=response: self.display_response(msg))
            except Exception as e:
                print(f"Error reading serial: {e}")
            time.sleep(0.05)  # Check every 50ms

    def on_closing(self):
        """Handle window closing"""
        self.reading = False
        self.master.destroy()

if __name__ == "__main__":
    root = tk.Tk()
    #root.protocol("WM_DELETE_WINDOW", None)  # Will be set by UI
    ser = serial.Serial('COM6', 9600, timeout=1)
    app = SensorSledUI(root, ser)
    #root.protocol("WM_DELETE_WINDOW", app.on_closing)
    
    # Start serial reader thread
    reader_thread = threading.Thread(target=app.read_serial, daemon=True)
    reader_thread.start()
    
    root.mainloop()
    ser.close()