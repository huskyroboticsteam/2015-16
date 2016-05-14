__author__ = 'Trevor'

from VisualObjects import *
from MapTile import *
import colors

class DisplayScreen:
    def __init__(self, pygame, screenWidth, screenHeight):
        self.pygame = pygame
        self.screenWidth = screenWidth
        self.screenHeight = screenHeight
        self.fullscreenToggle = False
        self.screen = pygame.display.set_mode((screenWidth,screenHeight))

        # Set the window title/caption TODO: add image
        pygame.display.set_caption("Husky Robotics Hot GUI")

        # Set fps
        self.clock = pygame.time.Clock()
        self.fps = 60

class DisplayBoxes(DisplayScreen):
    def __init__(self, pygame, screenWidth, screenHeight, numberOfJoysticks):
        DisplayScreen.__init__(self, pygame, screenWidth, screenHeight)
        margin = 20
        self.numberOfJoysticks = numberOfJoysticks
        self.rect = [(margin,margin,(self.screenWidth-margin*3)/2,(self.screenHeight-margin*3)/2), ((self.screenWidth-margin*3)/2 + 40,20,(self.screenWidth-margin*3)/2,(self.screenHeight-margin*3)/2), (20,(self.screenHeight-margin*3)/2+40,(self.screenWidth-margin*3)/2,(self.screenHeight-margin*3)/2), ((self.screenWidth-margin*3)/2 + 40,(self.screenHeight-margin*3)/2 + 40,(self.screenWidth-margin*3)/2,(self.screenHeight-margin*3)/2)]

    def display(self, numberOfConnectedJoysticks):
        self.clock.tick(self.fps)
        print numberOfConnectedJoysticks
        for i in range(0,self.numberOfJoysticks):
            if i < numberOfConnectedJoysticks:
                color = (255,0,0)
            else:
                color = (80,0,0)
            self.pygame.draw.rect(self.screen,color,self.rect[i])
        self.pygame.display.flip()

class DisplayInterface(DisplayScreen):
    def __init__(self, pygame, screenWidth, screenHeight):
        DisplayScreen.__init__(self, pygame, screenWidth, screenHeight)

        # Define maps.
        startingCoord = (47.656874,-122.312135) # Point more than 1km away from the URC site: (38.420358,-110.809686)
        ArraySize = [(4,4),(7,7),(11,11),(22,22),(16,21)] # Size of 640x640 tile array for each zoom level separated by commas.
        locationName = "UW"
        self.zoomLevels = [17,18,19,20,21]  # zoom levels to make displayable on map. Google does not go higher than 21 in Utah.
        self.MapArray = []

        self.MainAxis = axis(0,0)
        self.currentMap = 2  # Start at this map zoom level (entry in zoomLevel array: 2 corresponds to 19)

        for i in range(len(self.zoomLevels)):
            mapName = locationName + "Map" + str(self.zoomLevels[i])
            self.MapArray.append(Map(self.pygame, mapName, ArraySize[i], startingCoord, self.zoomLevels[i]))

        self.MapTiles = self.loadImages(self.MapArray[self.currentMap])
        """
        for i in range(len(self.zoomLevels)):
            mapName = locationName + "Map" + str(self.zoomLevels[i])
            self.MapArray.append(Map(self.pygame, mapName, ArraySize, startingCoord, self.zoomLevels[i]))
        """
        # Load other generic images
        self.Ball = pygame.image.load("ballcrosshair.png") # 50x50 pixel image
        self.ScaleIndicatorImage = pygame.image.load("ScaleIndicator320.png")
        self.SidebarRect = Sidebar(self.pygame,self.screenHeight)

        # Create other objects
        self.createButtons()
        self.InputTextbox = Textbox()
        self.markerList = []

        self.EmergencyButtonsEnabled = False

    def loadImages(self, map): # Take a map object and load its images into memory for display
        mapTiles = [[0 for m in xrange(0, self.MapArray[self.currentMap].dimensions[1])] for n in xrange(0, self.MapArray[self.currentMap].dimensions[0])] # tileArraySize[0] is rows, tileArraySize[1] is columns
        numSaves = 0
        mapUpperLeftCorner = (0,0)
        for n in range(0,map.dimensions[0]):
            for m in range(0,map.dimensions[1]):
                currentCoord = (map.startingLocation[0] - n*map.borderDistance[0], map.startingLocation[1] + m*map.borderDistance[1]) # Subtract from latitude to go south/down (northern hemisphere), add to longitude to go east/right
                mapTiles[n][m] = MapTile(map.name,currentCoord, map.zoom, (640,640), (mapUpperLeftCorner[0] + 640*m, mapUpperLeftCorner[1] + 640*n),self.pygame)
                if mapTiles[n][m].saved:
                    numSaves += 1

        if numSaves > 0:
            print str(numSaves) + " image tiles cached from the Internet."

        # Change the scale based on current map
        self.ScaleIndicatorLine = ScaleIndicator(self.pygame, self.MapArray[self.currentMap].borderDistanceKM/2) # ScaleIndicator image is 320 pixels in length, so take the 640-pixel distance and divide it by 2.
        return mapTiles

    def createButtons(self): # Button(pygame, Xoffset, Yoffset, alignment, widthRatio, heightRatio, screenWidth, screenHeight, staticText, toggleTextFalse, toggleTextTrue, startingStatus = False):
        Font = self.pygame.font.SysFont('Arial', 90*self.screenWidth/3200) # Define font to send to each button - font size adapts based on screen width
        eStopButton = Button(self.pygame,self.screenWidth*3.0/15,self.screenHeight*1.0/15,'Right-aligned',2.0/15,2.0/12,self.screenWidth,self.screenHeight,"ALL","STOP","STOPPED",Font)
        potStopButton = Button(self.pygame,self.screenWidth*5.2/15,self.screenHeight*1.0/15,'Right-aligned',2.0/15,2.0/12,self.screenWidth,self.screenHeight,"POT","STOP","STOPPED",Font)
        self.buttons = [eStopButton,potStopButton]

    def createMarker(self, coord):
        self.markerList.append(Marker(coord,self.MapArray[self.currentMap]))

    def display(self):
        self.clock.tick(self.fps)
        self.screen.fill(colors.BLACK) # Start with black background

        # Render all of the map tiles
        for n in range(0,self.MapArray[self.currentMap].dimensions[0]):
            for m in range(0,self.MapArray[self.currentMap].dimensions[1]):
                self.displaymap(self.MapTiles[n][m].image,self.MapTiles[n][m].screenlocation)

        for i in range(len(self.markerList)): # Display all markers
            self.markerList[i].display(self.pygame,self.screen,self.Ball,self.MainAxis)

        # Render sidebar
        self.SidebarRect.display(self.pygame,self.screen)

        # Display emergency buttons if enabled
        if self.EmergencyButtonsEnabled:
            for i in range(len(self.buttons)):
                self.buttons[i].display(self.pygame, self.screen) # Display every button

        # Display the input textbox
        self.InputTextbox.display(self.pygame,self.screen,self.pygame.font.Font(None,18))

        # Display the scale indicator
        self.ScaleIndicatorLine.display(self.screen,self.ScaleIndicatorImage)

        # Update display for current frame
        self.pygame.display.flip()

    def clickCheck(self,position):
        if self.EmergencyButtonsEnabled:
            for i in range(len(self.buttons)):
                self.buttons[i].clicked(position)
        #TODO: send in UDP sending class, make it send button signals to rover

    def getEntry(self):
        inputTextboxOut = self.InputTextbox.returnString()
        if inputTextboxOut[1] == 1:
            if inputTextboxOut[0].status == False:
                print "invalid entry"
            else:
                self.createMarker(inputTextboxOut[0])
        elif inputTextboxOut[1] == 2:
            dimensions = inputTextboxOut[0].split(',')
            if float(dimensions[0]) >= 500 and float(dimensions[0]) <= 4000 and float(dimensions[1]) >= 500 and float(dimensions[1]) <= 4000: # Sanity check: make sure window resize won't be anything crazy
                self.resizeDisplay(int(dimensions[0]),int(dimensions[1]))
            else:
                print "invalid entry"

    def displaymap(self,object,location):
        self.screen.blit(object, (self.MainAxis.x + location[0], self.MainAxis.y + location[1]))

    def resizeMap(self,outOrIn = 'In'):
        if outOrIn == 'In':
            possibleZoomLevel = self.currentMap + 1
        elif outOrIn == 'Out':
            possibleZoomLevel = self.currentMap - 1
        else:
            possibleZoomLevel = 20000 # Won't be in self.zoomLevels for sure
        if (possibleZoomLevel + self.zoomLevels[0]) in self.zoomLevels:
            # Find current central coordinate on screen before updating map
            centralX = self.MainAxis.x + self.screenWidth/2
            centralY = self.MainAxis.y + self.screenHeight/2
            mapPoint = MapPixelCoords.pixelToCoord(self,centralX,centralY)
            # Update map
            self.currentMap = possibleZoomLevel
            self.MapTiles = self.loadImages(self.MapArray[self.currentMap])
            # Update marker position
            for i in range(len(self.markerList)):
                self.markerList[i].updateZoom(self.MapArray[self.currentMap])
            # Update screen position
            newPixelCoord = MapPixelCoords.coordToPixel1(mapPoint[0],mapPoint[1],self.MapArray[self.currentMap])
            self.MainAxis.x = newPixelCoord[0] - self.screenWidth/2
            self.MainAxis.y = newPixelCoord[1] - self.screenHeight/2

        else:
            print "You are out of the possible zoom levels."

    def resizeDisplay(self, screenWidth, screenHeight):
        self.screenWidth = screenWidth
        self.screenHeight = screenHeight
        self.SidebarRect = Sidebar(self.pygame,self.screenHeight) # Recreate sidebar with new screenHeight
        self.createButtons() #Recreate all buttons w/ new screenWidth, screenHeight
        if self.fullscreenToggle == False:
            self.setScreenSize()

    def toggleFullscreen(self):
        self.fullscreenToggle = not self.fullscreenToggle
        if self.fullscreenToggle == True:
            self.pygame.display.set_mode((0,0),self.pygame.FULLSCREEN)
        else:
            self.setScreenSize()

    def escapeFullscreen(self):
        if self.fullscreenToggle == True:
            self.fullscreenToggle = False
            self.setScreenSize()

    def setScreenSize(self):
        self.pygame.display.set_mode((self.screenWidth,self.screenHeight))

class axis: #TODO: Move to separate file
    def __init__(self,x,y):
        self.x = x
        self.y = y