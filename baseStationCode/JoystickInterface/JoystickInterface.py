from __future__ import division
from __future__ import print_function

import pygame
import math
import struct
import socket

# Joystick Constants
x = 0
y = 0
joysticksConnected = False
joystick = []
joynum = 0
joycommands = ["Arm", "Drive"]

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
RECTCOLOR = PURPLE
TEXTCOLOR = GOLD

PI = math.pi

# Define characteristics of the screen
size = (700, 500)
screen = pygame.display.set_mode(size)
screen.fill(WHITE)
button = pygame.Rect(100, 100, 200, 200)
display = pygame.Rect(100, 400, 400, 100)
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

# Mpas
def float256(value, low, high):
    value = 256 * (value - low) / (high - low)
    value = max([value, 0])
    value = min([value, 255])
    return int(value)

def redraw_screen():
    screen.fill(WHITE)
    button = pygame.draw.rect(screen, RECTCOLOR, pygame.Rect(100, 100, 200, 200), 0)
    pygame.draw.rect(screen, RECTCOLOR, pygame.Rect(100, 350, 400, 100), 0)
    text = font.render('Connect Joysticks!', 1, TEXTCOLOR)
    textpos = text.get_rect()
    textpos.centerx = button.centerx
    textpos.centery = button.centery
    screen.blit(text, (textpos.centerx - textpos.width/2, textpos.centery - textpos.height/2))
    pygame.display.flip()

def create_user_commands():
    for i in range(0, len(joycommands)):
        joycommands[i] = "Connect " + joycommands[i] + " Joystick"

def connect_joysticks(rect):
    try:
        create_user_commands()
        font = pygame.font.Font(None, 36)
        text = font.render(joycommands[0], 1, (10, 10, 10))
        textpos = text.get_rect()
        textpos.centerx = rect.centerx
        textpos.centery = rect.centery

        # Joystick
        pygame.joystick.init()
        pygame.display.init()
        joynum = pygame.joystick.get_count()
        print(str(joynum) + " joysticks connected.")

        for i in range(joynum):
            pygame.joystick.Joystick(i).init()

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

if __name__ == '__main__':
    # UDP
    print("UDP Port: ", UDP_PORT)
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    sock.bind(('0.0.0.0', UDP_PORT))
    sock.settimeout(0.01)


# ------ Main Program Loop -------
while not done:
    screen.fill(WHITE)
    button = pygame.draw.rect(screen, RECTCOLOR, button, 0)
    display = pygame.draw.rect(screen, RECTCOLOR, pygame.Rect(100, 350, 400, 100), 0)
    text = font.render('Connect Joysticks!', 1, TEXTCOLOR)
    textpos = text.get_rect()
    textpos.centerx = button.centerx
    textpos.centery = button.centery
    screen.blit(text, (textpos.centerx - textpos.width/2, textpos.centery - textpos.height/2))
    text = font.render('Number of Connected Joysticks: ' + str(joynum), 1, TEXTCOLOR)
    textpos = text.get_rect()
    textpos.centerx = display.centerx
    textpos.centery = display.centery
    screen.blit(text, (textpos.centerx - textpos.width/2, textpos.centery - textpos.height/2))

    # --- Main event loop
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            done = True
        elif event.type == pygame.MOUSEBUTTONDOWN:
            click = button.collidepoint(pygame.mouse.get_pos())
            if click == 1:
                joynum = connect_joysticks(button)
                if joynum != 0:
                    joysticksConnected = True

    # -- update screen
    pygame.display.flip()

    # -- limit to 60 frames per second
    clock.tick(60)

    # Get and convert joystick value, print
    pygame.event.pump()

    if joysticksConnected == True:
        for i in range(0, len(joystick)):
            x = (joy2value(joystick[i].get_axis(0), (not joystick[i].get_button(0))))
            y = (joy2value(joystick[i].get_axis(1), (not joystick[i].get_button(0))))
            x = float256(x, -1, 1)
            y = float256(y, -1, 1)
            print("x" + str(i) + ": " + str(x))
            print("y" + str(i) + ": " + str(y))
            message = ''.join([chr(x), chr(y)])
            # Send data over UDP, print recv
            sock.sendto(message, (TARGET_IP, UDP_PORT))
            pygame.time.wait(100)

        pygame.time.wait(100)


pygame.quit()