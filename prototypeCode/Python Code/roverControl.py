#!/usr/bin/env python
# -*- coding: utf-8 -*-
# vi:ts=4 sw=4 et

from __future__ import division
from __future__ import print_function

import struct
import socket
import os

# Joystick import
import pygame

# Joystick Constants
A_RIGHT = 0


# UDP Constants
TARGET_IP = "192.168.1.177"
UDP_PORT = 8888
MAX_BUFFER_SIZE = 2048
# MAX_BUFFER_SIZE = 24

# UDP Defaults message
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
message = "I'm too old for this."


def get_response():
    try:
        message_buf = struct.pack(('!' + str(len(message)) + 's'), message.encode('utf-8'))
        sock.sendto(message_buf, (TARGET_IP, UDP_PORT))
        data, address = sock.recvfrom(MAX_BUFFER_SIZE)
        gps_tup = struct.unpack("%f%f", data)
        print("Response: ", gps_tup[0])
    except Exception as error:
     print(error)
    
    

def joy2value(value, half_control=False):
    if half_control:
        value /= 2.0
    if abs(value - 0) < 0.05:
        value = 0
    return value


def float256(value, low, high):
    value = 256 * (value - low) / (high - low)
    value = max([value, 0])
    value = min([value, 255])
    return int(value)


def float012(value):
    if (value < 0.5) and (value > -0.5):
        return 1
    if value < -0.5:
        return 0
    return 2


def bool012(forward, reverse):
    if forward:
        return 2
    if reverse:
        return 0
    return 1

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
            
        # Debugging: print process number to kill thread
        print('DEBUG: Operating on process', os.getpid())
     
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
        
            if joynum == 1:
                arm[0] = float012(joystick[A_RIGHT].get_axis(3) * -1)
                arm[2] = float012(joystick[A_RIGHT].get_axis(1))
                arm[1] = bool012(joystick[A_RIGHT].get_button(4), joystick[A_RIGHT].get_button(2))
                #arm[3] = bool012(joystick[A_RIGHT].get_button(5), joystick[A_RIGHT].get_button(3))

            # print("LEFT: " + str(ord(chr(left))))
            # print("RIGHT: " + str(ord(chr(right))))

            armByte = chr(arm[0] | (arm[1] << 2) | (arm[2] << 4))
            
            message = ''.join([armByte])

            # Send data over UDP, print recv    
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
