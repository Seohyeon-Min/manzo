/*!***********************************************************************
* File: CameraView.cpp
* Author: Gyuwon Na
* Course: CS200
* Last Updated: 09-26-2024
* Brief
    - Calculation about camera view
**************************************************************************/
#include "CameraView.h"

/*!***********************************************************************
* brief		set frame buffer size
* param		vec2 the_framebuffer_size (width, height)
*************************************************************************/
void CameraView::SetFramebufferSize(vec2 the_framebuffer_size) noexcept
{
    framebuffer_size = the_framebuffer_size;
}

/*!***********************************************************************
* brief		set frame buffer size
* param		int new_framebuffer_width (width)
            int new_framebuffer_height (height)
*************************************************************************/
void CameraView::SetFramebufferSize(int new_framebuffer_width, int new_framebuffer_height) noexcept
{
    SetFramebufferSize(vec2{ float(new_framebuffer_width), float(new_framebuffer_height) });
}

/*!***********************************************************************
* brief		get frame buffer size
*************************************************************************/
vec2 CameraView::GetViewSize() const
{
    return framebuffer_size;
}

/*!***********************************************************************
* brief		set zoom
* param		float new_zoom (set zoom into new zoom)
*************************************************************************/
void CameraView::SetZoom(float new_zoom) noexcept
{
    // if new zoom is not 0
        // save as zoom
    if (new_zoom != 0)
    {
        zoom = new_zoom;
    }
}

/*!***********************************************************************
* brief		build camera to ndc coordinate
*************************************************************************/
mat3 CameraView::BuildCameraToNDC() const noexcept
{
    //         (v.w/2,v.h/2)              (1,1)
    //      +--------o             +--------o
    //      |        |             |        |
    //      | cam    |     --->    | ndc    |
    //      o--------+             o--------+
    // (-v.w/2,-v.h/2)          (-1,-1)
    //

    // get view size with zoom
    vec2 view_size = GetViewSize();
    float zoomDegree = GetZoom();
    vec2 zoomed = { GetViewSize().x / zoomDegree, GetViewSize().y / zoomDegree };
    // build and return camera to NDC matrix
    return { {2.f / zoomed.x,0,0},{0,2.f / zoomed.y,0},{0,0,1} };
}


/*!***********************************************************************
* brief		build window device to camera coordinate
*************************************************************************/
mat3 CameraView::BuildWindowDeviceToCamera() const noexcept
{
    //    (0,0)             (-v.w/2,v.h/2)
    //      o--------+             o--------+
    //      |        |             |        |
    //      | win    |     --->    | cam    |
    //      +--------o             +--------o
    //           (f.w,f.h)            (v.w/2,-v.h/2)
    //

    // get view size with zoom
    // build and return window to camera matrix

    vec2 view_size = CalcViewSizeWithZoom();
    vec2 frame_size = GetViewSize();

    mat3 mat;
    mat.column0 = { view_size.x / frame_size.x ,    0.0f ,                                  0.0f };
    mat.column1 = { 0.0f ,                          view_size.y / frame_size.y,            0.0f };
    mat.column2 = { -view_size.x / 2.0f ,           -view_size.y / 2.0f ,                    1.0f };

    return mat;
}