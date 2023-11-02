#include "QH_Configuration.h"
#include "gcode_parser.h"
#include "QHStepper.h"

//change this depending on your button connections
int pen_up_b = 16;
int pen_down_b = 17;
int m1_up_b = 20;
int m1_down_b = 21;
int m2_up_b = 18;
int m2_down_b = 19;
int start_b = 13;

int jog_size = 100;



//starting X posn needs to be set in two places beloW AND in the IK call in stepper init.
String gcode_command = "";
float destination[XYZ] = {START_X, START_Y, 0};
//note -- starting position
float current_position[XYZ] = {START_X, START_Y, 0};
long current_steps_M1 = 0, current_steps_M2 = 0;


void setup() {
  Serial.begin(115200);
  Serial.println("1");
  //intit the buttons
  pinMode(pen_up_b, INPUT_PULLUP);
  pinMode(pen_down_b, INPUT_PULLUP);
  pinMode(m1_up_b, INPUT_PULLUP);
  pinMode(m1_down_b, INPUT_PULLUP);
  pinMode(m2_up_b, INPUT_PULLUP);
  pinMode(m2_down_b, INPUT_PULLUP);
  pinMode(start_b, INPUT_PULLUP);

  stepper_init();
  delay(1000);

  int debounce_count = 0;

  boolean start  = false;

  Serial.println("Waiting on start");
  while (!start) {
    gcode_command = "";
    if (get_command_start() > 0)
    {
      if (gcode_command.indexOf("?") > -1)
      {
        start = true;
        gcode_command = "";
        Serial.println("Grbl 1.1h ['$' for help]");
        delay(1000);
        Serial.println("<Idle|MPos:0.000,0.000,0.000|FS:0,0|Ov:100,100,100>");
        return;
      }
      if (gcode_command.equals("penup"))
      {
        Serial.println("pen up");
        pen_up();

      }
      if (gcode_command.equals("pendown"))
      {
        Serial.println("pen down");
        pen_down();
      }
      if (gcode_command.equals("m1up"))
      {
        Serial.println("M1 up");
        jog_m1(jog_size);
      }
      if (gcode_command.equals("m1down"))
      {
        Serial.println("M1 down");
        jog_m1(jog_size * -1);
      }
      if (gcode_command.equals("m2up"))
      {
        Serial.println("M2 up");
        jog_m2(jog_size);
      }
      if (gcode_command.equals("m2down"))
      {
        Serial.println("M2 down");
        jog_m2(jog_size * -1);
      }
    }
  }
}

void loop() {
  if ( get_command() > 0 ) {
    position_status("Run");
    process_parsed_command();
    gcode_command = "";
    position_status("Idle");
    Serial.println("ok");
    delay(10);
  }
  
}

void position_status (String Status)
{
  Serial.print("<");
  Serial.print(Status);
  Serial.print("|MPos:");
  Serial.print(current_position[X_AXIS]);
  Serial.print(",");
  Serial.print(current_position[Y_AXIS]);
  Serial.print(",");
  Serial.print(current_position[Z_AXIS]);
  Serial.println("|FS:0,0|Ov:100,100,100>");
}

byte get_command() {
  static int len = 0;
  static char tmpC = 0;
  while (Serial.available() > 0) {
    tmpC = Serial.read();
    if (tmpC == 0x3f) continue;
    if (tmpC == 10 || len > 35 ) {
      len = 0;
      return 1;
    }
    gcode_command += tmpC;
    len++;
  }
  return 0;
}

byte get_command_start() {
  static int len = 0;
  static char tmpC = 0;
  while (Serial.available() > 0) {
    tmpC = Serial.read();
    if (tmpC == 10 || len > 35 ) {
      len = 0;
      return 1;
    }
    gcode_command += tmpC;
    len++;
  }
  return 0;
}
