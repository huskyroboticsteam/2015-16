import sys, pygame, pygame.font, pygame.event, pygame.draw, string
from pygame.locals import *
from MapTile import MapTile
from GPSCoordinates import *

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
mapUpperLeftCorner = (320, 0)

# Define screen size, set screen
screenSize = (screenWidth, screenHeight) = (2200, 1500)
screen = pygame.display.set_mode(screenSize)

# Set fps
clock = pygame.time.Clock()
fps = 60

# Load images
ball = pygame.image.load("ball.png")
roverImage = pygame.image.load("rover.png")
pointer = pygame.image.load("pointer.png")

# Initialize preset variables
textboxEnabled = False
roverPos = CoordinatePair()
x, y, axisx, axisy = 900, 900, 0, 0
markerList = []
startingCoord = (20.001,-100) # Latitude, Longitude - center coordinate of upper-left tile
tileArraySize = (2,4) # (n,m) (rows,columns)
mapZoom = 13

startingTile = MapTile(startingCoord,mapZoom,(640,640),mapUpperLeftCorner)
borderDistance = startingTile.findBorderDistance() # Will return (longitude, latitude) lengths

mapTiles = [[0 for m in xrange(0, tileArraySize[1])] for n in xrange(0, tileArraySize[0])] # tileArraySize[0] is rows, tileArraySize[1] is columns
numSaves = 0

def pixelToCoord(x,y):
    coord = (startingCoord[0] - borderDistance[0] / 640 * (y - 320 - mapUpperLeftCorner[1]), #latitude
             startingCoord[1] + borderDistance[1] / 640 * (x - 320 - mapUpperLeftCorner[0])) #longitude
    return coord

def coordToPixel(latitude,longitude): # TODO: FIX MATH
    pixel = ( (latitude - startingCoord[0]) * 640 / borderDistance[0] + 320 + mapUpperLeftCorner[0],   # x-coordinate
              (longitude - startingCoord[1]) * 640 / borderDistance[1] + 320 + mapUpperLeftCorner[1] ) # y-coordinate
    return pixel

pointerlocation = coordToPixel(20.02,-99.92)
print pointerlocation
# --------------------------------------

for n in range(0,tileArraySize[0]):
    for m in range(0,tileArraySize[1]):
        currentCoord = (startingCoord[0] - n*borderDistance[0], startingCoord[1] + m*borderDistance[1]) # Subtract from latitude to go south/down (northern hemisphere), add to longitude to go east/right
        mapTiles[n][m] = MapTile(currentCoord, mapZoom, (640,640), (mapUpperLeftCorner[0] + 640*m, mapUpperLeftCorner[1] + 640*n))
        if mapTiles[n][m].saved:
            numSaves += 1

if numSaves > 0:
    print str(numSaves) + " image tiles cached from the Internet."

# ---------------------------------

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

def printList():
    print "start of list"
    for i in markerList:
        print "Latitude:"
        print str(i.lat.degrees) + "." + str(i.lat.min) + "." + str(i.lat.sec)
        print "Longitude:"
        print str(i.long.degrees) + "." + str(i.long.min) + "." + str(i.long.sec)
    print "end of list"

while True:

    clock.tick(fps)

    screen.fill((0, 0, 0)) # Black background

    for n in range(0,tileArraySize[0]):
        for m in range(0,tileArraySize[1]):
            screen.blit(mapTiles[n][m].image,mapTiles[n][m].screenlocation)

    screen.blit(pointer,pointerlocation)

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
                        if len(current_string) <= 30:
                            current_string.append(chr(inkey))

    screen.blit(fontCoordinateEntry, (10, 500))

    if textboxEnabled == True:
        display_box(screen, string.join(current_string,""), CoordBoxPosX, CoordBoxPosY)
        pygame.draw.rect(screen, (255, 0, 0), (125, 500, 150, 16), 1) # Draw white box (x, y, xlength, ylength, ?)
    else:
        pygame.draw.rect(screen, (255, 255, 255), (125, 500, 150, 16), 1) # Draw white box (x, y, xlength, ylength, ?)

    for balls in range(len(markerList)):
        screen.blit(ball, (markerList[balls].xPos(), markerList[balls].yPos())) # Add code to make this convert coordinates to screen position
    screen.blit(roverImage, (roverPos.xPos(), roverPos.yPos()))

    if joystickson == True:
        axisx = joysticks[0].get_axis(0)
        axisy = joysticks[0].get_axis(1)

    x += axisx
    y += axisy

    pygame.display.flip()

    pygame.event.pump()

# ---------------- end update loop -------------------------------