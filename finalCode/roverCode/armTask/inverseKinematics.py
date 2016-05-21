#import pygame

sholderRot = 0
sholder = 0
elbow = 0
elbowRot = 0
wrist = 0
wristRot = 0
wrist = 0
hand = 0
humLen = 20
forarmLen = 15
handLen = 5
simpleMode = False

def toggleSimple():
    global simpleMode
    simpleMode = (not simpleMode)

def up(dx):
    global elbow, wrist
    sholder += dx
    wrist -= dx

def down(dx):
    global elbow, wrist
    sholder -= dx
    wrist += dx

def left(dx):
    global elbowRot
    elbowRot += dx

def right(dx):
    global elbowRot
    elbowRot -= dx

def forward(dx):
    global sholder, elbow
    sholder += dx
    elbow += dx

def back(dx):
    global sholder, elbow
    sholder -= dx
    sholder -= dx

def twistHandC(dx):
    global wristRot
    wristRot += dx

def twistHandCC(dx):
    global wristRot
    wristRot -= dx

def twistForarmC(dx):
    global elbowRot
    elbowRot += dx

def twistForarmCC(dx):
    global elbowRot
    elbowRot -= dx

def openHand(dx):
    global hand
    hand += dx

def closeHand(dx):
    global hand
    hand -= dx

def InverseKin():
    return False

while(1):
    if simpleMode:
        twistForarmC(10)
    else:
        InverseKin()

