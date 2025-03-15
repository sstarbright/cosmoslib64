# Cosmos64 - TODO...

## costrans (Transform Modules for cosams)

## coscam (Camera Modules for cosams)

## coslite (Lighting Modules for cosams)

## cosmesh (Mesh Modules for cosams)
- Store skeletons and animations with hash
- Write functions to quick load/unload animations stored in mesh
- Write code to free skeletons and animations when mesh is killed
- Code to alter prim color of each material before rendering it

## coscut (Cutscene System)
- Store list of included skeletons that will be animated based on each animation
- Store an animation per skeleton, to use to animate them (find a way to cancel currently active animation)
- Store list of excluded modules that will not be updated/drawn while cutscene is playing

## coshit (Collision Modules for cosams)
- Implement AABB collision boxes as Modules for cosams

## cosams (Actor-Module System)
- Consider a way to optimize hash tables
    - Could involve calculating hashes based on a seed, then optimizing for specific seeds for each Actor's modules and tags
    - Reroll seed until zero collisions are detected
- Fix hashed module querying
- Stage list tool (blender?)
    - Should give us transforms to start loaded trans3dmodules with
    - Colors/light info too?
- Stage display list, to draw in order (sync pipe after each?)
- Add ambient light to stage, set when drawing stage

## cospad (Input system)
- Just a list of functions
- Any code can access these functions to check the values of various inputs on pad

## cosdebug (Debug tools)