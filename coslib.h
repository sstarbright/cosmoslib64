#ifndef __COSLIB_H
#define __COSLIB_H

#include <libdragon.h>
//#include <rspq_profile.h>
#include <t3d/t3ddebug.h>
#include <t3d/t3dmodel.h>
#include <t3d/t3dskeleton.h>
#include <t3d/t3danim.h>

void coslib_init(int asset_compress, resolution_t resolution, bitdepth_t color_depth, int num_buffers, gamma_t gamma_correct, filter_options_t filter);
void coslib_end();

typedef struct Mesh3D Mesh3D;
typedef struct SkMesh3D SkMesh3D;
typedef struct SoMesh3D SoMesh3D;

struct Mesh3D {
    T3DModel* model;
    color_t color;
    rspq_block_t* block;
    T3DMat4FP* mat_buffer;
};
struct SkMesh3D {
    T3DModel* model;
    color_t color;
    rspq_block_t* block;
    T3DMat4FP* mat_buffer;
    T3DSkeleton skel;
    T3DSkeleton b_skel;
};
struct SoMesh3D {
    T3DObject** layers;
    color_t color;
    T3DMat4FP* mat_buffer;
};

#endif