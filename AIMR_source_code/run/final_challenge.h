#ifndef FINAL_CHALLENGE_H
#define FINAL_CHALLENGE_H

void goto_and_avoid_monolithic_rules(float xt, float yt);
void go_to(float xt, float yt);
void run_trajectory(float* xarray, float* yarray, int n);
void run(Cell start_cell, Cell goal_cell, Map_custom map);
void final_challenge();

#endif
