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
void GoTo_FRB(float xt, float yt, bool avoid);

/** @brief
 * 
 */
double ResponseToVel(double response);

/** @brief
 * 
 */
double ResponseToRot(double response);

/** @brief
 * 
 */
void FuzzyTrack(float* xarray, float* yarray, int n, bool avoid);

/** @brief
 * 
 */
void printSets();

void lab4();

#endif
