#include <stdio.h>
#include <math.h>

#define TESRES 500

int tesselator(void){

    //Open input/output files
    FILE *in;
    FILE *out;
    in = fopen("layer.gcode","r");
    out = fopen("output.gcode","w");
    if(in == NULL){
        printf("Could not read input.gcode.");
        return 1;
    }
    if(out == NULL){
        printf("Unable to write to output.gcode");
        return 2;
    }
    
    float coord[TESRES][3];
    float tcoord[TESRES][3];
    int cout=0; 
    
    //Scan for precursor values to coordinates and input them to 2D array
    char tmpchar;
    float tmpcoord;
    while(feof(in)==0){
        tmpchar = getc(in);
        if((int)tmpchar == 88){ //if X read value after and store as X coord
            fscanf(in,"%f",&tmpcoord);
            coord[cout][0] = tmpcoord;
        }
        if((int)tmpchar == 89){ //if Y...
            fscanf(in,"%f",&tmpcoord);
            coord[cout][1] = tmpcoord;
        }
        if((int)tmpchar == 69){ //if E...
            fscanf(in,"%f",&tmpcoord);
            coord[cout][2] = tmpcoord;
            cout++;
        }
    }
    printf("HOW MANY PRINTHEADS: (MAX OF 3, 0=DEFAULT EXTRUDER)\n");
    float phead;
    scanf("\n%f",&phead);

    printf("HOW MANY SECTIONS: (MAX OF 3, 0=DEFAULT EXTRUDER)\n");
    int sect;
    scanf("\n%d",&sect);

    printf("WHAT ANGLE SHOULD THESE COORDINATES BE ROTATED: (DEGREES)\n");
    float angle[sect];
    for(int i=0; i<sect; i++){
        scanf("\n%f",&angle[i]);
        angle[i]=3.14159*angle[i]/180;
    }

    printf("FLIP X: (1=YES, 0=NO)\n");
    int fx;
    scanf("\n%i",&fx);

    printf("FLIP Y: (1=YES, 0=NO)\n");
    int fy;
    scanf("\n%i",&fy);

    printf("SWITCHBACK LAYERING: (1=YES, 0=NO)\n");
    int switchback;
    scanf("\n%i",&switchback);

    printf("RECURSIVE TRANSFORM: (1=YES, 0=NO)\n");
    int rt;
    scanf("\n%i",&rt);

    //Prompt layer size
    printf("WHAT IS THE LAYER SIZE: (MM)\n");
    float layer;
    scanf("\n%f",&layer);
    
    //Prompt max height
    printf("WHAT IS THE SCAFFOLD HEIGHT: (MM)\n");
    float zmax;
    scanf("\n%f", &zmax);

    printf("COOLDOWN TIME BETWEEN LAYERS: (MS)\n");
    float cooldown;
    scanf("\n%f",&cooldown);

    printf("PRINTHEAD SPEED: (MM/S)\n");
    float speed;
    scanf("\n%f",&speed);
    speed*=60;


    //transform coordinates using Euler rotation matrix /flip
    for(int i=0; i<cout;i++){
        tcoord[i][0] = coord[i][0]*cos(angle[0])+coord[i][1]*sin(angle[0]);
        tcoord[i][1] = -1*coord[i][0]*sin(angle[0])+coord[i][1]*cos(angle[0]);
        tcoord[i][2] = coord[i][2];
    }
    if(fx==1){
        for(int i=0; i<cout;i++){
            tcoord[i][0]=-1*tcoord[i][0];
        }
    }
    if(fy==1){
        for(int i=0; i<cout;i++){
            tcoord[i][1]=-1*tcoord[i][1];
        }
    }

    //Calculate bi/trilayer switch points
    float sw=zmax/phead;
    int head=0;


    //Output to .gcode/console
    printf("\nCODE: \n\n");
    printf("M107 ;\nG21 ;\nG90 ;\nM83 ;\n");
    fprintf(out,"M107 ;\nG21 ;\nG90 ;\nM83 ;\n");
    if(phead>1){
        printf("T0 ;\n");
    }
    printf("G1 Z%f F60.000;\n",layer);
    fprintf(out,"G1 Z%.3f F60.000;\n",layer);
    printf("G1 X%.3f Y%.3f E0;\n",-1*coord[0][1],coord[0][0]);
    fprintf(out,"G1 X%.3f Y%.3f E0;\n",-1*coord[0][1],coord[0][0]);
    int layercout=1;
    float sectiontrack=0;
    float headtrack=0;
    float previoushead=0;

    for(float z=layer; z<=zmax; z+=layer){
        sectiontrack=sect*z/zmax;
        previoushead=headtrack;
        headtrack=head*z/zmax;
        if((int)headtrack > (int)previoushead){
            printf("T%d;\n",(int)headtrack);
            fprintf(out,"T%d;\n",(int)headtrack);
        }
        printf("G1 Z%.3f F%.3f;\n",z,speed);
        fprintf(out,"G1 Z%.3f F%.3f;\n",z,speed);
        layercout++;
        if(layercout%2==1 && rt!=1){
            printf("G1 X%.3f Y%.3f E0;\n",coord[0][0],coord[0][1]);
            fprintf(out,"G1 X%.3f Y%.3f E0;\n",coord[0][0],coord[0][1]);
            for(int i=1; i<cout;i++){
                printf("G1 X%.3f Y%.3f E%.1f;\n",coord[i][0],coord[i][1],coord[i][2]);
                fprintf(out,"G1 X%.3f Y%.3f E%.1f;\n",coord[i][0],coord[i][1],coord[i][2]);
            }
            if(switchback==1 && (z+layer<=zmax)){
                printf("G4 P%f;\n",cooldown);
                z+=layer;
                printf("G1 Z%.3f F%.3f;\n",z,speed);
                fprintf(out,"G1 Z%.3f F%.3f;\n",z,speed);
                for(int i=cout-1; i>=0;i--){
                    printf("G1 X%.3f Y%.3f E%.1f;\n",coord[i][0],coord[i][1],coord[i][2]);
                    fprintf(out,"G1 X%.3f Y%.3f E%.1f;\n",coord[i][0],coord[i][1],coord[i][2]);
                }
                
            }
        } else if (layercout%2==0 && rt!=1) {
            printf("G1 X%.3f Y%.3f E0;\n",tcoord[0][0],tcoord[0][1]);
            fprintf(out,"G1 X%.3f Y%.3f E0;\n",tcoord[0][0],tcoord[0][1]);
            for(int i=1; i<cout;i++){
                printf("G1 X%.3f Y%.3f E%.1f;\n",tcoord[i][0],tcoord[i][1],tcoord[i][2]);
                fprintf(out,"G1 X%.3f Y%.3f E%.1f;\n",tcoord[i][0],tcoord[i][1],tcoord[i][2]);
            }
            if(switchback==1 && (z+layer<=zmax)){
                printf("G4 P%f;\n",cooldown);
                z+=layer;
                printf("G1 Z%.3f F%.3f;\n",z,speed);
                fprintf(out,"G1 Z%.3f F%.3f;\n",z,speed);
                for(int i=cout-1; i>=0;i--){
                    printf("G1 X%.3f Y%.3f E%.1f;\n",tcoord[i][0],tcoord[i][1],tcoord[i][2]);
                    fprintf(out,"G1 X%.3f Y%.3f E%.1f;\n",tcoord[i][0],tcoord[i][1],tcoord[i][2]);
                }
            }
        } else {
            for(int i=0; i<cout;i++){
                float transx = tcoord[i][0]*cos(angle[(int)sectiontrack])+tcoord[i][1]*sin(angle[(int)sectiontrack]);
                float transy = -1*tcoord[i][0]*sin(angle[(int)sectiontrack])+tcoord[i][1]*cos(angle[(int)sectiontrack]);
                tcoord[i][0] = transx;
                tcoord[i][1] = transy;
            }
            if(fx==1){
                for(int i=0; i<cout;i++){
                    tcoord[i][0]=-1*tcoord[i][0];
                }
            }
            if(fy==1){
                for(int i=0; i<cout;i++){
                    tcoord[i][1]=-1*tcoord[i][1];
                }
            }
            printf("G1 X%.3f Y%.3f E0;\n",tcoord[0][0],tcoord[0][1]);
            fprintf(out,"G1 X%.3f Y%.3f E0;\n",tcoord[0][0],tcoord[0][1]);
            for(int i=0; i<cout;i++){
                printf("G1 X%.3f Y%.3f E%.1f;\n",tcoord[i][0],tcoord[i][1],tcoord[i][2]);
                fprintf(out,"G1 X%.3f Y%.3f E%.1f;\n",tcoord[i][0],tcoord[i][1],tcoord[i][2]);
            }
            if(switchback==1 && (z+layer<=zmax)){
                printf("G4 P%f;\n",cooldown);
                z+=layer;
                printf("G1 Z%.3f F%.3f;\n",z,speed);
                fprintf(out,"G1 Z%.3f F%.3f;\n",z,speed);
                for(int i=cout-1; i>=0;i--){
                    printf("G1 X%.3f Y%.3f E%.1f;\n",tcoord[i][0],tcoord[i][1],tcoord[i][2]);
                    fprintf(out,"G1 X%.3f Y%.3f E%.1f;\n",tcoord[i][0],tcoord[i][1],tcoord[i][2]);
                }
            }
        }
    }
    printf("G4 P%f;\n",cooldown+1000);
    printf("M107 ;\nG0 Z50\nM84 ;\n");
    fprintf(out,"M107 ;\nG0 Z50\nM84 ;\n");

    fclose(in);
    fclose(out);

    return 0;
}