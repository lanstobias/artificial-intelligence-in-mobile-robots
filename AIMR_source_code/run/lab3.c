#include "interface.h"
#include "lab2.h"
#include "lab3.h"
float Kp_pos, Kp_th, delta_pos, delta_th;
//Motivate in report why we use posture and not position (float vs double)
Posture compute_distance_to_goal(float xt, float yt)
{
	Posture distance_to_goal_xy;
	Posture robot_temp_posture=GetPosture();
	
	distance_to_goal_xy.x=xt-robot_temp_posture.x;
	distance_to_goal_xy.y=yt-robot_temp_posture.y;
	
	return distance_to_goal_xy;
}

float convert_distance_to_mm(Posture distance_to_goal_xy)
{
	return (float)sqrt((pow(distance_to_goal_xy.x, 2)+(pow(distance_to_goal_xy.y,2))));
}

float law_of_cos(float a, float b, float c)
{
	return acos((pow(a, 2) + pow(b, 2) - pow(c, 2)) / (2*a*b));
}

float calculate_Eth_old(Posture distance_to_goal_xy, float Epos, float xt, float yt)
{
	Posture goal_posture;
	float distance_to_robot, distance_to_goal, angle_C;
	
	goal_posture.x=xt;
	goal_posture.y=yt;
	
	distance_to_robot=convert_distance_to_mm(GetPosture());
	distance_to_goal=convert_distance_to_mm(goal_posture);
	
	angle_C = law_of_cos(distance_to_robot,Epos,distance_to_goal);
	
	return PI-angle_C;	
}

float calculate_Eth(Posture distance_to_goal_xy, float Epos)
 {
	 float alpha, beta;
	 
	 alpha = asin(distance_to_goal_xy.x/Epos);
	 beta= (PI/2) - alpha;
	 
	 return (GetPosture().th + beta);
 }
 
Speed calculateVelocity_angle(float Kp_th, float Eth)
{
	Speed turn;
	
	turn.r = (int)((-(Kp_th) * Eth));
	turn.l = -(turn.r);
	
	return turn; 
}


Speed calculateVelocity_distance(float Kp_pos, float Epos)
{
	Speed move;
	
	move.r = (int)(Kp_pos * Epos);
	move.l = move.r;
	
	return move;
}


void GoTo_DaC(float xt, float yt)
{
	Speed velocity;
	Posture distance_to_goal_xy;
	float Eth, Epos;
	
	do 
	{
		update_position();
		distance_to_goal_xy = compute_distance_to_goal(xt, yt);
		Epos = convert_distance_to_mm(distance_to_goal_xy);
		Eth = calculate_Eth(distance_to_goal_xy, Epos);
		
		if (abs(Eth) > delta_th)
		{
			velocity=calculateVelocity_angle(Kp_th, Eth); 
		}
		else
		{
			velocity=calculateVelocity_distance(Kp_pos, Epos);
		}
		//printf("Velocity.l: %d velocity.r: %d\n", velocity.l, velocity.r);
		SetSpeed(velocity.l, velocity.r);
		Sleep(10);
		//printf("Epos: %f delta_pos: %f\n", Epos, delta_pos);
	}while (abs(Epos) > delta_pos);
	Stop();
}
void GoTo_MEMO()
{
	
}

void lab3()
{
	//ClearSteps();
	printf("Test lab3\n");
	Kp_pos=5;
	Kp_th = 5;
	delta_pos=1; 
	delta_th=0.2;
	float xt, yt;
	
	xt=5;
	yt=1;
	
	GoTo_DaC(xt, yt);
}

