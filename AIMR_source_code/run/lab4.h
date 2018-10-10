#ifndef LAB4H
#define LAB4H

/** @brief
 * 
 */
void GoToRules(float xt, float yt);

/** @brief
 */
void AvoidRules();

/** @brief
 * 
 */
void GoTo_FRB(float xt, float yt);

/** @brief
 * 
 */
double ResponseToVelGoto(double response);

/** @brief
 * 
 */
double ResponseToRotGoto(double response);

/** @brief
 * 
 */
double ResponseToVelAvoid(double response);

/** @brief
 * 
 */
double ResponseToRotAvoid(double response);

/** @brief
 * 
 */
void FuzzyTrack(float* xarray, float* yarray, int n);

/** @brief
 * 
 */
void printSets();

void lab4();

#endif
