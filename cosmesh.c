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

CachedModel* load_model_into_cache(const char* location, const char* name) {
    if (model_cache) {
        CachedModel* cached_model = malloc(sizeof(CachedModel));
        if (hash_add_pointer((void**)model_cache, model_cache_size, name, cached_model)) {
            strcpy(cached_model->name, name);
            cached_model->model = t3d_model_load(location);
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

void render3dm_create(Render3DM* module, const char* name) {
    trans3dm_create((Trans3DM*)module, name);
    ((Module*)module)->death = render3dm_death;

    module->color = (color_t){0xFF, 0xFF, 0xFF, 0xFF};
    module->draw = render3dm_draw;
}
void render3dm_draw(Render3DM* _, float __, uint32_t ___) {
    
}
void render3dm_death(Module* self) {
    trans3dm_simple_death((Trans3DM*)self);
    free((Render3DM*)self);
}

void mesh3dm_create(Mesh3DM* module, const char* name, uint32_t skeleton_count, uint32_t animation_count) {
    if (model_cache) {
        int found_cache = hash_get_pointer((void**)model_cache, model_cache_size, name, offsetof(CachedModel, name));
        if (found_cache >= 0) {
            render3dm_create((Render3DM*)module, name);
            
            ((Render3DM*)module)->draw = mesh3dm_draw;
            ((Module*)module)->death = mesh3dm_death;
            ((Module*)module)->life = mesh3dm_create;
            module->looping = NULL;
            module->oneshot = NULL;

            module->model = model_cache[found_cache];
            model_cache[found_cache]->uses += 1;
            module->matrix_buffer = malloc_uncached(sizeof(T3DMat4FP) * display_get_num_buffers());
            for (int i=0; i < display_get_num_buffers(); i++) {
                t3d_mat4fp_identity(&module->matrix_buffer[i]);
            }
            
            module->max_skeletons = skeleton_count;
            module->max_animations = animation_count;
            module->has_skeleton = false;
            if (skeleton_count > 0) {
                module->skeletons = malloc(sizeof(T3DSkeleton) * skeleton_count);
                module->skeletons[0] = t3d_skeleton_create_buffered(module->model->model, display_get_num_buffers());
                module->has_skeleton = true;
                module->num_skeletons = 1;
            }
            else
                module->skeletons = NULL;
            if (animation_count > 0)
                module->animations = malloc(sizeof(SkinnedAnimation) * animation_count);
            else
                module->animations = NULL;
            module->num_animations = 0;
            
            rspq_block_begin();
                t3d_matrix_push(t3d_segment_placeholder(MESH_MAT_SEGMENT_PLACEHOLDER));
                if (module->has_skeleton)
                    t3d_model_draw_skinned(module->model->model, &module->skeletons[0]);
                else
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
void mesh3dm_create(Module* self, float deltaTime) {
    Mesh3DM* mesh_module = (Mesh3DM*)self;
    if (mesh_module->has_skeleton) {
        if (mesh_module->oneshot) {
            t3d_skeleton_reset(&mesh_module->skeletons[0]);
            t3d_anim_update(&mesh_module->oneshot->animation, deltaTime);
            if (!mesh_module->oneshot->animation.isPlaying) {
                t3d_skeleton_reset(&mesh_module->skeletons[0]);
                mesh_module->oneshot = NULL;
            }
        } else if (mesh_module->looping) {
            t3d_anim_update(&mesh_module->looping->animation, deltaTime);
        }
        t3d_skeleton_update(&mesh_module->skeletons[0]);
    }
}
void mesh3dm_draw(Render3DM* self, float _, uint32_t frame_buffer) {
    Mesh3DM* mesh_module = (Mesh3DM*)self;
    if (mesh_module->block) {
        if (mesh_module->has_skeleton)
            t3d_skeleton_use(&mesh_module->skeletons[0]);
        memcpy(&mesh_module->matrix_buffer[frame_buffer], self->transform.matrix, sizeof(T3DMat4FP));
        t3d_segment_set(MESH_MAT_SEGMENT_PLACEHOLDER, &mesh_module->matrix_buffer[frame_buffer]);
        rspq_block_run(mesh_module->block);
    }
}
void mesh3dm_death(Module* self) {
    mesh3dm_simple_death((Mesh3DM*)self);
    
    free((Mesh3DM*)self);
}
void mesh3dm_simple_death(Mesh3DM* self) {
    trans3dm_simple_death((Trans3DM*)self);

    rspq_block_free(self->block);
    free_uncached(self->matrix_buffer);
    self->model->uses -= 1;
    for (int i = 0; i < self->num_skeletons; i++) {
        t3d_skeleton_destroy(&self->skeletons[i]);
    }
    for (int i = 0; i < self->num_animations; i++) {
        t3d_anim_destroy(&self->animations[i].animation);
    }
}