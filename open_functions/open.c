#include "open.h"

void* fvecs_open(const char *filename){
    FILE *fp = NULL;
    fp = fopen("path/siftsmall_base.fvecs", "r");

    if (fp == NULL) {
        perror("error opening file");
        exit(EXIT_FAILURE);
    }

    // d == 128??
    int d;
    fgets(&d, sizeof(int), fp);

    void* vector;
}