#include "interface.h"
#include "lab2.h"
#include "lab3.h"
float Kp_pos, Kp_th, delta_pos, delta_th;
//Motivate in report why we use posture and not position (float vs double)
Posture compute_distance_to_goal(float xt, float yt)
{
	
}

float convert_distance_to_angle(Posture distance_to_goal_xy)
{
	
}

float convert_distance_to_mm(Posture distance_to_goal_xy)
{
	
}

Speed calculateVelocity(float Kp, float E)
{
	
}


void GoTo_DaC(float xt, float yt)
{
	Speed velocity;
	Posture distance_to_goal_xy;
	float Eth, Epos;
	
	while (abs(Epos) > delta_pos)
	{
		update_position();
		distance_to_goal_xy = compute_distance_to_goal(xt, yt);
		Eth = convert_distance_to_angle(distance_to_goal_xy);
		Epos = convert_distance_to_mm(distance_to_goal_xy);
		
		if (abs(Eth) > delta_th)
		{
			velocity=calculateVelocity(Kp_th, Eth); 
		}
		else
		{
			velocity=calculateVelocity(Kp_pos, Epos);
		}
		setSpeed(velocity.l, velocity.r);
		Sleep(10);
	}
	Stop();
}
void GoTo_MEMO()
{
	
}

void lab3()
{
	
	printf("Test lab3\n");
	Kp_pos=10;
	Kp_th = 10;
	delta_pos=50; 
	delta_th=0.2;
	Goto_DaC();
}

