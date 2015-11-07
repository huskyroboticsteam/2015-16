void initializeWirelessCommunication()
{
    Ethernet.begin(MAC_ADDRESS);
    Udp.begin(UDP_PORT)
}

