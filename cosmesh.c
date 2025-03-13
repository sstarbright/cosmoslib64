#include "cosmesh.h"

CachedModel** model_cache;
uint32_t model_cache_size;

void cosmesh_init() {
    model_cache = NULL;
    model_cache_size = 0;
}

void model_cache_create(uint32_t size) {
    model_cache = malloc(sizeof(CachedModel*)*size);
    model_cache_size = size;
    memset(model_cache, 0, sizeof(CachedModel*)*size);
}

CachedModel* load_model_into_cache(const char* location, const char* name, uint32_t skeleton_count, uint32_t animation_count) {
    if (model_cache) {
        CachedModel* cached_model = malloc(sizeof(CachedModel));
        if (hash_add_pointer((void**)model_cache, model_cache_size, name, cached_model)) {
            strcpy(cached_model->name, name);
            cached_model->model = t3d_model_load(location);
            if (skeleton_count > 0)
                cached_model->skeletons = malloc(sizeof(CachedSkeleton) * skeleton_count);
            else
                cached_model->skeletons = NULL;
            if (animation_count > 0)
                cached_model->animations = malloc(sizeof(CachedAnimation) * animation_count);
            else
                cached_model->animations = NULL;

            return cached_model;
        }
        char debug_log_message[40];
        strcpy(debug_log_message, "Cache failure on Model '");
        strcpy(debug_log_message, name);
        strcpy(debug_log_message, "'.\n");
        debugf(debug_log_message);
        free(cached_model);
        return NULL;
    }
    char debug_log_message[40];
    strcpy(debug_log_message, "Cache is uninitialized.\n");
    debugf(debug_log_message);
    return NULL;
}

void model_cache_cleanup() {
    for (int i=0; i<model_cache_size; i++) {
        if (model_cache[i]->uses < 1) {
            t3d_model_free(model_cache[i]->model);
            model_cache[i]->model = NULL;
        }
    }
}

void model_cache_clear() {
    for (int i=0; i<model_cache_size; i++) {
        t3d_model_free(model_cache[i]->model);
    }
    free(model_cache);
}

void render3d_module_create(Render3DModule* module, const char* name) {
    trans3d_module_create((Trans3DModule*)module, name);
    ((Module*)module)->death = render3d_module_death;

    module->color = (color_t){0xFF, 0xFF, 0xFF, 0xFF};
    module->draw = render3d_module_draw;
}
void render3d_module_draw(Render3DModule* _, float __, uint32_t ___) {
    
}
void render3d_module_death(Module* self) {
    trans3d_simple_module_death((Trans3DModule*)self);
    free((Render3DModule*)self);
}

void mesh3d_module_create(Mesh3DModule* module, const char* name) {
    if (model_cache) {
        int found_cache = hash_get_pointer((void**)model_cache, model_cache_size, name, offsetof(CachedModel, name));
        if (found_cache >= 0) {
            render3d_module_create((Render3DModule*)module, name);
            
            ((Render3DModule*)module)->draw = mesh3d_module_draw;
            ((Module*)module)->death = mesh3d_module_death;

            module->model = model_cache[found_cache];
            model_cache[found_cache]->uses += 1;
            module->matrix_buffer = malloc_uncached(sizeof(T3DMat4FP) * display_get_num_buffers());
            for (int i=0; i < display_get_num_buffers(); i++) {
                t3d_mat4fp_identity(&module->matrix_buffer[i]);
            }
            
            rspq_block_begin();
                t3d_matrix_push(t3d_segment_placeholder(MESH_MAT_SEGMENT_PLACEHOLDER));
                t3d_model_draw(module->model->model);
                t3d_matrix_pop(1);
            module->block = rspq_block_end();
        } else {
            free(module);
            module = NULL;
            // Maybe switch this to an error model?
        }
    } else {
        free(module);
        module = NULL;
        // Maybe switch this to an error model?
    }
}
void mesh3d_module_draw(Render3DModule* self, float _, uint32_t frame_buffer) {
    Mesh3DModule* mesh_module = (Mesh3DModule*)self;
    if (mesh_module->block) {
        memcpy(&mesh_module->matrix_buffer[frame_buffer], self->transform.matrix, sizeof(T3DMat4FP));
        t3d_segment_set(MESH_MAT_SEGMENT_PLACEHOLDER, &mesh_module->matrix_buffer[frame_buffer]);
        rspq_block_run(mesh_module->block);
    }
}
void mesh3d_module_death(Module* self) {
    mesh3d_simple_module_death((Mesh3DModule*)self);
    
    free((Mesh3DModule*)self);
}
void mesh3d_simple_module_death(Mesh3DModule* self) {
    trans3d_simple_module_death((Trans3DModule*)self);

    rspq_block_free(self->block);
    free_uncached(self->matrix_buffer);
    self->model->uses -= 1;
}