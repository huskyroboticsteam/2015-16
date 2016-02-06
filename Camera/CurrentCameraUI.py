import sys
import pygtk
pygtk.require("2.0")
import gtk
import gtk.glade
import vlc

camOne = "rtsp://192.168.1.15:554/user=admin&password=&channel=1&stream=0.sdp"
camTwo = "rtsp://192.168.1.20:554/user=admin&password=&channel=1&stream=0.sdp"
camThree = "rtsp://192.168.1.10:554/user=admin&password=&channel=1&stream=0.sdp"
instance = vlc.Instance();

class VLCWidget(gtk.DrawingArea):

    def __init__(self, VLCi):
        gtk.DrawingArea.__init__(self)

        #Creates the VLC Player Object
        self.player = VLCi.media_player_new()

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

        self.widgets = create_widgets(urls, [instance, instance, instance]);

        for i in range(0, 3):
            self.Box.pack_start(self.widgets[i], expand=True)

        self.recordOne = self.wTree.get_widget("button1")
        self.recordOne.connect("clicked", self.button_clicked)
        self.recordTwo = self.wTree.get_widget("button2")
        self.recordTwo.connect("clicked", self.button_clicked)
        self.recordThree = self.wTree.get_widget("button3")
        self.recordThree.connect("clicked", self.button_clicked)

        #Get the Main Window, and connect the "destroy" event
        self.window = self.wTree.get_widget("MainWindow")
        if self.window:
            #Shows Window
            self.window.show_all()
            #Closes infinite loop on pressing exit button
            self.window.connect("destroy", gtk.main_quit)

    def button_clicked(self, widget):
        name = widget.get_label()

        if(name == "Record 1"):
            #widget.set_label("Stop Recording 1")

            #Destroy/Deleting the existing widgets doesn't allow new ones to be packed into the window!
            #Need to figure this problem out.
            for i in range(0, 3):
                #self.widgets[i].unparent()
                self.widgets[i].unrealize()
                self.widgets[i] = ""

            #recordInstance = vlc.Instance("--sout=#duplicate{dst=file{dst=cam1.mpg}}")
            instance = vlc.Instance();
            self.widgets = create_widgets([camOne, camTwo, camThree], [instance, instance, instance]);

            for i in range(0, 3):
                self.wTree.get_widget("Feeds").pack_start(self.widgets[i], expand=True)

            print(1)
        elif(name == "Record 2"):
            #widget.set_label("Stop Recording 2")
            print(2)
        elif(name == "Record 3"):
            #widget.set_label("Stop Recording 3")
            print(3)

def create_widgets(urls, VLCi):
    widgets = ["", "", ""]

    for i in range (0, 3):
        vlc_widget = VLCWidget(VLCi[i])
        player = vlc_widget.player
        player.set_media(VLCi[i].media_new(urls[i], ":network-caching=300"))
        player.play()
        vlc_widget.realize()
        widgets[i] = vlc_widget

    return widgets

#Main Function
if __name__ == "__main__":

    #Pass all URL RTSP parameters to the UI
    hwg = MainUI([camOne, camTwo, camThree])

    #Start the UI
    gtk.main()