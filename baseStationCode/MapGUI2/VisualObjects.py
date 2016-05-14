__author__ = 'Trevor'

import colors
import string
from GPSCoordinates import *
import MapPixelCoords

class Sidebar:
    def __init__(self,pygame,screenHeight):
        self.Rect = pygame.Surface((500,screenHeight), pygame.SRCALPHA, 32)
        self.Color = (0,0,0,175) # Black but transparent
        self.Rect.fill(self.Color)
    def display(self, pygame, screen):
        screen.blit(self.Rect, (0,0))

class Button:
    def __init__(self, pygame, Xoffset, Yoffset, alignment, widthRatio, heightRatio, screenWidth, screenHeight, staticText, toggleTextFalse, toggleTextTrue, Font, startingStatus = False):
        self.Status = startingStatus

        if alignment == 'Left-aligned':
            X = Xoffset
            Y = Yoffset
        elif alignment == 'Right-aligned':
            X = screenWidth - Xoffset
            Y = Yoffset

        self.Width = screenWidth*widthRatio
        self.Height = screenHeight*heightRatio
        self.X = X
        self.Y = Y
        self.Font = Font
        self.Rectangle = pygame.Rect(X,Y,self.Width,self.Height)

        self.StaticText = Font.render(staticText, 1, colors.GOLD)
        self.ToggleTextFalse = Font.render(toggleTextFalse, 1, colors.GOLD)
        self.ToggleTextTrue = Font.render(toggleTextTrue, 1, colors.GOLD)

        self.StaticTextPos = self.findTextCenterPos(staticText,'Top')
        self.ToggleTextFalsePos = self.findTextCenterPos(toggleTextFalse,'Bottom')
        self.ToggleTextTruePos = self.findTextCenterPos(toggleTextTrue,'Bottom')

        self.check()

    def findTextCenterPos(self, text, alignment = 'Center'):
        textSize = self.Font.size(text)
        X = self.X + (self.Width - textSize[0])/2
        Y = self.Y + (self.Height - textSize[1])/2
        if alignment == 'Top':
            Y -= textSize[1]/2
        elif alignment == 'Bottom':
            Y += textSize[1]/2
        return (X,Y)

    def check(self):
        if self.Status == True: # Update the button's color
            self.Color = colors.WARNINGBOXORANGE
            self.ToggleText = self.ToggleTextTrue
            self.ToggleTextPos = self.ToggleTextTruePos
        else:
            self.Color = colors.DARKBOXRED
            self.ToggleText = self.ToggleTextFalse
            self.ToggleTextPos = self.ToggleTextFalsePos

    def toggle(self):
        self.Status = not self.Status
        self.check()

    def display(self, pygame, screen):
        pygame.draw.rect(screen, self.Color, self.Rectangle)
        screen.blit(self.StaticText,self.StaticTextPos)
        screen.blit(self.ToggleText,self.ToggleTextPos)

    def clicked(self, position):
        if self.Rectangle.collidepoint(position):
            self.toggle()

class Textbox:
    def __init__(self):
        self.TextboxStatus = False
        self.currentString = []
        self.color = colors.WHITE
        self.status = 0
        self.X = 100
        self.Y = 400

    def enableCoordinateEntry(self):
        self.color = colors.HIGHLIGHTBOXRED
        self.status = 1

    def enableWindowResizing(self):
        self.color = colors.HIGHLIGHTBOXGREEN
        self.status = 2

    def disable(self):
        self.color = colors.WHITE
        self.currentString = []
        self.status = 0

    def display(self, pygame, screen, Font):
        if self.status != 0:
            textInBox = Font.render(string.join(self.currentString,""), 0, colors.WHITE) # (text, antialias, (r, g, b))
            screen.blit(textInBox,(self.X+4,self.Y))
        pygame.draw.rect(screen, self.color, (self.X, self.Y, 150, 16), 1) # Draw box (x, y, xlength, ylength, ?)

    def returnString(self):
        string = "".join(self.currentString) # Take an array of chars and join them all together into a single string.
        statusOut = self.status
        if len(string) > 30:
            statusOut = 3 # Make the status invalid if the string is too long and clearly invalid
        if statusOut == 1:
            newString = Coordinates(string)
        elif statusOut == 2:
            newString = string
        else:
            newString = "no"
        self.disable()
        return (newString, statusOut)

class Marker:
    def __init__(self, coord, initZoomMap):
        self.coord = coord
        self.updateZoom(initZoomMap)
    def display(self, pygame, screen, image, axes):
        imageSizeCorrection = (-image.get_rect().size[0]/2,-image.get_rect().size[1]/2) # Adds correction to image's coordinates to make it appear at the center of the coordinate given.
        axesPos = (axes.x,axes.y)
        Xactual = self.X+axesPos[0]+imageSizeCorrection[0]
        Yactual = self.Y+axesPos[1]+imageSizeCorrection[1]
        screen.blit(image, (Xactual,Yactual)) # Display each marker at center of given pixel coordinate
    def updateZoom(self, ZoomMap):
        PixelCoord = MapPixelCoords.coordToPixel1(float(self.coord.latitude), float(self.coord.longitude), ZoomMap)
        self.X = PixelCoord[0]
        self.Y = PixelCoord[1]

class ScaleIndicator:
    def __init__(self, pygame, distance):
        Font = pygame.font.SysFont('Arial', 30)
        if distance < 1.0:
            distance *= 1000 # Convert km value to meters if the distance is less than 1 km
            unit = 'meters'
        else:
            unit = 'km'
        distance = round(distance,2)
        self.text = Font.render(str(distance)+' '+unit,1,colors.HIGHLIGHTBOXRED)
        self.X = 100
        self.Y = 100
    def display(self, screen, image):
        screen.blit(image, (self.X,self.Y))
        screen.blit(self.text, (self.X+85,self.Y+50))