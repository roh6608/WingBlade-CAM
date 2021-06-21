#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// probably best to move my functions into header files etc for a cleaner build.
typedef struct coord
{
    double **x;
    double **u;
} coord;

// consier writing a function that can allocate and free the memory for my struct

coord block2tower(coord foam, double towerDist, int len);

double** alphaTransform(double **x, double alpha, int len);

double** chordTransform(double **x, double chord, int len);

coord dihedralTransform(coord foam, double dihedral, int len);

coord sweepTransform(coord foam, double sweep, int len);

// this is the function that will write the file. Options can read from a supplied file and then just add it
// into the head of the gcode program
void coord2gcode(coord tower, int len, char **options);

int main(int argc, char *argv[]){
    // write a test.c and automate the testing with make

    return 0;
}

// A function to transform the coordinates on the foam block to the coordinates the towers have to follow
coord block2tower(coord foam, double towerDist, int len){
    // defining variables
    int i;
    double tx, tu, uTower, xTower;
    coord towerCoords;

    uTower = -towerDist/2.0;
    xTower = towerDist/2.0;

    // alocating memory
    towerCoords.u = malloc(sizeof(towerCoords.u)*len);
    towerCoords.x = malloc(sizeof(towerCoords.x)*len);


    // calculating values
    for(i=0;i<len;i++){
        tu = (xTower - foam.x[i][0])/(foam.u[i][0] - foam.x[i][0]);
        tx = (uTower - foam.x[i][0])/(foam.u[i][0] - foam.x[i][0]);

        towerCoords.x[i][0] = xTower;
        towerCoords.u[i][0] = uTower;

        towerCoords.u[i][1] = foam.x[i][0] + tu*(foam.u[i][1]-foam.x[i][1]);
        towerCoords.u[i][2] = foam.x[i][2] + tu*(foam.u[i][2]-foam.x[i][2]);

        towerCoords.x[i][1] = foam.x[i][0] + tx*(foam.u[i][1]-foam.x[i][1]);
        towerCoords.x[i][2] = foam.x[i][2] + tx*(foam.u[i][2]-foam.x[i][2]);
    }

    // freeing memory
    free(foam.x);
    free(foam.u);
    
    return towerCoords;

}

// A function to change the angle of attack of the aerofoil by the value passed to it
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

// A function to scale an aerofoil to the chord passed to it
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

coord dihedralTransform(coord foam, double dihedral, int len){
    // defining variables
    int i;
    coord transform;

    // allocating memory
    transform.u = malloc(sizeof(transform.u)*len);
    transform.x = malloc(sizeof(transform.x)*len);

    // calculating values
    for(i=0;i<len;i++){
        transform.u[i][0] = foam.u[i][0];
        transform.u[i][1] = foam.u[i][1]-foam.u[i][0]*tan(dihedral);
        transform.u[i][2] = foam.u[i][2];
    }

    // freeing memory
    free(foam.u);
    free(foam.x);

    return transform;
}

coord sweepTransform(coord foam, double sweep, int len){
    // defining variables
    int i;
    coord transform;

    // allocating memory
    transform.u = malloc(sizeof(transform.u)*len);
    transform.x = malloc(sizeof(transform.x)*len);

    // calculating values
    for(i=0;i<len;i++){
        transform.u[i][0] = foam.u[i][0];
        transform.u[i][1] = foam.u[i][1]+foam.u[i][0]*tan(sweep);
        transform.u[i][2] = foam.u[i][2];
    }

    // freeing memory
    free(foam.u);
    free(foam.x);

    return transform;

}

void coord2gcode(coord tower, int len, char **options){
    // defining variables
    int i;
    // translate coordinates to be 0,0 at the front of foam cutter so will probably have to have some sort of
    // config file and parser.
    // also need the lead in path.

    // opening file (the head of the gcode program) this will get joined to the top of the written file
    // probably best to write standalone function for this.

    // writing file
    FILE *out = fopen("file.ngc", "w");

    for(i=0;i<len;i++){
        fprintf("G01 X%4.2f Y%4.2f U%4.2f Z%4.2f\n",&tower.x[i][1], &tower.x[i][2], &tower.u[i][1], &tower.u[i][2]);
    }

    // saving file
}