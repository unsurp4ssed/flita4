#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <error.h>
#define DATAFILE "../input2.txt"
#define STATFILE "../info.txt"
#define MAX_VERTICES 100
int matrix[MAX_VERTICES][MAX_VERTICES];

typedef struct Edge {
    int originNode, destNode;
} Edge_t;

int* getMaxValencies(int m[MAX_VERTICES][MAX_VERTICES], int v) {
    int* array = calloc(v, sizeof(int));
    for (int j = 0; j < v; j++) {
        for (int i = 0; i < v; i++) {
            if (i==j) array[j] += 2;
            if (m[j][i] != 0) array[j]++;
        }
    }
    int max = 0;
    int* res = calloc(v, sizeof(int));
    for (int i = 0; i < v; i++) res[i] = 0;
    for (int i = 0; i < v; ++i) {
        if (array[i] > max) max = array[i];
    }
    for (int i = 0; i < v; i++) {
        if (array[i] == max) res[i] = 1;
    }
    free(array);
    return res;
}

long long doTheStuff(int vertices) {
    struct timespec tv1;
    struct timespec tv2;
    /*
    FILE *f;
    f = fopen(DATAFILE, "r+");
    if (f == NULL) {
        printf("Couldn't find or open the file %s\n", DATAFILE);
        return 1;
    }
    */
    /*
    int matrix[MAX_VERTICES][MAX_VERTICES];
    char str[MAX_VERTICES];
    char element[2];
    const char sep[2] = " ";
    char *token;

    int vertices = (int)strlen(fgets(str, MAX_VERTICES, f))/2;
    //printf("\nVertices:\t%d\n", vertices);
    fclose(f);
    f = fopen(DATAFILE, "r");

    for (int j = 0; j < vertices; j++) {
        fgets(str, MAX_VERTICES, f);
        token = strtok(str, sep);
        strcpy(element, token);
        matrix[0][j] = atoi(element);
        for (int i = 1; i < vertices; i++) {
            token = strtok(NULL, sep);
            strcpy(element, token);
            matrix[i][j] = atoi(element);
        }
    }
    fclose(f);
    //printf("File have been read successfully. Data:\n");
    for (int j = 0; j < vertices; j++) {
        for (int i = 0; i < vertices; i++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
    */

    int *valencies = (int*)calloc(vertices, sizeof(int));

    if(clock_gettime(CLOCK_REALTIME, &tv1)) perror("error clock_gettime\n");
    valencies = getMaxValencies(matrix, vertices);
    if(clock_gettime(CLOCK_REALTIME, &tv2)) perror("error clock_gettime\n");
    long long int runtime = tv2.tv_nsec-tv1.tv_nsec;
/*
    FILE* dotFile = fopen("../graph.dot", "w");
    fprintf(dotFile, "graph G {\n");
    for (int i = 0; i < vertices; i++) {
        if (valencies[i]) fprintf(dotFile, "\t%d\t [color=red];", i);
    }

    Edge_t edge;

    for (int i = 0; i < vertices; i++) {
        for (int j = i; j < vertices; j++) {
            int stg = matrix[i][j];
            if (stg) {
                for (int q = 0; q < stg; q++) fprintf(dotFile, "\t%d -- %d; \n", i, j);
            }
        }
    }
    fprintf(dotFile, "\t%d -- %d; \n", edge.originNode, edge.destNode);


    fprintf(dotFile, "}");
    fclose(dotFile);

    system("dot -Tpng ../graph.dot -o ../graph.png"); //executing Graphviz which makes png of the graph
    system("start ../graph.png"); //opening image viewer automatically
*/
    //if (x > ((y - 1 ) * (y - 2)) / 2) printf("The graph is bonded"); //
    //else printf("The graph is NOT bonded");
    return runtime;
}

void generateAdjMatrix(int nodes) { //smezhoct'
    srand(time(NULL));
    for (int i = 0; i < nodes; i++) {
        for (int j = i; j < nodes; j++) {
            int val = rand() % 6;
            matrix[i][j] = val; matrix[j][i] = val;
        }
    }
}

void printMatrix(int nodes) {
    for (int i = 0; i < nodes; i++) {
        for (int j = 0; j < nodes; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}

int main() {
    FILE* gnuplotData = fopen(STATFILE,"w");
    for (int testNo = 10; testNo<100; testNo++) {
        if (testNo % 5 == 0) printf("|");
        long long runtime;
        generateAdjMatrix(testNo);
        runtime = doTheStuff(testNo);
        fprintf(gnuplotData,"%d %lld\n", testNo, runtime);
    }
    fclose(gnuplotData);

    system("gnuplot -e \"plot '../info.txt' u 1:2 with lines, '' u 1:2 with points, '' u 1:2 smooth bezier; pause -1\" && echo Plot generated successfully");
    return 0;
}