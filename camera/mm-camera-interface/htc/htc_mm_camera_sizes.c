/*
 *    Copyright (c) 2016, Ivan Radeljak
 *    Provide pre defined resolutions (remove later)
 */

#include "htc_mm_camera.h"
#include "QCamera_Intf.h"

typedef struct camera_size_type camera_size_type;

static camera_size_type default_video_sizes[] = {
  { 1920, 1080},// 1080p
  { 1280, 720}, // 720p
  { 800, 480},  // WVGA
  { 720, 480},  // 480p
  { 640, 480},  // VGA
  { 480, 320},  // HVGA
  { 352, 288},  // CIF
  { 320, 240},  // QVGA
  { 176, 144},  // QCIF
};

static camera_size_type default_hfr_sizes[] = {
  { 800, 480}, // WVGA
  { 640, 480} // VGA
};

static camera_size_type default_preview_sizes[] = {
  {1920, 1080},
  {1456, 832},
  {1440, 1088},
  {1280, 720},
  {1088, 1088},
  {960, 720},
  {960, 544},
  {800, 480},
  {768, 464},
  {768, 432},
  {720, 720},
  {720, 480},
  {640, 480},
  {640, 384},
  {640, 368},
  {576, 432},
  {544, 544},
  {480, 480},
  {480, 320},
  {384, 288},
  {352, 288},
  {320, 240},
  {240, 160},
  {176, 144}
};

static camera_size_type default_thumbnail_sizes[]  = {
{ 512, 288 },
{ 480, 288 },
{ 432, 288 },
{ 512, 384 },
{ 352, 288 },
{ 320, 240 },
{ 176, 144 },
{0,0}
};

#define DEFAULT_VIDEO_SIZES_COUNT (sizeof(default_video_sizes)/sizeof(camera_size_type))
#define DEFAULT_HFR_SIZES_COUNT (sizeof(default_hfr_sizes)/sizeof(camera_size_type))
#define DEFAULT_PREVIEW_SIZES_COUNT (sizeof(default_preview_sizes)/sizeof(camera_size_type))
#define DEFAULT_THUMB_SIZES_COUNT (sizeof(default_thumbnail_sizes)/sizeof(camera_size_type))

int32_t htc_mm_camera_get_parm_sizes_cnt(mm_camera_obj_t * my_obj,
                            mm_camera_parm_t *parm)
{
    int count = 0;
    switch(parm->parm_type) {
    case MM_CAMERA_PARM_PREVIEW_SIZES_CNT:
        count = DEFAULT_PREVIEW_SIZES_COUNT;
        break;
    case MM_CAMERA_PARM_VIDEO_SIZES_CNT:
        count = DEFAULT_VIDEO_SIZES_COUNT;
        break;
    case MM_CAMERA_PARM_THUMB_SIZES_CNT:
        count = DEFAULT_THUMB_SIZES_COUNT;
        break;
    case MM_CAMERA_PARM_HFR_SIZES_CNT:
        count = DEFAULT_HFR_SIZES_COUNT;
        break;
    default:
        return -1;
    }

    memcpy(parm->p_value, &count, sizeof(count));

    return MM_CAMERA_OK;
};

int32_t htc_mm_camera_get_parm_def_sizes(mm_camera_obj_t * my_obj,
                            mm_camera_parm_t *parm)
{
    default_sizes_tbl_t *tbl = (default_sizes_tbl_t*)parm->p_value;

    switch(parm->parm_type) {
    case MM_CAMERA_PARM_DEF_PREVIEW_SIZES:
        memcpy(tbl->sizes_tbl, default_preview_sizes, tbl->tbl_size*sizeof(camera_size_type));
        break;
    case MM_CAMERA_PARM_DEF_VIDEO_SIZES:
        memcpy(tbl->sizes_tbl, default_video_sizes, tbl->tbl_size*sizeof(camera_size_type));
        break;
    case MM_CAMERA_PARM_DEF_THUMB_SIZES:
        memcpy(tbl->sizes_tbl, default_thumbnail_sizes, tbl->tbl_size*sizeof(camera_size_type));
        break;
    case MM_CAMERA_PARM_DEF_HFR_SIZES:
        memcpy(tbl->sizes_tbl, default_hfr_sizes, tbl->tbl_size*sizeof(camera_size_type));
        break;
    default:
        return -1;
    }

    return MM_CAMERA_OK;
}
