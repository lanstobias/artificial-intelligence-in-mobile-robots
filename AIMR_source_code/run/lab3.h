#ifndef LAB3H
#define LAB3H

typedef struct
{
    float r, l;
} Velocity;

void compute_difference_to_target_point(float xt, float yt, float* dx, float* dy);
void GoTo_DaC(float xt, float yt);
float calculate_epos(float dx, float dy);
void lab3();
Velocity calculateVelocity_angle(float Kp_th, float Eth);
Velocity calculateVelocity_distance(float Kp_pos, float Epos);
float calculate_Eth(float dx, float dy);
float normalizeAngle(float angle);
void Track();
void printRobotValues(Velocity velocity, float Epos, float Eth);

#endif
