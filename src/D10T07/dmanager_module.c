#include <stdio.h>
#include <stdlib.h> 
#include <time.h> 

#include "door_struct.h"

#define DOORS_COUNT 15
#define MAX_ID_SEED 10000

void initialize_doors(struct door* doors);
void sort_doors(struct door* doors, int count);
void close_doors(struct door* doors, int count);
void print_doors(const struct door* doors, int count);

int main(void)
{
    struct door doors[DOORS_COUNT];
    
    initialize_doors(doors);
    sort_doors(doors, DOORS_COUNT);
    close_doors(doors, DOORS_COUNT);
    print_doors(doors, DOORS_COUNT);

    return 0;
}

void sort_doors(struct door* doors, int count)
{
    for (int i = 1; i < count; i++) {
        struct door key = doors[i];
        int j = i - 1;

        while (j >= 0 && doors[j].id > key.id) {
            doors[j + 1] = doors[j];
            j--;
        }

        doors[j + 1] = key;
    }
}

void close_doors(struct door* doors, int count)
{
    for (int i = 0; i < count; i++) {
        doors[i].status = 0;
    }
}

void print_doors(const struct door* doors, int count)
{
    for (int i = 0; i < count; i++) {
        printf("%d, %d\n", doors[i].id, doors[i].status);
    }
}
// Doors initialization function
// ATTENTION!!!
// DO NOT CHANGE!
void initialize_doors(struct door* doors)
{
    srand(time(0)); 
    
    int seed = rand() % MAX_ID_SEED;
    for (int i = 0; i < DOORS_COUNT; i++)
    {
        doors[i].id = (i + seed) % DOORS_COUNT;
        doors[i].status = rand() % 2;
    }
}