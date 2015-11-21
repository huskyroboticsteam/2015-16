from __future__ import division
from __future__ import print_function

import pygame

# Joystick Constants
x = 0
y = 0


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

# Main
if __name__ == '__main__':
    try:
        # Joystick
        pygame.joystick.init()
        pygame.display.init()
        joystick = []
        joynum = pygame.joystick.get_count()
        print(str(joynum) + " joysticks connected.")

        for i in range(joynum):
            pygame.joystick.Joystick(i).init()

        while len(joystick) < joynum:
            pygame.event.pump()
            for i in range(joynum):
                if pygame.joystick.Joystick(i).get_button(0):
                    joystick.append(pygame.joystick.Joystick(i))
                    pygame.time.wait(500)

        for i in range(joynum):
            print("Joystick " + str(i) + " is: " + joystick[i].get_name() + " with " + str(joystick[i].get_numaxes()) + " axes.")


        # Main Loop
        while True:
            # Get and convert joystick value, print
            pygame.event.pump()

            # Find out if values need to be halved or not and trim small values to zero
            x = (joy2value(joystick[0].get_axis(0), (not joystick[0].get_button(0))))
            y = (joy2value(joystick[0].get_axis(1), (not joystick[0].get_button(0))))

            # Map out
            x = float256(x, -1, 1)
            y = float256(y, -1, 1)

            print("x: " + str(x))
            print("y: " + str(y))
            pygame.time.wait(100)


    except KeyboardInterrupt:
        pygame.quit()