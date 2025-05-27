# Cosmoslib64 - TODO...

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