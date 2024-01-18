#ifndef RANDOMPARAMETERCREATER_H
#define RANDOMPARAMETERCREATER_H

struct random_parameter {
  float vt;
  int rot;
};

void random_parameter_initialize(void);
struct random_parameter random_parameter_create(void);

#endif