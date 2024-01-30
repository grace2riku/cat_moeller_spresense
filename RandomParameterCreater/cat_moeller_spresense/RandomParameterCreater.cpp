#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "RandomParameterCreater.h"

void random_parameter_initialize(void) {
  srand(time(NULL));
}

struct random_parameter random_parameter_create(void) {
  struct random_parameter val;

  // Speed(m/s)の乱数生成 -1.0〜1.0
  val.vt = 2.0f * ((float)rand() / (float)RAND_MAX) - 1.0f;

  // Rotの乱数生成 -1, 0, 1
  val.rot = rand() % 3 - 1;

  return val;
}