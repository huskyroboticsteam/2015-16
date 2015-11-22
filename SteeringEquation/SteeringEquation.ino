#define ANGLE_MAX 100 //The maximum angle in either direction that the rover should pivot (in radians)
#define ANGLE_MIN .01 //The angle at which we approximate it as going straight (in radians)
#define LENGTH_TO_PIVOT 100 //The straight line distance from the pivot point to the point between the wheels (the axle)
#define LENGTH_BETWEEN_WHEELS 100 //The distance between the wheels on the chassis (width of the rover)
#define CALCULATING_FREQ 100 //How many times we run the loop in a second

double angle; //Our desired angle

void setup()
{
  // put your setup code here, to run once:

}

void loop()
{
  // put your main code here, to run repeatedly:

}

/**
 * @return the tangential speed as given by our joystick input
 */
double getYValue()
{
    return y;
}

/**
 * @return the change in our desired pivot angle as given by our joystic input
 */
double getXValue()
{
    return x;
}

/**
 * @return the potentiometer angle value
 */
double getPotValue()
{
    return potValue;
}

/**
 * Sets the angle value
 */
void setAngle()
{
    angle = getPotValue();
    double x = getXValue();

    angle = angle + (x / CALCULATING_FREQ);
}


/**
 * Clips the angle variable to safe values
 */
void clipAngle()
{
    if (angle > ANGLE_MAX) {
        angle = ANGLE_MAX;
    } else if (angle < -ANGLE_MAX) {
        angle = -ANGLE_MAX;
    }

    if ((-ANGLE_MIN < angle) && (angle < ANGLE_MIN)) {
        angle = 0;
    }
}

/**
 * @return the rate in radians per second that the rover will travel around the circle
 */
double getAngularVelocity()
{
    double angVel;
    double y = getYValue();

    angVel = y * (1 - cos(angle));
    angVel = angVel / (LENGTH_TO_PIVOT);
    angVel = angVel / (sin(angle) - angle);

    return angVel;
}

/**
 * @return left motor speed
 */
double getLeftMotorSpeed()
{
    double y = getYValue();
    double angVel = getAngularVelocity();

    double leftVel;

    leftVel = y - angVel * LENGTH_BETWEEN_WHEELS / 2;

    return leftVel;
}

/**
 * @return right motor speed
 */
double getRightMotorSpeed()
{
    double y = getYValue();
    double angVel = getAngularVelocity();

    double rightVel;

    rightVel = y + angVel * LENGTH_BETWEEN_WHEELS / 2;

    return rightVel;
}
