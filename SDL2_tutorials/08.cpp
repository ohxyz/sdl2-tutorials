#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>

using namespace std;

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

SDL_Window* gWindow = NULL;
SDL_Surface* gScreenSurface = NULL;
SDL_Renderer* gRenderer = NULL;

bool init();
void close();

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

    gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED );

    if ( gRenderer == NULL ) {

        printf( "Renderer could not be created. SDL Error: %s\n", SDL_GetError() );
        return false;
    }

    SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

    int imgFlags = IMG_INIT_PNG;

    if ( !(IMG_Init( imgFlags ) & imgFlags) ) {

        printf( "SDL_image does not initialize! SDL_image Error: %s\n", IMG_GetError() );
        return false;
    }

    gScreenSurface = SDL_GetWindowSurface( gWindow );
    return true;
}

void close() {

    SDL_DestroyRenderer( gRenderer );
    SDL_DestroyWindow( gWindow );
    gRenderer = NULL;
    gWindow = NULL;

    IMG_Quit();
    SDL_Quit();
}

int main( int argc, char* argv[] ) {

    if ( !init() ) {

        printf( "Failed to initialize.\n" );
        return 1;
    }

    bool shouldQuit = false;
    SDL_Event event;

    while ( !shouldQuit ) {

        while ( SDL_PollEvent( &event ) ) {

            if ( event.type == SDL_QUIT ) {

                shouldQuit = true;
            }
        }

        SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
        SDL_RenderClear( gRenderer );

        SDL_Rect innerRect = { SCREEN_WIDTH/4, SCREEN_HEIGHT/4, SCREEN_WIDTH/2, SCREEN_HEIGHT/2 };
        SDL_SetRenderDrawColor( gRenderer, 0xFF, 0x00, 0x00, 0xFF );
        SDL_RenderFillRect( gRenderer, &innerRect );

        SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0xFF, 0xFF );
        for ( int i = 0; i < SCREEN_HEIGHT; i += 4 ) {

            SDL_RenderDrawPoint( gRenderer, SCREEN_WIDTH / 2, i );
        }

        SDL_RenderPresent( gRenderer );
    }

    close();

    return 0;
}