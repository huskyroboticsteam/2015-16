from __future__ import division
from __future__ import print_function

import pygame
import math
import tkFont
import sys
import socket
import struct

# Joystick Constants
x = 0
y = 0

pygame.init()

# Define some colors
BLACK = (0, 0, 0)
WHITE = (255, 255, 255)
GREEN = (0, 255, 0)
RED = (255, 0, 0)

PI = math.pi

size = (700, 500)
screen = pygame.display.set_mode(size)
screen.fill(WHITE)
rect = pygame.Rect(100, 100, 200, 200)
rect2 = pygame.Rect(100, 400, 400, 100)

pygame.display.set_caption("The Interactive Joystick Interface")

# Loop until the user clicks the close button.
done = False

joysticksConnected = False

# Used to manage how fast the screen updates
clock = pygame.time.Clock()

joystick = []
joynum = 0
joycommands = ["Connect Arm Joystick", "Connect Drive Joystick"]

font = pygame.font.SysFont('Arial', 25)

def joy2value(value, half_control=False):
    if half_control:
        value /= 2.0
    if abs(value - 0) < 0.05:
        value =0
    return value

def float256(value, low, high):
    value = 256 * (value - low) / (high - low)
    value = max([value, 0])
    value = min([value, 255])
    return int(value)

def redraw_screen():
    screen.fill(WHITE)
    rect = pygame.draw.rect(screen, GREEN, pygame.Rect(100, 100, 200, 200), 0)
    pygame.draw.rect(screen, GREEN, pygame.Rect(100, 350, 400, 100), 0)
    text = font.render('Connect Joysticks!', 1, (255,0,0))
    textpos = text.get_rect()
    textpos.centerx = rect.centerx
    textpos.centery = rect.centery
    screen.blit(text, (textpos.centerx - textpos.width/2, textpos.centery - textpos.height/2))
   # screen.blit(font.render('Number of Connected Joysticks: ' + str(joynum), True, (255,0,0)), (300, 300))
    pygame.display.flip()

def connect_joysticks(rect):
    try:
        font = pygame.font.Font(None, 36)
        text = font.render(joycommands[0], 1, (10, 10, 10))
        textpos = text.get_rect()
        textpos.centerx = rect.centerx
        textpos.centery = rect.centery
      #  screen.blit(text, (textpos.centerx - textpos.width/2, textpos.centery - textpos.height/2))
     #   pygame.display.flip()
        # Joystick
        pygame.joystick.init()
        pygame.display.init()
        joynum = pygame.joystick.get_count()
        print(str(joynum) + " joysticks connected.")

        for i in range(joynum):
            pygame.joystick.Joystick(i).init()

        while len(joystick) < joynum:
            pygame.event.pump()
            pygame.draw.rect(screen, GREEN, rect2, 0)
            text = font.render(joycommands[len(joystick)], 1, (10, 10, 10)) #array[len(joystick)]
            textpos = text.get_rect()
            textpos.centerx = rect2.centerx
            textpos.centery = rect2.centery
            screen.blit(text, (textpos.centerx - textpos.width/2, textpos.centery - textpos.height/2))
            pygame.display.flip()
            for i in range(joynum):
                if pygame.joystick.Joystick(i).get_button(0):
                    redraw_screen()
                    joystick.append(pygame.joystick.Joystick(i))
                    pygame.time.wait(500)

    #    text = font.render('Number of Connected Joysticks: ' + str(joynum), 1, (10, 10, 10)) #array[len(joystick)]
     #   textpos = text.get_rect()
      #  textpos.centerx = rect2.centerx
       # textpos.centery = rect2.centery
       # screen.blit(text, (textpos.centerx - textpos.width/2, textpos.centery - textpos.height/2))
        #pygame.display.flip()

        for i in range(joynum):
            print("Joystick " + str(i) + " is: " + joystick[i].get_name() + " with " + str(joystick[i].get_numaxes()) + " axes.")

                # Initialize
        arm = []
        for i in range(4):
            arm.append(1)

       # science = []
     #   for i in range(3):
      #      science.append(1)

        hand = []
        for i in range(4):
            hand.append(1)

        misc = []
        for i in range(4):
            misc.append(1)

        return joynum

    except KeyboardInterrupt:
        pygame.quit()


# ------ Main Program Loop -------
while not done:
    screen.fill(WHITE)
    rect = pygame.draw.rect(screen, GREEN, rect, 0)
    rect2 = pygame.draw.rect(screen, GREEN, pygame.Rect(100, 350, 400, 100), 0)
    text = font.render('Connect Joysticks!', 1, (255,0,0))
    textpos = text.get_rect()
    textpos.centerx = rect.centerx
    textpos.centery = rect.centery
    screen.blit(text, (textpos.centerx - textpos.width/2, textpos.centery - textpos.height/2))
    text = font.render('Number of Connected Joysticks: ' + str(joynum), 1, (255,0,0))
    textpos = text.get_rect()
    textpos.centerx = rect2.centerx
    textpos.centery = rect2.centery
    screen.blit(text, (textpos.centerx - textpos.width/2, textpos.centery - textpos.height/2))
    #screen.blit(text, True, (255,0,0), (300, 300))
    # --- Main event loop
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            done = True
        # elif event.type == pygame.KEYDOWN:
            #print("User let go of a key.")
        # elif event.type == pygame.KEYUP:
            #print("user let go of a key.")
        elif event.type == pygame.MOUSEBUTTONDOWN:
            click = rect.collidepoint(pygame.mouse.get_pos())
            if click == 1:
                joynum = connect_joysticks(rect)
                if joynum != 0:
                    joysticksConnected = True

    #screen.fill(WHITE)
    # --game logic
    # --drawing code

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
            # Find out if values need to be halved or not and trim small values to zero
           # x1 = (joy2value(joystick[0].get_axis(0), (not joystick[0].get_button(0))))
        # y1 = (joy2value(joystick[0].get_axis(1), (not joystick[0].get_button(0))))
    #  x2 = (joy2value(joystick[0].get_axis(2), (not joystick[0].get_button(0))))
    #  y2 = (joy2value(joystick[0].get_axis(3), (not joystick[0].get_button(0))))

        # Map out
       # x1 = float256(x1, -1, 1)
       # y1 = float256(y1, -1, 1)
    #  x2 = float256(x2, -1, 1)
    #  y2 = float256(y2, -1, 1)

       # print("x1: " + str(x1))
       # print("y1: " + str(y1))
    # print("x2: " + str(x2))
    # print("y2: " + str(y2))

    # x = ((x1 - 128) + (x2 - 128)) + 128
    # y = ((y1 - 128) + (y2 - 128)) + 128

        #if x > 255:
        #   x=255

        #      if x < 0:
        #         x=0

         #    if y > 255:
             #       y=255

            #  if y < 0:
            #     y=0

                #message = ''.join([chr(x), chr(y)])

        pygame.time.wait(100)


pygame.quit()
