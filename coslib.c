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
    
    layer = NULL;
}
void coslib_end() {
    t3d_destroy();
    joypad_close();
    rdpq_close();
    display_close();
}