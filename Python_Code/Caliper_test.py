
import time
import serial
import serial.tools.list_ports
import struct
import re

#USB_BAUD_RATE = 9600 origional value
USB_BAUD_RATE = 115200 #test value

def connect_to_port(enable):
    if (enable):
        try:
            #ports=serial.tools.list_ports.comports()
            #for port, desc, hwid in sorted(ports):
            #    if desc == "STM32 Virtual Port":
            #        print("COM PORT: {} used".format(port))
            #        COM_PORT = port
            COM_PORT = "/dev/cu.usbserial-1420" #Original COM_PORT
            COM_PORT = "COM6" #Works for my computer!
            global serial_link
            serial_link = serial.Serial(COM_PORT, USB_BAUD_RATE, timeout=0.5)
            if serial_link.isOpen():
                print("serial port opened")
        except Exception as exc:
                raise Exception("Could not connect to server")
    
    else:
        try:
            serial_link.close()
            if not serial_link.isOpen():
                print("Serial port is closed")
        except Exception as exc:
            raise

# receive messages
def receive(test_nr,start_time):
    try:
        current_time = time.gmtime()
        File_object = open(f"Data{test_nr}.txt", "a")

        to_print = serial_link.read_until()
        print(to_print)
        to_print=str(to_print)

        # Regular expression to match and extract numbers
        to_print = re.findall(r'-?\d+', to_print)[0]#index zero since re.findall returns a list

        File_object.write(to_print + " " + str(time.time() - start_time) + "\n")

        File_object.close()
        print(to_print, end=" ")
        
    except Exception as exc:
        raise

if __name__ == "__main__":
    connect_to_port(enable=True)
    test_nr = input("Test nr")

    File_object = open(f"Data{test_nr}.txt", "w")
    File_object.write("test nr: " + test_nr + "\n")
    File_object.close()
    start_time = time.time()
    while(True):
        receive(test_nr,start_time)
