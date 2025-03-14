# Cosmos64 - TODO...

## costrans (Transform Modules for cosams)

## coscam (Camera Modules for cosams)

## coslite (Lighting Modules for cosams)
- Ambient Lights
    - Figure out how I wanna do ambient lights

## cosmesh (Mesh Modules for cosams)
- Store skeletons and animations with hash
- Write functions to quick load/unload animations stored in mesh
- Write code to free skeletons and animations when mesh is killed
- Code to alter prim color of each material before rendering it

## coshit (Collision Modules for cosams)
- Implement AABB collision boxes as Modules for cosams

## cosams (Actor-Module System)
- Consider a way to optimize hash tables
    - Could involve calculating hashes based on a seed, then optimizing for specific seeds for each Actor's modules and tags
    - Reroll seed until zero collisions are detected
- Fix hashed module querying

## cosdebug (Debug tools)