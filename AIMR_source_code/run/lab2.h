#ifndef LAB2_H
#define LAB2_H

#include "interface.h"

Steps read_displacement_of_wheels();
Posture compute_relative_displacement(Steps steps_mm);
void convert_to_global_values(Posture posture_old, Posture relative_displacement);
void update_position();
void print_position();

void lab2();

#endif
