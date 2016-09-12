//
// Created by ye on 16-9-8.
//
extern "C"{
#include <libswscale/swscale.h>
#include <libavcodec/avcodec.h>
#include <libavutil/pixfmt.h>
#include <libavutil/avconfig.h>
}
#include <stdimage.hxx>
#include <rgbvalue.hxx>
#include <sized_int.hxx>
#include <imageinfo.hxx>
#include <impex.hxx>

#include <iostream>
using namespace std;
bool YUYVToBGR24_FFmpeg(uint8_t * pYUV,uint8_t* pRGB24,vigra::BRGBImage *image,int width,int height)
{
    if (width < 1 || height < 1 || pYUV == NULL || pRGB24 == NULL)
        return false;
    AVPicture pFrameYUV,pFrameRGB;
    avpicture_fill(&pFrameYUV,pYUV,AV_PIX_FMT_YUYV422,width,height);
    avpicture_fill(&pFrameRGB,pRGB24,AV_PIX_FMT_RGB24,width,height);
    struct SwsContext* imgCtx = NULL;
    imgCtx = sws_getContext(width,height,AV_PIX_FMT_YUYV422,width,height,AV_PIX_FMT_RGB24,SWS_BILINEAR,0,0,0);
    if (imgCtx != NULL){
        sws_scale(imgCtx,(const uint8_t* const*)pFrameYUV.data,pFrameYUV.linesize,0,height,pFrameRGB.data,pFrameRGB.linesize);
        if(imgCtx){
            sws_freeContext(imgCtx);
            imgCtx = NULL;
//            FILE *output=fopen("out.rgb","wb");
//            fwrite(pFrameRGB.data[0],(width)*(height)*3,1,output);

            int x=0;
            int y=0;
            for(int i = 0; i<width*height*3;i+=3){
                vigra::RGBValue<vigra::UInt8>  rgbPoint;
                rgbPoint.setRed(pFrameRGB.data[0][i]);
                rgbPoint.setGreen(pFrameRGB.data[0][i+1]);
                rgbPoint.setBlue(pFrameRGB.data[0][i+2]);
                (*image)[x][y]=rgbPoint;
                y++;
                if (y>=width){
                    y=0;
                    x++;
                }
            }
//          cout<<endl<<"linesize:"<<pFrameRGB.linesize[0]<<endl;
        }
        return true;
    }
    else{
        sws_freeContext(imgCtx);
        imgCtx = NULL;
        return false;
    }
}

int main(){
    int width=640;
    int height=480;
    FILE *in_file=fopen("out.yuyv","r");
    vigra::BRGBImage *image = new vigra::BRGBImage(width,height);
    uint8_t *in_buffer=new uint8_t[avpicture_get_size(AV_PIX_FMT_YUYV422, width, height)];
    uint8_t *out_buffer=new uint8_t[avpicture_get_size(AV_PIX_FMT_RGB24, width, height)];

    fread(in_buffer,avpicture_get_size(AV_PIX_FMT_YUYV422, width, height),1,in_file);
    YUYVToBGR24_FFmpeg(in_buffer,out_buffer,image,width,height);

    exportImage(srcImageRange(*image), vigra::ImageExportInfo("my-image.jpg").setCompression("80"));
    return 0;
}



