#include <SDL/SDL.h>
#include <SDL/SDL_main.h>
#include <iostream>
#include "Vector2d.h"

char* doTracing();

vector2d square_tl( 40, 40 );
vector2d square_br( 60, 60);

void drawArray( char* pixels, SDL_Surface* screen )
{
	SDL_Surface* hello = NULL; 

	//Load image 
	hello = SDL_LoadBMP( "hello.bmp" );

	
	hello->pixels = pixels;
	std::cout << "w : " << hello->w << " h : " << hello->h << " format : " << hello->format << std::endl;
	std::cout << "data[0] : " << (int) pixels[0]  << std::endl;

	//Apply image to screen 
	SDL_BlitSurface( hello, NULL, screen, NULL ); 

	//Update Screen 
	SDL_Flip( screen ); 


	//Free the loaded image 
	SDL_FreeSurface( hello ); 
}
int main( int argc, char* args[] ) { 

	//The images 
	SDL_Surface* screen = NULL;

	//Start SDL 
	SDL_Init( SDL_INIT_EVERYTHING ); 

	//Set up screen 
	screen = SDL_SetVideoMode( 640, 480, 32, SDL_SWSURFACE ); 

	drawArray( doTracing(), screen );

	std::cin.ignore();

	//Pause 
	//SDL_Delay( 2000 );

	//Quit SDL 
	SDL_Quit(); 

	//delete[] pixels;
	
	return 0; 
}

char* doTracing()
{

	char* pixels = new char[640*480*3];

	memset( pixels, 0, 640 * 480 * 3);

	for ( int x = 0; x < 640 ; ++x )
	{
		for ( int y = 0; y < 480; ++y)
		{

			if ( 
				( x >= square_tl.x && x <= square_br.x)
				&& ( y >= square_tl.y && y <= square_br.y)
			   )
			{
				std::cout << "intersection : " << x << " , " << y << std::endl;
				memset( &pixels[ (  ( x + (  y * 640  )  ) *  3 ) + 0 ] , 255, 1 ); // Red
				//memset( &pixels[ (  ( x + (  y * 640  )  ) *  3 ) + 1 ] , 255, 1 ); // Green
				memset( &pixels[ (  ( x + (  y * 640  )  ) *  3 ) + 2 ] , 255, 1 ); // Blue
			}

		}
	}
	return pixels;
}
