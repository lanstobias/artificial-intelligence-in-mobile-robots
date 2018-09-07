#include "interface.h"

void task2()
{
	ClearSteps();
	Steps steps, steps_mm;
	
	MoveSetSpeed(300);

	steps = GetSteps();
	steps_mm = enc2mm(steps);

	int d_r = steps_mm.r;
	int d_l = steps_mm.l;

	double d = (d_r + d_l) / 2;
	double delta = ((d_r - d_l) / ROBOT_DIAMETER);
	double d_x = (d * cos(delta / 2));
	double d_y = (d * sin(delta / 2));

	printf("d_r: %d, d_l: %d, d: %lf, delta: %lf, d_x: %lf, d_y: %lf\n", d_r, d_l, d, delta, d_x, d_y);
}

void lab2()
{
	task2();
}
