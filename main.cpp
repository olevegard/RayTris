#include <SDL/SDL.h>
#include <SDL/SDL_main.h>
#include <iostream>
#include "Vector2d.h"

char* doTracing();

vector2d square_tl( 40, 40 );
vector2d square_br( 60, 60);


int main( int argc, char* args[] ) { 

	//The images 
	SDL_Surface* screen = NULL;
	SDL_Surface* traced  = NULL;

	//Start SDL 
	SDL_Init( SDL_INIT_EVERYTHING ); 

	//Set up screen 
	screen = SDL_SetVideoMode( 640, 480, 32, SDL_SWSURFACE );

	char* pixels = doTracing();
	// Create square surfeace
	traced = SDL_CreateRGBSurfaceFrom( pixels, 640, 480, 32, 640 * 4, 0, 0, 0, 0);//0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF );

	//Apply image to screen 
	SDL_BlitSurface( traced, NULL, screen, NULL ); 

	//Update Screen 
	SDL_Flip( screen ); 

	//Free the loaded image 
	SDL_FreeSurface( traced ); 


	//Pause 
	SDL_Delay( 2000 );

	//Quit SDL 
	SDL_Quit(); 

	delete[] pixels;
	
	return 0; 
}

char* doTracing()
{
	// Create pixel array
	char* pixels = new char[640*480*4];

	// Set all values to 0 ( black )
	memset( pixels, 0, 640 * 480 * 4);

	for ( int x = 0; x < 640 ; ++x )
	{
		for ( int y = 0; y < 480; ++y)
		{

			if ( 
				( x >= square_tl.x && x <= square_br.x)
				&& ( y >= square_tl.y && y <= square_br.y)
			   )
			{
				// Set pixel ( x, y ) to 255, 255, 255 ( white )
				memset( &pixels[ (  ( x + (  y * 640  )  ) *  4 ) + 0 ] , 255, 1 ); // Red
				memset( &pixels[ (  ( x + (  y * 640  )  ) *  4 ) + 1 ] , 255, 1 ); // Green
				memset( &pixels[ (  ( x + (  y * 640  )  ) *  4 ) + 2 ] , 255, 1 ); // Blue
			}

		}
	}
	return pixels;
}
