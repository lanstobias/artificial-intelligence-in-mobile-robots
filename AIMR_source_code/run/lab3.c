#include "interface.h"
#include "lab1.h"
#include "lab2.h"
#include "lab3.h"

float Kp_pos, Kp_th, delta_pos, delta_th;

//Motivate in report why we use posture and not position (float vs double)

Posture compute_distance_to_goal(float xt, float yt)
{
	Posture distance_to_goal_xy;
	Posture robot_temp_posture = GetPosture();

	distance_to_goal_xy.x = fabsf(xt - robot_temp_posture.x);
	distance_to_goal_xy.y = fabsf(yt - robot_temp_posture.y);

	return distance_to_goal_xy;
}

float convert_distance_to_mm(Posture distance_to_goal_xy)
{
	return sqrtf((powf(distance_to_goal_xy.x, 2) + (powf(distance_to_goal_xy.y, 2))));
}

float calculate_Eth(Posture distance_to_goal_xy)
{
    float angle = (normalizeAngle((atan2f(distance_to_goal_xy.y, distance_to_goal_xy.x) - GetPosture().th)));

    return angle;
}
 

Velocity calculateVelocity_angle(float Kp_th, float Eth)
{
	Velocity turn;

    turn.r = -(Kp_th * Eth);
	turn.l = (turn.r);

	return turn;
}

Velocity calculateVelocity_distance(float Kp_pos, float Epos)
{
	Velocity move;

	move.r = (int)((Kp_pos * Epos));

	if (move.r > 1000)
	{
		move.r = 1000;
	}

	move.l = move.r;

	return move;
}

void GoTo_DaC(float xt, float yt)
{
	Velocity velocity;
	Posture distance_to_goal_xy;
	float Eth, Epos;

	do{
		update_position();
		
		distance_to_goal_xy = compute_distance_to_goal(xt, yt);

		Epos = convert_distance_to_mm(distance_to_goal_xy);

		Eth = calculate_Eth(distance_to_goal_xy);
		

	} while(fabsf(Epos) > delta_pos);

	printf("Goal has been reached.\n");
	Stop();
}

void GoTo_MEMO()
{

}

void lab3()
{
    ClearSteps();
	printf("Test lab3\n\n");

	Kp_pos = 5;
	Kp_th = 0.5;
	delta_pos = 10.0;
	delta_th = (float)(PI / 12);

	float xt = 130;
	float yt = 0;


	GoTo_DaC(xt, yt);
}
