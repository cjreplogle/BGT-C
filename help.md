# BGT Documentation

[About](#about)

[Infill Parameters](#infill-parameters)

[Transformation Parameters](#transformation-parameters)

[Printer Configuration](#printer-configuration)

[File Naming](#file-naming)

## About 

Hello, and welcome to WLSGT (Whitlock Lab Scaffold GCode Tool). This software was developed by CJ Replogle (a student-researcher) at Cincinnati Children's as a method of generating gcode files for the Whitlock lab for use in bioprinting. Existing software did not have good control over different parameters involving printhead movement and the built-in ability. This software in particular is designed for use with the Cellink BioX 3D printer, although created gcodes might work fine with others.to rapidly create gcodes to test things like scaffold porosity and infill structure so I decided to make a program to do it myself.

Some things to consider when using this. Firstly, I am a biomedical engineer, not a programmer. This software is developed in-house and probably (definitely) is not perfect. It should get the job done for testing purposes and potentially small scale clinical applications. I probably have not integrated a .stl slice method so this software works a bit different than most 'slicers' in that it generates a preset shape and allows you to mess with the specific printing parameters behind the scenes.This has its drawbacks with respect to model customization but as this is primarily designed for printing predesigned implant sizes for surgical use, this is less relevant for the time being. I might go about making a more general-use .stl processor using similar curve math used here eventually if I end up pursing another idea I have in mind. (Top secret!!!)

As for how this software works, it can be thought of as generating the 3D curve in two separate phases. A repeated pattern is first generated automatically within a parametrized shape along a 2D plane. Then, in consideration of the start/end points of the 2D plane, the programgenerates levels vertically beginning where the previous level curve ends. This allows the printer to extrude an entire shape using a singular path rather than the layer based approach currently used. No fancy CAD/3D modeling skills needed.

Why? Inflow of mensenchymal stem cells and other growth factors into the scaffolds this software is designed for is critical to fostering a regenerative environment for osteochondral tissue. That being said, the singular connected curves maintain superior tensile properties to lattice-based counterparts due to microstructural limitations of thermal cross-linking of the bioink.

With that word salad out of the way, if there are any issues encountered with this software, reach out to me at replogle.17@buckeyemail.osu.edu (should work until ~2026) and I'll try to help. [^](#bgt-documentation)

## Infill Parameters

### Infill Type
* **Rectilinear Cylinder** -> Generates a 2D circle layer with a rectilinear infill pattern. 

* **Rectilinear Square** -> Generates a 2D square layer with a rectilinear infill pattern.

* **Concentric Cylinder** -> Generates a concentric lattice 2D infill pattern. 

### Radius
* A radius for a circlar infill should be passed to the program in millimeters. 

### Pore Size
* The distance between the central point of the printhead between horizontal segments in the layer patterning for rectilinear infills. (Note: larger printheads will need larger pore sizes to compensate!)
Recommended Settings: 0.6mm nozzle -> 1.5-2mm pore size, 0.4mm nozzle -> 1.0->1.5mm pore size

### Path Linkage
* For cylindrical rectilinear patterns, generates a segment at each end of the 2D curve to close the circular structure.

### Pore Depth
* This is particular to the outer layer of the concentric cylinder infill. It just determines how far inwards the outer pattern's inner grid will go.

### Pores per Layer
* Also particular to the concentric cylinder infill layers. This just determines how many radial protrusions there are in a single 2D cross section. (Rather than a set distance for rectilinear infills)

### Fill (Concentric Infill Only)
* Determines whether to repeat the outer pattern on the inside of the structure, use a rectilinear infill (not yet), or keep the scaffold hollow. [^](#bgt-documentation)

## Transformation Parameters

### Printhead Count
* This implements multiple printheads in the construction of the scaffold. The scaffold is automatically divided in halves/thirds depending on the number of printheads indicated. Printheads are used left to right unless 1 is used, in which case whatever printhead is considered default is used.

### Section Count
* This divides the scaffold into multiple sections with different patterning between sections. Other transformation parameters below may be modified betwen layers (may implement different infill types in this as well if it is determined to be useful).

### Linkage Angle
* Determines the angle in degrees which the initial infill layer is transformed upon moving up a layer in the print. This produces unique results depending on the quotient of the passed angle and 180.
90/270 degree linkage forms a square pattern, 60/120/240/300 degree linkage forms a triangular pattern, 72 creates a pentagonal pattern but this is harder to see unless a small nozzle diameter is used. 
Linkage angle may be combined with other layer transformations to allow for the printer to move along a consistent path for the entire print, but this being possible depends on the modular arithmatic of the angular transformation.
(A simple single-path print configuration is rectilinear/90 degrees/flip x). Multi-section scaffolds may implement differnet linkage angles by region.

### Flip X/Y
* Flips the existing layer along the x-coordinate or y-coordinate each layer. Primarily useful for rectilinear-based prints. May implement a single path transformation for a 60 degree linkage that would need to alternate between x/y flips. 

### Recursive Transform
* Enables transformation by-layer rather than alternating between an original preset and a transformed second layer. Useful for trianglar linkages where the structure depends on infills at more than 2 angles.

### Switchback Layering
* This prints patterns in pairs of layers. The printer will take the same path it took to generate a layer backwards to the starting point (Doing this additional times has not been needed for our purposes but if it eventually is I can implement it fairly easily.) [^](#bgt-documentation)

## Printer Configuration

### Layer Height
* This is the z-coordinate separation between layers. I recommend a few hundreths of a mm smaller than the actual nozzle diameter for ideal results. (0.6mm->0.55mm,0.4mm->0.37)

### ZMax
* This is the stopping z-coordinate for the vertical transformation algorithm. Whatever you set this to is equivalent to the scaffold's approximate height (will add an amount less than the nozzle diameter due to each layer having a certain thickness).

### Cooldown
* The amount of time in milliseconds the printer stops before moving to the next layer. This can be modified to allow layers to properly harden before material is deposited on top. (I generally use ~10000).

### Speed
* This is the original speed the printhead moves at. Due to how the printer reads .gcode files you need to use an F(value) at least once for the printer to properly extrude. The actual value the printer reads is in mm/min but I generally just recommend setting it to around 60 and tweaking it from there until print properties are good. [^](#bgt-documentation)

## File Naming

Why is the naming system like this? Files on this drive (and in general for the sake of organization) are named based off their various properties so you can briefly look at them and know their properties without needing to open and view the file. Due to limitations of the Cellink BioX printer (their slicer is poorly programmed resulting in printhead jumping and otherwise lower quality prints), most prints in this lab as of late are printed from .gcode files rather than .stl. .gcode files are useful as they deliver direct instructions to bio printer hardware rather than letting the printer do the calculations (which has proven to be unsuccessful). This provides much more freedom with printing specific microstructures which otherwise we would have no control over. 

I’ve made my own slicer program tailored towards this specific hardware that you can use (it should be on the usb drive, if its not send me an email and I can provide the file, might upload versions of it online if possible too). That being said, with this freedom of configuration, you lose the built in (albeit poor) customization options Cellink provides. This means all the customization to the codes needs to be done externally and then supplied to the printer. Therefore, this means the SD card ends up with a lot of similar .gcode files with very similar setups. The naming system acts to distinguish these and provide some insight into what means what.

Suffixes:

* ```[diameter (mm]x[diameter (mm)]x[height (mm)]cyl``` -> diameter x diameter x height mm cylinder

* ```[side length (mm)]x[side length (mm)]x[vertical side length (mm)]sq``` -> x by y by z mm cube

* ```[distance (mm/10)]d``` -> Diameter of printhead that should be used

* ```[angle (degrees)]l``` -> Linkage angle (degrees), example: 90-120l means 90 degree/120 degree bilayer linkage

* ```+m``` -> Multi-extruder

Suffixes are strung together to form a general meaning behind whatever object is being represented. 

Example:
	“6x6x6cyl6d90l+m.gcode” would represent…
		a 0.6 x 0.6 x 0.6 cm cylinder using a 0.6mm printhead linked at 90 degrees containing a 4x4 porous pattern and using multiple (2) printheads. 

When creating file names, please try to make them in this order. I might create a functionality in the slicer I made to automatically name them later to save people the time in the future (15 seconds spent typing these strings of text adds up if you are testing en masse!).

Hopefully this shows how this naming system condenses information in a consistent way. If you have any other questions, email me. (replogle.17@buckeyemail.osu.edu) and I’d be happy to help out.  [^](#bgt-documentation)

