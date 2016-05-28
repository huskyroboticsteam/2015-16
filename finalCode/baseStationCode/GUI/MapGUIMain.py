# Python libraries
import sys
import pygame
from pygame.locals import *
import CameraMovement

# User-created files
from JoystickConnector import *
from Display import *
from sendOverUDP import *
from receiveOverUDP import *

# Initial screen parameters
screenWidth = 3200
screenHeight = 1800

roverAngle = 0

# Start pygame and necessary functionality
pygame.init()
pygame.font.init()

# Run the joystick connection interface if needed
JoystickH = JoystickHandler(pygame) # Joystick handler class
ScreenDisplay = DisplayBoxes(pygame, screenWidth,screenHeight,JoystickH.numberOfJoysticks)

while not JoystickH.joysticksEnabled: # Display loop for joystick connection interface (the 4 boxes), if no joysticks connected, the joystick handler will return True for joysticks enabled and this loop will not run.
    JoystickH.connectJoysticks() # See if any joystick buttons pressed
    ScreenDisplay.display(JoystickH.numberOfConnectedJoysticks) # Update the display
    pygame.event.pump()
    pygame.time.wait(50)

# Now done with joystick connection, switch to regular interface
ScreenDisplay = DisplayInterface(pygame, screenWidth,screenHeight)

# Initialize UDP-sending class that sends data to rover
UDPsender = sendOverUDP("192.168.1.51", 8888)
UDPreceiver = receiveOverUDP("192.168.1.40", 8887)

# Initialize preset variables
textboxEnabled = False
previousMouseStatus = False

UDPreceiver.start()
MESSAGE = " "
RECEIVERTHREADLOCK = threading.Lock()

# MAIN LOOP
while True:

    # Process values for the joysticks
    JoystickH.scanJoysticks()

    # Mouse panning TODO: add mouse class?
    mousestatus = pygame.mouse.get_pressed()
    if mousestatus[0]:
        movement = pygame.mouse.get_rel()
        if previousMouseStatus == False:
            previousMouseStatus = True
        else:
            ScreenDisplay.moveMap(movement)
    if not mousestatus[0]:
        previousMouseStatus = False

    keysDowned = pygame.key.get_pressed()
    # Other important keyboard states
    if keysDowned[pygame.K_LSHIFT] or keysDowned[pygame.K_RSHIFT]:
        ScreenDisplay.EmergencyButtonsEnabled = True
    else:
        ScreenDisplay.EmergencyButtonsEnabled = False

    #Everything to do with event queue - keys down, etc.
    for event in pygame.event.get():

        # Check if program closed
        if event.type == pygame.QUIT:
            UDPreceiver.End = True
            sys.exit()

        # Check mouse
        if event.type == pygame.MOUSEBUTTONDOWN:
            clickPosition = pygame.mouse.get_pos() # Find position of a mouse click
            ScreenDisplay.clickCheck(clickPosition)

        # Check keyboard
        if event.type == pygame.KEYDOWN:
            inputKey = event.key
            CameraMovement.keyPressed(inputKey)
            if (inputKey == pygame.K_f): # Enter fullscreen
                ScreenDisplay.toggleFullscreen()
            if (inputKey == pygame.K_ESCAPE): # Escape fullscreen, textbox, whatever
                if ScreenDisplay.InputTextbox.status == True:
                    ScreenDisplay.InputTextbox.disable()
                else:
                    ScreenDisplay.escapeFullscreen()
            if (inputKey == pygame.K_z): # Zoom out
                ScreenDisplay.resizeMap('Out')
            if (inputKey == pygame.K_x): # Zoom in
                ScreenDisplay.resizeMap('In')
            if (inputKey == pygame.K_m): # Enable coordinate entry mode
                ScreenDisplay.InputTextbox.enableCoordinateEntry()
            if (inputKey == pygame.K_r): # Enable window resizing mode
                ScreenDisplay.InputTextbox.enableWindowResizing()
            if (inputKey == pygame.K_BACKSPACE):
                ScreenDisplay.InputTextbox.currentString = ScreenDisplay.InputTextbox.currentString[0:-1]
            if (inputKey == pygame.K_RETURN):
                ScreenDisplay.getEntry()

            # TESTING INPUT
            if (inputKey == pygame.K_y):
                roverAngle += 10
                ScreenDisplay.RoverGraphic.rotateImage(pygame, roverAngle)

            #TODO: Input handling class
            if inputKey == 56 and (pygame.key.get_mods() and KMOD_SHIFT): # If asterisk pressed
                validKeyIn = True
                inputKey = 42
            elif (inputKey >= 48 and inputKey <= 57) or inputKey == 44 or inputKey == 46: # If key pressed is in the ASCII number range, or is a comma or period
                validKeyIn = True
            elif inputKey == 44 or inputKey == 46:
                validKeyIn = True
            elif inputKey == K_MINUS:
                validKeyIn = True
            else:
                validKeyIn = False

            if validKeyIn == True:
                ScreenDisplay.InputTextbox.currentString.append(chr(inputKey))

    # UDP sending stuff
    AllStopStatus = ScreenDisplay.buttons[0].Status
    PotStopStatus = ScreenDisplay.buttons[1].Status

    messageUDP = JoystickH.addInput(AllStopStatus, PotStopStatus)
#    messageUDP = CameraMovement.addInput(messageUDP)
    UDPsender.sendItOff(messageUDP)

    # UDP receiving stuff
    print UDPreceiver.Coord
    ScreenDisplay.giveReceivedInformation(UDPreceiver.Coord)


    # DRAW EVERYTHING TO SCREEN
    ScreenDisplay.display()

    pygame.event.pump()

# ---------------- end -------------------------------