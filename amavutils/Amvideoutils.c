
#define LOG_TAG "amavutils"

#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <strings.h>
#include <cutils/log.h>
#include <cutils/properties.h>
#include <sys/ioctl.h>
#include "include/Amvideoutils.h"
#include "include/Amsysfsutils.h"
#include "include/Amdisplayutils.h"

#include "amports/amstream.h"
#include "ppmgr/ppmgr.h"

#define SYSCMD_BUFSIZE 40
#define DISP_DEVICE_PATH "/sys/class/video/device_resolution"
#define FB_DEVICE_PATH   "/sys/class/graphics/fb0/virtual_size"
#define ANGLE_PATH       "/dev/ppmgr"
#define VIDEO_PATH       "/dev/amvideo"
#define VIDEO_GLOBAL_OFFSET_PATH "/sys/class/video/global_offset"
#define FREE_SCALE_PATH  "/sys/class/graphics/fb0/free_scale"
#define FREE_SCALE_PATH_FB2  "/sys/class/graphics/fb2/free_scale"
#define PPSCALER_PATH  "/sys/class/ppmgr/ppscaler"
#define HDMI_AUTHENTICATE_PATH "/sys/module/hdmitx/parameters/hdmi_authenticated"
#define FREE_SCALE_MODE_PATH   "/sys/class/graphics/fb0/freescale_mode"
#define WINDOW_AXIS_PATH       "/sys/class/graphics/fb0/window_axis"
#define DISPLAY_AXIS_PATH       "/sys/class/display/axis"
#define FREE_SCALE_AXIS_PATH   "/sys/class/graphics/fb0/free_scale_axis"
#define REQUEST_2XSCALE_PATH   "/sys/class/graphics/fb0/request2XScale"


static int rotation = 0;
static int disp_width = 1920;
static int disp_height = 1080;

#ifndef LOGD
    #define LOGV ALOGV
    #define LOGD ALOGD
    #define LOGI ALOGI
    #define LOGW ALOGW
    #define LOGE ALOGE
#endif

//#define LOG_FUNCTION_NAME LOGI("%s-%d\n",__FUNCTION__,__LINE__);
#define LOG_FUNCTION_NAME

int amvideo_utils_get_freescale_enable(void)
{
    return 0;
}

int  amvideo_utils_get_global_offset(void)
{
    LOG_FUNCTION_NAME
    int offset = 0;

    return offset;
}

int is_video_on_vpp2(void)
{
    int ret = 0;

    return ret;
}

int is_vertical_panel(void)
{
    int ret = 0;

    return ret;
}

int is_vertical_panel_reverse(void)
{
    int ret = 0;

    return ret;
}

typedef enum _OSD_DISP_MODE {
	OSD_DISP_480I,
	OSD_DISP_480P,
	OSD_DISP_576I,
	OSD_DISP_576P,
	OSD_DISP_720P,
	OSD_DISP_1080I,
	OSD_DISP_1080P,
	OSD_DISP_LVDS1080P,
}OSD_DISP_MODE;

OSD_DISP_MODE get_osd_display_mode()
{
    OSD_DISP_MODE ret = OSD_DISP_1080P; 
    char buf[32]; 
    memset(buf,0,sizeof(buf));
    amsysfs_get_sysfs_str("/sys/class/display/mode", buf, 32);
    if(!strncmp(buf,"720p",4)){
        ret = OSD_DISP_720P;
    }
    else if(!strncmp(buf,"480p",4)){
        ret = OSD_DISP_480P;
    }
    else if(!strncmp(buf,"480",3)){//for 480i&480cvbs
        ret = OSD_DISP_480I;
    }
    else if(!strncmp(buf,"576p",4)){
        ret = OSD_DISP_576P;
    }
    else if(!strncmp(buf,"576",3)){//for 576i&576cvbs
        ret = OSD_DISP_576I;
    }
    else if(!strncmp(buf,"1080i",5)){
        ret = OSD_DISP_1080I;
    }
    else if(!strncmp(buf,"1080p",5)){
        ret = OSD_DISP_1080P;
    }
    else if(!strncmp(buf,"lvds1080p",9)){			
        ret = OSD_DISP_LVDS1080P;
    }
    return ret;
}

int get_device_win(OSD_DISP_MODE dismod, int *x, int *y, int *w, int *h)
{
    char prop_value_h[32];
    memset(prop_value_h,0,32);
    char prop_value_w[32];
    memset(prop_value_w,0,32);
    char prop_value_x[32];
    memset(prop_value_x,0,32);
    char prop_value_y[32];
    memset(prop_value_y,0,32);
	
    switch(dismod)
    {
    case OSD_DISP_1080P:
		if (amsysfs_get_sysfs_str("/root/.kodi/temp/window_height", prop_value_h, sizeof(prop_value_h)) != 0) { strcpy(prop_value_h, "1080"); }
		if (amsysfs_get_sysfs_str("/root/.kodi/temp/window_width", prop_value_w, sizeof(prop_value_w)) != 0) { strcpy(prop_value_w, "1920"); }
		if (amsysfs_get_sysfs_str("/root/.kodi/temp/window_x", prop_value_x, sizeof(prop_value_x)) != 0) { strcpy(prop_value_x, "0"); }
		if (amsysfs_get_sysfs_str("/root/.kodi/temp/window_y", prop_value_y, sizeof(prop_value_y)) != 0) { strcpy(prop_value_y, "0"); }
        break;
    case OSD_DISP_1080I:
		if (amsysfs_get_sysfs_str("/root/.kodi/temp/window_height", prop_value_h, sizeof(prop_value_h)) != 0) { strcpy(prop_value_h, "1080"); }
		if (amsysfs_get_sysfs_str("/root/.kodi/temp/window_width", prop_value_w, sizeof(prop_value_w)) != 0) { strcpy(prop_value_w, "1920"); }
		if (amsysfs_get_sysfs_str("/root/.kodi/temp/window_x", prop_value_x, sizeof(prop_value_x)) != 0) { strcpy(prop_value_x, "0"); }
		if (amsysfs_get_sysfs_str("/root/.kodi/temp/window_y", prop_value_y, sizeof(prop_value_y)) != 0) { strcpy(prop_value_y, "0"); }
        break;
    case OSD_DISP_LVDS1080P:			
		if (amsysfs_get_sysfs_str("/root/.kodi/temp/window_height", prop_value_h, sizeof(prop_value_h)) != 0) { strcpy(prop_value_h, "1080"); }
		if (amsysfs_get_sysfs_str("/root/.kodi/temp/window_width", prop_value_w, sizeof(prop_value_w)) != 0) { strcpy(prop_value_w, "1920"); }
		if (amsysfs_get_sysfs_str("/root/.kodi/temp/window_x", prop_value_x, sizeof(prop_value_x)) != 0) { strcpy(prop_value_x, "0"); }
		if (amsysfs_get_sysfs_str("/root/.kodi/temp/window_y", prop_value_y, sizeof(prop_value_y)) != 0) { strcpy(prop_value_y, "0"); }
        break;
    case OSD_DISP_720P:
		if (amsysfs_get_sysfs_str("/root/.kodi/temp/window_height", prop_value_h, sizeof(prop_value_h)) != 0) { strcpy(prop_value_h, "720"); }
		if (amsysfs_get_sysfs_str("/root/.kodi/temp/window_width", prop_value_w, sizeof(prop_value_w)) != 0) { strcpy(prop_value_w, "1280"); }
		if (amsysfs_get_sysfs_str("/root/.kodi/temp/window_x", prop_value_x, sizeof(prop_value_x)) != 0) { strcpy(prop_value_x, "0"); }
		if (amsysfs_get_sysfs_str("/root/.kodi/temp/window_y", prop_value_y, sizeof(prop_value_y)) != 0) { strcpy(prop_value_y, "0"); }
        break;
    case OSD_DISP_576P:
		if (amsysfs_get_sysfs_str("/root/.kodi/temp/window_height", prop_value_h, sizeof(prop_value_h)) != 0) { strcpy(prop_value_h, "576"); }
		if (amsysfs_get_sysfs_str("/root/.kodi/temp/window_width", prop_value_w, sizeof(prop_value_w)) != 0) { strcpy(prop_value_w, "720"); }
		if (amsysfs_get_sysfs_str("/root/.kodi/temp/window_x", prop_value_x, sizeof(prop_value_x)) != 0) { strcpy(prop_value_x, "0"); }
		if (amsysfs_get_sysfs_str("/root/.kodi/temp/window_y", prop_value_y, sizeof(prop_value_y)) != 0) { strcpy(prop_value_y, "0"); }
        break;
    case OSD_DISP_576I:
		if (amsysfs_get_sysfs_str("/root/.kodi/temp/window_height", prop_value_h, sizeof(prop_value_h)) != 0) { strcpy(prop_value_h, "576"); }
		if (amsysfs_get_sysfs_str("/root/.kodi/temp/window_width", prop_value_w, sizeof(prop_value_w)) != 0) { strcpy(prop_value_w, "720"); }
		if (amsysfs_get_sysfs_str("/root/.kodi/temp/window_x", prop_value_x, sizeof(prop_value_x)) != 0) { strcpy(prop_value_x, "0"); }
		if (amsysfs_get_sysfs_str("/root/.kodi/temp/window_y", prop_value_y, sizeof(prop_value_y)) != 0) { strcpy(prop_value_y, "0"); }
        break;
    case OSD_DISP_480P:
		if (amsysfs_get_sysfs_str("/root/.kodi/temp/window_height", prop_value_h, sizeof(prop_value_h)) != 0) { strcpy(prop_value_h, "480"); }
		if (amsysfs_get_sysfs_str("/root/.kodi/temp/window_width", prop_value_w, sizeof(prop_value_w)) != 0) { strcpy(prop_value_w, "720"); }
		if (amsysfs_get_sysfs_str("/root/.kodi/temp/window_x", prop_value_x, sizeof(prop_value_x)) != 0) { strcpy(prop_value_x, "0"); }
		if (amsysfs_get_sysfs_str("/root/.kodi/temp/window_y", prop_value_y, sizeof(prop_value_y)) != 0) { strcpy(prop_value_y, "0"); }
        break;
    case OSD_DISP_480I:
		if (amsysfs_get_sysfs_str("/root/.kodi/temp/window_height", prop_value_h, sizeof(prop_value_h)) != 0) { strcpy(prop_value_h, "480"); }
		if (amsysfs_get_sysfs_str("/root/.kodi/temp/window_width", prop_value_w, sizeof(prop_value_w)) != 0) { strcpy(prop_value_w, "720"); }
		if (amsysfs_get_sysfs_str("/root/.kodi/temp/window_x", prop_value_x, sizeof(prop_value_x)) != 0) { strcpy(prop_value_x, "0"); }
		if (amsysfs_get_sysfs_str("/root/.kodi/temp/window_y", prop_value_y, sizeof(prop_value_y)) != 0) { strcpy(prop_value_y, "0"); }
        break;
    default :
        break;
    }

    LOGD("get_device_win h:%s , w:%s, x:%s, y:%s \n",prop_value_h,prop_value_w,prop_value_x,prop_value_y);
    if(h){
        *h=atoi(prop_value_h);
    }
    if(w){
        *w=atoi(prop_value_w);
    }
    if(x){
        *x=atoi(prop_value_x);
    }
    if(y){
        *y=atoi(prop_value_y);
    }
    return 0;
}

int amvideo_convert_axis(int32_t* x, int32_t* y, int32_t* w, int32_t* h, int *rotation,int osd_rotation){
	int fb0_w, fb0_h;
	amdisplay_utils_get_size(&fb0_w, &fb0_h);
	ALOGD("amvideo_convert_axis convert before %d,%d,%d,%d -- %d,%d",*x,*y,*w,*h,*rotation,osd_rotation);

    if(osd_rotation == 90){
        *rotation = (*rotation + osd_rotation)%360;
        int tmp = *w;
        *w = *h;
        *h = tmp;

        tmp = *y;
        *y = fb0_w - *h - *x + 1;
        *x = tmp;
    } else if(osd_rotation == 270){// 270
        *rotation = (*rotation + osd_rotation)%360;
        int tmp = *w;
        *w = *h;
        *h = tmp;

        tmp = *x;
        *x = fb0_h - *w - *y + 1;
        *y = tmp;
    } else {
        ALOGE("should no this rotation!");
    }
    ALOGD("amvideo_convert_axis convert end %d,%d,%d,%d -- %d",*x,*y,*w,*h,*rotation);
    return 0;
}

void get_axis(const char *path, int *x, int *y, int *w, int *h)
{
    int fd = -1;
    char buf[SYSCMD_BUFSIZE];

    fd = open(path, O_RDONLY);
    if (fd < 0)
        return;

    read(fd, buf, SYSCMD_BUFSIZE);
    if (sscanf(buf, "%d %d %d %d", x, y, w, h) == 4) {
        LOGI("%s axis: %d %d %d %d\n", path, *x, *y, *w, *h);
    }

    if (fd >= 0)
        close(fd);
}

void set_scale(int x, int y, int w, int h, int *dst_x, int *dst_y, int *dst_w, int *dst_h, int disp_w, int disp_h)
{
    if ((*dst_w >= disp_w - 1) || (*dst_w == 0)) {
        *dst_x = 0;
        *dst_w = disp_w;
    }
    if ((*dst_h >= disp_h - 1) || (*dst_h == 0)) {
        *dst_y = 0;
        *dst_h = disp_h;
    }
    *dst_x = (*dst_x) * w / disp_w + x;
    *dst_y = (*dst_y) * h / disp_h + y;
    *dst_w = (*dst_w) * w / disp_w;
    *dst_h = (*dst_h) * h / disp_h;
}

int amvideo_utils_set_virtual_position(int32_t x, int32_t y, int32_t w, int32_t h, int rotation)
{
    LOG_FUNCTION_NAME
	//for osd rotation, need convert the axis first
	int osd_rotation = amdisplay_utils_get_osd_rotation();
	if(osd_rotation > 0)
        amvideo_convert_axis(&x,&y,&w,&h,&rotation,osd_rotation);
	
    int video_fd;
    int dev_fd = -1, dev_w, dev_h, disp_w, disp_h, video_global_offset;
    int dst_x, dst_y, dst_w, dst_h;
    char buf[SYSCMD_BUFSIZE];
    int angle_fd = -1;
    int ret = -1;
    int axis[4];
    char enable_p2p_play[8] = {0};
    int video_on_vpp2 = is_video_on_vpp2();
    int vertical_panel = is_vertical_panel();
    int vertical_panel_reverse = is_vertical_panel_reverse();
    
    if (video_on_vpp2) {
        int fb0_w, fb0_h, fb2_w, fb2_h;
        
        amdisplay_utils_get_size(&fb0_w, &fb0_h);
        amdisplay_utils_get_size_fb2(&fb2_w, &fb2_h);
        
        if (fb0_w > 0 && fb0_h > 0 && fb2_w > 0 && fb2_h > 0) {
            if (vertical_panel) {
                int x1, y1, w1, h1;
                if (vertical_panel_reverse) {
                    x1 = (1.0 * fb2_w / fb0_h) * y;
                    y1 = (1.0 * fb2_h / fb0_w) * x;
                    w1 = (1.0 * fb2_w / fb0_h) * h;
                    h1 = (1.0 * fb2_h / fb0_w) * w;
                } else {
                    x1 = (1.0 * fb2_w / fb0_h) * y;
                    y1 = (1.0 * fb2_h / fb0_w) * (fb0_w - x - w);
                    w1 = (1.0 * fb2_w / fb0_h) * h;
                    h1 = (1.0 * fb2_h / fb0_w) * w;                
                }
                x = x1;
                y = y1;
                w = w1;
                h = h1;
            } else {
                int x1, y1, w1, h1;
                x1 = (1.0 * fb2_w / fb0_w) * x;
                y1 = (1.0 * fb2_h / fb0_h) * y;
                w1 = (1.0 * fb2_w / fb0_w) * w;
                h1 = (1.0 * fb2_h / fb0_h) * h;            
                x = x1;
                y = y1;
                w = w1;
                h = h1;        
            }
        }        
    }
    LOGI("amvideo_utils_set_virtual_position:: x=%d y=%d w=%d h=%d\n", x, y, w, h);

    bzero(buf, SYSCMD_BUFSIZE);

    dst_x = x;
    dst_y = y;
    dst_w = w;
    dst_h = h;

    video_fd = open(VIDEO_PATH, O_RDWR);
    if (video_fd < 0) {
        goto OUT;
    }

    dev_fd = open(DISP_DEVICE_PATH, O_RDONLY);
    if (dev_fd < 0) {
        goto OUT;
    }

    read(dev_fd, buf, SYSCMD_BUFSIZE);

    if (sscanf(buf, "%dx%d", &dev_w, &dev_h) == 2) {
        LOGI("device resolution %dx%d\n", dev_w, dev_h);
    } else {
        ret = -2;
        goto OUT;
    }

    if (video_on_vpp2)
        amdisplay_utils_get_size_fb2(&disp_w, &disp_h);
    else
        amdisplay_utils_get_size(&disp_w, &disp_h);
    
    LOGI("amvideo_utils_set_virtual_position:: disp_w=%d, disp_h=%d\n", disp_w, disp_h);    
        
    video_global_offset = amvideo_utils_get_global_offset();
   
    int free_scale_enable = 0;
    int ppscaler_enable = 0;
    int freescale_mode_enable = 0;
    
    if (((disp_w != dev_w) || (disp_h / 2 != dev_h)) &&
        (video_global_offset == 0)) {
        char val[256];

        memset(val, 0, sizeof(val));
        if (video_on_vpp2) {
            if (amsysfs_get_sysfs_str(FREE_SCALE_PATH_FB2, val, sizeof(val)) == 0) {
                /* the returned string should be "free_scale_enable:[0x%x]" */
                free_scale_enable = (val[21] == '0') ? 0 : 1;
            }
        } else {
            if (amsysfs_get_sysfs_str(FREE_SCALE_PATH, val, sizeof(val)) == 0) {
                /* the returned string should be "free_scale_enable:[0x%x]" */
                free_scale_enable = (val[21] == '0') ? 0 : 1;
            }            
        }

        memset(val, 0, sizeof(val));
        if (amsysfs_get_sysfs_str(PPSCALER_PATH, val, sizeof(val)) == 0) {
            /* the returned string should be "current ppscaler mode is disabled/enable" */            
            ppscaler_enable = (val[25] == 'd') ? 0 : 1;
        }

        memset(val, 0, sizeof(val));
        if (amsysfs_get_sysfs_str(FREE_SCALE_MODE_PATH, val, sizeof(val)) == 0) {
            /* the returned string should be "free_scale_mode:new/default" */
            freescale_mode_enable = (val[16] == 'd') ? 0 : 1;
        }
    }

    angle_fd = open(ANGLE_PATH, O_WRONLY);
    if (angle_fd >= 0) {
        if (video_on_vpp2 && vertical_panel)
            ioctl(angle_fd, PPMGR_IOC_SET_ANGLE, 0);
        else
            ioctl(angle_fd, PPMGR_IOC_SET_ANGLE, (rotation/90) & 3);
        LOGI("set ppmgr angle %d\n", (rotation/90) & 3);
    }

    /* this is unlikely and only be used when ppmgr does not exist
     * to support video rotation. If that happens, we convert the window
     * position to non-rotated window position.
     * On ICS, this might not work at all because the transparent UI
     * window is still drawn is it's direction, just comment out this for now.
     */
#if 0
    if (((rotation == 90) || (rotation == 270)) && (angle_fd < 0)) {
        if (dst_h == disp_h) {
            int center = x + w / 2;

            if (abs(center - disp_w / 2) < 2) {
                /* a centered overlay with rotation, change to full screen */
                dst_x = 0;
                dst_y = 0;
                dst_w = dev_w;
                dst_h = dev_h;

                LOGI("centered overlay expansion");
            }
        }
    }
#endif
    if (free_scale_enable == 0 && ppscaler_enable == 0) {
        char val[256];

        if (freescale_mode_enable == 1) {
            int left = 0, top = 0, right = 0, bottom = 0;
            int x = 0, y = 0, w = 0, h = 0;

            memset(val, 0, sizeof(val));
            if (amsysfs_get_sysfs_str(WINDOW_AXIS_PATH, val, sizeof(val)) == 0) {
                /* the returned string should be "window axis is [a b c d]" */
                if (sscanf(val + 15, "[%d %d %d %d]", &left, &top, &right, &bottom) == 4) {
                    x = left;
                    y = top;
                    w = right - left + 1;
                    h = bottom - top + 1;
                    if ((dst_w >= dev_w - 1) || (dst_w == 0)) {
                        dst_x = 0;
                        dst_w = w;
                    }
                    if ((dst_h >= dev_h - 1) || (dst_h == 0)) {
                        dst_y = 0;
                        dst_h = h;
                    }

                    dst_x = dst_x * w / dev_w + x;
                    dst_y = dst_y * h / dev_h + y;
                    LOGI("after scaled, screen position: %d %d %d %d", dst_x, dst_y, dst_w, dst_h);
                }
            }
        } else {
            int x = 0, y = 0, w = 0, h = 0;
            int fb_w = 0, fb_h = 0;
            int req_2xscale = 0;

            memset(val, 0, sizeof(val));
            if (amsysfs_get_sysfs_str(REQUEST_2XSCALE_PATH, val, sizeof(val)) == 0) {
                /* the returned string should be "a b c" */
                if (sscanf(val, "%d", &req_2xscale) == 1) {
                    if (req_2xscale == 7 || req_2xscale == 16) {
                        if (sscanf(val, "%d %d %d", &req_2xscale, &w, &h) == 3) {
                            if (req_2xscale == 7)
                                w *= 2;
                            get_axis(DISPLAY_AXIS_PATH, &x, &y, &fb_w, &fb_h);
                            if (fb_w == 0 || fb_h == 0) {
                                fb_w = 1280;
                                fb_h = 720;
                            }
                            set_scale(x, y, w, h, &dst_x, &dst_y, &dst_w, &dst_h, fb_w, fb_h);
                            LOGI("after scaled, screen position: %d %d %d %d", dst_x, dst_y, dst_w, dst_h);
                        }
                    }
                }
            }
        }
    } else if (free_scale_enable == 1 && ppscaler_enable == 0) {
        char val[256];
        int left = 0, top = 0, right = 0, bottom = 0;
        int x = 0, y = 0, w = 0, h = 0;
        int freescale_x = 0, freescale_y = 0, freescale_w = 0, freescale_h = 0;

        if (amsysfs_get_sysfs_str(WINDOW_AXIS_PATH, val, sizeof(val)) == 0) {
            /* the returned string should be "window axis is [a b c d]" */
            if (sscanf(val + 15, "[%d %d %d %d]", &left, &top, &right, &bottom) == 4) {
                x = left;
                y = top;
                w = right - left + 1;
                h = bottom - top + 1;

                get_axis(FREE_SCALE_AXIS_PATH, &freescale_x, &freescale_y, &freescale_w, &freescale_h);
                freescale_w = (freescale_w + 1) & (~1);
                freescale_h = (freescale_h + 1) & (~1);

                set_scale(x, y, w, h, &dst_x, &dst_y, &dst_w, &dst_h, freescale_w, freescale_h);
                LOGI("after scaled, screen position: %d %d %d %d", dst_x, dst_y, dst_w, dst_h);
            }
        }
    }

    axis[0] = dst_x;
    axis[1] = dst_y;
    axis[2] = dst_x + dst_w - 1;
    axis[3] = dst_y + dst_h - 1;
    
    ioctl(video_fd, AMSTREAM_IOC_SET_VIDEO_AXIS, &axis[0]);

    ret = 0;
OUT:
    if (video_fd >= 0) {
        close(video_fd);
    }
    
    if (dev_fd >= 0) {
        close(dev_fd);
    }
    
    if (angle_fd >= 0) {
        close(angle_fd);
    }
    LOGI("amvideo_utils_set_virtual_position (corrected):: x=%d y=%d w=%d h=%d\n", dst_x, dst_y, dst_w, dst_h);

    return ret;
}

int amvideo_utils_set_absolute_position(int32_t x, int32_t y, int32_t w, int32_t h, int rotation)
{
    LOG_FUNCTION_NAME
    int video_fd;
    int angle_fd = -1;
    int axis[4];
    int video_on_vpp2 = is_video_on_vpp2();
    int vertical_panel = is_vertical_panel();
    
    LOGI("amvideo_utils_set_absolute_position:: x=%d y=%d w=%d h=%d\n", x, y, w, h);

    video_fd = open(VIDEO_PATH, O_RDWR);
    if (video_fd < 0) {
        return -1;
    }

    angle_fd = open(ANGLE_PATH, O_WRONLY);
    if (angle_fd >= 0) {
        if (video_on_vpp2 && vertical_panel)
            ioctl(angle_fd, PPMGR_IOC_SET_ANGLE, 0);
        else
            ioctl(angle_fd, PPMGR_IOC_SET_ANGLE, (rotation/90) & 3);
        LOGI("set ppmgr angle %d\n", (rotation/90) & 3);
        close(angle_fd);
    }

    axis[0] = x;
    axis[1] = y;
    axis[2] = x + w - 1;
    axis[3] = y + h - 1;
    
    ioctl(video_fd, AMSTREAM_IOC_SET_VIDEO_AXIS, &axis[0]);

    close(video_fd);

    return 0;
}

int amvideo_utils_get_position(int32_t *x, int32_t *y, int32_t *w, int32_t *h)
{
    LOG_FUNCTION_NAME
    int video_fd;
    int axis[4];

    video_fd = open(VIDEO_PATH, O_RDWR);
    if (video_fd < 0) {
        return -1;
    }

    ioctl(video_fd, AMSTREAM_IOC_GET_VIDEO_AXIS, &axis[0]);

    close(video_fd);

    *x = axis[0];
    *y = axis[1];
    *w = axis[2] - axis[0] + 1;
    *h = axis[3] - axis[1] + 1;

    return 0;
}

int amvideo_utils_get_screen_mode(int *mode)
{
    LOG_FUNCTION_NAME
    int video_fd;
    int screen_mode = 0;

    video_fd = open(VIDEO_PATH, O_RDWR);
    if (video_fd < 0) {
        return -1;
    }

    ioctl(video_fd, AMSTREAM_IOC_GET_SCREEN_MODE, &screen_mode);

    close(video_fd);

    *mode = screen_mode;

    return 0;
}

int amvideo_utils_set_screen_mode(int mode)
{
    LOG_FUNCTION_NAME
    int screen_mode = mode;
    int video_fd;

    video_fd = open(VIDEO_PATH, O_RDWR);
    if (video_fd < 0) {
        return -1;
    }

    ioctl(video_fd, AMSTREAM_IOC_SET_SCREEN_MODE, &screen_mode);

    close(video_fd);

    return 0;
}

int amvideo_utils_get_video_angle(int *angle)
{
    LOG_FUNCTION_NAME
    int angle_fd;
    int angle_value = 0;
    
    angle_fd = open(ANGLE_PATH, O_RDONLY);
    if (angle_fd >= 0) {
        ioctl(angle_fd, PPMGR_IOC_GET_ANGLE, &angle_value);
        //LOGI("get ppmgr angle %d\n", angle_value);
        close(angle_fd);
    }
    
    *angle = angle_value;

    return 0;
}

int amvideo_utils_get_hdmi_authenticate(void)
{
    LOG_FUNCTION_NAME    
    int fd = -1;
    int val = -1;
    char  bcmd[16];
    fd = open(HDMI_AUTHENTICATE_PATH, O_RDONLY);
    if (fd >= 0) {
        read(fd, bcmd, sizeof(bcmd));
        val = strtol(bcmd, NULL, 10);
        close(fd);
    }
    return val;   
}
