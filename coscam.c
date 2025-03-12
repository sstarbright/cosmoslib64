#include "coscam.h"
#include <libdragon.h>

void camera3d_module_create(Camera3DModule* module, const char* name) {
    render3d_module_create((Render3DModule*)module, name);

    ((Module*)module)->life = camera3d_module_life;
    ((Module*)module)->death = camera3d_module_death;

    ((Trans3DModule*)module)->matup = camera3d_module_matup;
    ((Render3DModule*)module)->draw = camera3d_module_draw;

    module->viewport = t3d_viewport_create();
    module->fov = T3D_DEG_TO_RAD(85.0f);
    module->near = 10.f;
    module->far = 150.f;
    t3d_mat4_identity(&module->matrix);
}
void camera3d_module_life(Module* self, float delta) {
    t3d_viewport_set_projection(&((Camera3DModule*)self)->viewport, ((Camera3DModule*)self)->fov, ((Camera3DModule*)self)->near, ((Camera3DModule*)self)->far);
    t3d_viewport_set_view_matrix(&((Camera3DModule*)self)->viewport, &((Camera3DModule*)self)->matrix);
}
void camera3d_module_matup(Trans3DModule* self, const T3DMat4* global_mat) {
    fm_mat4_inverse(&((Camera3DModule*)self)->matrix, global_mat);
}
void camera3d_module_draw(Render3DModule* module, float delta, uint32_t frame_buffer) {
    t3d_viewport_attach(&((Camera3DModule*)module)->viewport);
}
void camera3d_module_death(Module* self) {
    camera3d_simple_module_death((Camera3DModule*)self);
    free((Camera3DModule*)self);
}
void camera3d_simple_module_death(Camera3DModule* self) {
    trans3d_simple_module_death((Trans3DModule*)self);
}