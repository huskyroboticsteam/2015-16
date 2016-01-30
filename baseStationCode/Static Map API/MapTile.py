import os.path
import urllib
import MercatorProjection
import pygame

class MapTile: # Contains information for each tile
    def __init__(self, coordinates, zoom, pixelsize, screenlocation):
        self.coordinates = coordinates # (20,-100)
        self.zoom = zoom # Google Maps zoom level of the tile, higher is closer in
        self.pixelSize = pixelsize # Size of tile in pixels, 640x640 is maximum for static maps API free version
        self.screenlocation = screenlocation
        self.fileIndex = 'cache/' + str(coordinates[0]) + ',' + str(coordinates[1]) + ' ' + str(zoom) + '.png' # Filenames are unique: exact coords, zoom level
        self.saved = False

        if not os.path.exists('cache'):
            os.makedirs('cache')

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
        query += "AIzaSyA4it7lRdo5H0mhPlRMM4n2CltZpFnnF4s" # API key
        urllib.urlretrieve(query, self.fileIndex)

    def findBorderDistance(self): # Used to find distance in degrees of latitude and longitude between edges of current tile
        centerPointObject = MercatorProjection.G_LatLng(self.coordinates[0], self.coordinates[1]) # Gets MercatorProjection object for the centerpoint of this tile, used only on next line
        self.edges = MercatorProjection.getCorners(centerPointObject, self.zoom, self.pixelSize[0], self.pixelSize[1]) # Uses MercatorProjection library to find edge coordinates of tile
        self.longitudinalLength = abs(self.edges['E'] - self.edges['W']) # Determine distance between edges of tile
        self.latitudinalLength = abs(self.edges['N'] - self.edges['S'])
        return (self.latitudinalLength, self.longitudinalLength)