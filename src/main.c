#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

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
void coord2gcode(double **x, double **u, int len);

int main(){
    // write a test.c and automate the testing with make
    int n = 4, i;
    double **u, **x;
    double arrx[4][3] = {{1,0,0},{1,1,0},{1,1,1},{1,0,1}}, arru[4][3] = {{-1,0,0},{-1,1,0},{-1,1,1},{-1,0,1}};

    u = malloc(sizeof(u)*n);
    x = malloc(sizeof(x)*n);

    for(i=0;i<n;i++){
        u[i] = malloc(sizeof(u)*3);
        x[i] = malloc(sizeof(x)*3);
    }

    for(i=0;i<3;i++){
        u[i][0] = arru[i][0];
        u[i][1] = arru[i][1];
        u[i][2] = arru[i][2];

        x[i][0] = arrx[i][0];
        x[i][1] = arrx[i][1];
        x[i][2] = arrx[i][2];
    }


    coord2gcode(x,u,n);

    

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

void coord2gcode(double **x, double **u, int len){
    // defining variables
    int i;
    char *feedRate = "F100"; //this argument will have to be input somewhere
    
    // writing file
    FILE *out = fopen("file.ngc", "w"); // filename given from command line

    char *head[8] = {"G17","G21","G90","G40","G49","G64","G94",feedRate};

    // printing current time at the head of the file
    //fprintf(out,"( Created on; %s )\n",ctime(&time));

    // printing the necessary g-codes at the head of the file
    for(i=0;i<8;i++){
        fprintf(out,"%s\n\n",head[i]);
    }

    //translate coordinates to work with the 'lead in' path and also add the lead in path.

    // printing the coordinates for the towers to follow
    for(i=0;i<len;i++){
        fprintf(out,"G01 X%4.2f Y%4.2f U%4.2f Z%4.2f\n",x[i][1], x[i][2], u[i][1], u[i][2]);
    }

    fclose(out);

    // freeing memory
    free(u);
    free(x);
}