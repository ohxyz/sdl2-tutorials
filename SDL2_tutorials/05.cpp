#include <SDL.h>
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
SDL_Surface* gKeyPressSurfaces[ KEY_PRESS_SURFACE_TOTAL ];
SDL_Surface* gCurrentSurface = NULL;

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

    gScreenSurface = SDL_GetWindowSurface( gWindow );
    return true;
}

SDL_Surface* loadSurface( string path ) {

    SDL_Surface* surface = SDL_LoadBMP( path.c_str() );

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

    for ( int i = 0; i < KEY_PRESS_SURFACE_TOTAL; i++ ) {

        SDL_FreeSurface( gKeyPressSurfaces[ i ] );
        gKeyPressSurfaces[ i ] = NULL;
    }

    SDL_DestroyWindow( gWindow );
    gWindow = NULL;

    SDL_Quit();
}

int main( int argc, char* argv[] ) {

    if ( !init() ) {

        printf( "Failed to initialize.\n" );
        return 1;
    }

    gKeyPressSurfaces[ KEY_PRESS_SURFACE_DEFAULT ] = loadSurface( "Images/press.bmp" );

    if ( gKeyPressSurfaces[ KEY_PRESS_SURFACE_DEFAULT ] == NULL ) {

        printf( "Failed to load default image." );
        return 1;
    }

    gKeyPressSurfaces[ KEY_PRESS_SURFACE_UP ] = loadSurface( "Images/up.bmp" );

    if ( gKeyPressSurfaces[ KEY_PRESS_SURFACE_UP ] == NULL ) {

        printf( "Failed to load up image." );
        return 1;
    }

    gKeyPressSurfaces[ KEY_PRESS_SURFACE_DOWN ] = loadSurface( "Images/down.bmp" );

    if ( gKeyPressSurfaces[ KEY_PRESS_SURFACE_DOWN ] == NULL ) {

        printf( "Failed to load down image." );
        return 1;
    }

    bool shouldQuit = false;
    SDL_Event event;

    gCurrentSurface = gKeyPressSurfaces[ KEY_PRESS_SURFACE_DEFAULT ];

    while ( !shouldQuit ) {

        while ( SDL_PollEvent( &event ) ) {

            if ( event.type == SDL_QUIT ) {

                shouldQuit = true;
            }
            else if ( event.type == SDL_KEYDOWN ) {

                switch ( event.key.keysym.sym ) {

                case SDLK_UP:
                    gCurrentSurface = gKeyPressSurfaces[ KEY_PRESS_SURFACE_UP ];
                    break;

                case SDLK_DOWN:
                    gCurrentSurface = gKeyPressSurfaces[ KEY_PRESS_SURFACE_DOWN ];
                    break;

                default:
                    gCurrentSurface = gKeyPressSurfaces[ KEY_PRESS_SURFACE_DEFAULT ];
                    break;
                }
            }

            SDL_Rect rectOfStrech;
            
            rectOfStrech.x = 0;
            rectOfStrech.y = 0;
            rectOfStrech.w = SCREEN_WIDTH / 2;
            rectOfStrech.h = SCREEN_HEIGHT / 2;

            SDL_BlitScaled( gCurrentSurface, NULL, gScreenSurface, &rectOfStrech );
            SDL_UpdateWindowSurface( gWindow );
        }
    }

    close();

    return 0;
}