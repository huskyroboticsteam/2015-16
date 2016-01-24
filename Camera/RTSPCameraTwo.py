import sys
import vlc
import pygame

#IP Address of camera 2
#Port 554
stream = "rtsp://192.168.1.11:554/user=admin&password=&channel=1&stream=0.sdp"

pygame.init()
screen = pygame.display.set_mode((640,360))

print "Using %s renderer" % pygame.display.get_driver()
print 'Playing: %s' % stream

# Create instane of VLC and create reference to movie.
vlcInstance = vlc.Instance()
media = vlcInstance.media_new(stream, ":network-caching=300")

# Create new instance of vlc player
player = vlcInstance.media_player_new()

# Pass pygame window id to vlc player, so it can render its contents there.
win_id = pygame.display.get_wm_info()['window']
if sys.platform == "linux2": # for Linux using the X Server
    player.set_xwindow(win_id)
elif sys.platform == "win32": # for Windows
    player.set_hwnd(win_id)

# Load movie into vlc player instance
player.set_media(media)
player.video_set_format("RV32", 1280, 720, 0);

# Start movie playback
player.play()

while player.get_state() != vlc.State.Ended:
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            sys.exit(0)


