#ifndef HELPER_H
#define HELPER_H

#include <SDL2/SDL.h>

void generate_array(int arr[], int size, int range);
void beep(double frequency);
void audio_callback(void *userdata, Uint8 *stream, int len);
void init_audio();
void close_audio();
void stop_audio();

#endif