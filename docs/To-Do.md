# Cosmos64 - TODO...

## costrans (Transform Modules for cosams)

## coscam (Camera Modules for cosams)

## coslite (Lighting Modules for cosams)

## cosmesh (Mesh Modules for cosams)
- Write functions to quick load/unload animations stored in mesh
- Write code to free skeletons and animations when mesh is killed
- Figure out prim color and blend mode

## coscut (Cutscene System)
- Store list of included skeletons that will be animated based on each animation
- Store an animation per skeleton, to use to animate them
- Store list of excluded modules that will not be updated/drawn while cutscene is playing

## coshit (Collision Modules for cosams)
- Implement AABB collision boxes as Modules for cosams

## cosams (Actor-Module System)
- Replace hash table usage with plain arrays, identify stuff by numeric id probably (enums would make it even easier)
    - ID for type of module, and unique ID for each module in an actor?
- Stage list tool (blender?)
    - Should give us transforms to start loaded trans3dmodules with
    - Colors/light info too?
- Stage display list, to draw in order (sync pipe after each?)
- Add ambient light to stage, set when drawing stage

## cospad (Input system)
- Just a list of functions
- Any code can access these functions to check the values of various inputs on pad

## cosdebug (Debug tools)