import io
import sys, pygame, pygame.font, pygame.event, pygame.draw, string
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

# Initialize preset variables
textboxEnabled = False
x, y, axisx, axisy = 0, 0, 0, 0
markerListX, markerListY = [], []

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

    for event in pygame.event.get():
        if event.type == pygame.QUIT:sys.exit()
        if event.type == pygame.KEYDOWN:
            if (event.key == pygame.K_LEFT) and (textboxEnabled == False):
                textboxEnabled = True
                current_string = []

            if textboxEnabled == True:
                if event.type == pygame.KEYDOWN:
                    inkey = event.key
                    if inkey == K_BACKSPACE:
                        current_string = current_string[0:-1]
                    elif inkey == K_RETURN:
                        textboxEnabled = False

                        coordRead = ""
                        for chars in range(len(current_string) + 1):
                            if chars == len(current_string):
                                markerListY.append(int(coordRead))
                            elif current_string[chars] == ",":
                                markerListX.append(int(coordRead))
                                coordRead = ""
                            else:
                                coordRead = coordRead + str(current_string[chars])
                    elif inkey == K_MINUS:
                        current_string.append("_")
                    elif (inkey >= 48 and inkey <= 57) or inkey == 44: # If key pressed is in the ASCII number range, or is a comma...
                        current_string.append(chr(inkey))
    # end event queue loop

    screen.blit(fontCoordinateEntry, (10, 500))

    if textboxEnabled == True:
        display_box(screen, string.join(current_string,""), CoordBoxPosX, CoordBoxPosY)
        pygame.draw.rect(screen, (255, 0, 0), (125, 500, 150, 16), 1) # Draw white box (x, y, xlength, ylength, ?)
    else:
        pygame.draw.rect(screen, (255, 255, 255), (125, 500, 150, 16), 1) # Draw white box (x, y, xlength, ylength, ?)

    for balls in range(len(markerListX)):
        screen.blit(ball, (markerListX[balls], markerListY[balls]))

    if joystickson == True:
        axisx = joysticks[0].get_axis(0)
        axisy = joysticks[0].get_axis(1)

    x += axisx
    y += axisy

    pygame.display.flip()

    pygame.event.pump()

# ---------------- end update loop -------------------------------