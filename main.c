#include <stdlib.h>
#include "kernel.h"
#include <stdio.h>
#include <string.h>
#include <time.h>
/* Will be generated: */
#include "kernel_id.h"
#include "ecrobot_interface.h"

/* add user headers here: */

int lighttest();
void waiting();
int istokenfound();
void tokenwasfound();
int getcompass();
void driveonknot(float driveway);
int Robot_GetIntersections();
void move_one_step(int direction);
void lightgrenzwert();
void turnaround();
void walkedge();
int selectnextvisit(int x1,int y1);
void simplesearch();
int knotsearch();
void clear_map();
void suchi2(int x1, int y1);

int search (int hex);
void append (int *way, int direction);
int selectnextNode ();
int numbervisited ();
int show_last_element (int way[]);
void delete_last (int *way);
int isempty (int way[]); //Checks whether direction array is empty, a condition of the search being finished
void suchi (int x, int y);
int is_unvisited_empty ();
int modulo(int x, int y);
int reverseDirection (int direction);
void filledge(); //fills in edge information in unvisited nodes.


// functions that check whether a certain direction is available or not.
int isNorth (int x, int y);
int isEast (int x, int y);
int isSouth (int x, int y);
int isWest (int x, int y);


/// DO NOT DELETE THIS METHOD
/// It is called every 1ms and e.g. can be used for implementing a
/// real time counter / clock.
void user_1ms_isr_type2(void) {
}

void ecrobot_device_initialize(void) {
}

void ecrobot_device_terminate(void) {
}

void hello_world() {
        ecrobot_status_monitor("Hello, World!");

}
// EXTERNAL_WAV_DATA(MC_Heart);
int token = 0;
float blackwert = 600;
int pos_x =  -1;
int pos_y = 6;
int init_x = 0; //records initial position
int init_y = 6; //records initial position

int direction_array [50];
int feld [8][14]; //x, y, visited/getIntersection
int globalcompass = 4;
int sessiondirections;
int initialdirection;
int x2, y2, next_node;





/*ROBOLAB SIM */

//Search neighbor nodes of current node and return number of adjacent nodes.
int search(int hex)
{
		if (token == 4) return 0;
        int north, west, south, east;
        north = 0;
        west = 0;
        south = 0;
        east = 0;
        int x = pos_x;
        int y = pos_y; // because y axis delivers mostly negative values


        switch (hex) //NORTH
        {
                        case 16: //NORTH
                        case 48: //NORTH SOUTH
                        case 80: //NORTH WEST
                        case 112: //NORTH SOUTH WEST
                        case 144:
                        case 176:
                        case 208:
                        case 240:
                        north = 1;
        }

        switch (hex)
        {
                        case 128: // East
                        case 144: // North East
                        case 160: // South East
                        case 176: //North South East
                        case 192: //West East
                        case 208: //North West East
                        case 224: //South West East
                        case 240: //North South West East
                        east = 1;
        }

        switch (hex)
        {
                        case 32: //South
                        case 48: //North South
                        case 96: //South West
                        case 112: //North South West
                        case 160: //South East
                        case 176: //North South East
                        case 224: //South West East
                        case 240: //South West East North
                        south = 1;
        }

        switch (hex)
        {
                        case 64: //West
                        case 80: //North West
                        case 96: //South West
                        case 112: //North South West
                        case 192: //West East
                        case 208: //North West East
                        case 224: //South West East
                        case 240: //North West East South
                        west = 1;
        }


        if (y < 13 && north == 1) //If y is not on the top edge of the maze
        {
                if (feld[x][y+1]/1000 != 2) feld[x][y+1] = north*1000;
        }

        if (x < 7 && east == 1) //If x is not on the right edge of the maze
        {
                if (feld[x+1][y]/1000 != 2) feld[x+1][y] = east*1000;
        }

        if (x > 0 && west == 1) //If x is not on the left edge of the maze
        {
                if (feld[x-1][y]/1000 != 2) feld[x-1][y] = west*1000;
        }

        if (y > 0 && south == 1) //If y is not on the bottom edge of the maze
        {
                if (feld[x][y-1]/1000 != 2) feld[x][y-1] = south*1000;
        }

        feld[x][y] = 2*1000;
        feld[x][y] += hex;

        filledge();
        return north + east + south + west;  //Return number of adjacent nodes
}

void filledge() //fills in edge information in unvisited nodes.
{
        int xindex, yindex;
        for (xindex = 0; xindex < 8; xindex++)
        {
                for (yindex = 0; yindex < 14; yindex++)
                {
                                int constraint = feld[xindex][yindex]/1000;
                        if (constraint == 1) //in the case that no value has been inserted so far
                        {
                                feld[xindex][yindex] = 0;
                                if (yindex != 0 && isNorth(xindex, yindex-1) == 1) feld[xindex][yindex] += 32;
                                if (xindex != 0 && isEast(xindex-1, yindex) == 1) feld[xindex][yindex] += 64;
                                if (yindex != 13 && isSouth(xindex, yindex+1) == 1) feld[xindex][yindex] += 16;
                                if (xindex != 7 &&  isWest(xindex+1, yindex) == 1) feld[xindex][yindex] += 128;
                                feld[xindex][yindex] += 1*1000; //reassign the discovered property,
                        }
                }
        }
}


int isNorth (int x, int y)
{
                int constraint = feld[x][y]/1000;
                constraint = feld[x][y] - constraint*1000;
        switch (constraint) //NORTH
        {
                case 16: //NORTH
                case 48: //NORTH SOUTH
                case 80: //NORTH WEST
                case 112: //NORTH SOUTH WEST
                case 144:
                case 176:
                case 208:
                case 240:
                return 1;
        }
        return 0;
}

int isEast (int x, int y)
{
                int constraint = feld[x][y]/1000;
                constraint = feld[x][y] - constraint*1000;
        switch (constraint)
        {
                case 128: // East
                case 144: // North East
                case 160: // South East
                case 176: //North South East
                case 192: //West East
                case 208: //North West East
                case 224: //South West East
                case 240: //North South West East
                return 1;
        }
        return 0;
}

int isSouth (int x, int y)
{
                int constraint = feld[x][y]/1000;
                constraint = feld[x][y] - constraint*1000;
        switch (constraint)
        {
                case 32: //South
                case 48: //North South
                case 96: //South West
                case 112: //North South West
                case 160: //South East
                case 176: //North South East
                case 224: //South West East
                case 240: //South West East North
                return 1;
        }
        return 0;
}

int isWest (int x, int y)
{
                int constraint = feld[x][y]/1000;
                constraint = feld[x][y] - constraint*1000;
        switch (constraint)
        {
                case 64: //West
                case 80: //North West
                case 96: //South West
                case 112: //North South West
                case 192: //West East
                case 208: //North West East
                case 224: //South West East
                case 240: //North West East South
                return 1;
        }
        return 0;
}


int numbervisited (int x, int y)
{
        int visited_counter = 0;

        if (y >= 0 && y < 13 && x >= 0 && x < 7)
        {

                if (y == 13 || isNorth(x, y) == 0  || (isNorth(x, y) == 1 && feld[x][y+1]/1000 >= 2)) visited_counter ++;
                if (x == 7 || isEast(x, y) == 0   || (isEast(x, y) == 1  && feld[x+1][y]/1000 >= 2)) visited_counter ++;
                if (y == 0 || isSouth(x, y) == 0  || (isSouth(x, y) == 1 && feld[x][y-1]/1000 >= 2)) visited_counter ++; // y=0 when at an edge 5 needs to be adjusted
                if (x == 0 || isWest(x, y) == 0   || (isWest(x, y) == 1  && feld[x-1][y]/1000 >= 2)) visited_counter ++;
        }
        return visited_counter;
}

int selectnextNode () //SELECTS NEXT NODE RANDOMLY
{
        int x = pos_x;
        int y = pos_y;
        int loop = 1;
        int random;

        while(loop <2)
        {
                random = rand() % 5 + 3;
                switch(random)
                {
                        case 3:
                                if(y < 13 && isNorth(x,y) == 1 && feld[x][y+1]/1000 != 2) // Random go NORTH
                                {
                                        loop = 2;
                                        return 3;
                                }
                                break;

                        case 4:
                                if (x < 7 && isEast(x,y) == 1 && feld[x+1][y]/1000 != 2) // Random go EAST
                                {
                                        loop = 2;
                                        return 4;
                                }
                                break;

                        case 5:
                                if (y>=1 && isSouth(x,y) == 1 && feld[x][y-1]/1000 != 2) // Random go SOUTH
                                {
                                        loop = 2;
                                        return 5;
                                }
                                break;

                        case 6:
                                if (x >= 1 && isWest(x,y) == 1 && feld[x-1][y]/1000 != 2) // Random go WEST
                                {
                                        loop = 2;
                                        return 6;
                                }
                                break;
                }
        }
}

void append (int *way, int direction) // appends "direction" to way[]
{
        int i = 0;
        while (way[i] != 0) i++;
        way[i] = direction;
}

int show_last_element (int way[]) // shows last element on the array
{
        int i = 0;
        while (way[i] != 0) i++;
        return way[i-1];
}

void delete_last (int *way) //deletes last element from array
{
        int i = 0;
        while (way[i] != 0) i++;
        if (i != 0) way[i-1] = 0;  // Might cause problems, when i = 0;
}

int reverseDirection (int direction) //reverses "direction"
{
        switch (direction)
        {
                case 3 : //NORTH
                return 5;

                case 4 : //EAST
                return 6;

                case 5 : //SOUTH
                return 3;

                case 6 : //WEST
                return 4;
        }
}

int isempty (int way[]) //CHECKS WHETHER THE ARRAY IS EMPTY
{
        int i = 0;
        while (way[i] == 0) i++;
        if (i == 50) return 1; //Depending on the chosen distance array
        else return 0;
}

int modulo(int x, int y) //returns composite integer of two x, y values
{
                int result ;
        if(y<10)  // y is smaller than 10
        {
                        result = 10 * x + y;
        }
        else {
                result = 100 * x +y;
        }
        return result;
}
 void suchi (int x1, int y1)
{
        int i = 0;
        int argument_x = x1;
        int argument_y = y1;
        int hilfi_x = pos_x;
        int hilfi_y = pos_y;

        while (direction_array[i] != 0 && i<50)//clear direction_array
        {
                direction_array[i] = 0;
                i++;
        }

        append(direction_array, modulo(x1, y1));
        int currentposi = direction_array[0];
        while (modulo(hilfi_x, hilfi_y) != currentposi)
        {
                direction_array[0] = 1;
                if(currentposi < 100){
                        x1 = (currentposi - currentposi %10)/10;
                        y1 = currentposi % 10;
                }
                else {
                        x1 = (currentposi - currentposi %100)/100;
                        y1 = currentposi %100;
                }
                int hex;
                if (isNorth(x1, y1) == 1 && y1 != 13 && feld[x1][y1+1]/1000 <= 2) // <=2 in order to prevent the function from overwriting already written directions
                {

                		append(direction_array,modulo(x1, y1+1));
                		hex = feld[x1][y1+1]/1000;
                		hex = feld[x1][y1+1] - hex*1000;
                        feld[x1][y1+1] = 5*1000 + hex;
                }

                if (isEast(x1, y1) == 1 && x1 != 7 && feld[x1+1][y1]/1000 <= 2)
                {
                        append(direction_array, modulo(x1+1, y1));
                        hex = feld[x1+1][y1]/1000;
                        hex = feld[x1+1][y1] - hex*1000;
                        feld[x1+1][y1] = 6*1000 + hex;
                }

                if (isSouth(x1, y1) == 1 && y1 != 0 && feld[x1][y1-1]/1000 <= 2)
                {
                        append(direction_array, modulo(x1, y1-1));
                        hex = feld[x1][y1-1]/1000;
                        hex = feld[x1][y1-1] - hex*1000;
                        feld[x1][y1-1] = 3*1000 + hex;
                }

                if (isWest(x1, y1) == 1 && x1 != 0 && feld[x1-1][y1]/1000 <= 2)
                {
                        append(direction_array, modulo(x1-1, y1));
                        hex = feld[x1-1][y1]/1000;
                        hex = feld[x1-1][y1] - hex*1000;
                        feld[x1-1][y1] = 4*1000 + hex;
                }
                i = 1;
                while (direction_array[i] != 0)
                {
                        direction_array[i-1] = direction_array[i];
                        i++;
                }
                direction_array [i-1] = 0;
                currentposi = direction_array[0];
        }

        feld[argument_x][argument_y] = 2;
}

int is_unvisited_empty () // Checks whether any unvisited nodes remain inside the maze. Returns 1 if no node are unvisited.
{
        int i, j;
        for (i = 0; i <= 7; i++)
        {
                for (j = 0; j <= 13; j++)
                {
                        if (feld[i][j]/1000 == 1) return 0;
                }
        }
        return 1;
}



/////////////////HARDWARE///////////////////
// Get input value of light sensor (value: 0 - 1023)
int lighttest() {
        int lightvalue = ecrobot_get_light_sensor(NXT_PORT_S1);
                        return lightvalue;
}

// Full stop and waiting over the given time (waittime in ms)
void waiting(int waittime) {
        int actualtime = ecrobot_get_systick_ms();
        while(ecrobot_get_systick_ms() <= (actualtime + waittime)) {
                        ecrobot_set_motor_speed(NXT_PORT_A,0);
                        ecrobot_set_motor_speed(NXT_PORT_B,0);
        }
}

// if touch is activated, token is found for == 1
int istokenfound() {
        int touchsign = ecrobot_get_touch_sensor(NXT_PORT_S2);
        touchsign = ecrobot_get_touch_sensor(NXT_PORT_S3);
        return touchsign;
}

// Token +1, short sound, wait 10 seks
void tokenwasfound() {
        token = token + 1;
        ecrobot_set_motor_speed(NXT_PORT_A,1);
        ecrobot_set_motor_speed(NXT_PORT_B,1);
        ecrobot_sound_tone(220,100,100);
        waiting(10000);
        walkedge();
}

// returns direction, relative to starting point direction
int getcompass() {
        return globalcompass;
}


// simple forward drive function, if knotsearch is executed (driveway in cm)
void driveonknot(float driveway) {
        float actualtime = ecrobot_get_systick_ms();
        float drivetime = (driveway / 0.0175); //in cm/ms
        while (ecrobot_get_systick_ms() < (actualtime + drivetime))
                        {
                                        ecrobot_set_motor_speed(NXT_PORT_A,50);
                                        ecrobot_set_motor_speed(NXT_PORT_B,50);
                                        if (lighttest() >=blackwert || istokenfound() == 1) {
                                                break;}
                        };
}

// Calls Knotsearch, returns decimal number from binary sessiondirections (e.g. 1101 -> 144)
int Robot_GetIntersections() {
	int binaryknots;
	binaryknots = knotsearch();
    return binaryknots;
}

// Westen 6, Süden 5, Osten 4, Norden 3

// turn to chosen direction, walk that way (direction: 1 North, 2 East, 3 South, 4 West)


void move_one_step(int direction) {
        if(direction == 3){
                switch(globalcompass){
                        case 3: turnaround(0,1);globalcompass = direction; pos_y++;break;//geradeaus
                        case 4: turnaround(-60,1);turnaround(-30,0);globalcompass = direction; pos_y++; break;//links
                        case 5: turnaround(150,1);turnaround(30,0);globalcompass = direction; pos_y++; break; //umdrehing
                        case 6: turnaround(60,1);turnaround(30,0);globalcompass = direction; pos_y++; break; //rechts
                }
        }
        else if(direction == 4){
                switch(globalcompass){
                        case 3: turnaround(60,1);turnaround(30,0);globalcompass = direction;pos_x++; break;
                        case 4: turnaround(0,1);globalcompass = direction; pos_x++;break;
                        case 5: turnaround(-60,1);turnaround(-30,0);globalcompass = direction; pos_x++; break;
                        case 6: turnaround(150,1);turnaround(30,0);globalcompass = direction; pos_x++; break;
                }
        }
        else if(direction == 5){
                switch(globalcompass){
                        case 3: turnaround(150,1);turnaround(30,0);globalcompass = direction; pos_y--; break;
                        case 4: turnaround(60,1);turnaround(30,0);globalcompass = direction; pos_y--; break;
                        case 5: turnaround(0,1);globalcompass = direction; pos_y--;break;
                        case 6: turnaround(-60,1);turnaround(-30,0);globalcompass = direction; pos_y--; break;
                }
        }
        else if (direction == 6) {
                switch(globalcompass){
                        case 3 : turnaround(-60,1);turnaround(-30,0);globalcompass = direction; pos_x--; break;
                        case 4 : turnaround(150,1);turnaround(30,0);globalcompass = direction; pos_x--; break;
                        case 5 : turnaround(60,1);turnaround(30,0);globalcompass = direction; pos_x--; break;
                        case 6 : turnaround(0,1);globalcompass = direction; pos_x--;break;
                }
        }
        waiting(300);
        walkedge();
}


void lightgrenzwert() {
        int n=1;
        float temp_blackwert = blackwert;
                        if (lighttest() <= (blackwert *1.07) && lighttest() >= (blackwert *0.93)) {
                                temp_blackwert = temp_blackwert + lighttest();
                                n++;
                        }

                        if (lighttest() <= (blackwert *1.07) && lighttest() >= (blackwert *0.93)) {
                                temp_blackwert = temp_blackwert + lighttest();
                                                n++;
                                        }

                        if (lighttest() <= (blackwert *1.07) && lighttest() >= (blackwert *0.93)) {
                                temp_blackwert = temp_blackwert + lighttest();
                                                n++;
                                        }
                        if (temp_blackwert >= 600) {
                                blackwert = ((temp_blackwert / n)*0.975);
                        }else{
                                blackwert = ((temp_blackwert / n)*0.99);
                        }

        }
// HEIMWEG
/*

*/

/*--------------------MOVING--------------------*/

// turn the robot on the given degree value, key for lighttest while turning
// (degrees 1 - 360 / key 1 -> off, 0 -> on
void turnaround(int degrees, int key) {
        float actualtime = ecrobot_get_systick_ms();
        float turntime = (degrees * 9.15);
        if (degrees >= 0) {
                        // to the right: A positive B negative
                        while (ecrobot_get_systick_ms() < (actualtime + turntime))
                                        {
                                                        ecrobot_set_motor_speed(NXT_PORT_A,45);
                                                        ecrobot_set_motor_speed(NXT_PORT_B,-45);
                                                        if (istokenfound() == 1) { tokenwasfound(); }
                                                        if (key == 0) {
                                                                        if (lighttest() >=blackwert || istokenfound() == 1) {break;}
                                                        }
                                        };
        }else{
                        // to the left: A negative B positive
                        if(turntime<=0){
                                        turntime = turntime*(-1);
                        }
                        while (ecrobot_get_systick_ms() < (actualtime + turntime))
                                        {
                                                        ecrobot_set_motor_speed(NXT_PORT_A,-45);
                                                        ecrobot_set_motor_speed(NXT_PORT_B,45);
                                                        if (istokenfound() == 1) {tokenwasfound();}
                                                        if (key == 0) {
                                                                        if (lighttest() >=blackwert) {break;}
                                                        }
                                        };
        }
}

// simple walk over black line, with simple search included on fail
void walkedge() {
        while (lighttest() >= blackwert && istokenfound() == 0) {
                        ecrobot_set_motor_speed(NXT_PORT_A,50);
                        ecrobot_set_motor_speed(NXT_PORT_B,50);
        }
        // searches for black line if lost
        if (lighttest() < blackwert) {
                simplesearch();
        }
        // Function when token is found
        if (istokenfound() == 1) {
                        tokenwasfound();
        }
}

/*--------------------SEARCH FOR SMTH--------------------*/

// if black line is lost, searches with short turns for line, knotsearch() if failed


void simplesearch() {
        int i;
                for (i=0; i<2; i++) {
                        turnaround(35,0);
                        turnaround(-35,0);
                        turnaround(-35,0);
                        turnaround(35,0);
                }
                if (lighttest() >= blackwert) {
                        walkedge();
                }else{
                        // HERE COMES THE SOFTWARE FUNCTION TO LOAD GETINTERSECTIONS
                	search(Robot_GetIntersections());
                }
}

// turns/scans for edges on a knot, returns (1111 for 4 full direction options)


int knotsearch() {

        sessiondirections = 0;
        int wes = 0, nor = 0, eas = 0;
        driveonknot(12);
        waiting(300);

        if (token < 4) {

                // set initial direction from compass for sessiondirections
                switch(getcompass()) {
                        case 3: initialdirection = 32;break;
                        case 4: initialdirection = 64;break;
                        case 5: initialdirection = 16;break;
                        case 6: initialdirection = 128;break;
                }

                sessiondirections = initialdirection;

                turnaround(70,1);
                turnaround(50,0);

                // East
                if (lighttest() >= blackwert) {
                                //sessiondirections += 10;
                                if (initialdirection == 32){
                                        sessiondirections = sessiondirections + 128;
                                }
                                else if(initialdirection == 64){
                                        sessiondirections = sessiondirections + 32;
                                }
                                else if(initialdirection == 16){
                                        sessiondirections = sessiondirections + 64;
                                }
                                else if(initialdirection == 128){
                                        sessiondirections = sessiondirections + 16;
                                }
                                eas = 1;
                        }

                turnaround(-160,1);
                turnaround(-65,0);

                // West
                if (lighttest() >= blackwert) {
                        //sessiondirections += 1000;
                        if (initialdirection == 32){
                                sessiondirections = sessiondirections + 64;
                        }
                        else if(initialdirection == 64){
                                sessiondirections = sessiondirections + 16;
                        }
                        else if(initialdirection == 16){
                                sessiondirections = sessiondirections + 128;
                        }
                        else if(initialdirection == 128){
                                sessiondirections = sessiondirections + 32;
                        }
                        wes = 1;
                }

                turnaround(65,1);
                turnaround(55,0);
                waiting(50);
                // evtl. zurückdrehen

                // North
                if (lighttest() >= blackwert) {
                                //sessiondirections += 1;
                                if (initialdirection == 32){
                                        sessiondirections = sessiondirections + 16;
                                }
                                else if(initialdirection == 64){
                                        sessiondirections = sessiondirections + 128;
                                }
                                else if(initialdirection == 16){
                                        sessiondirections = sessiondirections + 32;
                                }
                                else if(initialdirection == 128){
                                        sessiondirections = sessiondirections + 64;
                                }
                                nor = 1;
                }
        }
        if (eas == 1 || wes == 1 || nor == 1) {
                        display_clear(0);
                        if (eas == 1) {
                                display_goto_xy(7, 1);
                                display_string("RECHTS");
                        }
                        if (wes == 1) {
                                display_goto_xy(0, 1);
                                display_string("LINKS");
                        }
                        if (nor == 1) {
                                display_goto_xy(2, 0);
                                display_string("GERADEAUS");
                        }
                        display_update();
        }
        return sessiondirections;
}
int selectnextvisit (int x1, int y1) //Call with pos_x, pos_y returns next unvisited node
{
        int i = 0;

        while (direction_array[i] != 0 && i<50)//clear direction_array
        {
                direction_array[i] = 0;
                i++;
        }

        append(direction_array, modulo(x1, y1));
        int currentposi = direction_array[0];
        while (numbervisited(x1, y1) == 4)
        {
                        direction_array[0] = 1;

                        if(currentposi < 100){
                                        x1 = (currentposi - currentposi %10)/10;
                                        y1 = currentposi % 10;
                        }
                        else {
                                        x1 = (currentposi - currentposi %100)/100;
                                        y1 = currentposi % 100;
                        }

                        if(numbervisited(x1, y1) != 4) return modulo(x1, y1);

                        int hex;
                         if (isNorth(x1, y1) == 1 && y1 != 13 && feld[x1][y1+1]/1000 == 2) // <=2 in order to prevent the function from overwriting already written directions
                                {
                                                append(direction_array,modulo(x1, y1+1));
                                                hex = feld[x1][y1+1]/1000;
                                                hex = feld[x1][y1+1] - hex*1000;
                                                feld[x1][y1+1] = 5*1000 + hex;
                                }

                                if (isEast(x1, y1) == 1 && x1 != 7 && feld[x1+1][y1]/1000 == 2)
                                {
                                                append(direction_array, modulo(x1+1, y1));
                                                hex = feld[x1+1][y1]/1000;
                                                hex = feld[x1+1][y1] - hex*1000;
                                                feld[x1+1][y1] = 6*1000 + hex;
                                }

                                if (isSouth(x1, y1) == 1 && y1 != 0 && feld[x1][y1-1]/1000 == 2)
                                {
                                                append(direction_array, modulo(x1, y1-1));
                                                hex = feld[x1][y1-1]/1000;
                                                hex = feld[x1][y1-1] - hex*1000;
                                                feld[x1][y1-1] = 3*1000 + hex;
                                }

                                if (isWest(x1, y1) == 1 && x1 != 0 && feld[x1-1][y1]/1000 == 2)
                                {
                                                append(direction_array, modulo(x1-1, y1));
                                                hex = feld[x1-1][y1]/1000;
                                                hex = feld[x1-1][y1] - hex*1000;
                                                feld[x1-1][y1] = 4*1000 + hex;
                                }
                        i = 1;
                        while (direction_array[i] != 0)
                        {
                                        direction_array[i-1] = direction_array[i];
                                        i++;
                        }
                        direction_array [i-1] = 0;
                        currentposi = direction_array[0];
        }
}


void clear_map () // Checks whether any unvisited nodes remain inside the maze. Returns 1 if no node are unvisited.
{
        int i, j;
        for (i = 0; i <= 7; i++)
        {
                for (j = 0; j <= 13; j++)
                {
                        if (feld[i][j]/1000 > 2)
								{
										int hex = feld[i][j]/1000;
										hex = feld[i][j] - hex*1000;
										feld[i][j] = 2*1000 + hex;
								}
                }
        }
}


void suchi2 (int x1, int y1)
{
        int i = 0;
        int argument_x = x1;
        int argument_y = y1;
        int hilfi_x = pos_x;
        int hilfi_y = pos_y;

        while (direction_array[i] != 0 && i<100)//clear direction_array
        {
                direction_array[i] = 0;
                i++;
        }

        append(direction_array, modulo(x1, y1));
        int currentposi = direction_array[0];
        while (modulo(hilfi_x, hilfi_y) != currentposi)
        {
                        direction_array[0] = 1;
                        if (currentposi < 100) {
                                        x1 = (currentposi - currentposi %10)/10;
                                        y1 = currentposi % 10;
                        }
                        else {
                                        x1 = (currentposi - currentposi %100)/100;
                                        y1 = currentposi %100;
                        }
                        int hex;
                        if (isNorth(x1, y1) == 1 && y1 != 13 && feld[x1][y1+1]/1000 == 2) // <=2 in order to prevent the function from overwriting already written directions
                        {
                                        append(direction_array,modulo(x1, y1+1));
                                        hex = feld[x1][y1+1]/1000;
                                        hex = feld[x1][y1+1] - hex*1000;
                                        feld[x1][y1+1] = 5*1000 + hex;
                        }

                        if (isEast(x1, y1) == 1 && x1 != 7 && feld[x1+1][y1]/1000 == 2)
                        {
                                        append(direction_array, modulo(x1+1, y1));
                                        hex = feld[x1+1][y1]/1000;
                                        hex = feld[x1+1][y1] - hex*1000;
                                        feld[x1+1][y1] = 6*1000 + hex;
                        }

                        if (isSouth(x1, y1) == 1 && y1 != 0 && feld[x1][y1-1]/1000 == 2)
                        {
                                        append(direction_array, modulo(x1, y1-1));
                                        hex = feld[x1][y1-1]/1000;
                                        hex = feld[x1][y1-1] - hex*1000;
                                        feld[x1][y1-1] = 3*1000 + hex;
                        }

                        if (isWest(x1, y1) == 1 && x1 != 0 && feld[x1-1][y1]/1000 == 2)
                        {
                                        append(direction_array, modulo(x1-1, y1));
                                        hex = feld[x1-1][y1]/1000;
                                        hex = feld[x1-1][y1] - hex*1000;
                                        feld[x1-1][y1] = 4*1000 + hex;
                        }
                        i = 1;
                        while (direction_array[i] != 0)
                        {
                                        direction_array[i-1] = direction_array[i];
                                        i++;
                        }
                        direction_array [i-1] = 0;
                        currentposi = direction_array[0];
        }

        feld[argument_x][argument_y] = 2;
}

TASK(OSEK_Main_Task) {
        ecrobot_set_light_sensor_active(NXT_PORT_S1);
        lightgrenzwert();
        srand(1000);
        move_one_step(4);
        if (feld[0][6] == 2240) feld[0][6] = feld[0][6] - 64;
        ecrobot_show_int(isWest(pos_x, pos_y));
        systick_wait_ms(3000);
        do {
                        int direction = selectnextNode();

                        append(&direction_array, direction);
                        if (is_unvisited_empty() == 1) return;
                        move_one_step(direction);
                        if (feld[0][6] == 2240) feld[0][6] = feld[0][6] - 64;
                        if (token == 3){
                                token++;
                                break;
                        }

                        display_clear(0);
					   display_goto_xy(0,1);
					   display_int(feld[pos_x][pos_y],5);
					   display_goto_xy(0,2);
					   display_int(pos_x, 5);
					   display_goto_xy(0,3);
					   display_int(pos_y, 5);
					   display_update();
					   systick_wait_ms(2000);

                        if (is_unvisited_empty() == 1) {
                        	ecrobot_sound_tone(220,1000,100);
                        	return;
                        }



            if (numbervisited(pos_x, pos_y) == 4)
			{
            		next_node = selectnextvisit(pos_x, pos_y);
					clear_map();
					if(next_node < 100){
									x2 = (next_node - next_node %10)/10;
									y2 = next_node % 10;

									display_clear(0);
								   display_goto_xy(0,1);
								   display_int(feld[pos_x][pos_y],5);
								   display_goto_xy(0,2);
								   display_int(pos_x, 5);
								   display_goto_xy(0,3);
		 						   display_int(pos_y, 5);
								   display_update();
								   systick_wait_ms(2000);
					}
					else {
									x2 = (next_node - next_node %100)/100;
									y2 = next_node % 100;
					}
					int hilfziel = feld[x2][y2];
					suchi2(x2, y2);
					feld[x2][y2] = hilfziel;
					int hilfstoken = token;
					while(modulo(pos_x,pos_y) != modulo(x2, y2) && numbervisited(pos_x, pos_y) == 4)
					{
							token = 4;
							move_one_step (feld[pos_x][pos_y]/1000);
					        if (feld[0][6] == 2240) feld[0][6] = feld[0][6] - 64;

					}

				    token = hilfstoken;
					clear_map();
			}

        }
        while (token<10);


        suchi (init_x, init_y);



        while( modulo(pos_x,pos_y) != modulo(init_x,init_y))
        {
               move_one_step (feld[pos_x][pos_y]/1000);
               if (feld[0][6] == 2240) feld[0][6] = feld[0][6] - 64;
        }
        ecrobot_sound_tone(220,1000,100);
        systick_wait_ms(2000);
	ecrobot_sound_tone(1975.53,350,55);
	systick_wait_ms(100);
	ecrobot_sound_tone(2637.02,300,70);
	systick_wait_ms(100);
	ecrobot_sound_tone(2637.02,300,50);
	systick_wait_ms(100);
	ecrobot_sound_tone(2637.02,300,30);
	systick_wait_ms(100);
	ecrobot_sound_tone(2637.02,100,10);
        return ;

        while(1);

}