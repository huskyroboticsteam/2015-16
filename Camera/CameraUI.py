import sys
import pygtk
import gtk
import gtk.glade
import vlc
import random
pygtk.require("2.0")


class ButtonWrap:

    def __init__(self, id):
        self.button = gtk.Button()
        self.id = id
        self.recording = False


class VLCWidget(gtk.DrawingArea):

    def __init__(self, vlc_i):
        gtk.DrawingArea.__init__(self)

        # Creates the VLC Player Object
        self.player = vlc_i.media_player_new()
        self.id = ""
        self.url = ""

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


class VLCRecorder:

    def __init__(self, lowEnd, highEnd):

        self.instance = vlc.Instance()

        # Creates the VLC Player Object
        self.player = self.instance.media_player_new()
        self.numbers = []
        self.lowEnd = lowEnd
        self.highEnd = highEnd

    def instantiate_media(self, url, filename):

        sout = "#transcode{vcodec=h264,vb=800,width=640,height=480,acodec=mp3,ab=128,channels=2,samplerate=44100}" \
               ":file{mux=mp4,dst=" + self.create_random() + ".mp4}"
        self.instance.vlm_add_broadcast(filename, url, sout, 0, None, True, False)

    def reset_media(self, url, filename):
        self.instance.vlm_del_media(filename)

        sout = "#transcode{vcodec=h264,vb=800,width=640,height=480,acodec=mp3,ab=128,channels=2,samplerate=44100}" \
               ":file{mux=mp4,dst=" + self.create_random() + ".mp4}"
        self.instance.vlm_add_broadcast(filename, url, sout, 0, None, True, False)

    def start_recording(self, filename):
        self.instance.vlm_play_media(filename)

    def stop_recording(self, filename):
        self.instance.vlm_stop_media(filename)

    def create_random(self):
        number = random.randint(self.lowEnd, self.highEnd)
        keepChecking = True

        while(keepChecking):
            keepChecking = False
            for i in range(0, len(self.numbers)):
                if len(self.numbers) == self.highEnd - self.lowEnd:
                    print("Ran out of filespace")
                    quit("You didn't define the filespace properly")
                if self.numbers[i] == number:
                    number = random.randint(self.lowEnd, self.highEnd)
                    keepChecking = True
                    break

        self.numbers.append(number)
        print(number)
        return str(number)


class MainUI:

    def __init__(self, urls, lowEnd, highEnd):

        self.window = gtk.Window()
        self.window.set_title("Dope Cameras")
        self.window.set_icon_from_file("icon.png")
        self.vertBox = gtk.VBox()
        self.topHorBox = gtk.HBox()
        self.bottomHorBox = gtk.HBox()

        self.window.add(self.vertBox)
        self.vertBox.add(self.topHorBox)
        self.vertBox.add(self.bottomHorBox)

        self.urls = urls
        self.buttons = []
        self.recorder = VLCRecorder(lowEnd, highEnd)

        # Initialize the initial stream and pack into window
        self.widgets = create_widgets(self.urls)
        for i in range(0, len(self.urls)):
            self.topHorBox.pack_start(self.widgets[i], expand=True)

        for i in range(0, len(self.urls)):
            self.recorder.instantiate_media(self.urls[i], str(i))

        # Connect all buttons to the callback function
        for i in range(0, len(self.urls)):
            buttonWrap = ButtonWrap(i)
            buttonWrap.button.set_label("Record Feed " + str(i+1))
            buttonWrap.button.connect("clicked", self.button_clicked)
            self.bottomHorBox.add(buttonWrap.button)
            self.buttons.append(buttonWrap)

        # Get the Main Window, and connect the "destroy" event
        if self.window:
            # Shows Window and all children objects
            self.window.show_all()
            # Closes infinite loop on pressing exit button (destroys main window)
            self.window.connect("destroy", gtk.main_quit)

    def button_clicked(self, button):
        for i in range(0, len(self.buttons)):
            if self.buttons[i].button == button:
                record_toggle(self.buttons[i], self.urls, self.recorder)


def create_widgets(urls):
    # Empty list to hold vlc widgets
    widgets = []

    # Check if the instances and url lengths match, if not there is a critical error
    for i in range(0, len(urls)):
        instance = vlc.Instance()
        vlc_widget = VLCWidget(instance)
        vlc_widget.id = i
        vlc_widget.url = urls[i]
        player = vlc_widget.player
        player.set_media(instance.media_new(urls[i], ":network-caching=300"))
        player.play()
        widgets.append(vlc_widget)

    return widgets


def record_toggle(buttonWrap, urls, recorder):

    button = buttonWrap.button
    id = buttonWrap.id
    isRecording = buttonWrap.recording

    if isRecording == False:
        button.set_label("Stop Recording Feed " + str(id+1))
        recorder.start_recording(str(id))
        buttonWrap.recording = True
    elif isRecording:
        button.set_label("Record Feed " + str(id+1))
        buttonWrap.recording = False
        recorder.stop_recording(str(id))
        recorder.reset_media(urls[id], str(id))


# Main Function
def main(urls, lowEnd, highEnd):

    # Pass all URL RTSP parameters and random number generator bounds to the UI for initialization
    MainUI(urls, lowEnd, highEnd)

    # Start the UI loop
    gtk.main()
