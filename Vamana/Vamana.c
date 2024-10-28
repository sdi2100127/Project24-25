#include "Vamana.h"

int** Vamana(float** dataset, int vecs, int comps, int R) {
    // first we initialize G to a random R-regular directed graph
    int** G = (int**)malloc(R * sizeof(int*));
    for (int i = 0; i < R; i++) {
        G[i] = (int*)malloc(vecs * sizeof(int));
    }

    printf("neighbours\n");
    int x;
    // for every vector in the dataset
    for (int j = 0; j < vecs; j++) {
        printf("vector %d:", j);
        // for every one of its R neighbours
        for (int i = 0; i < R; i++) {

            int stop = 1;
            while (stop == 1) {
                x = rand() % (vecs - 1);    // pick another vector randomly
                stop = 0;
                for (int z = 0; z < i; z++) {
                    // as long as that vector is not a neighbour already and it is not the same as our current vector
                    if (x == G[z][j] || x == j) {   
                        stop = 1;
                        break;
                    }
                }    
            }
            G[i][j] = x;
            printf(" %d", G[i][j]);
            
        }
        printf("\n");
    }
}