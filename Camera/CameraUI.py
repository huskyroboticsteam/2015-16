import sys
import pygtk
import gtk
import gtk.glade
import vlc
pygtk.require("2.0")

camOne = "rtsp://192.168.1.15:554/user=admin&password=&channel=1&stream=0.sdp"
camTwo = "rtsp://192.168.1.20:554/user=admin&password=&channel=1&stream=0.sdp"
camThree = "rtsp://192.168.1.10:554/user=admin&password=&channel=1&stream=0.sdp"
instance = vlc.Instance()


class VLCWidget(gtk.DrawingArea):

    def __init__(self, vlc_i):
        gtk.DrawingArea.__init__(self)

        # Creates the VLC Player Object
        self.player = vlc_i.media_player_new()

        # Give the Windows ID to VLC
        def handle_embed(*args):
            if sys.platform == 'win32':
                self.player.set_hwnd(self.window.handle)
            else:
                self.player.set_xwindow(self.window.xid)
            return True
        # Once widget event map is run it will call the function handle_embed
        self.connect("map", handle_embed)
        # Set initial window size of widget
        self.set_size_request(320, 200)


class MainUI:

    def __init__(self, urls):

        # Set the Glade file
        self.gladefile = "CameraUI.glade"
        self.wTree = gtk.glade.XML(self.gladefile)
        # Get Vertical Box Object (With 3 Slots)
        self.Box = self.wTree.get_widget("Feeds")
        # Set a url reference for later
        self.urls = urls

        # Initialize the initial stream and pack into window
        self.widgets = create_widgets(self.urls, [instance, instance, instance])
        for i in range(0, len(self.urls)):
            self.Box.pack_start(self.widgets[i], expand=True)

        # Connect all buttons to the callback function
        self.recordOne = self.wTree.get_widget("button1")
        self.recordOne.connect("clicked", self.button_clicked)
        self.recordTwo = self.wTree.get_widget("button2")
        self.recordTwo.connect("clicked", self.button_clicked)
        self.recordThree = self.wTree.get_widget("button3")
        self.recordThree.connect("clicked", self.button_clicked)

        # Get the Main Window, and connect the "destroy" event
        self.window = self.wTree.get_widget("MainWindow")
        if self.window:
            # Shows Window
            self.window.show_all()
            # Closes infinite loop on pressing exit button (destroys main window)
            self.window.connect("destroy", gtk.main_quit)

    def button_clicked(self, button):
        # Destroy widgets from the window
        self.widgets = destroy_widgets(self.Box, self.widgets)
        # Toggle button label and create new widgets for packing
        self.widgets = feed_toggle(button, self.widgets)

        # Use URLs for the length here, if we have an extra widget urls.length() + 1
        # Will record but not be packed into the interface
        for i in range(0, len(self.urls)):
            self.Box.pack_start(self.widgets[i], expand=True, fill=True)


def destroy_widgets(box, widgets):
    # Get how many widgets are in the box
    length = len(widgets)

    # Remove all widgets from box and then delete reference to them
    for i in range(0, length):
        box.remove(widgets[i])
        widgets[i] = ""

    return widgets


def create_widgets(urls, vlc_i):
    # Empty list to hold vlc widgets
    widgets = []

    # Check if the instances and url lengths match, if not there is a critical error
    if len(urls) == len(vlc_i):
        for i in range(0, len(urls)):
            vlc_widget = VLCWidget(vlc_i[i])
            player = vlc_widget.player
            player.set_media(vlc_i[i].media_new(urls[i], ":network-caching=300"))
            player.play()
            widgets.append(vlc_widget)
    else:
        print("Number of connections and vlc instances do not match")

    return widgets


def feed_toggle(button, widgets):

    # Use the button label to decide what to do
    name = button.get_label()

    if name == "Record Feed 1":
        button.set_label("Stop Recording Feed 1")
        record_instance = vlc.Instance("--sout=#duplicate{dst=file{dst=cam1.mpg}}")
        widgets = create_widgets([camOne, camTwo, camThree, camOne], [instance, instance, instance, record_instance])

    elif name == "Record Feed 2":
        button.set_label("Stop Recording Feed 2")
        record_instance = vlc.Instance("--sout=#duplicate{dst=file{dst=cam2.mpg}}")
        widgets = create_widgets([camOne, camTwo, camThree, camTwo], [instance, instance, instance, record_instance])

    elif name == "Record Feed 3":
        button.set_label("Stop Recording Feed 3")
        record_instance = vlc.Instance("--sout=#duplicate{dst=file{dst=cam3.mpg}}")
        widgets = create_widgets([camOne, camTwo, camThree, camThree], [instance, instance, instance, record_instance])

    elif name == "Stop Recording Feed 1" or name == "Stop Recording Feed 2" or name == "Stop Recording Feed 3":
        widgets = create_widgets([camOne, camTwo, camThree], [instance, instance, instance])

        if name == "Stop Recording Feed 1":
            button.set_label("Record Feed 1")
        elif name == "Stop Recording Feed 2":
            button.set_label("Record Feed 2")
        elif name == "Stop Recording Feed 3":
            button.set_label("Record Feed 3")

    return widgets


# Main Function
if __name__ == "__main__":

    # Pass all URL RTSP parameters to the UI for initialization
    hwg = MainUI([camOne, camTwo, camThree])

    # Start the UI loop
    gtk.main()
