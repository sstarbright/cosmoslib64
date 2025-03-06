#include "cosmesh.h"

CachedModel** model_cache;
uint32_t model_cache_size;

void cosmesh_init() {
    model_cache = null;
    model_cache_size = 0
}

void model_cache_create(uint32_t size) {
    model_cache = malloc(sizeof(CachedModel*)*size);
    model_cache_size = size;
}

CachedModel* load_model_into_cache(const char* location, const char* name) {
    if (model_cache) {
        CachedModel* cached_model = malloc(sizeof(CachedModel));
        if (hash_add_pointer(model_cache, model_cache_size, name, cached_model)) {
            strcpy(cached_model->name, name);
            cached_model->model = t3d_model_load(location);

            return cached_model;
        }
        char debug_log_message[40];
        strcpy(debug_log_message, "Cache failure on Model '");
        strcpy(debug_log_message, name);
        strcpy(debug_log_message, "'.");
        debug_log_write(debug_log_message);
        free(cached_model);
        return NULL;
    }
    char debug_log_message[40];
    strcpy(debug_log_message, "Cache is uninitialized.");
    debug_log_write(debug_log_message);
    free(cached_model);
    return NULL;
}

void model_cache_clear() {
    free(model_cache);
}

Module* mesh3D_module_create(const char* name) {
    // Search model cache before creating module.
    Module* new_module = renderable_module_create("Mesh3D");
    new_module->birth = mesh3D_module_birth;
    new_module->death = mesh3D_module_death;

    RenderableModule* ren_module = (RenderableModule*)new_module->data;
    ren_module->draw = mesh3D_module_draw;

    Mesh3DModule* mesh_module = malloc(sizeof(Mesh3DModule));
    ren_module->data = mesh_module;
    mesh_module->block = null;

    return new_module;
}
void mesh3D_module_birth(Module* self) {
    renderable_module_birth(self);

    if (self->actor) {
        // Check for "Trans3D" indexed module. If not found, create one and attach to module->actor
        RenderableModule* ren_module = (RenderableModule*)self->data;
        Mesh3DModule* mesh_module = (Mesh3DModule*)ren_module->data;
        mesh_module->matrix_buffer = malloc_uncached(sizeof(T3DMat4FP) * display_get_num_buffers());
        
        rspq_block_begin();
            t3d_matrix_push(t3d_segment_placeholder(MESH_MAT_SEGMENT_PLACEHOLDER));
            t3d_model_draw(mesh_module->model);
        mesh_module->block = rspq_block_end();
    }
}
void mesh3D_module_draw(RenderableModule* self, float _, uint32_t frame_buffer) {
    Mesh3DModule* mesh_module = (Mesh3DModule*)self->data;
    if (mesh_module->block) {
        memcpy(&mesh_module->matrix_buffer[frame_buffer], mesh_module->transform->matrix, sizeof(T3DMat4FP));
        t3d_segment_set(MESH_MAT_SEGMENT_PLACEHOLDER, &mesh_module->matrix_buffer[frame_buffer]);
        rspq_block_run(mesh_module->block);
    }
}
void mesh3D_module_death(Module* self) {
    Mesh3DModule* mesh_module = (Mesh3DModule*)((RenderableModule*)self->data)->data;
    rspq_block_free(mesh_module->block);
    free(mesh_module->matrix_buffer);
    renderable_module_death(self);
}

Module* smesh3D_module_create() {

}
void smesh3D_module_draw(RenderableModule* self, float _, uint32_t frame_buffer) {
    t3d_matrix_set(((Mesh3DModule*)self->data)->transform, true);
}
void smesh3D_module_death(Module* self) {

}