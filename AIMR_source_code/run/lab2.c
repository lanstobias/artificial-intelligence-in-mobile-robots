#include "interface.h"

void update_position()
{
	Steps steps, steps_mm;
	Posture new_posture;
	
	// Read new step motor values
	steps = GetSteps();

	// Convert d_l, d_r to mm
	steps_mm = enc2mm(steps);
	int d_r = steps_mm.r;
	int d_l = steps_mm.l;

	// Compute d and delta
	double d = (d_r + d_l) / 2;
	double delta = ((d_r - d_l) / ROBOT_DIAMETER);

	// Compute d_x, d_y
	double d_x = (d * cos(delta / 2));
	double d_y = (d * sin(delta / 2));
	
	// Update posture
	double x = (d * cos(delta));
	double y = (d * sin(delta));

	printf("x: %lf, y: %lf\n", x, y);
	printf("steps.l: %d, steps.r: %d\n", steps.l, steps.r);
	printf("arcsin: %lf, arccos: %lf, arctan: %lf\n", DEG(asin(delta)), DEG(acos(delta)), DEG(atan(delta)));
	printf("d_r: %d, d_l: %d, d: %lf, delta: %lf, d_x: %lf, d_y: %lf\n", d_r, d_l, d, acos(delta), d_x, d_y);
	printf("\n");
}

void lab2()
{
	ClearSteps();
	printf("Lab 2..\n\n");

	update_position();
	SetTargetSteps(2000, 2020);
	// Sleep(3000);
	update_position();
	// Stop();
}
