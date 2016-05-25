__author__ = 'Trevor'

class JoystickHandler:
    def __init__(self, pygame):
        'Controls the Joystick connection process.'
        self.pygame = pygame
        self.pygame.joystick.init()
        self.numberOfJoysticks = self.pygame.joystick.get_count()
        self.connectedJoystickNumbers = [] # When joystick successfully connected, its ID will be added here
        self.connectedJoysticks = []
        self.numberOfConnectedJoysticks = 0
        self.joysticksEnabled = False
        self.Angle = 0
        self.Speed = 0

        for i in range(self.numberOfJoysticks):
            pygame.joystick.Joystick(i).init()

        print(str(self.numberOfJoysticks) + " joysticks connected.")

        if self.numberOfJoysticks > 0:
            JoystickHandler.joysticks_connected = True
        else:
            JoystickHandler.joysticks_connected = False

    def connectJoysticks(self):
        for i in range(self.numberOfJoysticks): # Iterate through all joysticks connected to computer.
            if self.pygame.joystick.Joystick(i).get_button(0): # Check every joystick to see if trigger pulled.
                if i not in self.connectedJoystickNumbers: # Check to make sure joystick wasn't already added
                    self.connectedJoysticks.append(self.pygame.joystick.Joystick(i))
                    self.connectedJoystickNumbers.append(i)
                    self.numberOfConnectedJoysticks += 1
        if self.numberOfConnectedJoysticks == self.numberOfJoysticks:
            self.joysticksEnabled = True

    def scanJoysticks(self):
        for i in range(self.numberOfJoysticks):
            angle = (self.joy2value(self.connectedJoysticks[i].get_axis(0), True))
            speed = (self.joy2value(self.connectedJoysticks[i].get_axis(1), (not self.connectedJoysticks[i].get_button(0))))
            self.Angle = self.float256(angle, -1, 1)
            self.Speed = self.float256(speed, -1, 1)

    def addInput(self,AllStopStatus,PotStopStatus):
        if AllStopStatus == True:
            allStopSend = chr(0)
        else:
            allStopSend = chr(1)
        if PotStopStatus == True:
            potStopSend = chr(0)
        else:
            potStopSend = chr(1)
        # send message in form of characters for the potentiometer flag, emergency stop flag, angle, and speed
        messageUDP = ''.join([potStopSend, allStopSend, chr(self.Angle), chr(self.Speed)]) #str originally instead of chr ? Also replace zeroes with bytes as above TODO: ADD BUTTON FUNCTIONALITY
        return messageUDP;

    # Processes the joystick values. Doubles if button for extra thrust is down, rounds values below 0.5 to 0
    def joy2value(self, value, half_control=False):
        if half_control:
            value /= 2.0
        if abs(value - 0) < 0.05:
            value = 0
        return value

    # Maps values to range from 0 to 255
    def float256(self, value, low, high):
        value = 256 * (value - low) / (high - low)
        value = max([value, 0])
        value = min([value, 255])
        return int(value)