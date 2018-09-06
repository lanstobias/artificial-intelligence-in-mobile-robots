# Steps to setup the robot and the simulator

------------------------------------------
1-Clone the repository
------------------------------------------

```
git clone https://aak@gitsvn-nt.oru.se/aak/AIMR_source_code.git
```
------------------------------------------
2-Real robot
------------------------------------------

2.1-epuck-computer connection:

- Turn on the robot

- Pairing epuck with computer:
  - Go to: Bluetooth manager -> search -> select epuck -> custom PIN (write the four digit number written on epuck)

- Connecting epuck with computer:
  - Go to Device -> serial service (Note down the number of rfcomm provided at the bottom of the window e.g., Serial port connected to /dev/rfcomm8, here the number is "8") 

- If everything works fine, an orange light will trun on the robot.


2.2-Compile library and run sample program:

- Open a new terminal
- cd into AIMR_source_code/real_robot/src

```
make clean
```

```
make
```

- cd into AIMR_source_code/run
- open main.c
- Assign the value of rfcomm number to ROBOT_NUMDER in Line 3 e.g., ROBOT_NUMDER = 8

```
cd ../../run/
```

```
make clean_real 
```

```
make real 
```

```
sudo ./real
```

- Enter your login password

- If everything works fine, the ring of leds on the epuck will turn on

- For more test examples remove the lines (/* remove this line , remove this line*/) from the main.cpp.

```
make clean_real 
```

```
make real
```

```
sudo ./real
```

------------------------------------------
3-Simulator
------------------------------------------

3.1-Compile the simulator:

- cd into AIMR_source_code/simulator/src/

```
make clean_lin
```

```
make sim_lin
```


- Run the simulator:

```
cd ../bin/
```

```
./sim_lin
```

- Run the simulator by clicking "run" button (simulator should be running while testing your code)

- Compile and run sample program:

- Open a new terminal
- cd into AIMR_source_code/run/

```
make clean_sim 
```

```
make sim
```

```
./sim
```
------------------------------------------
4-Documentation of library related to the course: 
------------------------------------------

- Go to AIMR_source_code/doc/doxygen/html and double click on index.html
- Documentation will appear in your browser (Google Chrome or Firefox).
- Click on "Files"
- The files related to AIMR course are:
  - epuck.hpp
  - fuzzy.h
  - lists.h
  - maps.h

- Go through the documentation and write your own code into "main.c"

------------------------------------------
5-enjoy!
------------------------------------------
