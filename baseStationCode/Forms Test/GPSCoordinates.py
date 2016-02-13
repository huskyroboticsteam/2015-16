# Classes and methods related to GPS coordinates and converting.

# Class for the degree, minute, second coordinate structure.
class degreeMin:
    def __init__(self, degree, minute, seconds):
        if(degree == ""):
            self.degrees = "0"
        else:
           self.degrees = degree
        if(minute == ""):
            self.min = "0"
        else:
            self.min = minute
        if(seconds == ""):
            self.sec = "0"
        else:
            self.sec = seconds

    def toDecimal(self):
        result = int(self.degrees) + (int(self.min) / 60.0) + (int(self.sec) / 360.0)
        return result

#defines a new class to store coordinates
#this class stores the string representations of GPS coordinates that can be converted to pixels when needed.
#TODO: fix coordinate return methods
#TODO: create mapping function
class CoordinatePair:
    def __init__(self):
        self.lat = degreeMin(0, 0, 0)
        self.long = degreeMin(0, 0, 0)
    #converts lat to a x position for display
    def xPos(self):
        return int(self.lat.toDecimal())
    #converts long to a y position for display
    def yPos(self):
        return int(self.long.toDecimal())
    def toString(self):
        return str(self.lat.degrees) + "." + str(self.lat.min) + "." + str(self.lat.sec) + "," + str(self.long.degrees) + "." + str(self.long.min) + "." + str(self.long.sec)

#this function does all of the conversion from input to coordinates
def convertCoords(current_string):

    """
    :rtype : CoordinatePair
    """

    commaSeen = False
    coordRead = ""
    newCoord = CoordinatePair()
    #loops through all characters in the text box
    for chars in range(len(current_string) + 1):
        #print range(len(current_string) + 1)
        #print len(current_string)
        if chars == len(current_string): # If final character in current_string has been reached...
            counter = 0
            long = ""
            for i in range(len(coordRead)): # Build the longitude coordinate.
                long = long + str(coordRead[i])
                if coordRead[i] == ".":
                    counter += 1
                    long = ""
                elif counter == 0:
                    newCoord.long.degrees = long
                elif counter == 1:
                    newCoord.long.min = long
                elif counter == 2:
                    newCoord.long.sec = long
        elif current_string[chars] == ",": # If a comma has been reached...
            if not commaSeen: # If this is the first comma...
                counter = 0
                lat = ""
                for i in range(len(coordRead)): # Build the latitude coordinate.
                    lat = lat + str(coordRead[i])
                    if coordRead[i] == ".":
                        counter += 1
                        lat = ""
                    elif counter == 0:
                        newCoord.lat.degrees = lat
                        # Update after every single character?
                    elif counter == 1:
                        newCoord.lat.min = lat
                    elif counter == 2:
                        newCoord.lat.sec = lat
                coordRead = ""
                commaSeen = True # Ignore any further commas.
        else: # If it's a number or period (not a separator)...
            coordRead = coordRead + str(current_string[chars]) # Add current character to the string
    return newCoord