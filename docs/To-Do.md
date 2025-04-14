# Cosmoslib64 - TODO...

## cosmesh (Mesh Rendering)
- Move Mesh Render Order data to Mesh3DM, right now it's in CachedModel but it seems more appropriate to manipulate it within the module
- Try blocks again
	- Move render block to CachedModel, since we set matrix and skeleton via segments we can just use a block
	- Maybe Mesh3DM can hold a block, and defaults to the basic block from CachedModel. Then we can make a new block if need be
	- Flag for if Mesh3DM owns it's block, if it does it should free it when killed
- Complete LagBone module

## General
- Review code as it is now, trim anything that's become deprecated and make sure there aren't any memory issues

## CC Stage Builder
- Start building the CC Stage Builder application
	- Accurate rendering of the Stage (this renderer will be repurposed for the PC port)
	- Generates C code that should be compiled into a .dso (should have a similar role for PC port, but for windows/linux/mac specific dynamic libraries)
	- Builds enums for the model cache, as well as for indexing in any part of the program
	- Each Stage, Actor, and Module will have a space sectioned off called "Func", "Pre", "Intra", and "Post"
		- Func, Pre and Post will be empty, allowing the user to write additional code other than that generated
		- Func will be added to the space before the code's "Load" function
		- Pre, Intra, and Post will be added in order within the code's "Load" function
		- Intra will be dynamically populated by the Builder as the user makes changes
		- Changes can be manually writted to Intra, but it will be completely overwritten by the builder if the user changes anything. Func, Pre, and Post are recommended

## coscut (Cutscene)
- Store list of included skeletons that will be animated based on each animation
- Store an animation per skeleton, to use to animate them
- Store list of excluded modules that will not be updated/drawn while cutscene is playing

## coshit (Collision)
- Implement 2D and 3D AABB (2D for use in battle, 3D for use in overworld)

## cosams (Actor-Module System)
- Some sort of signal system, signals have a simple call function and store a list of function pointers to call when the signal is called

## cosjoy (Input)
- Add support for multiple joypads