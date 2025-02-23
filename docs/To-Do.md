# Cosmos64 - TODO...

## cosdebug (Debug tools)
- Implement debug console

## cosams (Actor-Module System)
- Test AMS
    - One Scene
    - Four Actors
        - One with no modules
        - One with one module
        - One with two modules
        - One with three modules
    - Try scene_life()
    - Try scene_kill()
- Consider a way to optimize hash tables
    - Could involve calculating hashes based on a seed, then optimizing for specific seeds for each Actor's modules and tags
    - Reroll seed until zero collisions are detected

## costrans (Transform Modules for cosams)
- Implement Transform as Module for cosams

## coslites (Lighting Modules for cosams)
- Implement Lights as Module for cosams

## cosmesh (Mesh Modules for cosams)
- Implement Tiny3D Meshes as Module for cosams