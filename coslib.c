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

void load_ctx(context_o_t* ctx, int entry, void* data) {
    context_o_t* context = &ctx[entry];
    context->depends = 0;
    context->auto_close = true;
    char* ctx_path = context->path;
    int cont_len = strlen(ctx_path);
    char dso_path [cont_len+9];
    strcpy(dso_path, ctx_path);
    strcat(dso_path, "_ctxg.dso");
    void* found_dso = dlopen(dso_path, RTLD_GLOBAL);
    if (found_dso)
        context->glo = found_dso;
    else
        context->glo = NULL;
    strcpy(dso_path, ctx_path);
    strcat(dso_path, "_ctxl.dso");
    found_dso = dlopen(dso_path, RTLD_LOCAL);
    void* found_func;
    if (found_dso) {
        context->lo.dso = found_dso;
        found_func = dlsym(found_dso, "up");
        if (found_func)
            context->lo.up = found_func;
        else
            context->lo.up = NULL;
        void (*lo_init)(script_o_t* self, void* data) = dlsym(found_dso, "init");
        if (lo_init)
            lo_init(&context->lo, data);
    }
    else {
        context->lo.dso = NULL;
    }
}
void unload_ctx(context_o_t* ctx, int entry) {
    context_o_t* context = &ctx[entry];
    if (context && context->depends == 0) {
        if (context->glo)
            dlclose(context->glo);
        if (context->lo.dso)
            // TODO - Add code here to call a possible close function
            dlclose(context->lo.dso);
        ctx[entry] = NULL;
    }
}
void req_ctx(context_o_t* ctx, int entry, void* data) {
    context_o_t* context = &ctx[entry];
    if (!context) {
        load_ctx(ctx, entry, data);
    }
        context->depends++;
}
void unreq_ctx(context_o_t* ctx, int entry) {
    context_o_t* context = &ctx[entry];
    if (context) {
        context->depends--;
        if (context->auto_close)
            unload_ctx(ctx, entry);
    }
}

void load_act(actor_scr_o_t* act, const char* path, int size, int max, void* data) {
    load_scr(&act->script, path, data);
    act->size = size;
    act->inst = malloc(size*max);
    act->max_inst = max;
    act->last_empty = 0;
    actor_o_t* empty_actor;
    for (int i = 0; i < max; i++) {
        empty_actor = (actor_o_t*)(((int)act->inst)+i*size);
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
        int this_index = act->last_empty;
        actor_o_t* new_actor = (actor_o_t*)(((int)act->inst)+this_index*size);
        new_actor->exists = true;

        this_index++;
        actor_o_t* check_actor = (actor_o_t*)(((int)act->inst)+this_index*size);
        for (this_index < act->max_inst; this_index++) {
            if (!check_actor->exists)
                break;
            check_actor = (actor_o_t*)(((int)act->inst)+this_index*size);
        }
        act->last_empty = this_index;
        return new_actor;
    } else
        return NULL;
}
void kill_act(actor_o_t* act) {
    act->exists = false;
    int last_empty = act->base->last_empty;
    last_empty = act->index < last_empty ? act->index : last_empty;
    act->base->last_empty = last_empty;
    // TODO - Call possible kill function on actor
}

void load_scr(script_o_t* script, const char* path, void* data) {
    script->dso = dlopen(path, RTLD_LOCAL);
    if (script->dso) {
        script->up = dlsym(script->dso, "up");

        void (*scr_init)(script_o_t* self, void* data) = dlsym(script->dso, "init");
        if (scr_init)
            scr_init(script, data);
    }
}
void unload_scr(script_o_t* script) {
    if (script->dso) {
        // TODO - Add code here to call a possible close function
        dlclose(script->dso);
    }
    free(script);
}