#ifndef __COSLIB_H
#define __COSLIB_H

#include <libdragon.h>
#include "../cosmoslib64/coscam.h"
#include "../cosmoslib64/coslite.h"
#include "../cosmoslib64/cossnd.h"

float get_time_s();
void coslib_init(int asset_compress, resolution_t resolution, bitdepth_t color_depth, int num_buffers, gamma_t gamma_correct, filter_options_t filter);
void coslib_add_stage(Stage* stage);
void coslib_life(uint32_t frame);
void coslib_draw(uint32_t frame);
void coslib_set_back_color(color_t color);
void coslib_kill();

#endif