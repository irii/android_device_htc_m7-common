/*
 *    Copyright (c) 2016, Ivan Radeljak
 */

#include "htc_mm_camera.h"
// Default FPS range for now
static cam_sensor_fps_range_t default_fps_range = { 10000, 60000 };

int32_t htc_mm_camera_set_parm(mm_camera_obj_t * my_obj,
                            mm_camera_parm_t *parm)
{
    switch(parm->parm_type) {
    case MM_CAMERA_PARM_DIMENSION:
        return htc_mm_camera_set_parm_dimension(my_obj, parm);
    }

    return -1;
}

int32_t htc_mm_camera_get_parm(mm_camera_obj_t * my_obj,
                            mm_camera_parm_t *parm)
{
    switch(parm->parm_type) {
    case MM_CAMERA_PARM_DIMENSION:
        return htc_mm_camera_get_parm_dimension(my_obj, parm);

    case MM_CAMERA_PARM_PREVIEW_SIZES_CNT:
    case MM_CAMERA_PARM_VIDEO_SIZES_CNT:
    case MM_CAMERA_PARM_THUMB_SIZES_CNT:
    case MM_CAMERA_PARM_HFR_SIZES_CNT:
        return htc_mm_camera_get_parm_sizes_cnt(my_obj, parm);

    case MM_CAMERA_PARM_DEF_PREVIEW_SIZES:
    case MM_CAMERA_PARM_DEF_VIDEO_SIZES:
    case MM_CAMERA_PARM_DEF_THUMB_SIZES:
    case MM_CAMERA_PARM_DEF_HFR_SIZES:
        return htc_mm_camera_get_parm_def_sizes(my_obj, parm);

    case MM_CAMERA_PARM_FPS_RANGE:
        memcpy(parm->p_value, &default_fps_range, sizeof(cam_sensor_fps_range_t));
        return MM_CAMERA_OK;
    }

    return -1;
}
