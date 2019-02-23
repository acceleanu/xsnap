#include <stdio.h>
#include <X11/Xlib.h> //-lX11
#include <X11/Xutil.h>
#include <X11/Xmd.h> 
#include <X11/Xatom.h>
#include <jpeglib.h> //-ljpeg 
#include <iostream>

using namespace std;

void Save_XImage_to_JPG(XImage *image, std::string FileName, int Quality);

void CopyDesktopImage(std::string sFilePath_Name)
{  
    Display *dis=XOpenDisplay((char *)0);
    Screen *scr = XDefaultScreenOfDisplay(dis);
    Drawable drawable = XDefaultRootWindow(dis);

    XImage *image = XGetImage(dis, drawable, 0, 0, scr->width, scr->height, AllPlanes, ZPixmap);
    Save_XImage_to_JPG(image, sFilePath_Name.c_str(), 75);
    XDestroyImage(image);

    XCloseDisplay(dis); 
}

void Save_XImage_to_JPG(XImage *image, std::string FileName, int Quality)
{
    FILE* outfile = fopen(FileName.c_str(), "wb");
    if(outfile == NULL) return;

    jpeg_compress_struct cinfo;
    jpeg_error_mgr       jerr;

    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_compress(&cinfo);
    jpeg_stdio_dest(&cinfo, outfile);

    cinfo.image_width      = image->width;
    cinfo.image_height     = image->height;
    cinfo.input_components = image->bitmap_unit >> 3;
    cinfo.in_color_space   = JCS_EXT_BGRX;

    jpeg_set_defaults(&cinfo);
    /*set the quality [0..100]  */
    jpeg_set_quality(&cinfo, Quality, true);
    jpeg_start_compress(&cinfo, true);

    JSAMPROW row_pointer;          /* pointer to a single row */

    while (cinfo.next_scanline < cinfo.image_height) 
    {
        row_pointer = (JSAMPROW) &image->data[cinfo.next_scanline*image->bytes_per_line];
        jpeg_write_scanlines(&cinfo, &row_pointer, 1);
    }
    jpeg_finish_compress(&cinfo);

    fclose(outfile);
}

int main()
{
	cout << "Hello!" << endl;
	CopyDesktopImage("img1.jpeg");
	return 0;
}

