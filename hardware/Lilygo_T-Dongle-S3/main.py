import machine

# Test standard GPIO pins for initialization
try:
    pin_12 = machine.Pin(12, machine.Pin.OUT)  # Common GPIO pin on ESP32
    pin_13 = machine.Pin(13, machine.Pin.OUT)
    pin_14 = machine.Pin(14, machine.Pin.OUT)
    pin_15 = machine.Pin(15, machine.Pin.OUT)
    pin_18 = machine.Pin(18, machine.Pin.OUT)
    pin_19 = machine.Pin(19, machine.Pin.OUT)
    pin_23 = machine.Pin(23, machine.Pin.OUT)
    print("Pins initialized successfully.")
except ValueError as e:
    print(f"Error initializing pin: {e}")
