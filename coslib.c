#include "coslib.h"
#include <libdragon.h>

joypad_inputs_t joy_pad;
joypad_buttons_t joy_btn;

Stage* layer;

color_t back_color;

void coslib_init(int asset_compress, resolution_t resolution, bitdepth_t color_depth, int num_buffers, gamma_t gamma_correct, filter_options_t filter) {
    debug_init_isviewer();
    debug_init_usblog();
    dfs_init(DFS_DEFAULT_LOCATION);
    asset_init_compression(asset_compress);
    display_init(resolution, color_depth, num_buffers, gamma_correct, filter);
    joypad_init();

    rdpq_init();

    t3d_init((T3DInitParams){});

    cosmesh_init();
    
    layer = NULL;
    back_color = RGBA16(0x00, 0x00, 0x00, 0xFF);
}
void coslib_add_stage(Stage* stage) {
    if (layer) {
        linked_add_to_list(layer->prev, layer, stage, offsetof(Stage, prev), offsetof(Stage, next));
    } else {
        layer = stage;
    }
}
void coslib_life(uint32_t frame, float deltaTime) {
    cossnd_life();
    joypad_poll();
    joy_pad = joypad_get_inputs(JOYPAD_PORT_1);
    
    joy_btn = joypad_get_buttons_pressed(JOYPAD_PORT_1);

    Stage* current_stage = layer;
    do {
        if (current_stage->enabled) {
            stage_life(current_stage, deltaTime);
            current_stage = current_stage->next;
        }
    }
    while(current_stage != layer);
}
void coslib_draw(uint32_t frame, float deltaTime) {
    uint32_t matrix_id = frame % display_get_num_buffers();

    Stage* current_stage = layer;
    do {
        if (current_stage->enabled && current_stage->visible) {
            stage_predraw(current_stage, deltaTime, matrix_id);
            current_stage = current_stage->next;
        }
    }
    while(current_stage != layer);

    rdpq_attach(display_get(), display_get_zbuf());
    t3d_frame_start();

    t3d_screen_clear_color(back_color);

    current_stage = layer;
    do {
        if (current_stage->enabled && current_stage->visible) {
            stage_draw(current_stage, deltaTime, matrix_id);
            current_stage = current_stage->next;
        }
    }
    while(current_stage != layer);

    rdpq_sync_pipe();
    rdpq_detach_show();
}
void coslib_set_back_color(color_t color) {
    back_color = color;
}
void coslib_kill() {
    Stage* current_stage = layer;
    do {
        if (current_stage->enabled) {
            stage_simple_kill(current_stage);
            current_stage = current_stage->next;
        }
    }
    while(current_stage != layer);

    t3d_destroy();
    joypad_close();
    rdpq_close();
    display_close();
}