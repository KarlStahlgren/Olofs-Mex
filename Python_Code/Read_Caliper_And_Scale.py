import time
import serial
#import serial.tools.list_ports

USB_BAUD_RATE = 115200 #The same as the arduino

def connect_to_port(enable):
    if (enable):
        try:
            """
            ports=serial.tools.list_ports.comports()
            for port, desc, hwid in sorted(ports):
                if desc == "STM32 Virtual Port":
                    print("COM PORT: {} used".format(port))
                    COM_PORT = port
            """
            #COM_PORT = "/dev/cu.usbserial-1420" 
            COM_PORT = "COM6" #Works for my computer, choose the port you're using
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
            raise2

# receive messages
def receive(test_nr,start_time):
    try:
        current_time = time.gmtime()
        File_object = open(f"Data{test_nr}.txt", "a")
        reading = serial_link.read_until().decode().strip()
        File_object.write(str(reading) + " " + str(round(time.time() - start_time, 3)) + "\n")
        #voltage_reading = serial_link.read_until().decode().strip()
        #caliper_reading = serial_link.read_until().decode().strip()
        #scale_reading = serial_link.read_until().decode().strip()
        #File_object.write(str(voltage_reading) + "   " +str(caliper_reading) + "   " + str(scale_reading) + "   " + str(round(time.time() - start_time, 3)) + "\n")

        File_object.close()
        print(reading)
        #print(voltage_reading, end=" ") 
        #print(caliper_reading, end=" ")
        #print(scale_reading)

    except Exception as exc:
        raise

if __name__ == "__main__":
    connect_to_port(enable=True)
    test_nr = input("Test nr")

    File_object = open(f"Data{test_nr}.txt", "w")
    File_object.write("test nr: " + test_nr + "\n")
    File_object.write("Lastspänning i V, Längd i 100*mm, Vikt i ?, tid i s" + "\n")
    File_object.close()
    start_time = time.time()

    #First read seem to sometimes get no data, so this just removes the oddity     
    serial_link.read_until()

    while(True):
        receive(test_nr,start_time)