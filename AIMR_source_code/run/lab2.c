#include "interface.h"
#include "lab2.h"
#include "lab3.h"

//==========================================================================//
//                          Helper funciton (read)                          //
//==========================================================================//
Steps read_displacement_of_wheels()
{
	Steps steps, steps_mm;

	// Read new step motor values
	steps = GetSteps();

    ClearSteps();

	// Convert d_l, d_r to mm
	steps_mm = enc2mm(steps);


    return steps_mm;
}

//==========================================================================//
//                      Helper funciton (calculate)                         //
//==========================================================================//
Posture compute_relative_displacement(Steps steps_mm)
{
    Posture posture_displacement;

	int d_r = steps_mm.r;
	int d_l = steps_mm.l;

	// Compute d and delta)
	float d = (float)((d_r + d_l) / 2);
	posture_displacement.th = ( (float)(d_r - d_l) / ROBOT_DIAMETER );

	// Compute d_x, d_y
	posture_displacement.x = (d * cos(posture_displacement.th / 2));
	posture_displacement.y = (d * sin(posture_displacement.th / 2));

    // DEBUG -->
    printf("\ncompute_relative_displacement():\n");
    printf("d: %f, pd.th: %f, pd.x %f, pd.y %f:\n",
           d, posture_displacement.th, posture_displacement.x, posture_displacement.y);
    // <--

    return posture_displacement;
}

//==========================================================================//
//                 Helper funciton (conversion + update)                    //
//==========================================================================//
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

    posture_new.th = normalizeAngle(posture_new.th);

	// write new postures
    SetPosture(posture_new.x, posture_new.y, posture_new.th);
}

//==========================================================================//
//                             Position Update                              //
//==========================================================================//
void update_position()
{
    Posture posture_old = GetPosture();

    Steps steps_mm = read_displacement_of_wheels();
    Posture posture_displacement = compute_relative_displacement(steps_mm);
    convert_to_global_values(posture_old, posture_displacement);

}

//==========================================================================//
//                             Print Position                               //
//==========================================================================//
void print_position()
{
    Posture posture = GetPosture();
    printf("Position: x: %f, y: %f, th: %f\n", posture.x, posture.y, posture.th);
}

//==========================================================================//
//                                   Lab 2                                  //
//==========================================================================//
void lab2()
{
    SetPosture(0, 0, 0);

	ClearSteps();
	printf("Lab 2..\n\n");
	print_position();

	update_position();
  	print_position();

    /*
	for (int i = 0; i < 10; i++)
  	{
    	SetTargetSteps(100, 140);
    	update_position();
    	print_position();
	}
    */

	SetSpeed(250, 500);

	for (int i = 0; i < 1000; i++)
	{
		update_position();
		print_position();
		Sleep(100);
	}
}
