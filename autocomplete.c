#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include <ctype.h>
#include "autocomplete.h"

int comparator(const void *x_void, const void *y_void) {
    struct term* x = (struct term*)x_void;
    struct term* y = (struct term*)y_void;

    return strcmp(x -> term, y -> term);

}

int compare_weight(const void *p_a, const void *p_b) {
    double diff = ((term*)p_b)->weight - ((term*)p_a)->weight;
    return (diff > 0) - (diff < 0);  
}

void read_in_terms(term **terms, int *pnterms, char *filename) {
    FILE *fptr = fopen(filename, "r");

    // Read the number of terms
    char block[200];
    if (fgets(block, sizeof(block), fptr)) {
        *pnterms = atoi(block);
    } 

    // Allocate memory for terms
    *terms = (term *)malloc(*pnterms * sizeof(term));
   
    int line_num = 0;
    while (fgets(block, sizeof(block), fptr) && line_num < *pnterms) {
        int i = 0;

        while (block[i] == ' ') i++;

        double w;
        sscanf(&block[i], "%lf", &w);
        while (block[i] != '\t' && block[i] != ' ' && block[i] != '\0') i++;

        (*terms)[line_num].weight = w;

        // Skip tab
        if (block[i] == '\t') i++;

        // Copy the term (city name)
        strncpy((*terms)[line_num].term, &block[i], 199);
        (*terms)[line_num].term[199] = '\0';  

        // Remove trailing newline
        size_t len = strlen((*terms)[line_num].term);
        if (len > 0 && (*terms)[line_num].term[len - 1] == '\n') {
            (*terms)[line_num].term[len - 1] = '\0';
        }

        line_num++;
    }

    fclose(fptr);

    // Sort the terms 
    qsort(*terms, *pnterms, sizeof(term), comparator);
}

int lowest_match(struct term *terms, int nterms, char *substr) {
    int low = 0;
    int high = nterms - 1;
    int result = -1;

    while (low <= high) {
        int mid = low + (high - low)/2;
        int cmp = strncmp(terms[mid].term, substr, strlen(substr));

        if (cmp == 0) { 
            result = mid;
            high = mid - 1;
        } else if (cmp < 0) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }

    return result;
}

int highest_match(struct term *terms, int nterms, char *substr) {
    
    int low = 0;
    int high = nterms - 1;
    int result = -1;
    

    while (low <= high) {
        int mid = low + (high - low)/2;
        int cmp = strncmp(terms[mid].term, substr, strlen(substr));

        if (cmp ==0) {
            result = mid;
            low = mid + 1;
        } else if (cmp < 0) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return high;

}

void autocomplete(struct term **answer, int *n_answer, struct term *terms, int nterms, char *substr) {
    int first = lowest_match(terms, nterms, substr);
    int last = highest_match(terms, nterms, substr);

    *n_answer = (last - first) + 1;
    *answer = (term *)malloc((*n_answer)*(sizeof(term)));
    
    for (int i = 0; i < *n_answer; i++) {
        strcpy((*answer+i)->term, (terms + first + i)->term);
        (*answer+i)->weight = (terms + first + i)->weight;
    }

    qsort(*answer, *n_answer, sizeof(term), compare_weight);
}