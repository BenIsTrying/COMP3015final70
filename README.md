# COMP3015 30% assignment

## Which version Visual Studio and Operating System you used to test/write the code locally? 

### Visual stduio version

Visual Studio 2022

### OS 

Windows 11 Pro
Version 10.0.22631 


## How does it work? 

This code works by calling on fucntions from *Glad*, *GLM*, *GLFW*, and *openGL*.
All versions relate to the sugestted version from COMP3015 overview of assignments. 

Within the current version of the program the user presses run and th ecode witll then import all neccsary exsternal files and load a model held within the meadia folder, this then is displayed. Once the modle is loaded in the vertex shader then creates two functions actign as the lights with attributes like position, and Material this is how the shader assigns values on how reflective or dark an object should be.
after these two are made the program then calculates how this should affect the modle and applys this to the scene.

## How does your code fit together and how should a programmer navigate it (not intended to be an exercise in formally documenting the code)? 

The program consists of mutiple pages with many uses, there are:

#### Shaders
These files contain information to do with the vertec and fragment shaders, these play a role in how the objs or verticies are shaded and processed.
This is where anythng like lighting effects are done.

#### Header Files
The header files work as a page dedicated to one task which usualy involes creating and storing classes.

#### CPP Files
The CPP files are where many of the main processes happen, this is where the main structure of the program is where files like the *.H* files are called and used.


## Anything else which will help us to understand how your prototype works. 

The vertex shader works by using an abojects vertices to calculate how shaded or bright it needs to be, this means simple shapes that are flat usualy dont show the shadign as much as round shape would (refering to mostly phong and britness as flat during testing the flat model i used didnt have enough faces to even be brighted more than the rest of the object, this is now why the current modle is a round ball shaped character.)

## youtube video
https://youtu.be/CWZNnsWrXJM




## A Link to the unlisted YouTube Video 

https://youtu.be/o42DM04uFW8

