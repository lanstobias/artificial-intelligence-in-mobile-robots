void task2()
{
	ClearSteps();
	SetPosition(0, 0);

	SetTargetSteps(300, 600);

	Steps steps, steps_mm;

	steps = GetSteps();
	steps_mm = enc2mm(steps);

	double d_mm = ((steps_mm.r + steps_mm.l) / 2);
	double theta = (steps_mm.r - steps_mm.l) / ROBOT_DIAMETER;

	double d_x = (d_mm * cos(theta / 2));
	double d_y = (d_mm * sin(theta / 2));

	printf("steps_mm.l: %d, steps_mm.r: %d, d_mm: %f, theta: %f, d_x: %f, d_y: %f\n", steps_mm.l, steps_mm.r, d_mm, theta, d_x, d_y);
}

void test()
{
	printf("Test..\n");

	Posture posture;

	SetPosition(0, 0);
	posture = GetPosture();
	printf("posture.x: %f, posture.y: %f, posture.th:%f\n", posture.x, posture.y, posture.th);

	printf("Moving..\n");
	SetTargetSteps(0, 1000);
	Sleep(1000);

	posture = GetPosture();
	printf("posture.x: %f, posture.y: %f, posture.th:%f\n", posture.x, posture.y, posture.th);
}
