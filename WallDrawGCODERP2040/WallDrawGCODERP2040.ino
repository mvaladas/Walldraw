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
String gcode_command="";
float destination[XYZ] = {START_X,START_Y,0};
//note -- starting position
float current_position[XYZ] = {START_X,START_Y,0};
long current_steps_M1 = 0, current_steps_M2 = 0;


void setup() {
  Serial.begin(115200);
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

  while(debounce_count < 10) {

    if (digitalRead(start_b) == LOW) {
      debounce_count ++;
    }
    else {
      debounce_count--;
      if (debounce_count < 0) { debounce_count = 0;}
    }
    delay(100);
    if (digitalRead(pen_up_b) == LOW) {
      Serial.println("pen up");
      pen_up();
    }
    if (digitalRead(pen_down_b) == LOW) {
      Serial.println("pen down");
      pen_down();
    }
    if(digitalRead(m1_up_b) == LOW) {
      Serial.println("M1 up");
      jog_m1(jog_size);
    }
    if(digitalRead(m1_down_b) == LOW) {
      Serial.println("M1 down");
      jog_m1(jog_size*-1);
    }
    if(digitalRead(m2_up_b) == LOW) {
      Serial.println("M2 up");
      jog_m2(jog_size);
    }

    if(digitalRead(m2_down_b) == LOW) {
      Serial.println("M2 down");
      jog_m2(jog_size*-1);
    }

  }
  Serial.println("Grbl 1.1h ['$' for help]");
  delay(1200);
  Serial.println("<Idle|MPos:0.000,0.000,0.000|FS:0,0|Ov:100,100,100>");
}

void loop() {
  if( get_command() > 0 ){
    process_parsed_command();
    gcode_command = "";
    Serial.println("ok");
    delay(10);
  } 
}

byte get_command(){
  static int len = 0;
  static char tmpC = 0;
  while(Serial.available() > 0){
    tmpC = Serial.read();
    if(tmpC == 0x3f) continue;
    if(tmpC == 10 || len > 35 ){  len = 0; return 1;}
    gcode_command += tmpC;
    len++;
  }
  return 0;
}
