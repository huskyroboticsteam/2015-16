import MercatorProjection 
import urllib
import pygame as pg
import io
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

"""
# center is (x,y)
# scale is how big it is -> 
# 1 = 1280x1280
def createCache(longitude, latitude, scale):
	whiteScreen(scale * 640, scale * 640)
	for(i in range(scale)):
		for(j in range(4)):
			image = pg.image.load(str(i + j))
"""

# finds the coordinates of the edges
# can use these coordinates to find ratio of pixel to change in degrees
centerPoint = MercatorProjection.G_LatLng(40, -80)
corners = MercatorProjection.getCorners(centerPoint, 15, 640, 640)
cornersB = MercatorProjection.getCorners(centerPoint, 15, 1280, 1280)
print corners
print cornersB

# create an 1920 by 1080 white screen and put the image on that screen
# (r, g, b) color tuple, values 0 to 255
white = (255, 255, 255)
screen = pg.display.set_mode((1920,1080),  pg.RESIZABLE )
screen.fill(white)

def draw(image_file,x,y):
	# load the image from a file or stream
	image = pg.image.load(image_file)
	# draw image, position the image ulc at x=0, y=0
	screen.blit(image, (x, y))

draw("00000004.png", 640, 0) # northeast
draw("00000005.png", 0, 0) # northwest
draw("00000003.png", 0, 640) # southwest
draw("00000002.png", 640, 640) # southeast

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
            longitude = cornersB['N'] + pixCoord[1] * (cornersB['N'] - cornersB['S']) / 1280
            latitude = cornersB['E'] + pixCoord[0] * (cornersB['E'] - cornersB['W']) / 1280
            print "%s,%s" % (longitude,latitude)



