#include <SDL.h>
#include <stdio.h>

const int SCREEN_WIDTH = 323;
const int SCREEN_HEIGHT = 322;

SDL_Window* gWindow = NULL;
SDL_Surface* gScreenSurface = NULL;
SDL_Surface* gHelloWorld = NULL;

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

    gScreenSurface = SDL_GetWindowSurface( gWindow );
    return true;
}

bool loadMedia() {

    gHelloWorld = SDL_LoadBMP( "Images/winston.bmp1" );
    if ( gHelloWorld == NULL ) {

        printf( "Failed to load image. SDL Error: %s\n", SDL_GetError() );
        return false;
    }

    return true;
}

void close() {

    SDL_FreeSurface( gHelloWorld );
    gHelloWorld = NULL;

    SDL_DestroyWindow( gWindow );
    gWindow = NULL;

    SDL_Quit();
}

int main( int argc, char* argv[] ) {

    bool shouldQuit = false;
    SDL_Event event;

    if ( !init() ) {

        printf( "Failed to initialize.\n" );
        
    }
    else {

        if ( !loadMedia() ) {

            printf( "Failed to load media.\n" );
        }
        else {

            SDL_BlitSurface( gHelloWorld, NULL, gScreenSurface, NULL );
            SDL_UpdateWindowSurface( gWindow );

            while ( !shouldQuit ) {

                while ( SDL_PollEvent( &event ) ) {

                    if ( event.type == SDL_QUIT ) {

                        shouldQuit = true;
                    }
                }
            }
        }
    }

    close();

    return 0;
}