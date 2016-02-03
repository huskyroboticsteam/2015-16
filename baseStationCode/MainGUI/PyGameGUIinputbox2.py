import io
import sys, pygame, pygame.font, pygame.event, pygame.draw, string
import MercatorProjection
from pygame.locals import *

# Initialize pygame
pygame.init()
pygame.font.init()
pygame.joystick.init()

# Create array of connected joysticks
joysticks = [pygame.joystick.Joystick(x) for x in range(pygame.joystick.get_count())]
# https://www.pygame.org/docs/ref/joystick.html

# Define fonts to use in loop
fontobject = pygame.font.Font(None, 18) # Font size 18 in box
fontCoordinateEntry = fontobject.render("Coordinate Entry:", 0, (255,255,255)) # (text, antialias, (r, g, b))

# Define positions of certain GUI elements
CoordBoxPosX, CoordBoxPosY = 125, 500

# Define screen size, set screen
screenSize = (screenWidth, screenHeight) = (1600, 720)
screen = pygame.display.set_mode(screenSize)

# Set fps
clock = pygame.time.Clock()
fps = 60

# Load images
background = pygame.image.load("MarsDesertResearchStation.png")
ball = pygame.image.load("ball.png")
roverImage = pygame.image.load("rover.png")

# Initialize preset variables




#represents one coordinate, either a latitude or longitude
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
        if chars == len(current_string):
            counter = 0
            long = ""
            for i in range(len(coordRead)):
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
        elif current_string[chars] == ",":
            if not commaSeen:
                counter = 0
                lat = ""
                for i in range(len(coordRead)):
                    lat = lat + str(coordRead[i])
                    if coordRead[i] == ".":
                        counter += 1
                        lat = ""
                    elif counter == 0:
                        newCoord.lat.degrees = lat
                    elif counter == 1:
                        newCoord.lat.min = lat
                    elif counter == 2:
                        newCoord.lat.sec = lat
                commaSeen = True
            coordRead = ""
        else:
            coordRead = coordRead + str(current_string[chars])
    return newCoord

textboxEnabled = False
x, y, axisx, axisy = 0, 0, 0, 0
markerList = []
roverPos = CoordinatePair()

#gets rover GPS data:
def getRoverGPS():
    #TODO: get the serial input and save it in rover_string

    #parse and return the data
    rover_string = "800.100.0,400.200.100"
    return convertCoords(rover_string)

#this is supposed to display the coordinates in a box that can be deleted and edited.
#TODO: fix this. Figure out how to display a box and edit that box.
def displayCoordList():
    listXPos = 100
    listYPos = 600
    for x in markerList:
        xString = x.toString
        listDisplayBox = fontobject.render(xString, 0, (255,255,255)) # (text, antialias, (r, g, b))
        screen.blit(listDisplayBox, (listXPos + 4, ListYPos)) # (font object, (xpos, ypos))

        listYPos = listYPos + 50



#prints the coordinates list. for debugging
def printList():
    print "start of list"
    for i in markerList:
        print "Latitude:"
        print str(i.lat.degrees) + "." + str(i.lat.min) + "." + str(i.lat.sec)
        print "Longitude:"
        print str(i.long.degrees) + "." + str(i.long.min) + "." + str(i.long.sec)
    print "end of list"


#displays the text box for coord input
def display_box(screen, message, boxPosX, boxPosY): # Taken from inputbox.py library - display box on screen w/ inputted text

    if len(message) != 0:
        fontDisplayBox = fontobject.render(message, 0, (255,255,255)) # (text, antialias, (r, g, b))
        screen.blit(fontDisplayBox, (boxPosX + 4, boxPosY)) # (font object, (xpos, ypos))

# end

if len(joysticks) >= 1: # Determine if joystick input can be used, if not, run w/o this functionality
    joysticks[0].init()
    joystickson = True
else:
    joystickson = False

# end

while True:

    clock.tick(fps)

    screen.fill((0, 0, 0)) # Black background
    screen.blit(background,(screenWidth - 1280,0)) # Display map on right side of screen, map has size 1280x720

    screen.blit(ball,(x,y))

    #handles all input and other events
    for event in pygame.event.get():
        if event.type == pygame.QUIT:sys.exit()
        if event.type == pygame.KEYDOWN:
            if (event.key == pygame.K_LEFT) and (textboxEnabled == False):
                textboxEnabled = True
                current_string = []
            #reads in the coordinates
            if textboxEnabled == True:
                if event.type == pygame.KEYDOWN:
                    inkey = event.key
                    if inkey == K_BACKSPACE:
                        current_string = current_string[0:-1]
                    #converts typed coordinates to integers and adds them to the lists of coordinates
                    elif inkey == K_RETURN:
                        textboxEnabled = False
                        newCoord = convertCoords(current_string)
                        markerList.append(newCoord)
                        printList()
                    elif inkey == K_MINUS:
                        current_string.append("-")
                    #TODO: allow numberpad inputs.
                    elif (inkey >= 48 and inkey <= 57) or inkey == 44 or inkey == 46: # If key pressed is in the ASCII number range, or is a comma or period...
                        current_string.append(chr(inkey))

    roverPos = getRoverGPS()
    # end event queue loop


    screen.blit(fontCoordinateEntry, (10, 500))

    if textboxEnabled == True:
        display_box(screen, string.join(current_string,""), CoordBoxPosX, CoordBoxPosY)
        pygame.draw.rect(screen, (255, 0, 0), (125, 500, 150, 16), 1) # Draw white box (x, y, xlength, ylength, ?)
    else:
        pygame.draw.rect(screen, (255, 255, 255), (125, 500, 150, 16), 1) # Draw white box (x, y, xlength, ylength, ?)

    for balls in range(len(markerList)):
        screen.blit(ball, (markerList[balls].xPos(), markerList[balls].yPos()))
    screen.blit(roverImage, (roverPos.xPos(), roverPos.yPos()))

    if joystickson == True:
        axisx = joysticks[0].get_axis(0)
        axisy = joysticks[0].get_axis(1)

    x += axisx
    y += axisy

    pygame.display.flip()

    pygame.event.pump()

# ---------------- end update loop -------------------------------