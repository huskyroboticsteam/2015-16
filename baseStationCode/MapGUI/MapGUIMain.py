import sys, pygame, pygame.font, pygame.event, pygame.draw, string
from pygame.locals import *
from MapTile import *
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
mapUpperLeftCorner = (0, 0)
downloadMode = False

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
previousMouseStatus = False
roverPos = CoordinatePair()

startingCoord = (47.661225, -122.317530)

markerList = []

mainMap = Map("SmallMap19", (4,4), startingCoord, 19, True) # (n,m) (rows,columns), Latitude, Longitude - center coordinate of upper-left tile, zoom, loadOldMapData?
secondaryMap = Map("SmallMap18", (4,4), startingCoord, 18, False)

# startingTile = MapTile("hi",startingCoord,mapZoom,(640,640),mapUpperLeftCorner)
# borderDistance = startingTile.findBorderDistance() # Will return (longitude, latitude) lengths
# print borderDistance

class axis: #TODO: Move to separate file
    def __init__(self,x,y):
        self.x = x
        self.y = y

mainAxis = axis(-300,-200)

def pixelToCoord(x,y):
    coord = (mainMap.startingLocation[0] - mainMap.borderDistance[0] / 640 * (y - 320), #latitude
             mainMap.startingLocation[1] + mainMap.borderDistance[1] / 640 * (x - 320)) #longitude
    return coord

def coordToPixel(latitude,longitude):
    print latitude
    print longitude
    pixel = ( (longitude - mainMap.startingLocation[1]) * 640 / mainMap.borderDistance[1] + 320, # x-coordinate
              (latitude - mainMap.startingLocation[0]) * -640 / mainMap.borderDistance[0] + 320) # y-coordinate
    return pixel # (x,y)

def coordToPixel1(latitude,longitude,mainMap):
    #print mainMap.ULcoord
    #print mainMap.URcoord
    #print mainMap.LLcoord
    #print mainMap.LRcoord
    latSpan = abs(mainMap.ULcoord[0] - mainMap.LLcoord[0]) # Total latitudinal map span
    longSpan = abs(mainMap.URcoord[1] - mainMap.ULcoord[1]) # Total longitudinal map span
    pixSpanx = mainMap.dimensions[1]*640
    pixSpany = mainMap.dimensions[0]*640
    latTransform = abs(latitude - mainMap.ULcoord[0])
    longTransform = abs(longitude - mainMap.ULcoord[1])
    #print latSpan
    #print longSpan
    #print latTransform
    #print longTransform
    positionX = longTransform*(pixSpanx/longSpan)
    positionY = latTransform*(pixSpany/latSpan)
    #print (positionX,positionY)
    return (positionX,positionY)

def display(object,location):
    screen.blit(object, (mainAxis.x + location[0], mainAxis.y + location[1]))

def loadImages(map): # Take a map object and load its images into memory for display
    mapTiles = [[0 for m in xrange(0, mainMap.dimensions[1])] for n in xrange(0, mainMap.dimensions[0])] # tileArraySize[0] is rows, tileArraySize[1] is columns
    numSaves = 0
    for n in range(0,map.dimensions[0]):
        for m in range(0,map.dimensions[1]):
            currentCoord = (map.startingLocation[0] - n*map.borderDistance[0], map.startingLocation[1] + m*map.borderDistance[1]) # Subtract from latitude to go south/down (northern hemisphere), add to longitude to go east/right
            mapTiles[n][m] = MapTile(map.name,currentCoord, map.zoom, (640,640), (mapUpperLeftCorner[0] + 640*m, mapUpperLeftCorner[1] + 640*n))
            if mapTiles[n][m].saved:
                numSaves += 1
    if numSaves > 0:
        print str(numSaves) + " image tiles cached from the Internet."
    return mapTiles

#pointerlocation = coordToPixel(47.651225, -122.307530)
#print pointerlocation

mapTiles = loadImages(mainMap)

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

    for n in range(0,mainMap.dimensions[0]):
        for m in range(0,mainMap.dimensions[1]):
            display(mapTiles[n][m].image,mapTiles[n][m].screenlocation)

    # Keyboard panning
    keysDowned = pygame.key.get_pressed()
    if keysDowned[pygame.K_LEFT]:
        mainAxis.x -= 10
    if keysDowned[pygame.K_RIGHT]:
        mainAxis.x += 10
    if keysDowned[pygame.K_UP]:
        mainAxis.y -= 10
    if keysDowned[pygame.K_DOWN]:
        mainAxis.y += 10

    # Mouse panning
    mousestatus = pygame.mouse.get_pressed()
    if mousestatus[2]:
        if previousMouseStatus == False:
            previousMouseStatus = True
            pygame.mouse.get_rel()
        movement = pygame.mouse.get_rel()
        mainAxis.x += movement[0]
        mainAxis.y += movement[1]
    if not mousestatus[2]:
        previousMouseStatus = False

    # Everything to do with event queue - keys down, etc.
    for event in pygame.event.get():
        if event.type == pygame.QUIT:sys.exit()
        if event.type == pygame.KEYDOWN:
            if (event.key == pygame.K_z):
                mapTiles = loadImages(secondaryMap)
            if (event.key == pygame.K_x):
                mapTiles = loadImages(mainMap)
            if (event.key == pygame.K_m) and (textboxEnabled == False):
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
                        print newCoord
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

    #if len(markerList) > 0:
    #    print markerList[0].lat.degrees
    #    print markerList[0].long.degrees

    for balls in range(len(markerList)):
        print float(markerList[balls].lat.degrees)
        print float(markerList[balls].long.degrees)
        print coordToPixel(float(markerList[balls].lat.degrees),float(markerList[balls].long.degrees))

        display(ball, coordToPixel1(float(markerList[balls].lat.degrees), float(markerList[balls].long.degrees), mainMap)) # Add code to make this convert coordinates to screen position

        # screen.blit(roverImage, (roverPos.xPos(), roverPos.yPos()))

    #display(ball, coordToPixel1(47.660688,-122.314747,mainMap))

    if joystickson == True:
        axisx = joysticks[0].get_axis(0)
        axisy = joysticks[0].get_axis(1)

    pygame.display.flip()

    pygame.event.pump()

# ---------------- end update loop -------------------------------