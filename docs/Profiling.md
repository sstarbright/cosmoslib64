# Start Profiling:

1. Make sure no materials in the scene are using Environment Mapping.
2. Comment any code that says `COMMENT IF PROFILING` and uncomment any code that includes `UNCOMMENT IF PROFILING`.
3. Open libdragon/include/rspq_constants.h, set RSPQ_DEBUG to 0 and RSPQ_PROFILE to 1.
4. Run the proper command for your platform.

# Stop Profiling:

1. Add back in any Environment Mapped materials that you removed.
2. Comment any code that says `UNCOMMENT IF PROFILING` and uncomment any code that includes `COMMENT IF PROFILING`.
3. Open libdragon/include/rspq_constants.h, set RSPQ_DEBUG to 1 and RSPQ_PROFILE to 0.
4. Run the proper command for your platform.

# COMMAND:

`cd libdragon ; libdragon make clean ; libdragon make ; cd ../tiny3d ; libdragon make clean ; libdragon make ; cd ../` (LINUX)
`cd libdragon ; libdragon make clean ; libdragon make ; libdragon install ; cd ../tiny3d ; libdragon make clean ; libdragon make ; cd ../` (WINDOWS DOCKER)