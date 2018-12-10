#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <SDL2/SDL.h>

#include "stream_common.h"
#include "oggstream.h"
#include <pthread.h>



int main(int argc, char *argv[]) {
    int res;

    if (argc != 2) {
	fprintf(stderr, "Usage: %s FILE", argv[0]);
	exit(EXIT_FAILURE);
    }
    assert(argc == 2);


    // Initialisation de la SDL
    res = SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO|SDL_INIT_EVENTS);
    atexit(SDL_Quit);
    assert(res == 0);

    // start the two stream readers
    pthread_t vorbis_tid, theora_tid;

    int creation_error_vorbis = pthread_create(&vorbis_tid, NULL, vorbisStreamReader, argv[1]);
    if (creation_error_vorbis){
        perror("\nError during vorbis creation");
    } else { printf("\nVorbis thread created successfully");}

    int creation_error_theora = pthread_create(&theora_tid, NULL, theoraStreamReader, argv[1]);
    if (creation_error_theora){
        perror("\nError during theora creation");
    } else { printf("\nTheora thread created successfully");}

    // wait audio thread
    int thread_error = pthread_join(vorbis_tid, NULL);
    if (thread_error) {
        perror("\nError during wait for vorbis to terminate");
    } else {printf("\nWaited successfully");}
    // 1 seconde de garde pour le son,
    sleep(1);

    // tuer les deux threads videos si ils sont bloqués
    int cancel_theora = pthread_cancel(theora_tid);
    if (cancel_theora){
        perror("\nError while cancelling Theora thread");
    } else {printf("Theora thread successfully cancelled");}

    // attendre les 2 threads videos


    exit(EXIT_SUCCESS);
}
