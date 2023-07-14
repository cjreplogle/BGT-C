## BGT Changelog:

### v0.1 (early June 2023)
 - Existed before this program did. 
 - Generated infill shape of a circle and autofilled it to certain pore size then stacked/rotated/flipped it to make scaffold.
 - Very buggy but used to make some of the first proof of concept scaffolds

### v0.2 (6/27/23)
 - This central program made to connect the two tools (slicer.c + infill.c) and made more user friendly.
 - Some very major bug fixes (it doesn't always segmentation fault now)

### v0.3 (7/7/23)
 - Revamped sectioning system to make customized segments of scaffold more flexible. 
 - Reworked the recursive transform function to be faster and changed the order of operations (axis flip before rotation).
 - Properly documented tesselator (ish)

### v0.4 (7/12/23)
 - Square mode

### v0.5 (7/14/23)
 - Settings implementation (so it doesn't ask every minor detail in the generation sequence)
 - QOL Changes

### Planning to add:
 - Toggle advanced mode which gives more options
 - Load from .bpc (bioprintconfig) file on start so user settings are saved
 - Save/load .bpp (bioprintparameter) file for use in printing under preset settings
