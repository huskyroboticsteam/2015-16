import os.path
import urllib
import MercatorProjection
import pygame
import time

class Map:
    def __init__(self, name, dimensions = False, startingLocation = False, zoom = False, loadMode = True): #TODO: FIX THIS LOGIC
        self.name = name
        # If not all 3 optional args specified, find it on saved file
        if not dimensions or not startingLocation or not zoom or loadMode:
            if os.path.exists("maps/" + self.name + "/config.dat"):
                self.loadConfiguration()
            else:
                print "ERROR: cannot find pre-existing map configuration: provide arguments to generate new map"
        else:
            self.dimensions = dimensions
            self.startingLocation = startingLocation
            self.zoom = zoom
            self.saveConfiguration()
    def saveConfiguration(self):
        self.startingTile = MapTile(self.name,self.startingLocation,self.zoom,(640,640),(0,0))
        self.borderDistance = self.startingTile.findBorderDistance()

        if not os.path.exists("maps/" + self.name):
            os.makedirs("maps/" + self.name)
        configFile = open("maps/" + self.name + "/config.dat",'w')
        configFile.write(str(self.dimensions[0]) + "\n" + str(self.dimensions[1]) + "\n" + str(self.startingLocation[0])
                         + "\n" + str(self.startingLocation[1]) + "\n" + str(self.zoom) + "\n" + str(self.borderDistance[0])
                         + "\n" + str(self.borderDistance[1]) + "\n")
        configFile.close()
    def loadConfiguration(self):
        configFile = open("maps/" + self.name + "/config.dat",'r')
        self.dimensions = [0,0]
        self.dimensions[0] = int(configFile.readline())
        self.dimensions[1] = int(configFile.readline())
        self.startingLocation = [0,0]
        self.startingLocation[0] = float(configFile.readline())
        self.startingLocation[1] = float(configFile.readline())
        self.zoom = int(configFile.readline())
        self.borderDistance = [0,0]
        self.borderDistance[0] = float(configFile.readline())
        self.borderDistance[1] = float(configFile.readline())
        print "yes it exists"
        self.ULcoord = (self.startingLocation[0] + self.borderDistance[0]/2, self.startingLocation[1] - self.borderDistance[1]/2)
        self.URcoord = (self.ULcoord[0], self.ULcoord[1] + self.dimensions[1]*self.borderDistance[1])
        self.LLcoord = (self.ULcoord[0] - self.dimensions[0]*self.borderDistance[0], self.ULcoord[1])
        self.LRcoord = (self.ULcoord[0] - self.dimensions[0]*self.borderDistance[0],self.ULcoord[1] + self.dimensions[1]*self.borderDistance[1])

class MapTile: # Contains information for each tile
    def __init__(self, mapName, coordinates, zoom, pixelsize, screenlocation):
        self.map = mapName
        self.coordinates = coordinates # (20,-100)
        self.zoom = zoom # Google Maps zoom level of the tile, higher is closer in
        self.pixelSize = pixelsize # Size of tile in pixels, 640x640 is maximum for static maps API free version
        self.screenlocation = screenlocation
        self.fileIndex = "maps/" + mapName + "/" + str(coordinates[0]) + ',' + str(coordinates[1]) + ' ' + str(zoom) + '.png' # Filenames are unique: exact coords, zoom level
        self.saved = False

        if not os.path.exists('maps/' + mapName):
            os.makedirs('maps/' + mapName)

        if not os.path.exists(self.fileIndex): # Detect if tile has been rendered/saved in directory before - if not, go get it from API
            self.saveImage()
            self.saved = True

        self.image = pygame.image.load(self.fileIndex)
        self.findBorderDistance() # These are needed on all MapTiles when displaying, so do this on initializing

    def saveImage(self): # Saves image by creating Google Static Maps API URL and retrieving the image
        query = "https://maps.googleapis.com/maps/api/staticmap?"
        query += "center=%s,%s&" % (self.coordinates[0],self.coordinates[1])
        query += "zoom=%s&" % self.zoom
        query += "size=%sx%s&" % self.pixelSize
        query += "maptype=hybrid&"
        query += "AIzaSyD5X-lJtWiflX2wRFb_nQR-TIhwAakIbgA" # API key
        print "yes"
        urllib.urlretrieve(query, self.fileIndex)
        print "Down"
        time.sleep(1.5) # Don't query Google more than the limit when saving a bunch of images

    def findBorderDistance(self): # Used to find distance in degrees of latitude and longitude between edges of current tile
        centerPointObject = MercatorProjection.G_LatLng(self.coordinates[0], self.coordinates[1]) # Gets MercatorProjection object for the centerpoint of this tile, used only on next line
        self.edges = MercatorProjection.getCorners(centerPointObject, self.zoom, self.pixelSize[0], self.pixelSize[1]) # Uses MercatorProjection library to find edge coordinates of tile
        self.longitudinalLength = abs(self.edges['E'] - self.edges['W']) # Determine distance between edges of tile
        self.latitudinalLength = abs(self.edges['N'] - self.edges['S'])
        return (self.latitudinalLength, self.longitudinalLength)