#ifndef LAB3H
#define LAB3H

typedef struct
{
    float r, l;
} Velocity;

Posture compute_distance_to_goal(float xt, float yt);
void GoTo_DaC(float xt, float yt);
void GoTo_MEMO();
void lab3();
Velocity calculateVelocity_angle(float Kp_th, float Eth);
Velocity calculateVelocity_distance(float Kp_pos, float Epos);
float calculate_Eth(Posture distance_to_goal_xy);
float normalizeAngle(float angle);

#endif
