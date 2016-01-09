__author__ = 'Trevor'

'''PG_image_url.py
load and display a web image using pygame and io

tested with Python27 and Python34  by  vegaseat  10mar2015
'''

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
apikey = "AIzaSyA4it7lRdo5H0mhPlRMM4n2CltZpFnnF4s";
def getUrl(latitude, longitude, zoom, horizontal, vertical, key):
	query = "https://maps.googleapis.com/maps/api/staticmap?"
	query += "center=%s,%s&" % (latitude,longitude)
	query += "zoom=%s&" % zoom
	query += "size=%sx%s&" % (horizontal,vertical)
	query += key
	return query


# on a webpage right click on the image you want and use Copy image URL
image_url = getUrl(20, 100, 10, 600, 400, apikey)

image_str = urlopen(image_url).read()
# create a file object (stream)
image_file = io.BytesIO(image_str)

# (r, g, b) color tuple, values 0 to 255
white = (255, 255, 255)

# create a 600x400 white screen
screen = pg.display.set_mode((600,400),  pg.RESIZABLE )
screen.fill(white)

# load the image from a file or stream
image = pg.image.load(image_file)

# draw image, position the image ulc at x=20, y=20
screen.blit(image, (0, 0))

# nothing gets displayed until one updates the screen
pg.display.flip()

# start event loop and wait until
# the user clicks on the window corner x to exit
while True:
    for event in pg.event.get():
        if event.type == pg.QUIT:
            pg.quit()
            raise SystemExit