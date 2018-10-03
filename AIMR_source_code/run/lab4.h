#ifndef LAB4H
#define LAB4H

void GoToRules(float xt, float yt);
void AvoidRules();
void GoTo_FRB(float xt, float yt);
double ResponseToVelGoto(double response);
double ResponseToRotGoto(double response);
double ResponseToVelAvoid(double response);
double ResponseToRotAvoid(double response);
void FuzzyTrack(float* xarray, float* yarray, int n);
void printSets();

void lab4();

#endif
