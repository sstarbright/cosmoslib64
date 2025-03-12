#ifndef __COSLITE_H
#define __COSLITE_H
#include <libdragon.h>
#include "cosmesh.h"

typedef struct DirLite3DModule DirLite3DModule;
typedef struct PntLite3DModule PntLite3DModule;

uint32_t coslite_get_count();
void coslite_new_frame();

struct DirLite3DModule {
    Render3DModule render;
    T3DVec3 direction;
};

// Create a DirectionalLight3D module, initializing its members.
void dirlite3d_module_create(DirLite3DModule* module, const char* name);
void dirlite3d_module_matup(Trans3DModule* self, const T3DMat4* global_mat);
// A basic function to be called upon DirLite3DModule draw.
void dirlite3d_module_draw(Render3DModule* module, float delta, uint32_t frame_buffer);
// A basic function to be called upon DirLite3DModule death.
void dirlite3d_module_death(Module* self);

struct PntLite3DModule {
    Render3DModule render;
    T3DVec3 position;
    float size;
};

// Create a PointLight3D module, initializing its members.
void pntlite3d_module_create(PntLite3DModule* module, const char* name);
void pntlite3d_module_matup(Trans3DModule* self, const T3DMat4* global_mat);
// A basic function to be called upon PntLite3DModule draw.
void pntlite3d_module_draw(Render3DModule* module, float delta, uint32_t frame_buffer);
// A basic function to be called upon PntLite3DModule death.
void pntlite3d_module_death(Module* self);

#endif