/*

Written by Montek Singh
Copyright and all rights reserved by Montek Singh
Last Updated:  April 4, 2025

Permission granted to use this only for students, teaching/learning assistants
and instructors of the COMP 541 course at UNC Chapel Hill.
For any other use, contact Montek Singh first.

*/


/*

This is a C template for initial development
of your demo app for COMP541 find projects!

You must compile and run this code in an ANSI
compatible terminal.  You can use the terminal app
in the course VM.  For macOS and Linux users, the
standard terminal/shell on your laptop is also ANSI
compatible.

Open a terminal and compile and run the code as follows:

	gcc code.c
	./a.out

*/



/* Specify the keys here that get_key() will look for,
returning 1 if the first key was found, 2, for the second key, etc.,
and returning 0 if none of these keys was found.
In the actual board-level implementation, you will define
scancodes instead of characters, and you can use specify 
key releases as well.
*/
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
int key_array[] = {'a', 'd', 'w', 's'}; 	// define as many as you need

/* Specify the keys here that get_key2() will look for. */

int key_array2[] = {'j', 'l', 'i', 'k'}; 	// define as many as you need


/* Let us define our sprites.  These will be text approximations
of the actual sprites used in the board implementation.
Here, each sprite is specified by:
	{ text character, foreground color, background color }

For example, specifying a sprite as
	{'.', white, red},
means it is drawn as a white dot over a red background.

Specify the number of sprites first (Nchars), and then the
attributes of each sprite.
*/

// type definition for emulating sprites (see below)
typedef struct {
	char char_to_display;
	int fg_color;
	int bg_color;
} sprite_attr;


#define Nchars 4

enum colors {black, red, green, yellow, blue, magenta, cyan, white};

sprite_attr sprite_attributes[Nchars] = {
	{'0', black, white},
	{'|', white, white},
	{' ', black, black},
	{'\\', blue, white}
};


//===============================================================
// Here are the functions available for I/O.  These correspond
// one-to-one to functions available in MIPS assembly in the
// helper files provided.
//
// NOTE:  There is one function specific to the C implementation
// that is not needed in the assembly implementation:
//     void initialize_IO(char* smem_initfile);
//===============================================================

void my_pause(int N);  	// N is hundredths of a second

void putChar_atXY(int charcode, int col, int row);
	// puts a character at screen location (X, Y)

int getChar_atXY(int col, int row);
	// gets the character from screen location (X, Y)

int get_key();
	// if a key has been pressed and it matches one of the
	// characters specified in key_array[], return the
	// index of the key in that array (starting with 1),
	// else return 0 if no valid key was pressed.

int get_key2();
	// similar to get_key(), but looks for key in
	// key_array2[].

int pause_and_getkey(int N);
	// RECOMMENDED!
	// combines pause(N) with get_key() to produce a 
	// *responsive* version of reading keyboard input

void pause_and_getkey_2player(int N, int* key1, int* key2);
	// 2-player version of pause_and_getkey().

int get_accel();
	// returns the accelerometer value:  accelX in bits [31:16], accelY in bits [15:0]
	// to emulate accelerometer, use the four arrow keys

int get_accelX();
	// returns X tilt value (increases back-to-front)

int get_accelY();
	// returns Y tilt value (increases right-to-left)

void put_sound(int period);
	// visually shows approximate sound tone generated
	// you will not hear a sound, but see the tone highlighted on a sound bar

void sound_off();
	// turns sound off

void put_leds(int pattern);
	// put_leds: set the LED lights to a specified pattern
	//   displays on row #31 (below the screen display)

void initialize_IO(char* smem_initfile);


//===============================================================
// This is the code for your demo app!
//===============================================================
bool move_ball(int *ball_pos_x, int *ball_pos_y, int *ball_velo_x, int *ball_velo_y, int *boost, int paddleone[3], int paddletwo[3]){
		putChar_atXY(2, *ball_pos_x, *ball_pos_y);
                if(*ball_pos_x + *ball_velo_x == 39){
			if(!(*ball_pos_y >= paddletwo[0] && *ball_pos_y <= paddletwo[2])){
				return false;}
			*ball_velo_x = -*ball_velo_x;}
		if(*ball_pos_x + *ball_velo_x == 0){
			if(!(*ball_pos_y >= paddleone[0] && *ball_pos_y <= paddleone[2])){
				return false;}
                        *ball_velo_x = -*ball_velo_x;
                }
                if (*ball_pos_y + *ball_velo_y >= 30 || *ball_pos_y + *ball_velo_y <= 0){
                        *ball_velo_y = -*ball_velo_y;
                }
                *ball_pos_x += *ball_velo_x;
                *ball_pos_y += *ball_velo_y;
                putChar_atXY(0, *ball_pos_x, *ball_pos_y);
		my_pause(20/ *boost);
		return true;
}

void mvpad_one(int *prev, int array[3]){
	int raw = get_accel();
	short accelX = (raw >> 16);
	 accelX = ceil(((accelX * 60) / 1024));
	if(*prev!= -1){putChar_atXY(2, 0, *prev);}
	if(*prev!= -1 || *prev-1 > 0){putChar_atXY(2, 0, *prev-1);}
	if(*prev!= -1 || *prev+1 < 29){putChar_atXY(2, 0, *prev+1);}
    	putChar_atXY(1, 0, accelX);
	array[1] = accelX;
	if(accelX>0){putChar_atXY(1, 0, accelX-1);
	array[0] = accelX-1;}
	if(accelX<29){putChar_atXY(1, 0, accelX+1);
	array[2] = accelX+1;}

	*prev = accelX;
}
void mvpad_two(int *prev, int array[3]){
	int raw = get_accel();
	short accelY = (short)(raw & 0xFFFF);
	accelY = ceil(((accelY * 60) / 1024));
	if(*prev != -1){putChar_atXY(2, 39, *prev);}
	if(*prev != -1 || *prev-1 > 0){putChar_atXY(2, 39, *prev-1);}
	if(*prev != -1 || *prev+1 < 29){putChar_atXY(2, 39, *prev+1);}
   	putChar_atXY(1, 39, accelY);
	array[1] = accelY;
	if(accelY > 0){putChar_atXY(1, 39, accelY-1);
	array[0] = accelY-1;}
	if(accelY < 29){putChar_atXY(1, 39, accelY+1);
	array[2] = accelY+1;}
	*prev = accelY;
	
}
int main() {
	initialize_IO("screen.mem");
	 
	int booster = 1;	
	int ball_x = 10;
        int ball_y = 10;
        int velo_x = 1;
	int velo_y = 1;
	bool in_play = true;
	int prev_x = -1;
	int prev_y = -1;
	int paddleone[3] = {-1, -1, -1};
	int paddletwo[3] = {-1, -1, -1};
        while(in_play){
                in_play = move_ball(&ball_x, &ball_y, &velo_x, &velo_y, &booster, paddleone, paddletwo);
 		mvpad_one(&prev_x, paddleone);
 		mvpad_two(&prev_y, paddletwo);		
	
        }
}




// The file below has the implementation of all of the helper functions
#include "procs.c"
