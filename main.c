#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_error.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_scancode.h>
#include <SDL2/SDL_video.h>
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>

#define SCREEN_HEIGHT 800
#define OFFSET 4

void generate_array(int arr[], int size, int range)
{
    for (int i = 0; i < size; i++)
    {
        arr[i] = rand() % range;
    }
}

void BubbleSortStep(int arr[], int size, SDL_Renderer *renderer, int *j, int *i, bool *swapped, bool *completed, int screenWidth, int maxValue)
{
    int swappedIndex;

    if (*i < size - 1)
    {
        *swapped = false;
        if (*j < size - 1)
        {
            if (arr[*j] > arr[*j + 1])
            {
                int temp = arr[*j];
                arr[*j] = arr[*j + 1];
                arr[*j + 1] = temp;

                swappedIndex = *j + 1;

                *swapped = true;
            }

            for (int k = 0; k < size; k++)
            {
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

                if (swappedIndex == k)
                {
                    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
                }

                int barHeight = (SCREEN_HEIGHT - OFFSET) * (arr[k]) / (maxValue);
                if (barHeight < 0)
                    barHeight = 0;

                SDL_Rect rect = {k * (screenWidth / size), SCREEN_HEIGHT - barHeight, screenWidth / size - 2, barHeight};
                SDL_RenderFillRect(renderer, &rect);
            }

            SDL_RenderPresent(renderer);
            SDL_Delay(1);

            *j += 1;
        }
        else
        {
            *j = 0;
            *i += 1;
        }
    }
    else
    {
        if (*swapped == false)
        {
            *completed = true;
            swappedIndex = -1;
        }
    }
}

int partition(int arr[], int left, int right)
{
    int pivot = arr[right];
    int i = left - 1;

    for (int j = left; j < right; j++)
    {
        if (arr[j] <= pivot)
        {
            i++;
            int temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
        }
    }

    int temp = arr[i + 1];
    arr[i + 1] = arr[right];
    arr[right] = temp;

    return i + 1;
}

void QuickSortStep(int arr[], int size, SDL_Renderer *renderer, bool *completed, int stack[], int *stackSize, int screenWidth, int maxValue)
{
    if (*stackSize < 2)
    {
        *completed = true;
        return;
    }
    int high = stack[--(*stackSize)];
    int low = stack[--(*stackSize)];

    int pivotIndex = -1;

    if (low < high)
    {
        pivotIndex = partition(arr, low, high);

        if (pivotIndex - 1 > low)
        {
            stack[(*stackSize)++] = low;
            stack[(*stackSize)++] = pivotIndex - 1;
        }
        if (pivotIndex + 1 < high)
        {
            stack[(*stackSize)++] = pivotIndex + 1;
            stack[(*stackSize)++] = high;
        }
    }

    for (int k = 0; k < size; k++)
    {
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

        if (k == high)
        {
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        }
        if (k == low)
        {
            SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
        }
        if (pivotIndex != -1 && k == pivotIndex)
        {
            SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        }

        int barHeight = (SCREEN_HEIGHT - OFFSET) * (arr[k]) / (maxValue);
        SDL_Rect rect = {k * (screenWidth / size), SCREEN_HEIGHT - barHeight, screenWidth / size - 2, barHeight};
        SDL_RenderFillRect(renderer, &rect);
    }
    SDL_RenderPresent(renderer);
}

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
    SDL_Quit();

    for (int i = 0; i < ARRAY_SIZE; i++)
    {
        printf("%d ", array[i]);
    }

    free(array);

    return 0;
}