#include "interface.h"
#include "lab2.h"

Steps read_displacement_of_wheels()
{
	Steps steps, steps_mm;
	
	// Read new step motor values
	steps = GetSteps();

	// Convert d_l, d_r to mm
	steps_mm = enc2mm(steps);

    return steps_mm;
}

Posture compute_relative_displacement(Steps steps_mm)
{
    Posture posture_displacement;

	int d_r = steps_mm.r;
	int d_l = steps_mm.l;

	// Compute d and delta
	float d = (d_r + d_l) / 2;
	posture_displacement.th = ((d_r - d_l) / ROBOT_DIAMETER);

	// Compute d_x, d_y
	posture_displacement.x = (d * cos(posture_displacement.th / 2));
	posture_displacement.y = (d * sin(posture_displacement.th / 2));

    return posture_displacement;
}

void convert_to_global_values(Posture posture_old, Posture relative_displacement)
{
    Posture posture_new;

    float d_x = relative_displacement.x;
    float d_y = relative_displacement.y;
    float delta = relative_displacement.th;

    float x_0 = posture_old.x;
    float y_0 = posture_old.y;
    float th_0 = posture_old.th;

    posture_new.x = x_0 + d_x * cos(th_0) - d_y * sin(th_0);
    posture_new.y = y_0 + d_x * sin(th_0) + d_y * cos(th_0);
    posture_new.th = th_0 + delta;

    SetPosture(posture_new.x, posture_new.y, posture_new.th);
}

void update_position()
{
    Posture posture_old = GetPosture();

    Steps steps_mm = read_displacement_of_wheels();
    Posture posture_displacement = compute_relative_displacement(steps_mm);
    convert_to_global_values(posture_old, posture_displacement);

    ClearSteps();
}

void print_position()
{
    Posture posture = GetPosture();

    printf("Position: x: %f, y: %f, th: %f\n", posture.x, posture.y, posture.th);
}

void lab2()
{
	ClearSteps();
	printf("Lab 2..\n\n");

    print_position();
    for (int i = 0; i < 10; i++)
    {
        SetTargetSteps(200, 220);
        update_position();
        print_position();
    }
}
