# ZEngine
A real-time script parser using the Irrlicht API.
The purpose of this project is to allow users to create complex programs (e.g. games) by writing a simple script. My goal is for the project to be as light-weight and fast as possible, while still allowing users to create aesthetically pleasing programs.

The binary file is located in the bin/Debug folder, and should be able to run fine on any Windows machine. Any files required at runtime are also in that directory, so when copying the program to another directory it's a good idea to copy those as well. In particular, the program is dependent on Irrlicht.dll and will not run if both are not in the same directory.

To compile this program, download the Irrlicht API.
Documentation for linking and compiling Irrlicht projects can be found on the [Irrlicht website](irrlicht.sourceforge.net "irrlicht.sourceforge.net").

---

### To do:
+ **Script**
 - [ ] user-defined script functions
 - [ ] make command list more extensible
+ **Engine**
 - [ ] load settings from file at startup (if available)
 - [ ] save settings to file on exit
 - [x] make text less blurry
 - [ ] populate GUI settings window
 - [ ] allow user to change settings (not just bells & whistles)
+ **Documentation**
 - [ ] add documentation for math expressions
 - [ ] add documentation for functions
 - [ ] add documentation for commands and syntax
+ **Miscellaneous**
 - [ ] compile for Linux

---

### Bugs
+ UI font doesn't load on certain Windows machines