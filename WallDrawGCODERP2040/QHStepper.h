
#ifndef QH_STEPPER_H
#define QH_STEPPER_H

#include "QH_Configuration.h"

void stepper_init();
void buffer_line_to_destination();
void buffer_arc_to_destination( float (&offset)[2], bool clockwise );
void pen_up();
void pen_down();
void jog_m1(int steps);
void jog_m2(int steps);

#endif
