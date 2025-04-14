/*

Written by Montek Singh
Copyright and all rights reserved by Montek Singh
Last Updated:  Apr 6, 2025

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
	{'.', white, red},
	{'V', black, green},
	{'O', black, yellow},
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
	// puts a standard bell sound regardless of period
	// this is a poor approximation, but at least you will hear a beep

void sound_off();
	// turns sound off

void put_leds(int pattern);
	// put_leds: set the LED lights to a specified pattern
	//   displays on row #31 (below the screen display)

void initialize_IO(char* smem_initfile);


//===============================================================
// This is the code for your demo app!
//===============================================================


int main() {
	initialize_IO("smem.mem");

	int row1 = 15, row2 = 15;
	int col1 = 20, col2 = 30;
	int key1 = 0,  key2 = 0;
	int key1new, key2new;

	for(int i=0;i<512;i++) {
		put_sound(950000 - 1600*i);
		put_leds(i);

		putChar_atXY(1, col1, row1);
		putChar_atXY(2, col2, row2);
		pause_and_getkey_2player(10, &key1new, &key2new);
		if(key1new != 0) key1 = key1new;
		if(key2new != 0) key2 = key2new;

		switch (key1) {
		case 1: col1--; if(col1<0) col1=0; break;
		case 2: col1++; if(col1>39) col1=39; break;
		case 3: row1--; if(row1<0) row1=0; break;
		case 4: row1++; if(row1>29) row1=29; break;
		}
		switch (key2) {
		case 1: col2--; if(col2<0) col2=0; break;
		case 2: col2++; if(col2>39) col2=39; break;
		case 3: row2--; if(row2<0) row2=0; break;
		case 4: row2++; if(row2>29) row2=29; break;
		}
	}
}


// The file below has the implementation of all of the helper functions
#include "procs.c"
