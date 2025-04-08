#ifndef __COSLIB_H
#define __COSLIB_H

#include <libdragon.h>
#include <rspq_profile.h>
#include "../cosmoslib64/coscam.h"
#include "../cosmoslib64/coslite.h"
#include "../cosmoslib64/cosaud.h"
#include <t3d/t3ddebug.h>

void coslib_init(int asset_compress, resolution_t resolution, bitdepth_t color_depth, int num_buffers, gamma_t gamma_correct, filter_options_t filter);
void coslib_add_stage(Stage* stage);
void coslib_life(uint32_t frame, float deltaTime);
void coslib_draw(uint32_t frame, float deltaTime);
void coslib_set_back_color(color_t color);
joypad_inputs_t cosjoy_get_pad();
joypad_buttons_t cosjoy_get_btn();
void coslib_kill();

#endif