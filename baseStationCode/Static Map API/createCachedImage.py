import MercatorProjection 
import urllib
import pygame as pg
import io
import os
from math import ceil
try:
    # Python2
    from urllib2 import urlopen
except ImportError:
    # Python3
    from urllib.request import urlopen


# initialize pygame
pg.init()

"""
# on a webpage right click on the image you want and use Copy image URL
image_url2 = getUrl(corners['S'], corners['E'], 15, 640, 640)
image_url3 = getUrl(corners['S'], corners['W'], 15, 640, 640)
image_url4 = getUrl(corners['N'], corners['E'], 15, 640, 640)
image_url5 = getUrl(corners['N'], corners['W'], 15, 640, 640)

# caches the image in same directory
urllib.urlretrieve(image_url2, "00000002.png")
urllib.urlretrieve(image_url3, "00000003.png")
urllib.urlretrieve(image_url4, "00000004.png")
urllib.urlretrieve(image_url5, "00000005.png")
"""

# create url 
# can change variables to center on certain locations
# size maxes out at 640x640 for free version
# -90 < latitude < 90       -180 < longitude < 180
def getUrl(latitude, longitude, zoom, horizontal, vertical):
    apikey = "AIzaSyA4it7lRdo5H0mhPlRMM4n2CltZpFnnF4s";
    query = "https://maps.googleapis.com/maps/api/staticmap?"
    query += "center=%s,%s&" % (latitude,longitude)
    query += "zoom=%s&" % zoom
    query += "size=%sx%s&" % (horizontal,vertical)
    query += apikey
    return query

# # center is (x,y)
# # scale is how big it is -> 
# # 1 = 1280x1280
# def createCache(longitude, latitude, scale):
#     # horizontal, vertical = (scale+1)*640
#     # screen = whiteScreen(horizontal,vertical)
#     for(i in range(scale)):
#     	for(j in range(4)):
#     image = pg.image.load(str(i + j))

def draw(image_file,x,y,screen):
	# load the image from a file or stream
	image = pg.image.load(image_file)
	# draw image, position the image ulc at x=0, y=0
	screen.blit(image, (x, y))

# create centered fullscreen whitescreen image
def whiteScreen():
    os.environ['SDL_VIDEO_CENTERED'] = '1'
    white = (255, 255, 255)
    screen = pg.display.set_mode()
    screen.fill(white)
    return screen

# finds the coordinates of the edges
# can use these coordinates to find ratio of pixel to change in degrees
def findCorners(latitude, longitude, zoom, horizontal, vertical):
    centerPoint = MercatorProjection.G_LatLng(latitude, longitude)
    corners = MercatorProjection.getCorners(centerPoint, zoom, horizontal, vertical)
    return corners

# get the display information
info = pg.display.Info()
scale = ceil(max(info.current_w, info.current_h) / 640)
print scale
# print "horizontal = %s, vertical = %s" % (info.current_w, info.current_h)

# can use this the corners with varying sizes to find lat and long for 
# various pixel coordinates -> use those coords to query another segment of the map
# from the API.
cornersB = findCorners(40,-80, 15, 1280, 1280)
screen = whiteScreen();
bleh = findCorners(40,-80,15,1920,640)
longitude = bleh['E']
latitude = bleh['N']
print longitude
print latitude
image_url6 = getUrl(latitude, longitude, 15, 640, 640)
urllib.urlretrieve(image_url6, "00000006.png")


draw("00000004.png", 640, 0,screen) # northeast
draw("00000005.png", 0, 0,screen) # northwest
draw("00000003.png", 0, 640,screen) # southwest
draw("00000002.png", 640, 640,screen) # southeast
draw("00000006.png",1280,0,screen)

# saves the combined image
# might not be useful in the end
# pg.image.save(screen, "combined.png")

# nothing gets displayed until one updates the screen
pg.display.flip()


while True:
    for event in pg.event.get():
        if event.type == pg.QUIT:
            pg.quit()
            raise SystemExit
        if event.type == pg.MOUSEBUTTONDOWN:
            pixCoord = pg.mouse.get_pos() # get_pos returns tuple
            print pixCoord[1] * (cornersB['N'] - cornersB['S']) / info.current_h
            longitude = cornersB['W'] + pixCoord[0] * (cornersB['E'] - cornersB['W']) / 1280
            latitude = cornersB['N'] + pixCoord[1] * (cornersB['N'] - cornersB['S']) / 1280
            print "latitude = %s, longitude = %s" % (latitude, longitude)



