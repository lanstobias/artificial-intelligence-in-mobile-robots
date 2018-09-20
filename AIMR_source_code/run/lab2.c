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
	float delta = (float)((dR - dL) / ROBOT_DIAMETER);
	// Compute Cartesian  displacement dx, dy
	posture_displacement.x = (float)(d * cos(delta / 2));
	posture_displacement.y = (float)(d * sin(delta / 2));
	posture_displacement.th = delta;

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

	posture_new.x = x0 + dx * cos(th0) - dy * sin(th0);
	posture_new.y = y0 + dx * sin(th0) + dy * cos(th0);
	posture_new.th = normalizeAngle(th0 + dth);

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
