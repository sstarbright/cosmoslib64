#include "coslite.h"

// Max point/dir lights is 7
uint32_t light_count = 0;
uint32_t current_light = 0;

uint32_t coslite_get_count() {
    return light_count;
}

void coslite_new_frame() {
    current_light = 0;
}

void dirlite3d_module_create(DirLite3DModule* module, const char* name) {
    if (light_count < 7) {
        render3d_module_create((Render3DModule*)module, name);
        ((Trans3DModule*)module)->matup = dirlite3d_module_matup;
        trans3d_update_matrix(((Trans3DModule*)module));
        ((Render3DModule*)module)->draw = dirlite3d_module_draw;
        ((Module*)module)->death = dirlite3d_module_death;
        light_count += 1;
    } else {
        free(module);
        module = NULL;
    }
}
void dirlite3d_module_matup(Trans3DModule* self, const T3DMat4* global_mat) {
    ((DirLite3DModule*)self)->direction = (T3DVec3){{global_mat->m[2][0], global_mat->m[2][1], global_mat->m[2][2]}};
}
void dirlite3d_module_draw(Render3DModule* module, float delta, uint32_t frame_buffer) {
    t3d_light_set_directional(current_light, &module->color.r, &((DirLite3DModule*)module)->direction);
    current_light += 1;
}
void dirlite3d_module_death(Module* self) {
    trans3d_simple_module_death((Trans3DModule*)self);

    free((DirLite3DModule*)self);
}

void pntlite3d_module_create(PntLite3DModule* module, const char* name) {
    if (light_count < 7) {
        render3d_module_create((Render3DModule*)module, name);
        ((Trans3DModule*)module)->matup = pntlite3d_module_matup;
        trans3d_update_matrix(((Trans3DModule*)module));
        ((Render3DModule*)module)->draw = pntlite3d_module_draw;
        ((Module*)module)->death = pntlite3d_module_death;
        module->size = 0.f;
        light_count += 1;
    } else {
        free(module);
        module = NULL;
    }
}
void pntlite3d_module_matup(Trans3DModule* self, const T3DMat4* global_mat) {
    ((PntLite3DModule*)self)->position = (T3DVec3){{global_mat->m[3][0], global_mat->m[3][1], global_mat->m[3][2]}};
}
void pntlite3d_module_draw(Render3DModule* module, float delta, uint32_t frame_buffer) {
    t3d_light_set_point(current_light, &module->color.r, &((PntLite3DModule*)module)->position, ((PntLite3DModule*)module)->size, false);
    current_light += 1;
}
void pntlite3d_module_death(Module* self) {
    trans3d_simple_module_death((Trans3DModule*)self);

    free((PntLite3DModule*)self);
}