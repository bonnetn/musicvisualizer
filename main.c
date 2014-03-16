/**
 * \file main.c
 * \brief Main file
 * \author ALGERA Pieter & Nicolas BONNET
 * \date 17/03/2014
 *
 */

/**
 * \mainpage Music visualizer
 *
 */

#include <stdio.h>
#include <stdlib.h>

#ifdef __APPLE__
    #include <SDL/SDL.h>
#else
    #include <SDL.h>
#endif

#include "audio/musicdecomposer.h"
#include "bar/bar.h"


int main ( int argc, char** argv )
{

    struct WAVFile_DecomposedFile wav;
    SDL_Surface* display = NULL;

    SDL_Rect srcrect;

    srcrect.x = 0;
    srcrect.y = 0;

    /* SDL redirects the stdout and stderr to files, this reopens the stream into the console */
    freopen( "CON", "w", stdout );
    freopen( "CON", "w", stderr );

    puts("--- Visualizer ---");

    /* Initialize the SDL library */
    if( SDL_Init( SDL_INIT_AUDIO | SDL_INIT_TIMER | SDL_INIT_VIDEO ) < 0)
    {
        printf( "Unable to init SDL: %s\n", SDL_GetError() );
        return EXIT_FAILURE;
    }
    else
        puts( "SDL initialized" );

    /* Start decomposing the WAV file and send in through events */
    decomposeWav("test2.wav", &wav);

    display = SDL_SetVideoMode( 640, 480, 32, SDL_HWSURFACE );
    srcrect.w = display->w;
    srcrect.h = display->h;

    SDL_Event event;
    int quit = 0;

    FILE *log = fopen("lol.txt", "w");

    /* Receive the events */
    while( !quit )
    {
        SDL_WaitEvent( &event );
        switch( event.type )
        {

        case SDL_QUIT:
            quit = 1;
            break;

        case SDL_USEREVENT:

            if( event.user.code == 0x01 ) /* If it is an event which sends the level of the frequencies */
            {
                WAVFile_SingleSpectrum freq[26];

                memcpy( freq, event.user.data1, sizeof( WAVFile_SingleSpectrum ) * 26 );
                int i;
                for( i = 0; i < 26; i++) {
                    fprintf(log,"%d\t", freq[i]);
                    putpixel(display, i*20, display->h - freq[i], 0xff00ff);
                    // Tell SDL to update the whole screen
                    SDL_UpdateRect(display, 0, 0, display->w, display->h);
                }
                SDL_Delay(20);

                SDL_FillRect(display, &srcrect, 0x000000);
                fprintf(log, "\n");

            }
            else if( event.user.code == 0x02 ) /* If it is an event which indicates that it is done decomposing */
            {
                quit = 1;
            }

            break;

        default:
            break;
        }
    }

    fclose(log);
    freeDecomposedWav( &wav );

    puts("--- Visualizer ---");

    return EXIT_SUCCESS;
}
