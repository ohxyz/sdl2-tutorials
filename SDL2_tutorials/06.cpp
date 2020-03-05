#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>

using namespace std;

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

enum KeyPressSurfaces {

    KEY_PRESS_SURFACE_DEFAULT,
    KEY_PRESS_SURFACE_UP,
    KEY_PRESS_SURFACE_DOWN,
    KEY_PRESS_SURFACE_TOTAL
};

SDL_Window* gWindow = NULL;
SDL_Surface* gScreenSurface = NULL;

bool init();
void close();
SDL_Surface* loadSurface( string );

bool init() {

    if ( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {

        printf( "Failed to initialize SDL. SDL Error: %s\n", SDL_GetError() );
        return false;
    }

    gWindow = SDL_CreateWindow(
        "SDL_TUTE",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        SCREEN_WIDTH,
        SCREEN_HEIGHT,
        SDL_WINDOW_SHOWN
    );

    if ( gWindow == NULL ) {

        printf( "Failed to create window. SDL Error: %s\n", SDL_GetError() );
        return false;
    }

    int imgFlags = IMG_INIT_PNG;

    if ( !(IMG_Init( imgFlags ) & imgFlags) ) {

        printf( "SDL_image does not initialize! SDL_image Error: %s\n", IMG_GetError() );
        return false;
    }

    gScreenSurface = SDL_GetWindowSurface( gWindow );
    return true;
}

SDL_Surface* loadSurface( string path ) {

    SDL_Surface* surface = IMG_Load( path.c_str() );

    if ( surface == NULL ) {

        printf( "Failed to load image. SDL Error: %s\n", SDL_GetError() );
        return NULL;
    }

    SDL_Surface* optimizedSurface = SDL_ConvertSurface( surface, gScreenSurface->format, 0 );

    if ( optimizedSurface == NULL ) {

        printf( "Unable to optimize image %s!, SDL Error: %s\n", path.c_str(), SDL_GetError() );
    }

    SDL_FreeSurface( surface );

    return optimizedSurface;
}

void close() {

    SDL_DestroyWindow( gWindow );
    gWindow = NULL;

    SDL_Quit();
}

int main( int argc, char* argv[] ) {

    if ( !init() ) {

        printf( "Failed to initialize.\n" );
        return 1;
    }

    bool shouldQuit = false;
    SDL_Event event;
    SDL_Surface* surface = loadSurface( "Images/feb.png" );

    SDL_BlitSurface( surface, NULL, gScreenSurface, NULL );
    SDL_UpdateWindowSurface( gWindow );

    while ( !shouldQuit ) {

        while ( SDL_PollEvent( &event ) ) {

            if ( event.type == SDL_QUIT ) {

                shouldQuit = true;
            }
        }
    }

    close();

    return 0;
}