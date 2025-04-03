#ifndef SORTING_H
#define SORTING_H

#include <SDL2/SDL.h>
#include <stdbool.h>

void BubbleSortStep(int arr[], int size, SDL_Renderer *renderer, int *j, int *i, bool *swapped, bool *completed, int screenWidth, int maxValue);
void QuickSortStep(int arr[], int size, SDL_Renderer *renderer, bool *completed, int stack[], int *stackSize, int screenWidth, int maxValue);

#endif
