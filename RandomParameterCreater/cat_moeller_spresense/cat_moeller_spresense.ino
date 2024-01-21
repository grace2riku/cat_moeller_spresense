#ifndef SUBCORE
#error "Core selection is wrong!!"
#endif

#include <MP.h>

#include "src/ntshell/ntshell.h"
extern "C" {
#include "src/ntshell/ntshell_spresense_arduino.h"
}
#include "src/ntshell/usrcmd_cat_moeller_spresense.h"
#include "RandomParameterCreater.h"

#define PROMPT_STR ">"

#define DELAY_TIME (40)
#define DRIVE_PARAMETER_SEND_INTERVAL 1500000

static ntshell_t ntshell;

struct pid_data {
  float rKp;
  float rKi;
  float rKd;
  float lKp;
  float lKi;
  float lKd;
  float vt;
  float rot;
};

struct torque_data {
  int R_duty;
  int L_duty;
};


static int func_read(char* buf, int cnt, void* extobj) {
  if (Serial.available())
    return Serial.readBytes(buf, cnt);
  else
    return 0;
}

static int func_write(const char* buf, int cnt, void* extobj) {
  return Serial.write((uint8_t*)buf, cnt);
}

static int func_callback(const char* text, void* extobj) {
  return usrcmd_execute(text);
}

unsigned int drive_parameter_send() {
    digitalWrite(LED0, LOW);
    digitalWrite(LED1, HIGH);

    struct pid_data pid;
    pid.rKp = pid.lKp = 120;
    pid.rKi = pid.lKi = 30;
    pid.rKd = pid.lKd = 3;

    struct random_parameter val;
    val = random_parameter_create();
    pid.vt = val.vt;
    pid.rot = val.rot;

    int8_t snd_id = 100;
    MP.Send(snd_id, &pid);    

  return DRIVE_PARAMETER_SEND_INTERVAL;
}

void setup() {
  // put your setup code here, to run once:
  random_parameter_initialize();

  MP.begin();
  MP.RecvTimeout(MP_RECV_POLLING); 
  MP.EnableConsole();
  
  Serial.begin(115200);
  while (!Serial) {
    ;;
  }

  ntshell_init(
    &ntshell,
    func_read,
    func_write,
    func_callback,
    (void *)(&ntshell));

  ntshell_set_prompt(&ntshell, PROMPT_STR);

  Serial.println("Wellcome to Spresense Arduino.\r\n type 'help' for help.");
  Serial.print(PROMPT_STR);
  Serial.flush();

  attachTimerInterrupt(drive_parameter_send, DRIVE_PARAMETER_SEND_INTERVAL);  
}

void loop() {
  // put your main code here, to run repeatedly:
  while(Serial.available())
    ntshell_execute_spresense_arduino(&ntshell);

  int8_t rid;

  struct torque_data td;
  int ret = MP.Recv(&rid, &td);
  if (ret == 101) {
    digitalWrite(LED0, HIGH);
    digitalWrite(LED1, LOW);
  }

  usleep(DELAY_TIME*1000);
}
