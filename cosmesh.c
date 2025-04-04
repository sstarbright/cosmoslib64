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

CachedModel* load_model_into_cache(const char* location, int slot, bool unshaded) {
    if (model_cache) {
        CachedModel* cached_model = malloc(sizeof(CachedModel));
        T3DModel* target_model = t3d_model_load(location);
        cached_model->model = target_model;
        model_cache[slot] = cached_model;

        if (unshaded) {
            for(uint32_t i = 0; i < target_model->chunkCount; i++) {
                if(target_model->chunkOffsets[i].type == T3D_CHUNK_TYPE_MATERIAL) {
                    uint32_t offset = target_model->chunkOffsets[i].offset & 0x00FFFFFF;
                    T3DMaterial *mat = (T3DMaterial*)((char*)target_model + offset);
                    mat->renderFlags |= T3D_FLAG_NO_LIGHT;
                }
            }
        }

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
    module->prev = module;
    module->next = module;
}
void render3dm_predraw(Render3DM* _, float __, uint32_t ___) {

}
void render3dm_draw(Render3DM* _, float __, uint32_t ___) {
    
}
void render3dm_death(Module* self) {
    trans3dm_simple_death((Trans3DM*)self);

    linked_pop_from_list(self, offsetof(Render3DM, prev), offsetof(Render3DM, next));

    free(self);
}

AnimSt* animst_create(StateM* machine, AnimSt* anim, int slot, int trans_count, int ev_count, T3DModel* source, const char* name) {
    BasicSt* state = (BasicSt*)anim;
    basicst_create(machine, state, slot, trans_count);
    state->entry = animst_entry;
    state->life = animst_life;
    state->exit = animst_exit;
    state->death = animst_death;
    anim->time = .0f;
    anim->anim = t3d_anim_create(source, name);
    t3d_anim_attach(&anim->anim, machine->main_skel);
    anim->event_count = ev_count;
    if (ev_count > 0) {
        anim->events = malloc(sizeof(AnimEv)*ev_count);
        anim->next_event = 0;
    } else {
        anim->events = NULL;
        anim->next_event = -1;
    }
    
    if (machine->blend_skel) {
        anim->blend_anim = t3d_anim_create(source, name);
        t3d_anim_attach(&anim->blend_anim, machine->blend_skel);
    }
    return anim;
}
void animst_entry(BasicSt* state, float time) {
    basicst_entry(state, time);
    state->leaving = false;
    AnimSt* anim = (AnimSt*)state;
    anim->time = .0f;
    if (anim->events) {
        anim->next_event = 0;
    }
    T3DAnim* main_anim = &anim->anim;
    if (time > .0f) {
        t3d_anim_set_playing(main_anim, true);
        t3d_anim_set_time(main_anim, fm_fmodf(time, main_anim->animRef->duration));

        T3DAnim* blend_anim = &anim->blend_anim;
        t3d_anim_set_playing(blend_anim, true);
        t3d_anim_set_time(blend_anim, .0f);
    } else {
        t3d_anim_set_playing(main_anim, true);
        t3d_anim_set_time(main_anim, .0f);
    }
}
void animst_life(BasicSt* state, float delta, bool is_first, float strength) {
    AnimSt* anim = (AnimSt*)state;
    T3DSkeleton* main_skeleton = state->module->main_skel;
    T3DAnim* target_anim;
    if (is_first) {
        target_anim = &anim->anim;
        t3d_anim_update(target_anim, delta);
    } else {
        target_anim = &anim->blend_anim;
        t3d_anim_update(target_anim, delta);
        T3DSkeleton* blend_skeleton = state->module->blend_skel;
        t3d_skeleton_blend(main_skeleton, main_skeleton, blend_skeleton, strength);
    }
    if (!state->leaving && anim->events) {
        float current_time = anim->time + target_anim->speed*delta;

        int next_event_id = anim->next_event;
        int event_count = anim->event_count;
        AnimEv* events = anim->events;
        AnimEv* next_event;
        while (next_event_id < event_count && current_time >= (next_event = &events[next_event_id])->time) {
            next_event->action(anim, next_event);
            next_event_id++;
        }

        if (target_anim->time < anim->time) {
            next_event_id = 0;
        }

        anim->time = target_anim->time;
        anim->next_event = next_event_id;
    }
}
void animst_exit(BasicSt* state, bool has_time) {
    basicst_exit(state, has_time);
    AnimSt* anim = (AnimSt*)state;
    if (!has_time) {
        t3d_anim_set_playing(&anim->anim, false);
    }
}
void animst_death(BasicSt* state) {
    basicst_death(state);
    AnimSt* anim = (AnimSt*)state;
    t3d_anim_destroy(&anim->anim);
    t3d_anim_destroy(&anim->blend_anim);
    if (anim->events) {
        AnimEv* events = anim->events;
        for (int i = 0; i < anim->event_count; i++) {
            AnimEv* event = &events[i];
            if (event->free_data && event->data)
                free(event->data);
        }
        free(events);
    }
}

AnimEv* animev_create(AnimSt* state, int slot, float time) {
    AnimEv* event = &state->events[slot];
    event->action = animev_action;
    event->time = time;
    event->free_data = false;
    event->data = NULL;
    return event;
}
void animev_action(AnimSt* _, AnimEv* __) {
    debugf("EMPTY EVENT CALLED, WHY ARE YOU DOING THIS???? :(\n");
}

void transev_action(AnimSt* state, AnimEv* event) {
    state->state.module->target_state = (int)event->data;
}
void audioev_action(AnimSt* state, AnimEv* event) {
    
}

void mesh3dm_create(Stage* stage, Mesh3DM* module, int model_slot, int skeleton_count) {
    if (model_cache) {
        if (model_slot >= 0 && model_slot < model_cache_size && model_cache[model_slot]) {
            Render3DM* render = (Render3DM*)module;
            render3dm_create(render);
            
            Trans3DM* trans = (Trans3DM*)module;
            trans->scale = (T3DVec3){{ONE_SCALE, ONE_SCALE, ONE_SCALE}};
            trans3dm_update_matrix(trans);
            
            (render)->predraw = mesh3dm_predraw;
            (render)->draw = mesh3dm_draw;
            ((Module*)module)->death = mesh3dm_death;

            module->model = model_cache[model_slot];
            T3DModel* target_model = module->model->model;
            model_cache[model_slot]->uses += 1;
            module->matrix_buffer = malloc_uncached(sizeof(T3DMat4FP) * display_get_num_buffers());
            for (int i=0; i < display_get_num_buffers(); i++) {
                t3d_mat4fp_identity(&module->matrix_buffer[i]);
            }
            
            int total_skeletons = skeleton_count*2;
            module->max_skeletons = total_skeletons;
            module->has_skeleton = false;
            if (skeleton_count > 0) {
                module->skeletons = malloc(sizeof(T3DSkeleton) * total_skeletons);
                module->has_skeleton = true;
                for (int i=0; i < total_skeletons; i += 2) {
                    module->skeletons[i] = t3d_skeleton_create_buffered(target_model, display_get_num_buffers());
                    module->skeletons[i+1] = t3d_skeleton_clone(&module->skeletons[i], false);
                }
                module->num_skeletons = total_skeletons;
            }
            else
                module->skeletons = NULL;

            for(uint32_t i = 0; i < target_model->chunkCount; i++) {
                if(target_model->chunkOffsets[i].type == T3D_CHUNK_TYPE_MATERIAL) {
                  uint32_t offset = target_model->chunkOffsets[i].offset & 0x00FFFFFF;
                  T3DMaterial *mat = (T3DMaterial*)((char*)target_model + offset);
                  debugf("Material %s\n", mat->name);
                }
            }
            
            rspq_block_begin();
                t3d_matrix_push(t3d_segment_placeholder(MESH_MAT_SEGMENT_PLACEHOLDER));
                if (module->has_skeleton) {
                    t3d_model_draw_skinned(target_model, &module->skeletons[0]);
                }
                else
                    t3d_model_draw(target_model);
                t3d_matrix_pop(1);
            module->base_block = rspq_block_end();
            module->block = module->base_block;

            if (stage->draw) {
                linked_add_to_list(stage->draw->prev, stage->draw, module, offsetof(Render3DM, prev), offsetof(Render3DM, next));
            } else {
                stage->draw = (Render3DM*)module;
            }
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
void mesh3dm_predraw(Render3DM* self, float delta, uint32_t frame_buffer) {
    Mesh3DM* mesh_module = (Mesh3DM*)self;
    if (mesh_module->has_skeleton) {
        t3d_skeleton_update(&mesh_module->skeletons[0]);
    }
    memcpy(&mesh_module->matrix_buffer[frame_buffer], self->transform.fp_matrix, sizeof(T3DMat4FP));
}
void mesh3dm_draw(Render3DM* self, float _, uint32_t frame_buffer) {
    Mesh3DM* mesh_module = (Mesh3DM*)self;
    if (mesh_module->block) {
        t3d_segment_set(MESH_MAT_SEGMENT_PLACEHOLDER, &mesh_module->matrix_buffer[frame_buffer]);
        if (mesh_module->has_skeleton)
            t3d_skeleton_use(&mesh_module->skeletons[0]);
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
}