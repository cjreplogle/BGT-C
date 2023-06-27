#include <stdio.h>
#include <stdlib.h>
#include "infill.c"
#include "tesselator.c"

int main(void){

        int state=0;
        while(state==0){
                printf("\e[1;1H\e[2J");
                printf("/------------------------------------------------------\\\n");
                printf("|--------------- Bioprinting GCode Tool ---------------|\n");
                printf("|---------- v0.1 made by CJ Replogle c. 2023 ----------|\n");
                printf("|------------------------------------------------------|\n");
                printf("|                                                      |\n");
                printf("| 1: Generate New Scaffold                             |\n");
                printf("| 2: Use Preset Infill Pattern                         |\n");
                printf("| 3: Print Variable Test (Not Working)                 |\n");
                printf("| 4: Help                                              |\n");
                printf("| 5: About this Program                                |\n");
                printf("| 6: Exit                                              |\n");
                printf("|                                                      |\n");
                printf("\\-----------------------------------------------------/\n\n-> ");
                scanf("%i",&state);

                switch (state) {
                        case 1:
                                infill();
                                tesselator();
                                printf("\e[1;1H\e[2J");
                                printf("/------------------------------------------------------\\\n");
                                printf("| Scaffold successfully generated!                     |\n");
                                printf("\\------------------------------------------------------/\n\n");
                                printf("Press enter to return to the menu -> ");
                                scanf("%d",&state);
                                printf("\e[1;1H\e[2J");
                                state=0;
                                break;
                        case 2: 
                                tesselator();
                                printf("\e[1;1H\e[2J");
                                printf("/------------------------------------------------------\\\n");
                                printf("| Scaffold successfully generated!                     |\n");
                                printf("\\------------------------------------------------------/\n\n");
                                printf("Press enter to return to the menu -> ");
                                scanf("%d",&state);
                                printf("\e[1;1H\e[2J");
                                state=0;
                                break;
                        case 3: 
                                state=0;
                                printf("\e[1;1H\e[2J");
                                break;
                        case 4:
                                printf("\e[1;1H\e[2J");
                                printf("/------------------------------------------------------\\\n");
                                printf("| Haven't wrote this out yet sorry. Good luck.         |\n");
                                printf("\\------------------------------------------------------/\n\n");
                                printf("Press enter to return to the menu -> ");
                                scanf("%d",&state);
                                printf("\e[1;1H\e[2J");
                                state=0;
                                break;
                        case 5:
                                printf("\e[1;1H\e[2J");
                                printf("/------------------------------------------------------\\\n");
                                printf("\nHello, and welcome to BGT (Bioprinting GCode Tool). This software was developed by CJ Replogle (a student-researcher) at Cincinnati Children's as a method of generating gcode files for the Whitlock lab for use in bioprinting. ");
                                printf("Existing software did not have good control over different parameters involving printhead movement and the built-in ability. This software in particular is designed for use with the Cellink BioX 3D printer, although created gcodes might work fine with others.");
                                printf("to rapidly create gcodes to test things like scaffold porosity and infill structure so I decided to make a program to do it myself.\n\n");
                                printf("Some things to consider when using this. Firstly, I am a biomedical engineer, not a programmer. This software is developed in-house and probably (definitely) is not perfect. It should get the job done for testing purposes and potentially small scale clinical applications. ");
                                printf("I probably have not integrated a .stl slice method so this software works a bit different than most 'slicers' in that it generates a preset shape and allows you to mess with the specific printing parameters behind the scenes.");
                                printf("This has its drawbacks with respect to model customization but as this is primarily designed for printing predesigned implant sizes for surgical use, this is ");
                                printf("less relevant for the time being. I might go about making a more general-use .stl processor using similar curve math ");
                                printf("used here eventually if I end up pursing another idea I have in mind. (Top secret!!!)\n\n");
                                printf("As for how this software works, it can be thought of as generating the 3D curve in two separate phases. A repeated pattern is first ");
                                printf("generated automatically within a parametrized shape along a 2D plane. Then, in consideration of the start/end points of the 2D plane, the program");
                                printf("generates levels vertically beginning where the previous level curve ends. This allows the printer to extrude an entire ");
                                printf("shape using a singular path rather than the layer based approach currently used. No fancy CAD/3D modeling skills needed.\n\n");
                                printf("Why? Inflow of mensenchymal stem cells and other growth factors into the scaffolds this software is designed for is critical to fostering ");
                                printf("a regenerative environment for osteochondral tissue. That being said, the singular connected curves maintain superior tensile properties ");
                                printf("to lattice-based counterparts due to microstructural limitations of thermal cross-linking of the bioink.\n\n");
                                printf("With that word salad out of the way, if there are any issues encountered with this software, reach out to me at ");
                                printf("replogle.17@buckeyemail.osu.edu (should work until ~2026) and tell me my code is bad. (I already know)\n\n");
                                printf("\\-----------------------------------------------------/\n\n");
                                printf("Press enter to return to the menu -> ");
                                scanf("%d",&state);
                                printf("\e[1;1H\e[2J");
                                state=0;
                                break;
                        case 6:
                                state=1;
                                break;
                }
        }
        exit(0);
}
