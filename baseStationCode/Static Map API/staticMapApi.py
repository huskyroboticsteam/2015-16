__author__ = 'Trevor'

'''PG_image_url.py
load and display a web image using pygame and io

tested with Python27 and Python34  by  vegaseat  10mar2015
'''

# testing this function
import MercatorProjection 
import time

import urllib
import io
import pygame as pg
try:
    # Python2
    from urllib2 import urlopen
except ImportError:
    # Python3
    from urllib.request import urlopen

# initialize pygame
pg.init()

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


centerPoint = MercatorProjection.G_LatLng(0, 0)
corners = MercatorProjection.getCorners(centerPoint, 10, 640, 640)

# on a webpage right click on the image you want and use Copy image URL
image_url = getUrl(0, 0, 10, 640, 640)

# caches the image in same directory
urllib.urlretrieve(image_url, "00000002.jpg")

# opens image
image_str = urlopen(image_url).read()

# create a file object (stream)
image_file = io.BytesIO(image_str)

# create an x by y white screen and put the image on that screen
def display(x,y):
    # (r, g, b) color tuple, values 0 to 255
    white = (255, 255, 255)
    screen = pg.display.set_mode((x,y),  pg.RESIZABLE )
    screen.fill(white)
    # load the image from a file or stream
    image = pg.image.load(image_file)
    # draw image, position the image ulc at x=0, y=0
    screen.blit(image, (0, 0))
    # nothing gets displayed until one updates the screen
    pg.display.flip()


display(640,640)
# start event loop and wait until
# the user clicks on the window corner x to exit
# on mouse click prints the pixel location of mouse
# currently working on converting pixels to long/lat
while True:
    for event in pg.event.get():
        if event.type == pg.QUIT:
            pg.quit()
            raise SystemExit
        if event.type == pg.MOUSEBUTTONDOWN:
            pixCoord = pg.mouse.get_pos() # get_pos returns tuple
            print pixCoord[0] * (corners['E'] - corners['W']) / 600
            print pixCoord[1] * (corners['N'] - corners['S']) / 600 


