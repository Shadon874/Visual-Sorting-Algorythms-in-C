#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_error.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_scancode.h>
#include <SDL2/SDL_video.h>
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>
#include "sorting.h"
#include "helper.h"

#define SCREEN_HEIGHT 800
#define OFFSET 4

int main()
{
    int ARRAY_SIZE;
    printf("Enter Array size: ");
    scanf("%d", &ARRAY_SIZE);

    int *array = malloc(ARRAY_SIZE * sizeof(int));
    if (array == NULL)
    {
        printf("Memory allocation failed\n");
        return EXIT_FAILURE;
    }

    int SCREEN_WIDTH = fmax(600, ARRAY_SIZE * 4);

    int array_range;
    printf("Enter Array range: ");
    scanf("%d", &array_range);
    if (array_range < 0)
    {
        printf("Invalid range value. Setting to 10.\n");
        array_range = 10;
    }

    generate_array(array, ARRAY_SIZE, array_range);

    int delay;
    printf("Enter delay in milliseconds: ");
    scanf("%d", &delay);
    if (delay < 0)
    {
        printf("Invalid delay value. Setting to 0.\n");
        delay = 0;
    }

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
    {
        printf("Couldn't initialize SDL: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }
    init_audio();

    SDL_Window *window = SDL_CreateWindow("3D Cube", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window)
    {
        printf("Could not create window for SDL: %s\n", SDL_GetError());
        SDL_Quit();
        return EXIT_FAILURE;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer)
    {
        printf("Could not create renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return EXIT_FAILURE;
    }

    SDL_Event event;
    int running = 1;
    int i = 0;
    int j = 0;
    bool swapped = false;
    bool completed = false;
    int stack[ARRAY_SIZE * 2];
    int stackSize = 0;
    stack[stackSize++] = 0;
    stack[stackSize++] = ARRAY_SIZE - 1;

    int maxValue = array[0];
    for (int i = 1; i < ARRAY_SIZE; i++)
    {
        if (array[i] > maxValue)
        {
            maxValue = array[i];
        }
    }

    char sortType;
    printf("Enter 'b' for Bubble Sort or 'q' for Quick Sort: ");
    scanf(" %c", &sortType);

    while (running)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                running = 0;
                break;
            }
            if (event.type == SDL_KEYDOWN)
            {
                if (event.key.keysym.scancode == SDL_SCANCODE_SPACE)
                {
                    generate_array(array, ARRAY_SIZE, array_range);
                    i = 0;
                    j = 0;
                    swapped = false;
                    stackSize = 0;
                    stack[stackSize++] = 0;
                    stack[stackSize++] = ARRAY_SIZE - 1;
                    completed = false;
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        if (!completed)
        {
            if (sortType == 'q')
            {
                QuickSortStep(array, ARRAY_SIZE, renderer, &completed, stack, &stackSize, SCREEN_WIDTH, maxValue);
            }
            else if (sortType == 'b')
            {
                BubbleSortStep(array, ARRAY_SIZE, renderer, &j, &i, &swapped, &completed, SCREEN_WIDTH, maxValue);
            }
            SDL_Delay(delay);
        }
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    close_audio();
    SDL_Quit();

    for (int i = 0; i < ARRAY_SIZE; i++)
    {
        printf("%d ", array[i]);
    }

    free(array);

    return 0;
}