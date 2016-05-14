# this program prints if the computer is connected to the internet via wifi or ethernet
# requires pywin32 (web download and install) and wmi (pip install wmi)

import wmi

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
        print 'do WiFi stuff'
    elif na[0].NetConnectionID == "Local Area Connection":
        print 'do Ethernet stuff'
else: # if not connected to internet
    print 'not connected to ethernet or wifi'