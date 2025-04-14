#ifndef __COSLIB_H
#define __COSLIB_H

#include <libdragon.h>
#include <rspq_profile.h>
#include "../cosmoslib64/coscam.h"
#include "../cosmoslib64/coslite.h"
#include "../cosmoslib64/cosaud.h"
#include <t3d/t3ddebug.h>

void coslib_init(int asset_compress, resolution_t resolution, bitdepth_t color_depth, int num_buffers, gamma_t gamma_correct, filter_options_t filter);
void coslib_end();

#endif