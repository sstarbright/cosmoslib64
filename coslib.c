#include "coslib.h"
#include <libdragon.h>

joypad_inputs_t joy_pad;
joypad_buttons_t joy_btn;

void coslib_init(int asset_compress, resolution_t resolution, bitdepth_t color_depth, int num_buffers, gamma_t gamma_correct, filter_options_t filter) {
    // UNCOMMENT WHILE WORKING
    debug_init_isviewer();
    debug_init_usblog();

    dfs_init(DFS_DEFAULT_LOCATION);
    asset_init_compression(asset_compress);
    display_init(resolution, color_depth, num_buffers, gamma_correct, filter);
    joypad_init();

    rdpq_init();
    rdpq_debug_start();

    t3d_init((T3DInitParams){});
}
void coslib_end() {
    t3d_destroy();
    joypad_close();
    rdpq_close();
    display_close();
}

void load_ctx(context_o_t* ctx, void* data) {
    ctx->depends = 0;
    ctx->auto_close = true;
    ctx->loaded = true;
    char* ctx_path = ctx->path;
    load_scr((script_o_t*)ctx, ctx_path, true, data);
}
void unload_ctx(context_o_t* ctx) {
    if (ctx->depends == 0) {
        ctx->loaded = false;
        if (ctx->script.dso) {
            void (*ctx_close)() = dlsym(ctx->script.dso, "close");
            if (ctx_close)
                ctx_close();
            dlclose(ctx->script.dso);
            ctx->script.dso = NULL;
        }
    }
}
void req_ctx(context_o_t* ctx, int entry, void* data) {
    context_o_t* context = &ctx[entry];
    if (!context->loaded) {
        load_ctx(context, data);
    }
        context->depends++;
}
void unreq_ctx(context_o_t* ctx, int entry) {
    context_o_t* context = &ctx[entry];
    if (context->loaded) {
        context->depends--;
        if (context->auto_close)
            unload_ctx(context);
    }
}

void load_scn(scene_o_t* scn, const char* path, int acts, void* data) {
    load_scr(&scn->script, path, false, data);
    
    void* script = scn->script.dso;
    if (script) {
        int (*script_acts)() = dlsym(script, "acts");
        if (script_acts)
            acts += script_acts();
        scn->actor = malloc(acts*sizeof(actor_scr_o_t));
        void (*scr_init)(script_o_t* self, void* data) = dlsym(script->dso, "init");
        if (scr_init)
            scr_init(script, data);
    }
}
void unload_scn(scene_o_t* scn) {
    unload_scr(&scn->script);
}

void load_act(actor_scr_o_t* act, const char* path, int size, int max, void* data) {
    load_scr(&act->script, path, true, data);
    act->size = size;
    act->inst = malloc(size*max);
    act->max_inst = max;
    act->last_empty = 0;
    act->new = dlsym(((script_o_t*)act)->dso, "new");
    act->kill = dlsym(((script_o_t*)act)->dso, "kill");
    actor_o_t* empty_actor;
    int inst_base = (int)act->inst;
    
    for (int i = 0; i < max; i++) {
        empty_actor = (actor_o_t*)(inst_base+i*size);
        empty_actor->exists = false;
        empty_actor->base = act;
        empty_actor->index = i;
    }
}
void unload_act(actor_scr_o_t* act) {
    unload_scr(&act->script);
    free(act->inst);
}
actor_o_t* new_act(actor_scr_o_t* act, void* data) {
    if (act->last_empty < act->max_inst) {
        int inst_base = (int)act->inst;
        int this_index = act->last_empty;
        int size = act->size;
        actor_o_t* new_actor = (actor_o_t*)(inst_base+this_index*size);
        new_actor->exists = true;
        act->new(new_actor, data);

        this_index++;
        actor_o_t* check_actor = (actor_o_t*)(inst_base+this_index*size);
        for (;this_index < act->max_inst; this_index++) {
            if (!check_actor->exists)
                break;
            check_actor = (actor_o_t*)(inst_base+this_index*size);
        }
        act->last_empty = this_index;
        return new_actor;
    } else
        return NULL;
}
void update_act(actor_scr_o_t* act, float delta, int buffer) {
    int size = act->size;
    int inst_base = (int)act->inst;
    void(*act_up)(float delta, int buffer, void* data) = act->script.up;
    actor_o_t* up_actor;
    for (int i=0; i < act->max_inst; i++) {
        up_actor = (actor_o_t*)(inst_base+i*size);
        if (up_actor->exists)
            act_up(delta, buffer, (void*)up_actor);
    }
}
void kill_act(actor_o_t* act) {
    act->exists = false;
    int last_empty = ((actor_scr_o_t*)act)->last_empty;
    last_empty = act->index < last_empty ? act->index : last_empty;
    ((actor_scr_o_t*)act)->last_empty = last_empty;
    ((actor_scr_o_t*)act)->kill(act);
}

void load_scr(script_o_t* script, const char* path, bool auto_init, void* data) {
    script->dso = dlopen(path, RTLD_LOCAL);
    if (script->dso) {
        script->up = dlsym(script->dso, "up");

        if (auto_init) {
            void (*scr_init)(script_o_t* self, void* data) = dlsym(script->dso, "init");
            if (scr_init)
                scr_init(script, data);
        }
    }
}
void unload_scr(script_o_t* script) {
    if (script->dso) {
        void (*scr_close)() = dlsym(script->dso, "close");
        scr_close();
        dlclose(script->dso);
        script->dso = NULL;
    }
}