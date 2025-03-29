#include "cosmesh.h"

CachedModel** model_cache;
int model_cache_size;

void cosmesh_init() {
    model_cache = NULL;
    model_cache_size = 0;
}

void model_cache_create(int size) {
    model_cache = malloc(sizeof(CachedModel*)*size);
    model_cache_size = size;
    memset(model_cache, 0, sizeof(CachedModel*)*size);
}

CachedModel* load_model_into_cache(const char* location, int slot) {
    if (model_cache) {
        CachedModel* cached_model = malloc(sizeof(CachedModel));
        cached_model->model = t3d_model_load(location);
        model_cache[slot] = cached_model;
        return cached_model;
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

void render3dm_create(Render3DM* module) {
    trans3dm_create((Trans3DM*)module);
    ((Module*)module)->death = render3dm_death;

    module->color = (color_t){0xFF, 0xFF, 0xFF, 0xFF};
    module->predraw = render3dm_predraw;
    module->draw = render3dm_draw;
}
void render3dm_predraw(Render3DM* module, float delta, uint32_t frame_buffer) {

}
void render3dm_draw(Render3DM* _, float __, uint32_t ___) {
    
}
void render3dm_death(Module* self) {
    trans3dm_simple_death((Trans3DM*)self);
    free(self);
}

void mesh3dm_create(Mesh3DM* module, int model_slot, int skeleton_count, int animation_count) {
    if (model_cache) {
        if (model_slot >= 0 && model_slot < model_cache_size && model_cache[model_slot]) {
            render3dm_create((Render3DM*)module);
            
            ((Render3DM*)module)->predraw = mesh3dm_predraw;
            ((Render3DM*)module)->draw = mesh3dm_draw;
            ((Module*)module)->death = mesh3dm_death;
            ((Module*)module)->life = mesh3dm_life;
            module->looping = NULL;
            module->oneshot = NULL;

            module->model = model_cache[model_slot];
            T3DModel* target_model = module->model->model;
            model_cache[model_slot]->uses += 1;
            module->matrix_buffer = malloc_uncached(sizeof(T3DMat4FP) * display_get_num_buffers());
            for (int i=0; i < display_get_num_buffers(); i++) {
                t3d_mat4fp_identity(&module->matrix_buffer[i]);
            }
            
            module->max_skeletons = skeleton_count;
            module->max_animations = animation_count;
            module->has_skeleton = false;
            if (skeleton_count > 0) {
                module->skeletons = malloc(sizeof(T3DSkeleton) * skeleton_count);
                module->skeletons[0] = t3d_skeleton_create_buffered(target_model, display_get_num_buffers());
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

            for(uint32_t i = 0; i < target_model->chunkCount; i++) {
                if(target_model->chunkOffsets[i].type == T3D_CHUNK_TYPE_MATERIAL) {
                  uint32_t offset = target_model->chunkOffsets[i].offset & 0x00FFFFFF;
                  T3DMaterial *mat = (T3DMaterial*)((char*)target_model + offset);
                }
            }
            
            rspq_block_begin();
                t3d_matrix_push(t3d_segment_placeholder(MESH_MAT_SEGMENT_PLACEHOLDER));
                if (module->has_skeleton)
                    t3d_model_draw_skinned(target_model, &module->skeletons[0]);
                else
                    t3d_model_draw(target_model);
                t3d_matrix_pop(1);
            module->base_block = rspq_block_end();
            module->block = module->base_block;
        } else {
            free(module);
            module = NULL;
            debugf("No model found in slot %i.\n", model_slot);
            // Maybe switch this to an error model?
        }
    } else {
        free(module);
        module = NULL;
        char debug_log_message[40];
        strcpy(debug_log_message, "Cache is uninitialized.\n");
        debugf(debug_log_message);
        // Maybe switch this to an error model?
    }
}
void mesh3dm_life(Module* self, float deltaTime) {
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
    }
}
void mesh3dm_predraw(Render3DM* self, float delta, uint32_t frame_buffer) {
    Mesh3DM* mesh_module = (Mesh3DM*)self;
    if (mesh_module->has_skeleton)
        t3d_skeleton_update(&mesh_module->skeletons[0]);
    memcpy(&mesh_module->matrix_buffer[frame_buffer], self->transform.fp_matrix, sizeof(T3DMat4FP));
}
void mesh3dm_draw(Render3DM* self, float _, uint32_t frame_buffer) {
    Mesh3DM* mesh_module = (Mesh3DM*)self;
    if (mesh_module->block) {
        if (mesh_module->has_skeleton)
            t3d_skeleton_use(&mesh_module->skeletons[0]);
        t3d_segment_set(MESH_MAT_SEGMENT_PLACEHOLDER, &mesh_module->matrix_buffer[frame_buffer]);
        rdpq_set_prim_color(self->color);
        rspq_block_run(mesh_module->block);
    }
}
void mesh3dm_death(Module* self) {
    mesh3dm_simple_death((Mesh3DM*)self);
    
    free(self);
}
void mesh3dm_simple_death(Mesh3DM* self) {
    trans3dm_simple_death((Trans3DM*)self);

    rspq_block_free(self->base_block);
    rspq_block_free(self->trans_block);
    free_uncached(self->matrix_buffer);
    self->model->uses -= 1;
    for (int i = 0; i < self->num_skeletons; i++) {
        t3d_skeleton_destroy(&self->skeletons[i]);
    }
    for (int i = 0; i < self->num_animations; i++) {
        t3d_anim_destroy(&self->animations[i].animation);
    }
}