#include "coscam.h"
#include <libdragon.h>

void camera3d_module_create(Camera3DModule* module, const char* name) {
    render3d_module_create((Render3DModule*)module, name);

    ((Module*)module)->birth = camera3d_module_birth;
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
void camera3d_module_draw(Render3DModule* module, float delta, uint32_t frame_buffer) {
    t3d_viewport_set_view_matrix(&((Camera3DModule*)module)->viewport, &module->transform.matrixf);
}
void camera3d_module_death(Module* self) {

}