#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct coord
{
    double **x;
    double **u;
} coord;


coord block2tower(coord foam, double towerDist, int len);

double** alphaTransform(double **x, double alpha, int len);

double** chordTransform(double **x, double chord, int len);

coord dihedralTransform(coord foam, double dihedral);

coord sweepTransform(coord foam, double sweep);

void coord2gcode(coord tower, char **options);

int main(){
    // write a test.c and automate the testing with make

    return 0;
}

coord block2tower(coord foam, double towerDist, int len){
    // defining variables
    int i;
    double *tx, *tu, uTower, xTower;
    coord towerCoords;

    uTower = -towerDist/2.0;
    xTower = towerDist/2.0;

    // alocating memory
    towerCoords.u = malloc(sizeof(towerCoords.u)*len);
    towerCoords.x = malloc(sizeof(towerCoords.x)*len);

    tu = malloc(sizeof(tu)*len);
    tx = malloc(sizeof(tx)*len);

    // calculating values
    for(i=0;i<len;i++){
        tu[i] = (xTower - foam.x[i][0])/(foam.u[i][0] - foam.x[i][0]);
        tu[i] = (uTower - foam.x[i][0])/(foam.u[i][0] - foam.x[i][0]);
    }

    for(i=0;i<len;i++){
        towerCoords.x[i][0] = xTower;
        towerCoords.u[i][0] = uTower;

        towerCoords.u[i][1] = foam.x[i][0] + tu[i]*(foam.u[i][1]-foam.x[i][1]);
        towerCoords.u[i][2] = foam.x[i][2] + tu[i]*(foam.u[i][2]-foam.x[i][2]);

        towerCoords.x[i][1] = foam.x[i][0] + tx[i]*(foam.u[i][1]-foam.x[i][1]);
        towerCoords.x[i][2] = foam.x[i][2] + tx[i]*(foam.u[i][2]-foam.x[i][2]);
    }

    // freeing memory
    free(foam.x);
    free(foam.u);
    free(tx);
    free(tu);
    
    return towerCoords;

}

double** alphaTransform(double **x, double alpha, int len){
    // defining variables
    int i;
    double **transform;

    // allocating memory
    transform = malloc(sizeof(transform)*len);

    // calculating values
    for(i=0;i<len;i++){
        transform[i][0] = x[i][0];
        transform[i][1] = x[i][1]*cos(-alpha)+x[i][2]*sin(-alpha);
        transform[i][2] = -x[i][1]*sin(-alpha)+x[i][2]*cos(-alpha);
    }

    // freeing memory
    free(x);

    return transform;
}

double** chordTransform(double **x, double chord, int len){
    // defining variables
    int i;
    double **transform;

    // allocating memory
    transform = malloc(sizeof(transform)*len);

    // calculating values
    for(i=0;i<len;i++){
        transform[i][0] = x[i][0];
        transform[i][1] = x[i][1]*chord;
        transform[i][2] = x[i][2]*chord;
    }

    // freeing memory
    free(x);

    return transform;
}