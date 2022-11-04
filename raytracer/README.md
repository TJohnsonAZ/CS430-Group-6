# CS430-Group-6
Repository for group 6's CS430 projects.

# Authors
Trevor Johnson - tkj44@nau.edu  
Noah Nannen - ncn23@nau.edu  
Johnathan Ray - jdr479@nau.edu  
  
# Usage
To create the object file that runs the program, type "make all" into your command line and it will create "raycast". To execute it, write "./raycast" followed by three arguments: width, height, input file (.csv), and output file (.ppm). To reset, type "make clean" into your command line and it will remove any object and miscellaneous files.
  
# Known Issues
- The program is capabable of outputting ppm images that display spheres and planes, but is incapable of utilizing the lights in an input file to properly illuminate the scene.
- Valgrind produces several "Invalid read/write of size 1" errors.
