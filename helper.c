#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <SDL2/SDL.h>

#define SAMPLE_RATE 44100
#define AMPLITUDE 28000

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

SDL_AudioDeviceID device = 0; // Global variable to hold the audio device

// This will store the frequency to be played
double beepFrequency = 0.0;

void generate_array(int arr[], int size, int range)
{
    for (int i = 0; i < size; i++)
    {
        arr[i] = rand() % range;
    }
}

void audio_callback(void *userdata, Uint8 *stream, int len) {
    static double phase = 0.0;
    double frequency = *(double *)userdata;
    int16_t *buffer = (int16_t *)stream;
    int samples = len / sizeof(int16_t);

    for (int i = 0; i < samples; i++) {
        buffer[i] = (int16_t)(AMPLITUDE * sin(phase));
        phase += 2.0 * M_PI * frequency / SAMPLE_RATE;
        if (phase > 2.0 * M_PI) phase -= 2.0 * M_PI;
    }
}

// Initializes the audio device
void init_audio() {
    SDL_AudioSpec spec;
    spec.freq = SAMPLE_RATE;
    spec.format = AUDIO_S16SYS;
    spec.channels = 1;
    spec.samples = 1024;
    spec.callback = audio_callback;
    spec.userdata = &beepFrequency;

    device = SDL_OpenAudioDevice(NULL, 0, &spec, NULL, 0);
    if (!device) {
        printf("Failed to open audio: %s\n", SDL_GetError());
    }

    SDL_PauseAudioDevice(device, 1); // Start the audio device
}

// Call this function to stop audio playback
void stop_audio() {
    SDL_PauseAudioDevice(device, 1); // Pause audio playback
}

// Play beep sound with given frequency
void beep(double frequency) {
    beepFrequency = frequency;  // Update the frequency
    SDL_PauseAudioDevice(device, 0);  // Resume audio
}

// Function to close audio device after finishing all operations
void close_audio() {
    SDL_CloseAudioDevice(device);
}

