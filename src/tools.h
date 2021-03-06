#ifndef TOOLS_H
#define TOOLS_H

#include <allegro.h>
#include <alpng.h>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <math.h>

using namespace std;

extern bool keyboard_keypressed();
extern bool joy_buttonpressed();

//Collision
extern bool collisionAny(int xMin1, int xMax1, int xMin2, int xMax2, int yMin1, int yMax1, int yMin2, int yMax2);
extern bool collisionBottom(int yMin1, int yMax1, int yMin2, int yMax2);
extern bool collisionTop(int yMin1, int yMax1, int yMin2, int yMax2);
extern bool collisionRight(int xMin1, int xMax1, int xMin2, int xMax2);
extern bool collisionLeft(int xMin1, int xMax1, int xMin2, int xMax2);

//Finds angle of point 2 relative to point 1
extern float find_angle( float x_1, float y_1, float x_2, float y_2);
extern float find_distance( float x_1, float y_1, float x_2, float y_2);

// Mouse position including resolution difference
extern int mouseX();
extern int mouseY();

//Checks if file exists
extern bool fexists(const char *filename);

//Random number generator
extern int random(int newLowest, int newHighest);

//Convert int to string
extern string convertIntToString(int number);

//Convert double to string
extern string convertDoubleToString(double number);

//Convert bool to string
extern string convertBoolToString(bool boolean);

// Convert string to bool
extern int convertStringToBool(string newString);

// Convert radians to allegro units
extern float convertRadiansToAllegro( float newRadians);

// Convert allegro to radians units
extern float convertAllegroToRaidans( float newAllegro);

// Fade in and out
extern void highcolor_fade_in(BITMAP* bmp_orig, int speed);
extern void highcolor_fade_out(int speed);

// Next state
extern void set_next_state( int newState );

// Error reporting
extern void abort_on_error(const char *message);


#endif // TOOLS_H
