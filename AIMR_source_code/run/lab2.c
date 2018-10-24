#include "interface.h"
#include "lab2.h"
#include "lab3.h"

Posture posture_old;

//==========================================================================//
//                    Helper funciton (init postures)                       //
//==========================================================================//
void init_postures() {

	SetPosture(0, 0, 0);
	posture_old.x = 0;
	posture_old.y = 0;
	posture_old.th = 0;

	ClearSteps();
}

//==========================================================================//
//                          Helper funciton (read)                          //
//==========================================================================//
Steps read_displacement_of_wheels()
{
	Steps steps, steps_mm;
	// Read new step motor values
	steps = GetSteps();
	// Clear steps
	ClearSteps();
	// Compute difference dL, dR with previous values (GetSteps())
	// Convert dL, dR to mm
	steps_mm = enc2mm(steps);

	//printf("steps_mm: (l: %d, r: %d), steps: (l: %d, r: %d)\n", steps_mm.l, steps_mm.r, steps.l, steps.r);

  return steps_mm;
}

//==========================================================================//
//                      Helper funciton (calculate)                         //
//==========================================================================//
Posture compute_relative_displacement(Steps steps_mm)
{
    Posture posture_displacement;

	int dR = steps_mm.r;
	int dL = steps_mm.l;

	// Compute d and delta as above
	float d = (float)((dR + dL) / 2);
	float delta = (float)((dR - dL) / 47.0);
	// Compute Cartesian  displacement dx, dy
	posture_displacement.x = (float)(d * cosf(delta / 2));
	posture_displacement.y = (float)(d * sinf(delta / 2));
	posture_displacement.th = normalizeAngle(delta);

    return posture_displacement;
}

//==========================================================================//
//                 Helper funciton (angle normalization)                    //
//==========================================================================//
float normalizeAngle(float angle)
{
	// Range is from -PI to PI
	if (angle >= PI)
	{
		angle -= (PI * 2);
	}
	else if (angle < (-PI))
	{
		angle += (PI * 2);
	}

	return angle;
}

//==========================================================================//
//                 Helper funciton (conversion + update)                    //
//==========================================================================//
void convert_to_global_values(Posture relative_displacement)
{
	Posture posture_new;

	// Save previous robot position as x0, y0, th0
	float x0 = posture_old.x;
	float y0 = posture_old.y;
	float th0 = posture_old.th;

	// Compute new robot position x, y, th
	float dx = relative_displacement.x;
	float dy = relative_displacement.y;
	float dth = relative_displacement.th;

	posture_new.x = x0 + dx * cosf(th0) - dy * sinf(th0);
	posture_new.y = y0 + dx * sinf(th0) + dy * cosf(th0);
	posture_new.th = normalizeAngle(th0 + dth);

	printf("----- Convert to global values: ----- \n");
	printf("x0: %f, y0: %f, th0: %f\n", x0, y0, th0);
	printf("dx: %f, dy: %f, dth: %f\n", dx, dy, dth);
	float angle_rad = posture_new.th;
    float angle_deg = angle_rad * (180.0 / M_PI);
    printf("Posture_new_x: %.1f, posture_new_y: %.1f, rad: %.3f, deg: %.4f\n", posture_new.x, posture_new.y, angle_rad, angle_deg);

	// Write new posture
	SetPosture(posture_new.x, posture_new.y, posture_new.th);
	// Update old posture
	posture_old = GetPosture();
}

//==========================================================================//
//                             Position Update                              //
//==========================================================================//
void update_position()
{
	/* Read displacement of right and left wheels */
	Steps steps_mm = read_displacement_of_wheels();

	/* Compute relative displacement at origin */
	Posture posture_displacement = compute_relative_displacement(steps_mm);

	/* Convert to global coordinates */
	convert_to_global_values(posture_displacement);
}

//==========================================================================//
//                             Print Position                               //
//==========================================================================//
void print_position()
{
    Posture posture = GetPosture();
	float degree = posture.th * 180/PI;
    printf("Position: x: %f, y: %f, th: %f, degree: %f\n", posture.x, posture.y, posture.th, degree);
}

//==========================================================================//
//                                   Lab 2                                  //
//==========================================================================//
void lab2()
{
	init_postures();

	printf("Lab 2..\n\n");

	print_position();
	printf("\n");

	// Test-Scenario
	for (int i = 0; i < 14; i++)
	{
		if (i<3) {
			printf("Move straight ahead (Speed: 300)\n");
			SetSpeed(300, 300);
		}
		if (i>=3 && i<6) {
			if (i==3) printf("\n\n");
			printf("Move only with right wheel (Speed: 300)\n");
			SetSpeed(0, 300);
		}
		if (i>=6 && i<9) {
			if (i==6) printf("\n\n");
			printf("Move straight ahead (Speed: 300)\n");
			SetSpeed(300, 300);
		}
		if (i>=9) {
			if (i==9) printf("\n\n");
			printf("Turn by Speed(100, -100)\n");
			SetSpeed(100, -100);
		}

		update_position();
		print_position();
		Sleep(100);
	}

	Stop();
}
