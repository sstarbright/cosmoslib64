#include "coslite.h"

void dirlite3dm_create(Stage* stage, DirLite3DM* module) {
    if (stage->light_count < 7) {
        render3dm_create((Render3DM*)module);
        ((Trans3DM*)module)->matup = dirlite3dm_matup;
        trans3dm_update_matrix(((Trans3DM*)module));
        ((Render3DM*)module)->draw = dirlite3dm_draw;
        ((Module*)module)->death = dirlite3dm_death;
        stage->light_count += 1;
        if (stage->light) {
            linked_add_to_list(stage->light->prev, stage->light, module, offsetof(Render3DM, prev), offsetof(Render3DM, next));
        } else {
            stage->light = (Render3DM*)module;
        }
    } else {
        free(module);
        module = NULL;
    }
}
void dirlite3dm_matup(Trans3DM* self, const T3DMat4* ref_mat) {
    trans3dm_matup(self, ref_mat);
    T3DMat4* global_mat = self->matrix;
    ((DirLite3DM*)self)->direction = (T3DVec3){{global_mat->m[2][0], global_mat->m[2][1], global_mat->m[2][2]}};
}
void dirlite3dm_draw(Render3DM* module, float delta, uint32_t frame_buffer) {
    uint32_t* current_light = &((Module*)module)->actor->stage->current_light;
    t3d_light_set_directional(*current_light, &module->color.r, &((DirLite3DM*)module)->direction);
    *current_light += 1;
}
void dirlite3dm_death(Module* self) {
    trans3dm_simple_death((Trans3DM*)self);

    linked_pop_from_list(self, offsetof(Render3DM, prev), offsetof(Render3DM, next));

    free(self);
}

void pntlite3dm_create(Stage* stage, PntLite3DM* module) {
    if (stage->light_count < 7) {
        render3dm_create((Render3DM*)module);
        ((Trans3DM*)module)->matup = pntlite3dm_matup;
        trans3dm_update_matrix(((Trans3DM*)module));
        ((Render3DM*)module)->draw = pntlite3dm_draw;
        ((Module*)module)->death = pntlite3dm_death;
        module->size = 0.f;
        stage->light_count += 1;
        if (stage->light) {
            linked_add_to_list(stage->light->prev, stage->light, module, offsetof(Render3DM, prev), offsetof(Render3DM, next));
        } else {
            stage->light = (Render3DM*)module;
        }
    } else {
        free(module);
        module = NULL;
    }
}
void pntlite3dm_matup(Trans3DM* self, const T3DMat4* ref_mat) {
    trans3dm_matup(self, ref_mat);
    T3DMat4* global_mat = self->matrix;
    ((PntLite3DM*)self)->position = (T3DVec3){{global_mat->m[3][0], global_mat->m[3][1], global_mat->m[3][2]}};
}
void pntlite3dm_draw(Render3DM* module, float delta, uint32_t frame_buffer) {
    uint32_t* current_light = &((Module*)module)->actor->stage->current_light;
    t3d_light_set_point(*current_light, &module->color.r, &((PntLite3DM*)module)->position, ((PntLite3DM*)module)->size, false);
    *current_light += 1;
}
void pntlite3dm_death(Module* self) {
    trans3dm_simple_death((Trans3DM*)self);

    linked_pop_from_list(self, offsetof(Render3DM, prev), offsetof(Render3DM, next));

    free(self);
}