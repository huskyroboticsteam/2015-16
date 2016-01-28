import sys
import pygtk
pygtk.require("2.0")
import gtk
import gtk.glade
import vlc

instance = vlc.Instance()

class VLCWidget(gtk.DrawingArea):

    def __init__(self):
        gtk.DrawingArea.__init__(self)

        #Creates the VLC Player Object
        self.player = instance.media_player_new()

        #Give the Windows ID to VLC
        def handle_embed(*args):
            if sys.platform == 'win32':
                self.player.set_hwnd(self.window.handle)
            else:
                self.player.set_xwindow(self.window.xid)
            return True
        #Fancy Mumbo Jumbo
        self.connect("map", handle_embed)
        #Set initial size taken by Widget
        self.set_size_request(320, 200)

class MainUI:

    def __init__(self, urls):

        #Set the Glade file
        self.gladefile = "CameraUI.glade"
        self.wTree = gtk.glade.XML(self.gladefile)

        #Get Vertical Box Object (With 3 Slots)
        self.Box = self.wTree.get_widget("Feeds")

        #Doesn't add any reference to the Widget so bad code but is for example purposes
        #Creates 3 widgets for 3 different feeds
        for i in range(0, 3):
            self._vlc_widget = VLCWidget()
            self.player = self._vlc_widget.player
            self.player.set_media(instance.media_new(urls[i], ":network-caching=300"))
            self.player.play()
            self.Box.pack_start(self._vlc_widget, expand=True)

        #Get the Main Window, and connect the "destroy" event
        self.window = self.wTree.get_widget("MainWindow")
        if self.window:
            #Shows Window
            self.window.show_all()
            #Closes infinite loop on pressing exit button
            self.window.connect("destroy", gtk.main_quit)

#Main Function
if __name__ == "__main__":

    #Pass all URL RTSP parameters to the UI
    hwg = MainUI(["rtsp://192.168.1.10:554/user=admin&password=&channel=1&stream=0.sdp","rtsp://192.168.1.11:554/user=admin&password=&channel=1&stream=0.sdp", "rtsp://192.168.1.12:554/user=admin&password=&channel=1&stream=0.sdp"])

    #Start the UI
    gtk.main()