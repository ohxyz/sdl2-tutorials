#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>

using namespace std;

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

SDL_Window* gWindow = NULL;
SDL_Surface* gScreenSurface = NULL;
SDL_Renderer* gRenderer = NULL;
SDL_Texture* gTexture = NULL;

bool init();
void close();
bool loadMedia();
SDL_Texture* loadTexture( string );

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

    SDL_SetRenderDrawColor( gRenderer, 0x00, 0xff, 0xff, 0xff );

    int imgFlags = IMG_INIT_PNG;

    if ( !(IMG_Init( imgFlags ) & imgFlags) ) {

        printf( "SDL_image does not initialize! SDL_image Error: %s\n", IMG_GetError() );
        return false;
    }

    gScreenSurface = SDL_GetWindowSurface( gWindow );
    return true;
}

SDL_Texture* loadTexture( string path ) {

    SDL_Surface* surface = IMG_Load( path.c_str() );

    if ( surface == NULL ) {

        printf( "Unable to laod image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
        return NULL;
    }

    SDL_Texture* newTexture = SDL_CreateTextureFromSurface( gRenderer, surface );

    if ( newTexture == NULL ) {

        printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
        return NULL;
    }

    SDL_FreeSurface( surface );

    return newTexture;
}

bool loadMedia() {

    gTexture = loadTexture( "Images/texture.png" );

    if ( gTexture == NULL ) {

        printf( "Unable to load texture image!\n" );
        return false;
    }

    return true;
}

void close() {

    SDL_DestroyTexture( gTexture );
    gTexture = NULL;

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

    loadMedia();

    while ( !shouldQuit ) {

        while ( SDL_PollEvent( &event ) ) {

            if ( event.type == SDL_QUIT ) {

                shouldQuit = true;
            }
        }

        SDL_RenderClear( gRenderer );

        SDL_Rect topRightViewport = { 20, 20, SCREEN_WIDTH/2, SCREEN_HEIGHT/2 };
        SDL_RenderSetViewport( gRenderer, &topRightViewport );

        SDL_RenderCopy( gRenderer, gTexture, NULL, NULL );
        SDL_RenderPresent( gRenderer );
    }

    close();

    return 0;
}