#include "coslite.h"

// Max point/dir lights is 7
uint32_t light_count = 0;
uint32_t current_light = 0;

color_t ambient_cache;
CachedLight light_cache[7];

void coslite_set_ambient(color_t color) {
    ambient_cache = color;
    t3d_light_set_ambient((const uint8_t*)&color);
}
uint32_t coslite_get_count() {
    return light_count;
}
void coslite_new_frame() {
    current_light = 0;
    ambient_cache = RGBA32(0xFF, 0xFF, 0xFF, 0xFF);
}
void coslite_renew() {
    for (int i = 0; i < current_light; i++) {
        t3d_light_set_count(light_count);
        t3d_light_set_ambient((const uint8_t*)&ambient_cache);
        CachedLight* light = &light_cache[i];
        if (light->is_point) {
            t3d_light_set_point(light->index, light->color, light->vector, light->size, light->ignoreNormals);
        } else {
            t3d_light_set_directional(light->index, light->color, light->vector);
        }
    }
}

void coslite_add_directional(int index, uint8_t *color, T3DVec3 *dir) {
    light_cache[index] = (CachedLight){
        index,
        false,
        color,
        dir,
        0.f,
        false
    };
    t3d_light_set_directional(index, color, dir);
}
void coslite_add_point(int index, uint8_t *color, T3DVec3 *pos, float size, bool ignoreNormals) {
    light_cache[index] = (CachedLight){
        index,
        true,
        color,
        pos,
        size,
        ignoreNormals
    };
    t3d_light_set_point(index, color, pos, size, ignoreNormals);
}

void dirlite3dm_create(DirLite3DM* module) {
    if (light_count < 7) {
        render3dm_create((Render3DM*)module);
        ((Trans3DM*)module)->matup = dirlite3dm_matup;
        trans3dm_update_matrix(((Trans3DM*)module));
        ((Render3DM*)module)->draw = dirlite3dm_draw;
        ((Module*)module)->death = dirlite3dm_death;
        light_count += 1;
    } else {
        free(module);
        module = NULL;
    }
}
void dirlite3dm_matup(Trans3DM* self, const T3DMat4* global_mat) {
    ((DirLite3DM*)self)->direction = (T3DVec3){{global_mat->m[2][0], global_mat->m[2][1], global_mat->m[2][2]}};
}
void dirlite3dm_draw(Render3DM* module, float delta, uint32_t frame_buffer) {
    coslite_add_directional(current_light, &module->color.r, &((DirLite3DM*)module)->direction);
    current_light += 1;
}
void dirlite3dm_death(Module* self) {
    trans3dm_simple_death((Trans3DM*)self);

    free((DirLite3DM*)self);
}

void pntlite3dm_create(PntLite3DM* module) {
    if (light_count < 7) {
        render3dm_create((Render3DM*)module);
        ((Trans3DM*)module)->matup = pntlite3dm_matup;
        trans3dm_update_matrix(((Trans3DM*)module));
        ((Render3DM*)module)->draw = pntlite3dm_draw;
        ((Module*)module)->death = pntlite3dm_death;
        module->size = 0.f;
        light_count += 1;
    } else {
        free(module);
        module = NULL;
    }
}
void pntlite3dm_matup(Trans3DM* self, const T3DMat4* global_mat) {
    ((PntLite3DM*)self)->position = (T3DVec3){{global_mat->m[3][0], global_mat->m[3][1], global_mat->m[3][2]}};
}
void pntlite3dm_draw(Render3DM* module, float delta, uint32_t frame_buffer) {
    coslite_add_point(current_light, &module->color.r, &((PntLite3DM*)module)->position, ((PntLite3DM*)module)->size, false);
    current_light += 1;
}
void pntlite3dm_death(Module* self) {
    trans3dm_simple_death((Trans3DM*)self);

    free((PntLite3DM*)self);
}