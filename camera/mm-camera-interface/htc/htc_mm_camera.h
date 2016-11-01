/*
 *    Copyright (c) 2016, Ivan Radeljak
 */

#include "mm_camera_dbg.h"
#include "mm_camera_sock.h"
#include "mm_camera_interface2.h"
#include "mm_camera.h"

extern int32_t htc_mm_camera_get_parm(mm_camera_obj_t * my_obj, mm_camera_parm_t *parm);
extern int32_t htc_mm_camera_set_parm(mm_camera_obj_t * my_obj, mm_camera_parm_t *parm);

/* htc_mm_camera_dimension.c */
extern int32_t htc_mm_camera_get_parm_dimension(mm_camera_obj_t * my_obj, mm_camera_parm_t *parm);
extern int32_t htc_mm_camera_set_parm_dimension(mm_camera_obj_t * my_obj, mm_camera_parm_t *parm);

/* htc_mm_camera_sizes.c */
extern int32_t htc_mm_camera_get_parm_sizes_cnt(mm_camera_obj_t * my_obj, mm_camera_parm_t *parm);
extern int32_t htc_mm_camera_get_parm_def_sizes(mm_camera_obj_t * my_obj, mm_camera_parm_t *parm);
