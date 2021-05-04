#define _POSIX_C_SOURCE 199309L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

#include "StringCreator.c"
#include "Plot.c"

#define A 1000           //lunghezza minima
#define MAXLENGTH 500000 //lunghezza massima
#define Emax 0.001       //Errore relativo massimo
#define BILLION 1000000000L;

int main()
{
    int generationMethod;
    printf("Scegli un metodo per creare la stringa:\n");
    printf("1- Generazione pseudo-casuale\n");
    printf("2- s(i) = s((i-1) mod q+1)\n");
    printf("3- Variante del metodo 3 con aggiunta di lettera c\n");
    printf("Metodo da usare: ");
    scanf("%d", &generationMethod);

    double B = exp((log(MAXLENGTH) - log(A)) / 99);
    double n;

    char *S = NULL;

    double x[100];
    double y[100];

    double tn = 0;

    for (int j = 0; j <= 99; j++)
    {
        n = A * pow(B, j);

        struct timespec start, end;

        long R = getResolution() / (double)BILLION;

        int k = 0;

        double tempo = 0;

        clock_gettime(CLOCK_MONOTONIC, &start);
        do
        {
            S = malloc((int)floor(n) + 1); //TODO: controlla uso memoria

            switch (generationMethod)
            {
            case 1:
                firstMethod((int)floor(n), S);
                clock_gettime(CLOCK_MONOTONIC, &end);
                break;
            case 2:
                secondMethod((int)floor(n), S);
                clock_gettime(CLOCK_MONOTONIC, &end);
                break;
            case 3:
                thirdMethod((int)floor(n), S);
                clock_gettime(CLOCK_MONOTONIC, &end);
                break;
            default:
                printf("Valore non valido.\n");
                break;
            }

            free(S);

            k++;
            tempo += (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / (double)BILLION;

        } while (tempo < ((R / Emax) + R));

        tn = (tempo / k);

        printf("%i   %lf\n", (int)floor(n), tn);
        x[j] = n;
        y[j] = tn;
    }
    //-----------------------------------------------

    plotStringGenerationTime(x, y);

    return 0;
}