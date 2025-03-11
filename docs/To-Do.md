# Cosmos64 - TODO...

## costrans (Transform Modules for cosams)
- Implement matup (matrix update) function pointer (Trans3DModule* self, float delta_time, T3DMat4* matrix);

## coscam (Camera Modules for cosams)
- Use Trans3DModule->matup to call code to invert the received matrix

## coslites (Lighting Modules for cosams)
- Implement Lights as Module for cosams
- Directional Lights
    - Use Trans3DModule->matup to call code to copy the first three entries of column 2 of the matrix.
    - Use Render3DModule->draw to call code to set the Directional Light.
- Point Lights
    - Use Trans3DModule->position to determine positioning.
    - Use Render3DModule->draw to call code to set the Point Light.
- Ambient Lights
    - Create RenderModule, like Render3DModule but based on module instead of Trans3DModule
    - Use RenderModule->draw to call code to set the Ambient Light.

## cosmesh (Mesh Modules for cosams)
- Implement Skinned Mesh for cosams
- Primitive Color Module to alter color of model before rendering it

## coshit (Collision Modules for cosams)

## cosams (Actor-Module System)
- Consider a way to optimize hash tables
    - Could involve calculating hashes based on a seed, then optimizing for specific seeds for each Actor's modules and tags
    - Reroll seed until zero collisions are detected
- Fix hashed module querying

## cosdebug (Debug tools)