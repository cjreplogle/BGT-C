#include <math.h>
#include <stdio.h>

#define PI 3.14159
#define RES 500

void infill(){

    //Scan radius
    printf("\nRADIUS (MM): \n");
    float radius;
    scanf("\n%f",&radius);

    //Scan pore size
    printf("\nPORE SIZE (MM):\n");
    float ps;
    scanf("\n%f",&ps);

    printf("\nPATH LINKAGE: (1=YES, 0=NO)\n");
    float pl;
    scanf("\n%f",&pl);

    //Open input/output files
    FILE *in;
    in = fopen("layer.gcode","w");
    if(in == NULL){
        printf("Could not open layer.gcode.");
    }

    //Generate points along circular curve
    float pset[RES][2];
    for(int i=0;i<RES;i++){
        pset[i][1]=radius*sin(i*PI/RES-PI);
        pset[i][0]=radius+radius*cos(i*PI/RES-PI);
    }

    //Rectilinear curve fill algorithm (tldr: move extruder to opposite side each time the x coord reaches a multiple of the pore size along x axis)
    int side=0;
    float j=-1*radius;
    int k=0;
    for(float i=ps;i<=2*radius+ps;i+=ps){
        j=-1*radius;
        if(side==0){
            while(j<i){
                j=pset[k][0];
                k++;
            }
            k--;
            pset[k][1]=-1*pset[k][1];
            k++;
            side=1;
        } else {
            while(j<i){
                j=pset[k][0];
                pset[k][1]=-1*pset[k][1];
                k++;
            }
            k--;
            pset[k][1]=-1*pset[k][1];
            k++;
            side=0;
        }
    }

    //Center coordinates
    for(int i=0;i<(RES-1);i++){
        pset[i][0]=pset[i][0]-radius;
    }

    //Curve smoothing function
        //To be added if needed.
            //Parametrize points by arc length (pass thru points and calculate arc length numerically, divide by res, pass through points again and each time the distance reaches length/res replace the point)
            //Convulute coordinate set over Gaussian distribution

    //Output to console/file
        //This thing right here just figures out how to fully close the structure

    float indextest=-1*radius;
    int poreindex=0;
    while(indextest<(-1*radius+ps)){
        indextest=pset[poreindex][0];
        poreindex++;
    }
    poreindex--; 

    if(pl==1){
        for(int i=poreindex-RES/15; i>=0;i--){
            fprintf(in,"G1 X%f Y%f E1;\n",pset[i][0],-1*pset[i][1]);
        } 
    }
    for(int i=0;i<(RES-1);i++){
        fprintf(in,"G1 X%f Y%f E1;\n",pset[i][0],pset[i][1]);
    }
    if(pl==1){
        for(int i=0; i<poreindex-RES/15;i++){
            fprintf(in,"G1 X%f Y%f E1;\n",-1*pset[i][0],-1*pset[i][1]);
        }
    }

    //properly documenting code is pointlessly time consuming and silly! if you can't read this and have questions email me
    //might make an actual gui if this garners lab support and people besides me actually want to use this to test things (probably no path rendering bc thats too hard)
    //also there will be no cpp used in this lab
}