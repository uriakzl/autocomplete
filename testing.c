#include <stdio.h>
#include <stdlib.h>

void kairu(int *array) {
    array[2] = 1;
}

int main() {
    char block[] = {'8', '7', '9', '3', '4', '2'};
    char heft[3];
        for (int i = 0; i < 6; i++) {
            printf("lebron");
            heft[i] = block[i];
            
        }
    
    printf("%s\n", heft);
    double weight = atof(heft);

    printf("THis many pounds is kairu: %f", weight);



}