
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
simpleMode = True

def toggleSimple():
    global simpleMode
    simpleMode = (not simpleMode)

def up_down(dx):
    global sholder, wrist
    sholder += dx
    wrist -= dx


def left_right(dx):
    global sholderRot
    sholderRot += dx

def forward_back(dx):
    global sholder, elbow
    sholder += dx
    elbow += dx

def twistHand(dx):
    global wristRot
    wristRot += dx

def twistForarm(dx):
    global elbowRot
    elbowRot += dx

def hand_open_close(dx):
    global hand
    hand += dx


def InverseKin():
    return False

def ZeroPosition():
    global sholderRot, sholder, elbow, elbowRot, wrist, wristRot, hand
    sholderRot = 0
    sholder = 0
    elbow = 0
    elbowRot = 0
    wrist = 0
    wristRot = 0
    wrist = 0
    hand = 0

# takes in joystick imput values and coverts to motor vals
# returns a 7-length array of vals
def getArmVals(arm_forward_back, arm_left_right, arm_up_down, wrist_twist, claw_open_close):
    if simpleMode:
        forward_back(arm_forward_back)
        up_down(arm_up_down)
        left_right(arm_left_right)
        twistHand(wrist_twist)
        hand_open_close(claw_open_close)
    else:
        InverseKin()
    return [sholderRot, sholder, elbow, elbowRot, wrist, wristRot, hand]

