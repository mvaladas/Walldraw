#include "QHStepper.h"
#include <TinyStepper_28BYJ_48.h>    // Stepper motor library. Search for TinyStepper_28BYJ_48 in the library manager

#include <Servo.h>

TinyStepper_28BYJ_48 m1; // (7,8,9,10);  // M1 L stepper motor in1~4 ports correspond to UNO 7 8 9 10
TinyStepper_28BYJ_48 m2; // (2,3,5,6);  // M2 R stepper motor in1~4 ports correspond to UNO 2 3 5 6

Servo pen;

static int ps;



void IK(float x,float y,long &target_steps_m1, long &target_steps_m2) {
  float dy = y - Y_MIN_POS;
  float dx = x - X_MIN_POS;
  target_steps_m1 = round(sqrt(dx*dx+dy*dy) / DEFAULT_XY_MM_PER_STEP);
  dx = x - X_MAX_POS;
  target_steps_m2 = round(sqrt(dx*dx+dy*dy) / DEFAULT_XY_MM_PER_STEP);
}

void stepper_init(){
  long target_steps_m1,target_steps_m2;
  IK(START_X, START_Y, target_steps_m1, target_steps_m2);
  current_steps_M1 = target_steps_m1;
  current_steps_M2 = target_steps_m2;

  m1.connectToPins(6,7,8,9); //M1 L stepper motor ports 1~4 ports correspond to UNO 7 8 9 10
  m2.connectToPins(0,1,2,3);
  m1.setSpeedInStepsPerSecond(600);
  m1.setAccelerationInStepsPerSecondPerSecond(10000);
  m2.setSpeedInStepsPerSecond(600);
  m2.setAccelerationInStepsPerSecondPerSecond(10000);

  // stuff copied from servo example (yes, I know it's an array of one value)

  //also init servo
  pen.attach(14);
  ps=PEN_UP_ANGLE;
  pen.write(ps);

  //end stuff copied from servo example

}

void pen_up() {
  pen.write(PEN_UP_ANGLE);
}

void pen_down() {
  pen.write(PEN_DOWN_ANGLE);
}

void jog_m1(int steps) {
  m1.moveRelativeInSteps(steps);
}

void jog_m2(int steps) {
  m2.moveRelativeInSteps(steps);
}

// Move directly from the current position to the target position
void moveto(float target_X,float target_Y) {
  long target_steps_m1,target_steps_m2;
  IK(target_X, target_Y, target_steps_m1, target_steps_m2);
  long dif_abs_steps_run_m1 = abs(target_steps_m1 - current_steps_M1);
  long dif_abs_steps_run_m2 = abs(target_steps_m2 - current_steps_M2);
  int dir1 = (target_steps_m1 - current_steps_M1) > 0 ? (-1*INVERT_M1_DIR) : INVERT_M1_DIR;
  int dir2 = (target_steps_m2 - current_steps_M2) > 0 ? (-1*INVERT_M2_DIR) : INVERT_M2_DIR;
  long over=0;
  long i;
  if(dif_abs_steps_run_m1 > dif_abs_steps_run_m2){
    for( i=0; i < dif_abs_steps_run_m1; ++i){
      m1.moveRelativeInSteps(dir1);
      over+=dif_abs_steps_run_m2;
      if(over>=dif_abs_steps_run_m1){
        over-=dif_abs_steps_run_m1;
        m2.moveRelativeInSteps(dir2);
      }
      //delayMicroseconds(LINE_DELAY);
     }
  } 
  else {
    for(i=0;i<dif_abs_steps_run_m2;++i) {
      m2.moveRelativeInSteps(dir2);
      over+=dif_abs_steps_run_m1;
      if(over>=dif_abs_steps_run_m2) {
        over-=dif_abs_steps_run_m2;
        m1.moveRelativeInSteps(dir1);
      }
      //delayMicroseconds(LINE_DELAY);
    }
  }
  current_steps_M1 = target_steps_m1;
  current_steps_M2 = target_steps_m2;
  
  current_position[X_AXIS] = target_X;
  current_position[Y_AXIS] = target_Y;  
}

void buffer_line_to_destination(){
  if(destination[Z_AXIS]==1 ) {
        ps=PEN_DOWN_ANGLE;
        pen.write(ps);
        Serial.println("Pen down");
      } 
  if(destination[Z_AXIS]>1) {
        ps=PEN_UP_ANGLE;
        pen.write(ps);
        Serial.println("Pen up");
      }

  float cartesian_mm=sqrt( (current_position[X_AXIS] - destination[X_AXIS])* (current_position[X_AXIS] - destination[X_AXIS]) \
            + (current_position[Y_AXIS] - destination[Y_AXIS])* (current_position[Y_AXIS] - destination[Y_AXIS]));
  
  if(cartesian_mm<=DEFAULT_XY_MM_PER_STEP) { moveto(destination[X_AXIS],destination[Y_AXIS]); return; }
  
  long  steps=floor(cartesian_mm/DEFAULT_XY_MM_PER_STEP);
  float init_X = current_position[X_AXIS];
  float init_Y = current_position[Y_AXIS];
  float scale;
  for(long s=0;s<=steps;++s) {
    scale=(float)s/(float)steps;
    moveto((destination[X_AXIS]-init_X)*scale + init_X,
         (destination[Y_AXIS]-init_Y)*scale + init_Y);
  }
  moveto(destination[X_AXIS],destination[Y_AXIS]);
  
}

void buffer_arc_to_destination( float (&offset)[2], bool clockwise ){
  float r_P = -offset[0], r_Q = -offset[1];
  byte p_axis = X_AXIS, q_axis = Y_AXIS, l_axis = Z_AXIS;
    const float radius = HYPOT(r_P, r_Q),
                center_P = current_position[p_axis] - r_P,
                center_Q = current_position[q_axis] - r_Q,
                rt_X = destination[p_axis] - center_P,
                rt_Y = destination[q_axis] - center_Q;
  float angular_travel = ATAN2(r_P * rt_Y - r_Q * rt_X, r_P * rt_X + r_Q * rt_Y);
  Serial.println(angular_travel);

    if (angular_travel < 0) angular_travel += RADIANS(360);
    if (clockwise) angular_travel -= RADIANS(360);
  
  Serial.println(angular_travel);
  
  if (angular_travel == 0 && current_position[p_axis] == destination[p_axis] && current_position[q_axis] == destination[q_axis])
      angular_travel = RADIANS(360);

    const float mm_of_travel = HYPOT(angular_travel * radius, 0);  // The first parameter is the arc length calculation
    if (mm_of_travel < 0.001) return;
    Serial.println(mm_of_travel);
  
  uint16_t segments = floor(mm_of_travel / (MM_PER_ARC_SEGMENT));
    if(segments < 1) segments = 1;
  
  float raw[XY];
  const float theta_per_segment = angular_travel / segments,
                sin_T = theta_per_segment,
                cos_T = 1 - 0.5 * sq(theta_per_segment); // Small angle approximation
        
  int8_t arc_recalc_count = N_ARC_CORRECTION;
  
  for (uint16_t i = 1; i < segments; i++) {
    if (--arc_recalc_count) {
          const float r_new_Y = r_P * sin_T + r_Q * cos_T;
          r_P = r_P * cos_T - r_Q * sin_T;
          r_Q = r_new_Y;
        }else{
          arc_recalc_count = N_ARC_CORRECTION;
      const float cos_Ti = cos(i * theta_per_segment), sin_Ti = sin(i * theta_per_segment);
      r_P = -offset[0] * cos_Ti + offset[1] * sin_Ti;
      r_Q = -offset[0] * sin_Ti - offset[1] * cos_Ti;
      }
    
    raw[p_axis] = center_P + r_P;
      raw[q_axis] = center_Q + r_Q;
    
   // moveto(raw[p_axis], raw[q_axis]); // Inverse solution execution function
    
    #if 1
      Serial.print("G0 X");
      Serial.print(raw[p_axis]);
      Serial.print("Y");
      Serial.println(raw[q_axis]);
    #endif
    
  }
  
}
