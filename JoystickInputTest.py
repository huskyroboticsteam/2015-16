from __future__ import division
from __future__ import print_function

import pygame
import socket
import struct

# Joystick Constants
x = 0
y = 0

# Joystick Constants
C_RIGHT = 1
C_LEFT = 0
A_RIGHT = 3
A_LEFT = 2

# UDP Constants
TARGET_IP = "192.168.1.51"
UDP_PORT = 8888
MAX_BUFFER_SIZE =24

# UDP Defaults message
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
message = "Hopefully that worked."

def get_response():
    try:
        sock.sendto(message, (TARGET_IP, UDP_PORT))
        data, address = sock.recvfrom(MAX_BUFFER_SIZE)
        gps_tup = struct.unpack("%f%f", data)
        print("Response: ", gps_tup[0:1])
    except Exception as error:
        print("")

def joy2value(value, half_control=False):
    if half_control:
        value /= 2.0
    if abs(value - 0) < 0.05:
        value =0
    return value

def float256(value, low, high):
    value = 256 * (value - low) / (high - low)
    value = max([value, 0])
    value = min([value, 255])
    return int(value)

# Main
if __name__ == '__main__':
    try:
        # Joystick
        pygame.joystick.init()
        pygame.display.init()
        joystick = []
        joynum = pygame.joystick.get_count()
        print(str(joynum) + " joysticks connected.")

        for i in range(joynum):
            pygame.joystick.Joystick(i).init()

        while len(joystick) < joynum:
            pygame.event.pump()
            for i in range(joynum):
                if pygame.joystick.Joystick(i).get_button(0):
                    joystick.append(pygame.joystick.Joystick(i))
                    pygame.time.wait(500)

        for i in range(joynum):
            print("Joystick " + str(i) + " is: " + joystick[i].get_name() + " with " + str(joystick[i].get_numaxes()) + " axes.")

                # Initialize
        arm = []
        for i in range(4):
            arm.append(1)

       # science = []
     #   for i in range(3):
      #      science.append(1)

        hand = []
        for i in range(4):
            hand.append(1)

        misc = []
        for i in range(4):
            misc.append(1)

        # UDP
        print("UDP Port: ", UDP_PORT)
        print("Test Message: ", message)
        print("Max Buffer Size: ", MAX_BUFFER_SIZE)

        sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

        sock.bind(('0.0.0.0', UDP_PORT))
        sock.settimeout(0.01)

        # Main Loop
        while True:
            # Get and convert joystick value, print
            pygame.event.pump()

            # Find out if values need to be halved or not and trim small values to zero
            x = (joy2value(joystick[0].get_axis(0), (not joystick[0].get_button(0))))
            y = (joy2value(joystick[0].get_axis(1), (not joystick[0].get_button(0))))

            # Map out
            x = float256(x, -1, 1)
            y = float256(y, -1, 1)

            print("x: " + str(x))
            print("y: " + str(y))
            pygame.time.wait(100)

            message = ''.join([chr(x), chr(y)])

            get_response()

            '''p = Process(target=getResponse)
            p.start()
            p.join(100)
            if(p.is_alive()):
                print("Disconnect... Attempting to fix.")
                p.terminate()
                p.join()'''

            pygame.time.wait(100)

    except KeyboardInterrupt:
        pygame.quit()