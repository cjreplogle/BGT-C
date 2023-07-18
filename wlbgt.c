#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

#define PI 3.14159

void infill(int RES, float lh);
void tesselator(int RES, float layer, float zmax, float cooldown, float speed);

int main(void){

        //backup default values if start sequence fails
        float lh=0.6;
        float zm=6;
        float cl=10000;
        float sp=60;
        int lr=200;
        int state=0;
        int sel=0;

        /*
        FILE *config;
        config = fopen("config.bpc","r");
        //default placeholders if fscan sequence doesnt work+init "global" variables
        if(config == NULL){
                printf("Could not read config.bpc, creating file...\n");
                fprintf(config,"0.6 6 10000 60 200");
        } else {
                fscanf(config,"%f %f %f %f %d",&lh,&zm,&cl,&sp,&lr);
                fprintf(config,"%f %f %f %f %d",lh,zm,cl,sp,lr);
                printf("config.bpc successfully opened.");
        }
        fclose(config);
        printf("\n");
        //printf("\e[1;1H\e[2J");
        */


        while(state==0){
                printf("\e[1;1H\e[2J");
                printf("/-------------------------------------------------------\\\n");
                printf("|--------- Whitlock Lab Bioprinting GCode Tool ---------|\n");
                printf("|---------- v0.5, made by CJ Replogle c. 2023 ----------|\n");
                printf("|-------------------------------------------------------|\n");
                printf("|                                                       |\n");
                printf("| 1: Generate New Scaffold                              |\n");
                printf("| 2: Generate From Preset Infill Pattern                |\n");
                printf("| 3: Generate Infill                                    |\n");
                printf("| 4: Settings                                           |\n");
                printf("| 5: About this Program / Help                          |\n");
                printf("| 6: Exit                                               |\n");
                printf("|                                                       |\n");
                printf("\\------------------------------------------------------/\n\n-> ");
                scanf("%i",&state);

                switch (state) { //decision structure to determine what to do depending on user input
                        case 1:
                                printf("\e[1;1H\e[2J");
                                infill(lr,lh);
                                tesselator(lr,lh,zm,cl,sp);
                                printf("/-------------------------------------------------------\\\n");
                                printf("| Scaffold successfully generated!                      |\n");
                                printf("\\-------------------------------------------------------/\n\n");
                                printf("Input any value to return to the menu -> ");
                                scanf(" %d",&state);
                                printf("\e[1;1H\e[2J");
                                state=0;
                                break;
                        case 2: 
                                printf("\e[1;1H\e[2J");
                                tesselator(lr,lh,zm,cl,sp);
                                printf("/-------------------------------------------------------\\\n");
                                printf("| Scaffold successfully generated!                      |\n");
                                printf("\\-------------------------------------------------------/\n\n");
                                printf("Input any value to return to the menu -> ");
                                scanf(" %d",&state);
                                printf("\e[1;1H\e[2J");
                                state=0;
                                break;
                        case 3:
                                printf("\e[1;1H\e[2J");
                                infill(lr,lh);
                                printf("/-------------------------------------------------------\\\n");
                                printf("| Infill successfully generated!                        |\n");
                                printf("\\-------------------------------------------------------/\n\n");
                                printf("Input any value to return to the menu -> ");
                                scanf(" %d",&state);
                                printf("\e[1;1H\e[2J");
                                state=0;
                                break;

                        case 4:
                                while(sel!=6){
                                        printf("\e[1;1H\e[2J");
                                        printf("\n1: Layer Height - %.3f mm\n",lh);
                                        printf("2: Scaffold Height - %.3f mm\n",zm);
                                        printf("3: Layer Cooldown - %.0f s\n",cl);
                                        printf("4: Printhead Speed - %.1f mm/min\n",sp);
                                        printf("5: Layer Resolution - %d mm/min\n",lr);
                                        printf("\n6: Back\n");
                                        printf("\n-> ");
                                        scanf("%d",&sel);
                                        switch (sel){
                                                case 1:
                                                        printf("\n New Layer Height (mm) -> ");
                                                        scanf("%f",&lh);
                                                        break;
                                                case 2:
                                                        printf("\n New Scaffold Height (mm) -> ");
                                                        scanf("%f",&zm);
                                                        break;
                                                case 3:
                                                        printf("\n Layer Cooldown (s) -> ");
                                                        scanf("%f",&cl);
                                                        break;
                                                case 4:
                                                        printf("\n Printhead Speed (mm/min) -> ");
                                                        scanf("%f",&sp);
                                                        break;
                                                case 5:
                                                        printf("\n Layer Resolution (points/layer) -> ");
                                                        scanf("%d",&lr);
                                                        break;

                                        }
                                        printf("\e[1;1H\e[2J");
                                }
                                FILE *fset;
                                fset = fopen("config.bpc","w");
                                fprintf(fset,"%f %f %f %f %d",lh,zm,cl,sp,lr);
                                fclose(fset);
                                state=0;
                                sel=0;
                                break;
                        case 5:
                                printf("\e[1;1H\e[2J");
                                printf("/-------------------------------------------------------\\\n");
                                printf("|                                                       |\n");
                                printf("| For detailed description of different variables, see: |\n| https://github.com/cjreplogle/BGT/blob/main/help.md   |\n");
                                printf("|                                                       |\n");
                                printf("\\-------------------------------------------------------/\n\n");
                                printf(" Input any value to return to the menu -> ");
                                char bk;
                                scanf(" %c",&bk);
                                state=0;
                                break;
                        case 6:
                                state=1;
                                break;
                }
        }
        
        exit(0);
}

void infill(int RES, float lh){ //Parametric cylinder slicer made by CJ Replogle. Might generalize fill algorithms to any shape later.
            //If my code explanations are not good enough, just email me or something (replogle.17@buckeyemail.osu.edu)

        //Open input/output files
    FILE *layer;
    layer = fopen("layer.gcode","w");
    if(layer == NULL){
        printf("Could not open layer.gcode.");
    }

    printf("\nINFILL PATTERNING\n-----------------\n");

    //Prompt user for different parameters
    printf("\nINFILL TYPE (0=RECTILINEAR CYLINDER, 1=CONCENTRIC CYLINDER, 2=RECTILINEAR SQUARE): ");
    int type;
    scanf("\n%d",&type);

    printf("\nRADIUS (MM): ");
    float radius;
    scanf("\n%f",&radius);
    float ps;
    float pd;
    float pl;
    float fill;
    switch (type){
        case 0:
            //Scan pore size
            printf("\nPORE SIZE (MM): ");
            scanf("\n%f",&ps);
            printf("\nPATH LINKAGE (1=YES, 0=NO): ");
            scanf("\n%f",&pl);
            pd=0;
            fill=0;
            break;
        case 1:
            //Scan pore size
            printf("\nPORE DEPTH (MM): ");
            scanf("\n%f",&pd);
            //Scan print number of pores
            printf("\nPORES PER LAYER(MM): ");
            scanf("\n%f",&ps);
            //Scan print fill
            printf("\nFILL?:\n");
            scanf("\n%f",&fill);
            pl=0;
            break;
        case 2:
            //Scan pore size
            printf("\nPORE SIZE (MM): ");
            scanf("\n%f",&ps);
            pl=0;
            pd=0;
            fill=0;
            break;
    }

    int sqindex=0;
    //Generate points along circular curve
    float pset[((int)ps+1)*RES][2];
    float opd=pd;
    int lc=0; //(layer count)
    switch (type) {
    case 0: //Generate circular 180 degree arc of "RES" points (RES=Resolution)
        for(int i=0;i<RES;i++){
            pset[i][0]=radius+radius*cos(i*PI/RES-PI);
            pset[i][1]=radius*sin(i*PI/RES-PI);
        }
        int side=0;
        float j=-1*radius;
        int k=0;
        for(float i=ps;i<=2*radius+ps;i+=ps){ //for each multiple of pore size x coord below diameter
            j=-1*radius;
            if(side==0){ //if odd
                while(j<i){ 
                    j=pset[k][0]; //generate positive y point along circle
                    k++;
                }
                k--;
                pset[k][1]=-1*pset[k][1]; //switch sides
                k++;
                side=1;
            } else { //otherwise (if even)
                while(j<i){
                    j=pset[k][0]; //generate negative y points along circle
                    pset[k][1]=-1*pset[k][1];
                    k++;
                }
                k--;
                pset[k][1]=-1*pset[k][1]; //switch sides
                k++;
                side=0;
            }
        }
        //Center coordinates along x y plane
        for(int i=0;i<(RES);i++){
            pset[i][0]=pset[i][0]-radius;
        }
        break;
    case 1: //Radial mesh (higher interior porosity has inferior biologics)
        do{ //do while moment!!! (it just does thing once first before checking loop condition)
            pset[lc*RES][0]=(radius-pd)*-1;
            pset[lc*RES][1]=0;
            for(int i=1;i<RES;i++){ //move in radial pattern for certain angular distance, then move to inner radius and do same. When circle closed, move inwards/repeat.
                if(((int)(2*ps*i/((float)RES)) % 2 ) == 1){
                    pset[i+lc*RES][0]=(radius-pd+opd)*cos(2*i*PI/RES-PI);
                    pset[i+lc*RES][1]=(radius-pd+opd)*sin(2*i*PI/RES-PI);
                } else {
                    pset[i+lc*RES][0]=(radius-pd)*cos(2*i*PI/RES-PI);
                    pset[i+lc*RES][1]=(radius-pd)*sin(2*i*PI/RES-PI);
                }
            }
            lc++;
            pd+=(opd+lh);
            ps=round(ps*(radius-opd)/(radius))-1;
        } while (fill==1 && (radius-pd>0));
        //Center coordinates along x y plane
        for(int i=0;i<(RES);i++){
            pset[i][0]=pset[i][0]-radius;
        }
        break;
    case 2: //Square
        sqindex=0;
        float y=radius;
        float x;
        for(x=-1*radius; x<=radius+0.02;x+=ps){
            pset[sqindex][0]=x;
            pset[sqindex][1]=y;
            sqindex++;
            y*=-1;
            if(x>radius){
                break;
            }
            pset[sqindex][0]=x;
            pset[sqindex][1]=y;
            sqindex++;
        }
        pset[sqindex][0]=x;
        pset[sqindex][1]=y;
        sqindex++;
    }

    //Curve smoothing function
        //To be added if needed.
            //Parametrize points by arc length (pass thru points and calculate arc length numerically, divide by res, pass through points again and each time the distance reaches length/res replace the point)
            //Convulute coordinate set over Gaussian distribution

    //Output to console/file
    float indextest=-1*radius; //This thing right here just figures out how to fully close the structure
    int poreindex=0;
    while(indextest<(-1*radius+ps)){
        indextest=pset[poreindex][0];
        poreindex++;
    }
    poreindex--; 

    if(pl==1){ //closes beginning of curve to side
        for(int i=poreindex/1.5; i>=0;i--){
            fprintf(layer,"G1 X%f Y%f E1;\n",pset[i][0],-1*pset[i][1]);
        }  
    }

    if(type==0){ //prints main curve segment
        for(int i=0; i<RES;i++){
            fprintf(layer,"G1 X%f Y%f E1;\n",pset[i][0],pset[i][1]);
        } 
    }

    if(type==2){ //prints main curve segment
        for(int i=0; i<sqindex;i++){
            fprintf(layer,"G1 X%f Y%f E1;\n",pset[i][0],pset[i][1]);
        } 
    }

    if(type==1){ //prints main curve segment for radially generated scaffolds
        for(int i=0;i<lc;i++){
            for(int j=i*RES;j<i*RES+RES;j++){
                fprintf(layer,"G1 X%f Y%f E1;\n",pset[j][0],pset[j][1]);
            }
            fprintf(layer,"G1 X%f Y%f E0;\n",pset[i*RES][0],pset[i*RES][1]);
        }
    }

    if(pl==1){ //prints end of curve to connect
        for(int i=0; i<poreindex/2.1;i++){
            fprintf(layer,"G1 X%f Y%f E1;\n",-1*pset[i][0],pset[i][1]);
        }  
    }

    fclose(layer);

}

void tesselator(int RES, float layer, float zmax, float cooldown, float speed){ //Main tesselator function for 2D cross section. Written by CJ Replogle.

    //Open input/output files
    FILE *in;
    FILE *out;
    in = fopen("layer.gcode","r");
    out = fopen("output.gcode","w");
    if(in == NULL){ //error thrown if files not opening properly (check read perms/run with admin)
        printf("Could not read input.gcode. Try running this program w/ administrator or check that your root file has a 'layer.gcode' file.");
    }
    if(out == NULL){
        printf("Unable to write to output.gcode. Try running this program w/ administrator.");
    }
    
    float coord[RES*2][3];
    float tcoord[RES*2][3];
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

    printf("\nLAYERING\n--------\n");

    printf("\nHOW MANY PRINTHEADS: ");
    float phead;
    scanf("\n%f",&phead);
    printf("\nHOW MANY SECTIONS: ");
    int sect=1;
    scanf("\n%d",&sect);

    printf("\nWHAT ANGLE SHOULD THESE COORDINATES BE ROTATED: (DEGREES)\n\n");
    float angle[sect];
    for(int i=0; i<sect; i++){ //the for loops here just memory manage in the case specific setups are used in bilayer scaffolds
        printf("  SECTION %d: ",i+1);
        scanf("%f",&angle[i]);
        angle[i]=3.14159*angle[i]/180;
    }

    printf("\nFLIP X: (1=YES, 0=NO)\n\n");
    int fx[sect];
    for(int i=0; i<sect; i++){
        printf("  SECTION %d: ",i+1);
        scanf("\n%i",&fx[i]);
    }

    printf("\nFLIP Y: (1=YES, 0=NO)\n\n");
    int fy[sect];
    for(int i=0; i<sect; i++){
        printf("  SECTION %d: ",i+1);
        scanf("\n%i",&fy[i]);
    }

    printf("\nRECURSIVE TRANSFORM: (1=YES, 0=NO)\n\n");
    int rt[sect];
    for(int i=0; i<sect; i++){
        printf("  SECTION %d: ",i+1);
        scanf("\n%i",&rt[i]); 
    } 

    //Prompt switchback layering
    printf("\nSWITCHBACK LAYERING: (1=YES, 0=NO)\n\n");
    int switchback[sect];
    for(int i=0; i<sect; i++){
        printf("  SECTION %d: ",i+1);
        scanf("\n%i",&switchback[i]);
    }
    /*
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
    speed*=60; */

    /*int switchback[sect];
    for(int i=0;i<sect;i++){
        switchback[i]=0;
    }*/

    printf("\nDEBUG OUTPUT: \n\n");


    //Calculate bi/trilayer switch points
    float sw=zmax/phead;
    int head=0;

    if(fx[0]==1){ //initial transformation sequence (used if layer transformation is non recursive)
        for(int i=0; i<cout;i++){
            tcoord[i][0]=-1*coord[i][0];
        }
    } else {
        for(int i=0; i<cout;i++){
            tcoord[i][0]=coord[i][0];
        }
    }
    if(fy[0]==1){
        for(int i=0; i<cout;i++){
            tcoord[i][1]=-1*coord[i][1];
        }
    } else {
        for(int i=0; i<cout;i++){
            tcoord[i][1]=coord[i][1];
        }
    }
    for(int i=0; i<cout;i++){
        float transx = tcoord[i][0]*cos(angle[0])+tcoord[i][1]*sin(angle[0]);
        float transy = -1*tcoord[i][0]*sin(angle[0])+tcoord[i][1]*cos(angle[0]);
        tcoord[i][0] = transx;
        tcoord[i][1] = transy;
    }

    //Output to .gcode/console
    fprintf(out,"M107 ;\nG21 ;\nG90 ;\nM83 ;\n"); //initiation gcode statement for biox cellink printer
    if(phead>1){
        printf("T0 ;\n"); //select printhead 1 if applicable
    }
    fprintf(out,"G1 Z%.3f;\n",layer); //move to starting z coordinate
    fprintf(out,"G1 X0 Y0 F60;\n"); //move to starting xy coordinate
    fprintf(out,"G4 P1000;\n"); //give printer a sec for extrusion to catch up (thanks cellink)
    int layercout=1;
    float sectiontrack=0;
    float headtrack=0;
    float previoushead=0;

    for(float z=layer; z<=zmax; z+=layer){ //layer generator/transformer
        sectiontrack=sect*z/zmax;
        previoushead=headtrack;
        headtrack=head*z/zmax;
        if((int)headtrack > (int)previoushead){ //printhead selection
            fprintf(out,"T%d;\n",(int)headtrack);
        }
        if(z!=layer){ //z axis movement
            fprintf(out,"G4 P%f;\n",cooldown);
            fprintf(out,"G1 Z%.3f;\n",z);
        }
        layercout++;
        if(rt[(int)sectiontrack]!=1){ // odd layer non-recursive case
            if(layercout%2==1){
                fprintf(out,"G1 X%.3f Y%.3f ;\n",coord[0][0],coord[0][1]);     
                for(int i=1; i<cout;i++){
                    fprintf(out,"G1 X%.3f Y%.3f E1;\n",coord[i][0],coord[i][1]);
                }
                if(switchback[(int)sectiontrack]==1 && (z+layer<=zmax) && (layercout>2)){
                    printf("G4 P%f;\n",cooldown);
                    z+=layer;
                    fprintf(out,"G1 Z%.3f F%.3f;\n",z,speed);
                    for(int i=cout-1; i>=0;i--){
                        fprintf(out,"G1 X%.3f Y%.3f E1;\n",coord[i][0],coord[i][1]);
                    }
                }
            } else { //even layer non-recursive case
                fprintf(out,"G1 X%.3f Y%.3f ;\n",tcoord[0][0],tcoord[0][1]);
                for(int i=1; i<cout;i++){
                    fprintf(out,"G1 X%.3f Y%.3f E1;\n",tcoord[i][0],tcoord[i][1]);
                }
                if(switchback[(int)sectiontrack]==1 && (z+layer<=zmax) && (layercout>2)){ //switchback case
                    z+=layer;
                    fprintf(out,"G1 Z%.3f F%.3f;\n",z,speed);
                    for(int i=cout-1; i>=0;i--){
                        fprintf(out,"G1 X%.3f Y%.3f E1;\n",tcoord[i][0],tcoord[i][1]); //you reached the deepest part in the code congrats
                    }
                }
            }
        } else { //recursive transformation case
            if(fx[(int)sectiontrack]==1){ //check x layer swap
                for(int i=0; i<cout;i++){
                    tcoord[i][0]=pow(-1,layercout)*coord[i][0];
                }
            } else {
                for(int i=0; i<cout;i++){
                    tcoord[i][0]=coord[i][0];
                }
            }
            if(fy[(int)sectiontrack]==1){ //check y layer swap
                for(int i=0; i<cout;i++){
                    tcoord[i][1]=pow(-1,layercout)*coord[i][1];
                }
            } else {
                for(int i=0; i<cout;i++){
                    tcoord[i][1]=coord[i][1];
                }
            }
            for(int i=0; i<cout;i++){ //euler rotation matrix transformation (i am a better mathematician than euler)
                float transx = tcoord[i][0]*cos(layercout*angle[(int)sectiontrack])+tcoord[i][1]*sin(layercout*angle[(int)sectiontrack]);
                float transy = -1*tcoord[i][0]*sin(layercout*angle[(int)sectiontrack])+tcoord[i][1]*cos(layercout*angle[(int)sectiontrack]);
                tcoord[i][0] = transx;
                tcoord[i][1] = transy;
            }
            fprintf(out,"G1 X%.3f Y%.3f;\n",tcoord[0][0],tcoord[0][1]);
            for(int i=1; i<cout-1;i++){ //print result :3
                fprintf(out,"G1 X%.3f Y%.3f E1;\n",tcoord[i][0],tcoord[i][1]);
            }
            fprintf(out,"G1 X%.3f Y%.3f E1;\n",tcoord[cout-1][0],tcoord[cout-1][1]);
            fprintf(out,"G1 X%.3f Y%.3f;\n",tcoord[cout-1][0],tcoord[cout-1][1]);
            if(switchback[(int)sectiontrack]==1 && (z+layer<=zmax) && (layercout>2)){ //switchback layering case
                fprintf(out,"G4 P%f;\n",cooldown);
                z+=layer;
                fprintf(out,"G1 Z%.3f F%.3f;\n",z,speed);
                for(int i=cout-1; i>=0;i--){
                    fprintf(out,"G1 X%.3f Y%.3f E1;\n",tcoord[i][0],tcoord[i][1]);
                }
                fprintf(out,"G1 X%.3f Y%.3f;\n",tcoord[0][0],tcoord[0][1]);
            }
        }
    }
    fprintf(out,"M107 ;\nG0 \nM84 ;\n"); //printer end statement (dont touch unless you know what you're doing)

    fclose(in);
    fclose(out);

}