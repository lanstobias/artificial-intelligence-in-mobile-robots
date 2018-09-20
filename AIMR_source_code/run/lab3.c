#include "interface.h"
#include "lab1.h"
#include "lab2.h"
#include "lab3.h"

float Kp_pos, Kp_th, delta_pos, delta_th;

//Motivate in report why we use posture and not position (float vs double)

void compute_difference_to_target_point(float xt, float yt, float* dx, float* dy)
{
	Posture robot_temp_posture = GetPosture();

	*dx = (xt - robot_temp_posture.x);
	*dy = (yt - robot_temp_posture.y);
}

float calculate_epos(float dx, float dy)
{
	float a2, b2, c;
	
	//Pythagoras: c = sqrt( a² + b²)
	a2 = powf(dx, 2);
	b2 = powf(dy, 2);
	c= sqrtf(a2 + b2);
	
	return c;
}

float calculate_Eth(float dx, float dy)
{
    float angle;
    
    angle = atan2f(dy, dx);
    angle -= GetPosture().th;
    angle = normalizeAngle(angle);  
     
    return angle;
}
 

Velocity calculateVelocity_angle(float Kp_th, float Eth)
{
	Velocity turn;

    turn.r = (Kp_th * Eth);
    
    //Limit max and min speeds - positive turn values
    if (turn.r>950)
    turn.r=950;
    if (turn.r < 200)
    turn.r=200;
    
    //Limit max and min speeds - negative turn values
    if (turn.r<0 && turn.r<-950)
    turn.r=-950;
    
    if (turn.r<0 && turn.r > -200)
    turn.r=-200;
    
	turn.l = -(turn.r);

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
	float dx,dy;
	float Eth, Epos;
	do{
		//Compute current robot position (xr, yr)
		update_position();
		print_position();
		
		//Compute distances to goal (dx, dy)
		compute_difference_to_target_point(xt, yt, &dx, &dy);

		//Convert (dx,dy) to errors (Eth, Epos)
		Epos = calculate_epos(dx,dy);
		
		Eth = calculate_Eth(dx,dy);
		
		if (fabsf(Eth) > delta_th)
		{
			velocity=calculateVelocity_angle(Kp_th, Eth);
			SetSpeed(velocity.l, velocity.r);
			//SetPolarSpeed(0,velocity.l);
			// Turn
			
		}
		else if (fabsf(Epos) > delta_pos)
		{
			SetSpeed(500,500);
			
		}

		printf("Velocity.l: %f, velocity.r: %f\n", velocity.l, velocity.r);
		
		
		printf("Eth: %f, Eth_degrees: %f, delta_th: %f, delta_th_degrees: %f\n", Eth, (Eth * 180/PI), delta_th, (delta_th * 180/PI));
		printf("Epos: %f, Delta_pos: %f\n\n", Epos, delta_pos);
	} while(fabsf(Epos) > delta_pos);

	printf("Goal has been reached.\n");
	Stop();
}


void lab3()
{
    ClearSteps();
	printf("Test lab3\n\n");

	Kp_pos = 5;
	Kp_th = 200;
	delta_pos = 10.0;
	delta_th = (float)(PI / 48);

	float xt = 0;
	float yt = 300;
	
	//float dx, dy;
	//compute_difference_to_target_point(xt, yt, &dx, &dy);

	
	/*
	int i=0;
	SetSpeed(500, 1000);
	while(1)
	{
		i +=1;
		update_position();
		if (i%10==0)
		{
			print_position();
		}
		Sleep(5);
	}
	
	*/
	/*
	Eth=calculate_Eth(dx, dy);
	printf("Eth: %f, Eth_degrees: %f\n", Eth, (Eth * 180/PI));
	xt=120;
	yt=120;
	compute_difference_to_target_point(xt, yt, &dx, &dy);
	Eth=calculate_Eth(dx, dy);
	printf("Eth: %f, Eth_degrees: %f\n", Eth, (Eth * 180/PI));
	
	xt=0;
	yt=120;
	compute_difference_to_target_point(xt, yt, &dx, &dy);
	Eth=calculate_Eth(dx, dy);
	printf("Eth: %f, Eth_degrees: %f\n", Eth, (Eth * 180/PI));
	
	xt=-120;
	yt=120;
	compute_difference_to_target_point(xt, yt, &dx, &dy);
	Eth=calculate_Eth(dx, dy);
	printf("Eth: %f, Eth_degrees: %f\n", Eth, (Eth * 180/PI));
	
	xt=-120;
	yt=0;
	compute_difference_to_target_point(xt, yt, &dx, &dy);
	Eth=calculate_Eth(dx, dy);
	printf("Eth: %f, Eth_degrees: %f\n", Eth, (Eth * 180/PI));
	
	xt=-120;
	yt=-120;
	compute_difference_to_target_point(xt, yt, &dx, &dy);
	Eth=calculate_Eth(dx, dy);
	printf("Eth: %f, Eth_degrees: %f\n", Eth, (Eth * 180/PI));
	
	xt=0;
	yt=-120;
	compute_difference_to_target_point(xt, yt, &dx, &dy);
	Eth=calculate_Eth(dx, dy);
	printf("Eth: %f, Eth_degrees: %f\n", Eth, (Eth * 180/PI));
	
	xt=120;
	yt=-120;
	compute_difference_to_target_point(xt, yt, &dx, &dy);
	Eth=calculate_Eth(dx, dy);
	printf("Eth: %f, Eth_degrees: %f\n", Eth, (Eth * 180/PI));
	*/
	
	/*
	Eth=calculate_Eth(dx, dy);
	printf("Eth: %f, Eth_degrees: %f\n", Eth, (Eth * 180/PI));
	Move(120);
	update_position();
	print_position();
	compute_difference_to_target_point(xt, yt, &dx, &dy);
	Eth=calculate_Eth(dx, dy);
	printf("Eth: %f, Eth_degrees: %f\n", Eth, (Eth * 180/PI));
	
	float Epos;
	Eth=calculate_Eth(dx, dy);
	printf("Eth: %f, Eth_degrees: %f\n", Eth, (Eth * 180/PI));
	MoveSetSpeed(120);
	update_position();
	print_position();
	compute_difference_to_target_point(xt, yt, &dx, &dy);
	Eth=calculate_Eth(dx, dy);
	printf("Eth: %f, Eth_degrees: %f\n", Eth, (Eth * 180/PI));
	SetSpeed(-400, 400);
	Sleep(500);
	update_position();
	print_position();
	compute_difference_to_target_point(xt, yt, &dx, &dy);
	Eth=calculate_Eth(dx, dy);
	printf("Eth: %f, Eth_degrees: %f\n", Eth, (Eth * 180/PI));
	MoveSetSpeed(120);
	update_position();
	print_position();
	compute_difference_to_target_point(xt, yt, &dx, &dy);
	Eth=calculate_Eth(dx, dy);
	printf("Eth: %f, Eth_degrees: %f\n", Eth, (Eth * 180/PI));
	*/

	GoTo_DaC(xt, yt);
}
