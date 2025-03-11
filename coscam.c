#include "coscam.h"
#include <libdragon.h>

void camera3d_module_create(Camera3DModule* module, const char* name) {
    render3d_module_create((Render3DModule*)module, name);

    ((Module*)module)->birth = camera3d_module_birth;
    ((Module*)module)->life = camera3d_module_life;
    ((Module*)module)->death = camera3d_module_death;

    ((Render3DModule*)module)->draw = camera3d_module_draw;

    module->viewport = t3d_viewport_create();
    module->fov = T3D_DEG_TO_RAD(85.0f);
    module->near = 10.f;
    module->far = 150.f;
}
void camera3d_module_birth(Module* self) {
    render3d_module_birth(self);
}
void camera3d_module_life(Module* self, float delta) {
    t3d_viewport_set_projection(&((Camera3DModule*)self)->viewport, ((Camera3DModule*)self)->fov, ((Camera3DModule*)self)->near, ((Camera3DModule*)self)->far);
    t3d_viewport_set_view_matrix(&((Camera3DModule*)self)->viewport, &((Trans3DModule*)self)->inv_matrix);
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