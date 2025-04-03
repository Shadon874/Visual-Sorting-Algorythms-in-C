#include "sorting.h"
#include "helper.h"

#define SCREEN_HEIGHT 800
#define OFFSET 4

void BubbleSortStep(int arr[], int size, SDL_Renderer *renderer, int *j, int *i, bool *swapped, bool *completed, int screenWidth, int maxValue)
{
    int swappedIndex = -1;

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
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // Default white color

                // Highlight the swapped elements in green
                if (k == swappedIndex || k == swappedIndex - 1) 
                {
                    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); // Green for swapped elements
                    beep(fmin(arr[k] * 4, 2000)); // Beep based on the bar value
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
            stop_audio();
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
            beep(fmin(arr[j] * 4, 2000));
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
        stop_audio();
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