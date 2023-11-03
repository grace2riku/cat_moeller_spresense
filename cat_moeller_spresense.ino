#include "ntshell.h"
extern "C" {
#include "ntshell_spresense_arduino.h"
}
#include "usrcmd_cat_moeller_spresense.h"

#define PROMPT_STR ">"

static ntshell_t ntshell;


static int func_read(char* buf, int cnt, void* extobj) {
  if (Serial.available())
    return Serial.readBytes(buf, cnt);
  else
    return 0;
}

static int func_write(const char* buf, int cnt, void* extobj) {
  return Serial.write(buf, cnt);
}

static int func_callback(const char* text, void* extobj) {
  return usrcmd_execute(text);
}

void setup() {
  // put your setup code here, to run once:
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
}

void loop() {
  // put your main code here, to run repeatedly:
  while(Serial.available())
    ntshell_execute_spresense_arduino(&ntshell);
}
