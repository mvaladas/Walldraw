#ifndef QH_CONFIGURATION_H
#define QH_CONFIGURATION_H

/**
What config options should I bring to the front?
start X and Y?
*/
#define START_X 0
#define START_Y 0

#define X_SEPARATION  430
#define X_MAX_POS       ( X_SEPARATION*0.5)   // The maximum value of the x-axis is 0 in the center of the drawing board
#define X_MIN_POS       (-X_SEPARATION*0.5)   // x-axis minimum value
#define PEN_UP_ANGLE    15  // Lift the pen
#define PEN_DOWN_ANGLE  110  // Put pen to paper
#define INVERT_M1_DIR     -1        
#define INVERT_M2_DIR     1
// 1,1
// 1,-1

#define Y_MAX_POS         (-200)   // Maximum value of y-axis at the bottom of the drawing board
#define Y_MIN_POS         (200)

#include <Arduino.h>

#define XY 2
#define XYZ 3
#define X_AXIS 0
#define Y_AXIS 1
#define Z_AXIS 2

#define BAUDRATE            (115200)    // Serial port rate, used for transmitting G code or debugging, optional 9600, 57600, 115200 or other commonly used rates

#define STEPS_PER_TURN  (2048)  // The stepper motor has a step length of 2048 steps and rotates 360 degrees.
#define SPOOL_DIAMETER  (28)    // Bobbin diameter mm
#define SPOOL_CIRC      (SPOOL_DIAMETER * 3.1416)  // Bobbin circumference 35*3.14=109.956
#define DEFAULT_XY_MM_PER_STEP    (SPOOL_CIRC / STEPS_PER_TURN)  // Stepper motor step pitch, minimum resolution distance the wire is pulled at each step 0.053689mm

#define MM_PER_ARC_SEGMENT  1  // Minimum distance per step for arc segmentation
#define N_ARC_CORRECTION   25  // Number of polarization segments between corrections


          // Horizontal distance above the two cords mm


/* Parameters of vertical distance: Positive values are placed below the drawing board. Theoretically, as long as the drawing board is large enough, it can be infinite. Negative value areas are placed above the pen (before power on) */

// The minimum value of the y-axis is the vertical distance from the fixed points of the left and right lines to the pen at the top of the drawing board. Try to measure and place it accurately. Excessive errors will cause distortion.

#define LINE_DELAY      1   // Stepper motor step waiting time (microseconds)

// Rotation direction of the two motors: 1 forward -1 reverse
// Adjust the in and out direction to reverse the image vertically


#define M_PI 3.14159265358979323846
#define RADIANS(d) ((d)*M_PI/180.0)
#define ATAN2(y, x) atan2(y, x)
#define SQRT(x)     sqrt(x)
#define HYPOT2(x,y) (sq(x)+sq(y))
#define HYPOT(x,y)  SQRT(HYPOT2(x,y))

extern String gcode_command;
extern float destination[XYZ];
extern float current_position[XYZ];
extern long current_steps_M1, current_steps_M2; // The total number of steps of the current stepper motor relative to the 0-point position


// The above are the parameters that need to be adjusted =============================================


#define PEN_DOWN 1  // Pen status Writing
#define PEN_UP 0    // Pen status: Pen raised

#endif
