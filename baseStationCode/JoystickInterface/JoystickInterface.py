from __future__ import division
from __future__ import print_function

import pygame
import math
import socket

# Joystick Constants
angle = 0
speed = 0
joysticksConnected = False
joystick = []
joynum = 0
# Array with the types of joysticks that we have
joycommands = ["Drive"]

# Start Pygame
pygame.init()
# Loop until the user clicks the close button.
done = False

# UDP Constants
TARGET_IP = "192.168.1.51"
UDP_PORT = 8888
MAX_BUFFER_SIZE = 24

#UDP Defaults message
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
message = ""

# Define some colors
WHITE = (255, 255, 255)
PURPLE = (60, 45, 112)
GOLD = (213, 202, 148)
RED = (205, 40, 40)
BLACK = (0, 0, 0)
RECTCOLOR = PURPLE
TEXTCOLOR = GOLD
POTCOLOR = RED
STOPCOLOR = RED

PI = math.pi

# Define initial characteristics of the screen
size = (700, 500)
screen = pygame.display.set_mode(size)
screen.fill(WHITE)
BUTTON = pygame.Rect(100, 100, 250, 200)
DISPLAY = pygame.Rect(100, 350, 400, 100)
STOP = pygame.Rect(500, 100, 175, 200)
POT = pygame.Rect(362, 100, 125, 200)
pygame.display.set_caption("The Interactive Joystick Interface")
font = pygame.font.SysFont('Arial', 25)

# Used to manage how fast the screen updates
clock = pygame.time.Clock()

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

# Method to redraw the default screen
def redraw_screen():
    screen.fill(WHITE)
    button = pygame.draw.rect(screen, RECTCOLOR, BUTTON, 0)
    pygame.draw.rect(screen, RECTCOLOR, DISPLAY, 0)
    pygame.draw.rect(screen, STOPCOLOR, STOP, 0)    # is this needed?
    pygame.draw.rect(screen, POTCOLOR, POT, 0)
    text = font.render('Connect Joysticks!', 1, TEXTCOLOR)
    textpos = text.get_rect()
    textpos.centerx = button.centerx
    textpos.centery = button.centery
    screen.blit(text, (textpos.centerx - textpos.width/2, textpos.centery - textpos.height/2))
    pygame.display.flip()

# Method to create the commands for connecting different joysticks
def create_user_commands():
    for i in range(0, len(joycommands)):
        joycommands[i] = "Connect " + joycommands[i] + " Joystick"

# Handles the connecting of the joysticks
def connect_joysticks(rect):
    try:
        # create the command to tell user to connect a certain joystick
        create_user_commands()
        font = pygame.font.Font(None, 36)
        text = font.render(joycommands[0], 1, (10, 10, 10))
        textpos = text.get_rect()
        textpos.centerx = rect.centerx
        textpos.centery = rect.centery

        # Find out how many joysticks are connected to the computer
        pygame.joystick.init()
        pygame.display.init()
        joynum = pygame.joystick.get_count()
        print(str(joynum) + " joysticks connected.")

        # Initialize joysticks
        for i in range(joynum):
            pygame.joystick.Joystick(i).init()

        # Loop through joysticks to connect each one to interface individually
        while len(joystick) < joynum:
            pygame.event.pump()
            pygame.draw.rect(screen, RECTCOLOR, display, 0)
            text = font.render(joycommands[len(joystick)], 1, TEXTCOLOR) #array[len(joystick)]
            textpos = text.get_rect()
            textpos.centerx = display.centerx
            textpos.centery = display.centery
            screen.blit(text, (textpos.centerx - textpos.width/2, textpos.centery - textpos.height/2))
            pygame.display.flip()
            for i in range(joynum):
                if pygame.joystick.Joystick(i).get_button(0):
                    redraw_screen()
                    joystick.append(pygame.joystick.Joystick(i))
                    pygame.time.wait(500)

        for i in range(joynum):
            print("Joystick " + str(i) + " is: " + joystick[i].get_name() + " with " + str(joystick[i].get_numaxes()) + " axes.")

        return joynum

    except KeyboardInterrupt:
        pygame.quit()

# UDP initialization stuff
if __name__ == '__main__':
    # UDP
    print("UDP Port: ", UDP_PORT)
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    sock.bind(('0.0.0.0', UDP_PORT))
    sock.settimeout(0.01)


# ------ Main Program Loop -------
while not done:
    # Reset screen
    screen.fill(WHITE)
    # Draw the rectangles on the screen
    logo = pygame.image.load("robotics.jpeg")
    logorect = logo.get_rect()
    logorect.centerx = 575
    logorect.centery = 400
    screen.blit(logo, logorect)
    button = pygame.draw.rect(screen, RECTCOLOR, BUTTON, 0)
    display = pygame.draw.rect(screen, RECTCOLOR, DISPLAY, 0)
    stop = pygame.draw.rect(screen, STOPCOLOR, STOP, 0)
    pot = pygame.draw.rect(screen, POTCOLOR, POT, 0)
    text = font.render('Connect Joysticks!', 1, TEXTCOLOR)
    textpos = text.get_rect()
    textpos.centerx = button.centerx
    textpos.centery = button.centery
    screen.blit(text, (textpos.centerx - textpos.width/2, textpos.centery - textpos.height/2))
    text = font.render('Emergency Stop', 1, TEXTCOLOR)
    textpos = text.get_rect()
    textpos.centerx = stop.centerx
    textpos.centery = stop.centery
    screen.blit(text, (textpos.centerx - textpos.width/2, textpos.centery - textpos.height/2))
    text = font.render('Pot Stop', 1, TEXTCOLOR)
    textpos = text.get_rect()
    textpos.centerx = pot.centerx
    textpos.centery = pot.centery
    screen.blit(text, (textpos.centerx - textpos.width/2, textpos.centery - textpos.height/2))
    text = font.render('Number of Connected Joysticks: ' + str(joynum), 1, TEXTCOLOR)
    textpos = text.get_rect()
    textpos.centerx = display.centerx
    textpos.centery = display.centery
    screen.blit(text, (textpos.centerx - textpos.width/2, textpos.centery - textpos.height/2))

    # --- Main event loop
    # TODO: add feature to send potentiometer off and emergency stop over UDP
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            done = True
        elif event.type == pygame.MOUSEBUTTONDOWN:
            click = button.collidepoint(pygame.mouse.get_pos())
            if click == 1:
                joynum = connect_joysticks(button)
                if joynum != 0:
                    joysticksConnected = True
            click = stop.collidepoint(pygame.mouse.get_pos())    # ESTOP button clicked
            if click == 1:
                if STOPCOLOR == RED:
                    STOPCOLOR = BLACK
                else:
                    STOPCOLOR = RED
            click = pot.collidepoint(pygame.mouse.get_pos())     # Pot Stop button clicked
            if click == 1:
                if POTCOLOR == RED:
                    POTCOLOR = BLACK
                else:
                    POTCOLOR = RED

    # -- update screen
    pygame.display.flip()

    # -- limit to 60 frames per second
    clock.tick(60)

    # Get and convert joystick value, print
    pygame.event.pump()

    if joysticksConnected == True:
        # Process values for the joysticks
        for i in range(0, len(joystick)):
            angle = (joy2value(joystick[i].get_axis(0), True))
            speed = (joy2value(joystick[i].get_axis(1), (not joystick[i].get_button(0))))
            angle = float256(angle, -1, 1)
            speed = float256(speed, -1, 1)
            print("ANGLE: " + str(ord(chr(angle))))
            print("SPEED: " + str(ord(chr(speed))))
            message = ''.join([chr(angle), chr(speed)])
            # Send data over UDP, print recv
            sock.sendto(message, (TARGET_IP, UDP_PORT))
            pygame.time.wait(100)

        pygame.time.wait(100)


pygame.quit()