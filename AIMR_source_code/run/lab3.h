#ifndef LAB3H
#define LAB3H

Posture compute_distance_to_goal(float xt, float yt);
void GoTo_DaC(float xt, float yt);
void GoTo_MEMO();
void lab3();
Speed calculateVelocity(float Kp, float E);
float calculate_Eth(Posture distance_to_goal_xy, float Epos);
float calculate_Eth_old(Posture distance_to_goal_xy, float Epos, float xt, float yt);
float law_of_cos(float a, float b, float c);

#endif
