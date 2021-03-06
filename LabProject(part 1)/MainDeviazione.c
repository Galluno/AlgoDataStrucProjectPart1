#define _POSIX_C_SOURCE 199309L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include "StringCreator.c"
#include "PeriodSmart.h"
#include "PeriodNaive.h"
#include "Plot.c"
#include "StringsTime.h"

#define BILLION 1000000000L;
#define A 1000           //lunghezza minima
#define MAXLENGTH 500000 //lunghezza massima
#define Emax 0.001       //Errore relativo massimo

int main()
{
    int firstChoice, secondChoice;
    printf("Scegli un metodo per creare la stringa:\n");
    printf("1- Generazione pseudo-casuale\n");
    printf("2- s(i) = s((i-1) mod q+1)\n");
    printf("3- Variante del metodo 3 con aggiunta di lettera c\n");
    printf("Metodo da usare: ");
    scanf("%d", &firstChoice);
    printf("\nScegi quale funzione usare:\n");
    printf("1- PeriodNaive\n");
    printf("2- PeriodSmart\n");
    printf("Funzione da usare: ");
    scanf("%d", &secondChoice);

    //-----------------------------------------------

    double B = exp((log(MAXLENGTH) - log(A)) / 99);
    double n = 100000;

    char *S = NULL;

    double x[100];                                                              //array contenente lunghezza della stringa all'iterazione j
    double y[100];                                                              //array contenente il tempo impiegato dall'algoritmo all'iterazione j 

    double tn = 0;

    double strGenTime;                                                          //array contenente i tempi medi impiegati per generare le strighe
    strGenTime = SingleStringGenerationTime(firstChoice, n);                    //strGenTime[j] = tempo medio misurato per generare la stringa di lunghezza n=floor(A*B^j)

    double times[100];                                                              
    double sum = 0.0;

    for (int j = 0; j < 100; j++)
    {

        struct timespec start, end;

        long R = getResolution()/ (double)BILLION;

        int k = 0;

        double tempo = 0;

        clock_gettime(CLOCK_MONOTONIC, &start);
        do
        {
            S = malloc((int)floor(n) + 1);

            switch (firstChoice)
            {
            case 1:
                firstMethod((int)floor(n), S);
                break;
            case 2:
                secondMethod((int)floor(n), S);
                break;
            case 3:
                thirdMethod((int)floor(n), S);
                break;
            default:
                printf("Valore non valido.\n");
                break;
            }

            switch (secondChoice)
            {
            case 1:
                periodNaiveMethod1(S);
                clock_gettime(CLOCK_MONOTONIC, &end);
                break;
            case 2:
                periodSmart(S);
                clock_gettime(CLOCK_MONOTONIC, &end);
                break;
            default:
                printf("Valore non valido.\n");
                break;
            }

            free(S);

            k++;

            tempo = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / (double)BILLION;

        } while (tempo < ((R / Emax) + R));

        

        tn = (tempo / k) - strGenTime;                                                      //tempo medio di esecuzione dell'algoritmo
        sum = sum + tn;                                                                     //incremento della somma dei tempi medi per il calcolo della deviazione
        
        printf("%i   %lf\n", (int)floor(n), tn);
        times[j] = tn;

        x[j] = j + 1;
        y[j] = tn;
    }

    double mean = sum / 100;                                                                //calcolo della media dei tempi medi di esecuzione (nel nostro caso 100) con una stringa di lunghezza 100k

    double sommatoria = 0.00;                                                               //inizio del calcolo della deviazione
    
    for (int i = 0; i < 100; i++)                                                           
    {                                                                                       
        sommatoria = sommatoria + pow((times[i] - mean), 2);                                
    }                                                                                       

    double deviation = sqrt(sommatoria / 100);                                              //fine calcolo deviazione standard

    printf("La deviazione standard vale %f\n", deviation);                                  //Print del valore su terminale

    plotDeviazione(x, y, mean, deviation);                                                  //Creazione del grafico utilizzando GNUPlot

    return 0;                                                                               
}