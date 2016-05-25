__author__ = 'Trevor'

import socket
import threading
from GPSCoordinates import *
import MapPixelCoords

class receiveOverUDP(threading.Thread):
    def __init__(self, ownIP, udpPort):
        threading.Thread.__init__(self)
        self.OwnIP = ownIP
        self.UDPPort = udpPort
        self.sockCome = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        try:
            self.sockCome.bind((self.OwnIP, self.UDPPort))
        except socket.error:
            print "Error connecting to rover."
        self.End = False # Make True to end receiving thread
        self.Coord = [0,0]
        #self.sockCome.settimeout(0.01)

    def run(self):
        while not self.End:
            data, addr = self.sockCome.recvfrom(1024)
            print data

            dataArray = data.split(',')
            LatitudeChars = list(dataArray[0])
            LatitudeDegrees = LatitudeChars[0] + LatitudeChars[1]
            LatitudeMinutes = LatitudeChars[2] + LatitudeChars[3] + LatitudeChars[4] + LatitudeChars[5] + LatitudeChars[6] + LatitudeChars[7] + LatitudeChars[8]
            LatitudeString = LatitudeDegrees + '*' + LatitudeMinutes

            LongitudeChars = list(dataArray[1])
            LongitudeDegrees = LongitudeChars[0] + LongitudeChars[1] + LongitudeChars[2]
            LongitudeMinutes = LongitudeChars[3] + LongitudeChars[4] + LongitudeChars[5] + LongitudeChars[6] + LongitudeChars[7] + LongitudeChars[8] + LongitudeChars[9]
            LongitudeString = '-' + LongitudeDegrees + '*' + LongitudeMinutes
            CoordinateString = LatitudeString + ',' + LongitudeString
            DecimalCoord = Coordinates(CoordinateString)

            self.Coord[0] = DecimalCoord.latitude
            self.Coord[1] = DecimalCoord.longitude

            #self.Latitude = dataStrings[0]
