/*
 *    Copyright (c) 2016, Ivan Radeljak
 *
 *    TODO: Add format switch (For 1 or more than 2 planes)
 */

#include "htc_mm_camera.h"

static int32_t htc_correct_display_dim(cam_ctrl_dimension_t * dim);
static int32_t htc_correct_thumb_dim(cam_ctrl_dimension_t * dim);
static int32_t htc_correct_picture_dim(cam_ctrl_dimension_t * dim);
static int32_t htc_correct_video_dim(cam_ctrl_dimension_t * dim);

/*
 * Correct cam_ctrl_dimension object, htc has a different data structure so we need to calculate the values.
 */
int32_t htc_mm_camera_set_parm_dimension(mm_camera_obj_t * my_obj,
                            mm_camera_parm_t *parm)
{
    cam_ctrl_dimension_t *dim =
        (cam_ctrl_dimension_t *)parm->p_value;

    // Correct Preview
    htc_correct_display_dim(dim);

    // Correct Preview
    htc_correct_picture_dim(dim);

    // Correct Thumbnail
    htc_correct_thumb_dim(dim);

    // Correct Video
    htc_correct_video_dim(dim);

    return -1; // Continue with default logic
}

/*
 * Hacked up mm_camera_get_parm method to return valid dimension values until we know how htc handles this values.
 */
int32_t htc_mm_camera_get_parm_dimension(mm_camera_obj_t * my_obj,
                            mm_camera_parm_t *parm)
{
    // Correct Preview
    htc_correct_display_dim(&my_obj->dim);

    // Correct Preview
    htc_correct_picture_dim(&my_obj->dim);

    // Correct Thumbnail
    htc_correct_thumb_dim(&my_obj->dim);

    // Correct Video
    htc_correct_video_dim(&my_obj->dim);

    memcpy(parm->p_value, &my_obj->dim, sizeof(my_obj->dim));

    CDBG("%s: dw=%d,dh=%d,vw=%d,vh=%d,pw=%d,ph=%d,tw=%d,th=%d,ovx=%x,ovy=%d,opx=%d,opy=%d, m_fmt=%d, t_ftm=%d\n",
         __func__,
         my_obj->dim.display_width,my_obj->dim.display_height,
         my_obj->dim.video_width,my_obj->dim.video_height,
         my_obj->dim.picture_width,my_obj->dim.picture_height,
         my_obj->dim.ui_thumbnail_width,my_obj->dim.ui_thumbnail_height,
         my_obj->dim.orig_video_width,my_obj->dim.orig_video_height,
         my_obj->dim.orig_picture_width,my_obj->dim.orig_picture_height,
         my_obj->dim.main_img_format, my_obj->dim.thumb_format);

    return MM_CAMERA_OK;
}



static int32_t htc_correct_display_dim(cam_ctrl_dimension_t * dim) {
    uint8_t num_planes;
    uint32_t planes[VIDEO_MAX_PLANES];

    dim->display_frame_offset.frame_len = 
        mm_camera_get_msm_frame_len(dim->prev_format, CAMERA_MODE_2D,
            dim->display_width, dim->display_height, OUTPUT_TYPE_P, 
             &num_planes, planes);

    dim->display_frame_offset.num_planes = num_planes;

    dim->display_frame_offset.mp[0].len = planes[0];
    dim->display_frame_offset.mp[0].offset = 0;

    for(int i = 1; i < num_planes; i++) {
        dim->display_frame_offset.mp[i].len = planes[i];
        dim->display_frame_offset.mp[i].offset = dim->display_frame_offset.mp[i-1].len +
             dim->display_frame_offset.mp[i - 1].offset;
    }

    return MM_CAMERA_OK;
}

static int32_t htc_correct_picture_dim(cam_ctrl_dimension_t * dim) {
    int32_t frame_len, y_off, cbcr_off;
    uint8_t num_planes;
    uint32_t planes[VIDEO_MAX_PLANES];
    int width, height;

    width = dim->picture_width;
    height = dim->picture_height;

    mm_jpeg_encoder_get_buffer_offset(width, height, &y_off, &cbcr_off, &frame_len, &num_planes, planes);

    dim->picture_frame_offset.num_planes = num_planes;
    dim->picture_frame_offset.mp[0].len = planes[0];
    dim->picture_frame_offset.mp[0].offset = y_off;

    dim->picture_frame_offset.mp[1].len = planes[1];
    dim->picture_frame_offset.mp[1].offset = cbcr_off;

    dim->picture_frame_offset.frame_len = frame_len;

    return MM_CAMERA_OK;
}

static int32_t htc_correct_thumb_dim(cam_ctrl_dimension_t * dim) {
    int32_t frame_len, y_off, cbcr_off;
    uint8_t num_planes;
    uint32_t planes[VIDEO_MAX_PLANES];
    int width, height;

    width = dim->ui_thumbnail_width;
    height = dim->ui_thumbnail_height;

    mm_jpeg_encoder_get_buffer_offset(width, height, &y_off, &cbcr_off, &frame_len, &num_planes, planes);

    dim->thumb_frame_offset.num_planes = num_planes;
    dim->thumb_frame_offset.mp[0].len = planes[0];
    dim->thumb_frame_offset.mp[0].offset = y_off;

    dim->thumb_frame_offset.mp[1].len = planes[1];
    dim->thumb_frame_offset.mp[1].offset = cbcr_off;

    dim->thumb_frame_offset.frame_len = frame_len;

    return MM_CAMERA_OK;
}

static int32_t htc_correct_video_dim(cam_ctrl_dimension_t * dim) {
    /* Hold in sync with media_profiles.xml */

    int width = dim->video_width;
    int height = dim->video_height;

    dim->video_frame_offset.num_planes = 2;

    // Setup 1080p recording
    if((width == 1920 && height == 1080) || (width == 1080 && height == 1920)) {
        dim->video_frame_offset.mp[0].len = (width*height) + 1024;
        dim->video_frame_offset.mp[0].offset = 0;

        dim->video_frame_offset.mp[1].len = PAD_TO_WORD(width*height/2) + 1536;
        dim->video_frame_offset.mp[1].offset = dim->video_frame_offset.mp[0].len;
    }

    // Setup 720p recording
    if((width == 1280 && height == 720) || (width == 720 && height == 1280)) {
        dim->video_frame_offset.mp[0].len = (width*height);
        dim->video_frame_offset.mp[0].offset = 0;

        dim->video_frame_offset.mp[1].len = PAD_TO_WORD(width*height/2) + 2048;
        dim->video_frame_offset.mp[1].offset = dim->video_frame_offset.mp[0].len;
    }

    // Setup 480p recording
    if((width == 720 && height == 480) || (width == 480 && height == 720)) {
        dim->video_frame_offset.mp[0].len = (width*height) + 512;
        dim->video_frame_offset.mp[0].offset = 0;

        dim->video_frame_offset.mp[1].len = PAD_TO_WORD(width*height/2) + 1280;
        dim->video_frame_offset.mp[1].offset = dim->video_frame_offset.mp[0].len;
    }

    // Setup CIF recording
    if((width == 352 && height == 288) || (width == 288 && height == 352)) {
        dim->video_frame_offset.mp[0].len = (width*height) + 1024;
        dim->video_frame_offset.mp[0].offset = 0;

        dim->video_frame_offset.mp[1].len = PAD_TO_WORD(width*height/2) + 207360;
        dim->video_frame_offset.mp[1].offset = dim->video_frame_offset.mp[0].len;
    }

    // Setup QVGA recording
    if((width == 320 && height == 240) || (width == 240 && height == 320)) {
        dim->video_frame_offset.mp[0].len = (width*height) + 1024;
        dim->video_frame_offset.mp[0].offset = 0;

        dim->video_frame_offset.mp[1].len = PAD_TO_WORD(width*height/2) + 178688;
        dim->video_frame_offset.mp[1].offset = dim->video_frame_offset.mp[0].len;
    }

    // Set new frame_len value
    dim->video_frame_offset.frame_len = dim->video_frame_offset.mp[0].len
                                          + dim->video_frame_offset.mp[1].len;
    
    return MM_CAMERA_OK;
}
