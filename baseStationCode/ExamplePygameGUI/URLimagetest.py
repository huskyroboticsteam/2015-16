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

# on a webpage right click on the image you want and use Copy image URL
image_url = "http://maps.googleapis.com/maps/api/staticmap?center=hanksville,+utah&zoom=13&scale=false&size=600x300&maptype=hybrid&format=png&visual_refresh=true&markers=size:mid%7Ccolor:0xff0000%7Clabel:1%7Chanksville,+utah"

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
screen.blit(image, (20, 20))

# nothing gets displayed until one updates the screen
pg.display.flip()

# start event loop and wait until
# the user clicks on the window corner x to exit
while True:
    for event in pg.event.get():
        if event.type == pg.QUIT:
            pg.quit()
            raise SystemExit