__author__ = 'Trevor'

import socket

class sendOverUDP:
    def __init__(self, targetIP, udpPort):
        self.TargetIP = targetIP
        self.UDPPort = udpPort
        self.sockBalls = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

        self.sockBalls.bind(('0.0.0.0', udpPort)) # TODO: WTF DOES THIS DO?
        self.sockBalls.settimeout(0.01) # see one line above

    def sendItOff(self, messageUDP):
        self.sockBalls.sendto(messageUDP, (self.TargetIP, self.UDPPort))