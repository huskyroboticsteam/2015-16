import sys, string
import math
import socket
from pygame.locals import *
from MapTile import *
from GPSCoordinates import *

# Initialize pygame and necessary functionality
pygame.init()
pygame.font.init()
pygame.joystick.init()

# start screen
screenSize = (screenWidth, screenHeight) = (2200, 1500)
screen = pygame.display.set_mode(screenSize)

# Input Constants
angle = 0
speed = 0

# JOYSTICK STUFF

# Initialize rover status
roverStopped = False
potStopped = False

# Find out how many joysticks are connected to the computer
joynum = pygame.joystick.get_count()
print(str(joynum) + " joysticks connected.")

# Create array of connected joysticks
joysticks = []
# https://www.pygame.org/docs/ref/joystick.html

# Initialize joysticks
for i in range(joynum):
    pygame.joystick.Joystick(i).init()

# Force user to determine joystick order
if joynum > 0: # Assuming joysticks are connected at all
    joysticks_connected = True
    inputReceived = False
    connectedJoysticks = []
    numberConnected = 0
    rectValues = [(20,20,screenWidth/2-20,screenHeight/2-20),(screenWidth/2+20,20,screenWidth/2-20,screenHeight/2-20),(20,screenHeight/2+20,screenWidth/2-20,screenHeight/2-20),(screenWidth/2+20,screenHeight/2+20,screenWidth/2-20,screenHeight/2-20)]
    for i in range(joynum):
        pygame.draw.rect(screen,(80,0,0),rectValues[i]) #rect(Surface, color, Rect, width=0) -> Rect

    pygame.display.flip()

    while not inputReceived:
        for i in range(joynum):
            if pygame.joystick.Joystick(i).get_button(0):
                if i not in connectedJoysticks: # Check to make sure joystick wasn't already added
                    joysticks.append(pygame.joystick.Joystick(i))
                    connectedJoysticks.append(i)
                    numberConnected += 1
                    pygame.draw.rect(screen,(200,0,0),rectValues[numberConnected-1]) #rect(Surface, color, Rect, width=0) -> Rect
                if len(joysticks) == joynum:
                    inputReceived = True
                print len(joysticks)
                print joynum

        pygame.event.pump()
        pygame.display.flip()
        pygame.time.wait(50)
else:
    joysticks_connected = False

# END OF JOYSTICK STUFF



# UDP STUFF

# UDP Constants
TARGET_IP = "192.168.1.51"
UDP_PORT = 8888
MAX_BUFFER_SIZE = 24

#UDP Defaults message
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
message = ""

# UDP initialization stuff
if __name__ == '__main__':
    # UDP
    print("UDP Port: ", UDP_PORT)
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    sock.bind(('0.0.0.0', UDP_PORT))
    sock.settimeout(0.01)

# END UDP STUFF



# Define colors
GOLD = (213, 202, 148)
DARKBOXRED = (175,0,0)
WARNINGBOXORANGE = (225,125,0)

# Define pi in simple-to-write way
PI = math.pi

# Define fonts
stopFont = pygame.font.SysFont('Arial', 70)

# Buttons
eStopButton = pygame.Rect(screenWidth-400, 50, 350, 350)
potStopButton = pygame.Rect(screenWidth-750, 50, 350, 350)

# Emergency stop text
allText = stopFont.render('ALL', 1, GOLD)
potText = stopFont.render('POT', 1, GOLD)
StopText = stopFont.render('STOP', 1, GOLD)
StoppedText = stopFont.render('STOPPED', 1, GOLD)

# Positions for the boxes
# e-stop "all" button
allTextpos = (eStopButton.centerx-70,eStopButton.centery-80) # For the "all stop" button
allStopTextpos = (eStopButton.centerx-100,eStopButton.centery) # text that says "STOP" drawn in 2 locations
allStoppedTextpos = (eStopButton.centerx-160,eStopButton.centery)
# pot-stop button
potTextpos = (potStopButton.centerx-70,potStopButton.centery-80) # For the "pot stop" button
potStopTextpos = (potStopButton.centerx-100,potStopButton.centery) # text that says "STOP" drawn in 2 locations
potStoppedTextpos = (potStopButton.centerx-160,potStopButton.centery) # text that says "stopped", shifted left relative to just "stop" to be more centered

# Set the window title/caption TODO: add image
pygame.display.set_caption("Husky Robotics Hot GUI")

# Processes the joystick values. Doubles if button for extra thrust is down, rounds values below 0.5 to 0
def joy2value(value, half_control=False):
    if half_control:
        value /= 2.0
    if abs(value - 0) < 0.05:
        value = 0
    return value

# Maps values to range from 0 to 255
def float256(value, low, high):
    value = 256 * (value - low) / (high - low)
    value = max([value, 0])
    value = min([value, 255])
    return int(value)

# Define fonts to use in loop
fontobject = pygame.font.Font(None, 18) # Font size 18 in box
fontCoordinateEntry = fontobject.render("Coordinate Entry:", 0, (255,255,255)) # (text, antialias, (r, g, b))

# Define positions of certain GUI elements
CoordBoxPosX, CoordBoxPosY = 125, 500
mapUpperLeftCorner = (0, 0)
downloadMode = False

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

increment = 0 #TODO: Remove debug

startingCoord = (38.4085624,-110.7966106)

markerList = []

mainMap = Map("MarsMap20", (8,8), startingCoord, 20, True) # (n,m) (rows,columns), Latitude, Longitude - center coordinate of upper-left tile, zoom, loadOldMapData?
secondaryMap = Map("MarsMap19", (8,8), startingCoord, 19, True)

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

mapTiles = loadImages(mainMap)

def display_box(screen, message, boxPosX, boxPosY): # Taken from inputbox.py library - display box on screen w/ inputted text
    if len(message) != 0:
        fontDisplayBox = fontobject.render(message, 0, (255,255,255)) # (text, antialias, (r, g, b))
        screen.blit(fontDisplayBox, (boxPosX + 4, boxPosY)) # (font object, (xpos, ypos))

if len(joysticks) >= 1: # Determine if joystick input can be used, if not, run w/o this functionality
    joysticks[0].init()
    joystickson = True
else:
    joystickson = False



# MAIN LOOP

while True:

    clock.tick(fps)

    # PROCESS INPUT

    # Process values for the joysticks
    if joysticks_connected == True:
        for i in range(0, len(joysticks)):
            angle = (joy2value(joysticks[i].get_axis(0), True))
            speed = (joy2value(joysticks[i].get_axis(1), (not joysticks[i].get_button(0))))
            angle = float256(angle, -1, 1)
            speed = float256(speed, -1, 1)
            print("ANGLE: " + str(ord(chr(angle))))
            print("SPEED: " + str(ord(chr(speed))))

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

    # Other important keyboard states
    if keysDowned[pygame.K_LSHIFT] or keysDowned[pygame.K_RSHIFT]:
        emergencyStopButtonsEnabled = True
    else:
        emergencyStopButtonsEnabled = False

    # Everything to do with event queue - keys down, etc.
    for event in pygame.event.get():

        # Check if program closed
        if event.type == pygame.QUIT:sys.exit()

        # Check mouse
        if event.type == pygame.MOUSEBUTTONDOWN:
            clickPosition = pygame.mouse.get_pos() # Find position of a mouse click
            if emergencyStopButtonsEnabled: # Collision detection if shift held to enable E-stop buttons
                if eStopButton.collidepoint(clickPosition):
                    roverStopped = not roverStopped # TOGGLE boolean status
                    print "EMERGENCY STOP!" #TODO: add functionality to send stop signal over UDP
                if potStopButton.collidepoint(clickPosition):
                    potStopped = not potStopped # TOGGLE boolean status
                    print "Potentiometer stop."

        # Check keyboard
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
                        newCoord = Coordinates("".join(current_string)) #turn char array into string
                        print "".join(current_string)
                        if newCoord.status == False:
                            print "invalid entry"
                        else:
                            markerList.append(newCoord)
                            print str(newCoord.latitude) + " " + str(newCoord.longitude)
                            print coordToPixel1(float(markerList[increment].latitude), float(markerList[increment].longitude), mainMap)
                            increment = increment + 1 #TODO: remove debug shit
                    elif inkey == K_MINUS:
                        current_string.append("-")
                    elif inkey == 56 and (pygame.key.get_mods() and KMOD_SHIFT): # If asterisk pressed
                        current_string.append("*")
                    #TODO: allow numberpad inputs. Above 255 ascii range?
                    elif (inkey >= 48 and inkey <= 57) or inkey == 44 or inkey == 46: # If key pressed is in the ASCII number range, or is a comma, period or star
                        if len(current_string) <= 30:
                            current_string.append(chr(inkey))

    # Send information to rover
    if potStopped:
        pot_flag = 1
    else:
        pot_flag = 0

    # send 0 for normal rover operation, 1 for emergency stop
    if roverStopped:
        stop_flag = 1
    else:
        stop_flag = 0

    # send message in form of characters for the potentiometer flag, emergency stop flag, angle, and speed
    message = ''.join([chr(pot_flag), chr(stop_flag), chr(angle), chr(speed)]) #str originally instead of chr ?
    print(message)
    print(pot_flag)
    # Send data over UDP, print recv
    sock.sendto(message, (TARGET_IP, UDP_PORT))

    # END PROCESSING INPUT


    # DRAW EVERYTHING TO SCREEN

    screen.fill((0, 0, 0)) # Start with black background

    # Render all of the map tiles
    for n in range(0,mainMap.dimensions[0]):
        for m in range(0,mainMap.dimensions[1]):
            display(mapTiles[n][m].image,mapTiles[n][m].screenlocation)

    if emergencyStopButtonsEnabled: # Draw emergency boxes if enabled (shift held)

        # E-stop button
        if not roverStopped:
            pygame.draw.rect(screen, DARKBOXRED, eStopButton)
            screen.blit(StopText, allStopTextpos) # Display word "stop" at its position
        elif roverStopped:
            pygame.draw.rect(screen, WARNINGBOXORANGE, eStopButton)
            screen.blit(StoppedText, allStoppedTextpos) # Display word "stop" at its position
        screen.blit(allText, allTextpos) # Word "all" on all-stop button

        # Pot-stop button
        if not potStopped:
            pygame.draw.rect(screen, DARKBOXRED, potStopButton)
            screen.blit(StopText, potStopTextpos) # "stop"
        elif potStopped:
            pygame.draw.rect(screen, WARNINGBOXORANGE, potStopButton)
            screen.blit(StoppedText, potStoppedTextpos)
        screen.blit(potText, potTextpos) # Word "pot" on pot-stop button



    # Textbox entry stuff
    screen.blit(fontCoordinateEntry, (10, 500))
    if textboxEnabled == True:
        display_box(screen, string.join(current_string,""), CoordBoxPosX, CoordBoxPosY)
        pygame.draw.rect(screen, (255, 0, 0), (125, 500, 150, 16), 1) # Draw white box (x, y, xlength, ylength, ?)
    else:
        pygame.draw.rect(screen, (255, 255, 255), (125, 500, 150, 16), 1) # Draw white box (x, y, xlength, ylength, ?)

    # Render all markers
    for balls in range(len(markerList)):
        string1 = markerList[balls]
        display(ball, coordToPixel1(float(markerList[balls].latitude), float(markerList[balls].longitude), mainMap)) # Add code to make this convert coordinates to screen position

    # Re-render entire display, pump event queue
    pygame.display.flip()
    pygame.event.pump()

# ---------------- end update loop -------------------------------