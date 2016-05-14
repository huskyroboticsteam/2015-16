# this program prints if the computer is connected to the internet via wifi or ethernet
# requires pywin32 (web download and install) and wmi (pip install wmi)

import wmi
import os, platform # for ping function
import sys

c = wmi.WMI()
index = -1
for adapter in c.Win32_NetworkAdapterConfiguration():
    #print s
    if type(adapter.IPAddress) is tuple: # only care about the internet network adapter
        index = adapter.InterfaceIndex # save the index of the desired adapter
        #print s.IPAddress
        break
if not index == -1: # if the internet network adapter is active
    na = c.Win32_NetworkAdapter(InterfaceIndex=index)
    #print na[0]
    if na[0].NetConnectionID == "Wireless Network Connection":
        print 'connected to wifi'
    elif na[0].NetConnectionID == "Local Area Connection":
        print 'connected to ethernet'
else: # if not connected to internet
    print 'not connected to ethernet or wifi'

# pings the IP address host given as a string. returns true if a response is received.
def ping(host):
    # Ping parameters as function of OS
    ping_str = "-n 1" if  platform.system().lower()=="windows" else "-c 1"
    return os.system("ping " + ping_str + " " + host + " > NUL") == 0
'''
if not ping('192.168.1.20'):
    print 'base com not connected'
else:
    print 'base com connected'
if not ping('192.168.1.50'):
    print 'rover com not connected'
else:
    print 'rover com connected'
if not ping('192.168.1.51'):
    print 'main board not connected'
else:
    print 'main board connected'
    '''
if not ping('192.168.1.10'):
    print 'camera 1 not connected'
else:
    print 'camera 1 connected'
if not ping('192.168.1.11'):
    print 'camera 2 not connected'
else:
    print 'camera 2 connected'
if not ping('192.168.1.12'):
    print 'camera 3 connected'
else:
    print 'camera 3 connected'
if not ping('192.168.1.13'):
    print 'camera 4 not connected'
else:
    print 'camera 4 connected'
if not ping('192.168.1.14'):
    print 'camera 5 not connected'
else:
    print 'camera 5 connected'
if not ping('192.168.1.15'):
    print 'camera 6 not connected'
else:
    print 'camera 6 connected'
'''

ping('192.168.1.10')
ping('192.168.1.11')
ping('192.168.1.12')
ping('192.168.1.13')
ping('192.168.1.14')
ping('192.168.1.15')
'''