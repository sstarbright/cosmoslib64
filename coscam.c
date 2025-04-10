#include "coscam.h"
#include <libdragon.h>

void camera3dm_create(Camera3DM* module) {
    render3dm_create((Render3DM*)module);

    ((Module*)module)->life = camera3dm_life;
    ((Module*)module)->death = camera3dm_death;

    ((Trans3DM*)module)->matup = camera3dm_matup;
    ((Render3DM*)module)->draw = camera3dm_draw;

    module->viewport = t3d_viewport_create();
    module->fov = 0.527423705125325;
    module->near = 1.f;
    module->far = 150.f;
    t3d_mat4_identity(&module->matrix);
}
void camera3dm_life(Module* self, float delta) {
    t3d_viewport_set_projection(&((Camera3DM*)self)->viewport, ((Camera3DM*)self)->fov, ((Camera3DM*)self)->near, ((Camera3DM*)self)->far);
    t3d_viewport_set_view_matrix(&((Camera3DM*)self)->viewport, &((Camera3DM*)self)->matrix);
}
void camera3dm_matup(Trans3DM* self, const T3DMat4* ref_mat) {
    trans3dm_matup(self, ref_mat);
    T3DMat4* global_mat = self->matrix;
    fm_mat4_inverse(&((Camera3DM*)self)->matrix, global_mat);
}
void camera3dm_draw(Render3DM* module, float delta, uint32_t frame_buffer) {
    t3d_viewport_attach(&((Camera3DM*)module)->viewport);
}
void camera3dm_death(Module* self) {
    camera3dm_simple_death((Camera3DM*)self);
    free(self);
}
void camera3dm_simple_death(Camera3DM* self) {
    trans3dm_simple_death((Trans3DM*)self);
}