from __future__ import division
from __future__ import print_function

import pygame
import math

# Joystick Constants
x = 0
y = 0
joysticksConnected = False
joystick = []
joynum = 0
joycommands = ["Arm", "Drive"]

pygame.init()
# Loop until the user clicks the close button.
done = False

# Define some colors
BLACK = (0, 0, 0)
WHITE = (255, 255, 255)
GREEN = (0, 255, 0)
RED = (255, 0, 0)
PURPLE = (60, 45, 112)
GOLD = (213, 202, 148)

PI = math.pi

# Define characteristics of the screen
size = (700, 500)
screen = pygame.display.set_mode(size)
screen.fill(WHITE)
rect = pygame.Rect(100, 100, 200, 200)
rect2 = pygame.Rect(100, 400, 400, 100)
pygame.display.set_caption("The Interactive Joystick Interface")
font = pygame.font.SysFont('Arial', 25)

# Used to manage how fast the screen updates
clock = pygame.time.Clock()

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
    rect = pygame.draw.rect(screen, PURPLE, pygame.Rect(100, 100, 200, 200), 0)
    pygame.draw.rect(screen, PURPLE, pygame.Rect(100, 350, 400, 100), 0)
    text = font.render('Connect Joysticks!', 1, GOLD)
    textpos = text.get_rect()
    textpos.centerx = rect.centerx
    textpos.centery = rect.centery
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
            pygame.draw.rect(screen, PURPLE, rect2, 0)
            text = font.render(joycommands[len(joystick)], 1, GOLD) #array[len(joystick)]
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

        for i in range(joynum):
            print("Joystick " + str(i) + " is: " + joystick[i].get_name() + " with " + str(joystick[i].get_numaxes()) + " axes.")

        return joynum

    except KeyboardInterrupt:
        pygame.quit()


# ------ Main Program Loop -------
while not done:
    screen.fill(WHITE)
    rect = pygame.draw.rect(screen, PURPLE, rect, 0)
    rect2 = pygame.draw.rect(screen, PURPLE, pygame.Rect(100, 350, 400, 100), 0)
    text = font.render('Connect Joysticks!', 1, GOLD)
    textpos = text.get_rect()
    textpos.centerx = rect.centerx
    textpos.centery = rect.centery
    screen.blit(text, (textpos.centerx - textpos.width/2, textpos.centery - textpos.height/2))
    text = font.render('Number of Connected Joysticks: ' + str(joynum), 1, GOLD)
    textpos = text.get_rect()
    textpos.centerx = rect2.centerx
    textpos.centery = rect2.centery
    screen.blit(text, (textpos.centerx - textpos.width/2, textpos.centery - textpos.height/2))

    # --- Main event loop
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            done = True
        elif event.type == pygame.MOUSEBUTTONDOWN:
            click = rect.collidepoint(pygame.mouse.get_pos())
            if click == 1:
                joynum = connect_joysticks(rect)
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

        pygame.time.wait(100)


pygame.quit()
