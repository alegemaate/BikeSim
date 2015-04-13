#include <allegro.h>
#include <alpng.h>
#include "fmod/fmod.h"
#include "fmod/fmod_errors.h"
#include "tools.h"

using namespace std;

// Close button
volatile int close_button_pressed = FALSE;

// FPS System
volatile int ticks = 0;
int updates_per_second = 100;
volatile int game_time = 0;

int fps;
int frames_done;
int old_time;

void ticker(){
    ticks++;
}
END_OF_FUNCTION(ticker)

void game_time_ticker(){
    game_time++;
}
END_OF_FUNCTION(ticker)

void close_button_handler(void){
  close_button_pressed = TRUE;
}
END_OF_FUNCTION(close_button_handler)


// Images
BITMAP* sully_bike[4];
BITMAP* tire;
BITMAP* drawBike;

BITMAP* buffer;
BITMAP* background;

float x;
float y;
float speed;
float tireRotation;
float bikeRotation;

const float rate_deceleration = 0.01;
const float rate_acceleration = 0.01;
const float rate_brake_decleration = 0.05;
const float max_speed = 4;
const float max_steering_amount = 0.6;

// Setup character gen
void setup(){
  // Allegro
  install_sound(DIGI_AUTODETECT,MIDI_AUTODETECT,".");

  // Set max FPS
  updates_per_second = 60;

  // Set screenmode
  if(set_gfx_mode( GFX_AUTODETECT_WINDOWED, 800, 800, 0, 0) !=0){

  }

  // Create random number generator
  srand( time( NULL));

   // Setup for FPS system
  LOCK_VARIABLE( ticks);
  LOCK_FUNCTION( ticker);
  install_int_ex( ticker, BPS_TO_TIMER( updates_per_second));

  LOCK_VARIABLE( game_time);
  LOCK_FUNCTION( game_time_ticker);
  install_int_ex( game_time_ticker, BPS_TO_TIMER(10));

  // Close button
  LOCK_FUNCTION( close_button_handler);
  set_close_button_callback( close_button_handler);


  // Bike images
  sully_bike[0] = load_bitmap( "images/biker/bike_1.png", NULL);
  sully_bike[1] = load_bitmap( "images/biker/bike_2.png", NULL);
  sully_bike[2] = load_bitmap( "images/biker/bike_3.png", NULL);
  sully_bike[3] = load_bitmap( "images/biker/bike_4.png", NULL);

  // Tire image, also front bars
  tire = load_bitmap( "images/biker/bike_tire.png", NULL);

  // Create images not loaded from file
  buffer = create_bitmap( 400, 400);
  background = create_bitmap( 400, 400);
  drawBike = create_bitmap( 9, 18);

  // Set background to white rectangle
  rectfill( background, 0, 0, 400, 400, makecol( 255, 255, 255));

  // initial x and y
  x = 400/2;
  y = 400/2;
}

// Main Loop
void update(){
  // Steer
  // Left
  if( key[KEY_A] || key[KEY_LEFT]){
    // Initial
    if( tireRotation >= 0)
      tireRotation = -0.1;
    // Compounded
    else if( tireRotation > -max_steering_amount)
      tireRotation += tireRotation/10;
  }
  else if( key[KEY_D] || key[KEY_RIGHT]){
    // Initial
    if( tireRotation <= 0)
      tireRotation = 0.1;
    // Compounded
    else if( tireRotation < max_steering_amount)
      tireRotation += tireRotation/10;
  }
  else
    tireRotation = 0;

  // Motion
  if( key[KEY_W] || key[KEY_UP]){
    // Initial
    if( speed == 0)
      speed = 0.2;
    // Compounded
    else{
      if( speed < max_speed)
        speed *= (1 + rate_acceleration);
    }
  }
  // BRAKE!
  else if( key[KEY_S] || key[KEY_DOWN]){
    if( speed > max_speed/2){
      line( background, x + 4, y + 9, x - (sin( tireRotation + bikeRotation) * speed) + 4, y + (cos( tireRotation + bikeRotation) * speed) + 9, makecol(0,0,0));
      line( background, x + 4, y + 9, x - (sin( bikeRotation) * speed) + 4, y + (cos( bikeRotation) * speed) + 9, makecol(0,0,0));
    }
    speed /= (1 + rate_brake_decleration);
  }
  // Decelerate
  else{
    speed /= (1 + rate_deceleration);
  }

  // Melting tyres?
  if( key[KEY_Z]){
    line( background, x + 4, y + 9, x - (sin( tireRotation + bikeRotation) * speed) + 4, y + (cos( tireRotation + bikeRotation) * speed) + 9, makecol(0,0,0));
    line( background, x + 4, y + 9, x - (sin( bikeRotation) * speed) + 4, y + (cos( bikeRotation) * speed) + 9, makecol(0,0,0));
  }

  // Turning bike
  if( speed > 0){
    bikeRotation += tireRotation/(10 * ((max_speed + 1) - speed));
  }

  // Move
  x += sin( bikeRotation) * speed;
  y -= cos( bikeRotation) * speed;

  // Stop
  if( speed <= 0.1)
    speed = 0;

  // Close Game
  if( key[ KEY_ESC])
    close_button_pressed = true;
}

// Draw to screen
void draw(){
  // Lovely white background
  draw_sprite( buffer, background, 0, 0);

  // Vectors (x, y, compound)
  line( buffer, x, y, x, y - (cos( bikeRotation) * speed * 10), makecol(255,255,0));
  line( buffer, x, y, x + (sin( bikeRotation) * speed * 10), y, makecol(255,0,0));
  line( buffer, x, y, x + (sin( bikeRotation) * speed * 10), y - (cos( bikeRotation) * speed * 10), makecol(0,0,255));

  // Clear temp bike image
  rectfill( drawBike, 0, 0, drawBike -> w, drawBike -> h, makecol(255,0,255));

  // Draw tire, and bike to a temp bike then draw to screen
  rotate_sprite( drawBike, tire, 2, 1, itofix( tireRotation * 40.5845104792));
  draw_sprite( drawBike, sully_bike[0], 0, 0);
  rotate_sprite( buffer, drawBike, x, y, itofix( bikeRotation * 40.5845104792));

  // Debug, vector stuff...
  textprintf_ex( buffer, font, 0, 0, makecol(0,0,0), makecol(255,255,255), "R:%f XR:%f YR:%f", bikeRotation, (sin( bikeRotation) * speed * 10), (cos( bikeRotation) * speed * 10));

  // Buffer
  stretch_sprite( screen, buffer, 0, 0, SCREEN_W, SCREEN_H);
}

// Main function of program
int main(){
  //Initializing
  allegro_init();
  alpng_init();
  install_keyboard();
  install_timer();
  install_mouse();

  set_color_depth(32);

  set_window_title("Bike Simulator 2000");

  //Setup game
  setup();

  //Starts Game
  while(!close_button_pressed){
    //Runs FPS system
    while(ticks == 0){
      rest(1);
    }
    while(ticks > 0){
      int old_ticks = ticks;
      //Update always
      update();
      ticks--;
      if(old_ticks <= ticks){
        break;
      }
    }
    if(game_time - old_time >= 10){
      fps = frames_done;
      frames_done = 0;
      old_time = game_time;
    }
    //Update every set amount of frames
    draw();
  }

  allegro_exit();

  return 0;
}
END_OF_MAIN()
