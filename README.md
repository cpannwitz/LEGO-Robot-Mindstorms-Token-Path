=================== LEGO Robot Mindstorms Projekt ===================
====== 2014 | TU Dresden | Task: Find paths to 3 tokens & the shortest way back ======

Used hardware parts: 2 motor engines, light sensor, 2 touch sensors, 2 tires

main.c file contains:
-------------------------------------------------------------------------------------------------------------------
+ Basic functions: 
	- walkedge(): walk down an edge, secured by simplesearch()
	- turnaround(): turns the robot with 45% speed around its axis
	- driveonknot(): simple forward drive function to walk on a knot
	- simplesearch(): little turnarounds to find an edge while walking, exits on fail to knotsearch()
	- knotsearch(): detecting edges while standing on a knot
	- move_one_step(): turns to given direction, walks to next knot (includes every function above)
+ Algorithm:
	- Finding the best way to the next unvisited knot
	- Finding all tokens (3 in this example)
	- after every knot is visited => full stop + sound
	- after every token is found => return home (shortest path found while exploring

TODO:
-------------------------------------------------------------------------------------------------------------------
+ driveonknot() needs to be seperated in two calls, first without lighttest() - break, second with
=> fixes possible simplesearch()-fails at the end of an edge
+ sound output for every detected edge while knotsearch()
+ listing problems and fixes for future work