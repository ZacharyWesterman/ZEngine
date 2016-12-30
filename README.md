# ZEngine
A real-time script parser using the Irrlicht API. 

The binary file is located in the bin/Debug folder, and should be able to run fine on any Windows machine. Any files required at runtime are also in that directory, so when copying the program to another directory it's a good idea to copy those as well. In particular, the program is dependent on Irrlicht.dll and will not run if both are not in the same directory.

To compile this program, download the Irrlicht API.
Documentation for linking and compiling Irrlicht projects can be found on the [Irrlicht website](irrlicht.sourceforge.net "irrlicht.sourceforge.net").

--------------

### To do:
+ **script**
 - [ ] user-defined script functions
+ **engine**
 - [ ] load settings from file at startup (if available)
 - [ ] save settings to file on exit
 - [x] make text less blurry
 - [ ] populate GUI settings window
 - [ ] allow user to change settings (not just bells & whistles)

Definitely a *lot* of other stuff to be added, but I'll write it down as it occurs to me.