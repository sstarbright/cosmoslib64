# Cosmos64 - TODO...

## costrans (Transform Modules for cosams)

## coscam (Camera Modules for cosams)

## coslite (Lighting Modules for cosams)

## cosmesh (Mesh Modules for cosams)

## coscut (Cutscene System)
- Store list of included skeletons that will be animated based on each animation
- Store an animation per skeleton, to use to animate them
- Store list of excluded modules that will not be updated/drawn while cutscene is playing

## coshit (Collision Modules for cosams)
- Implement AABB collision boxes as Modules for cosams

## cosams (Actor-Module System)
- ID for type of module, and unique ID for each module in an actor?
- Stage list tool (blender?)
    - Should give us transforms to start loaded trans3dmodules with
    - Colors/light info too?
- Stage display list, to draw in order (sync pipe after each?)
- Add ambient light to stage, set when drawing stage
- Some sort of signal system, signals have a simple call function and store a list of function pointers to call when the signal is called

## cosjoy (Input system)
- Just a list of functions
- Any code can access these functions to check the values of various inputs on pad

## cosdebug (Debug tools)