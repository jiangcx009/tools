/*
 * =====================================================================================
 *
 *       Filename:  image_fmt_cvt.c
 *
 *    Description:  convert raw image format
 *
 *        Version:  1.0
 *        Created:  2015年09月25日 15时21分19秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Morning
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include <stdio.h>

#include "cinclude.h"

#define clip(a) ((a) & 0x200) ? 0 : ((a) & 0x100) ? 255 : ((a) & 0xff)

enum fmt_convert
{
    UYVY2YUV422,
    RASTER2TILE,
    TILE2RASTER,
    YUV4202UYVY422,
    YUV4222RGB
};

int uyvy2yuv422(UNSG8 *Y, UNSG8 *U, UNSG8 *V, UNSG16 w, UNSG16 h, UNSG8 *pfull)
{
	int		i, j;

	for(j = 0; j < h; j++) {
		for(i = 0; i < w;i++) {
			*(Y + j * w + i)	   = *(pfull + j * 2*w + (i << 1) + 1);					
			*(U + j * w/2 + i/2)   = *(pfull + j * 2*w + (((i >> 1) << 1) << 1));		
			*(V + j * w/2 + i/2)   = *(pfull + j * 2*w + (((((i >> 1) << 1) + 1) << 1)));
		}
	}

	return 0;
}

void raster_to_tile(UNSG8 *Y, UNSG8 *U, UNSG8 *V, UNSG8 *tile_buf, UNSG32 width, UNSG32 height)
{
	UNSG32 i,j,k,l,m;
	UNSG32 mb_idx, blk_idx;
	UNSG32 mb_width = (width+15) >> 4;
	UNSG32 mb_height = (height+15) >> 4;
	UNSG32 raster_adr;
	UNSG32 u_offset;
	UNSG8 *tile = tile_buf;

	//////////////////////////////////Y///////////////////////////////////////
	for (j=0; j<mb_height; ++j)
	{
		for (i=0; i<mb_width; ++i)
		{
			mb_idx = j * mb_width + i;
			for (k=0; k<4; ++k)
			{
				for (l=0; l<4; ++l)
				{
					blk_idx = (k<<2) | l;

					raster_adr = ((j<<4) + (k<<2))*width + (i<<4) + (l<<2);

					for (m=0; m<4; ++m)
					{
						*tile++ = Y[raster_adr + 0];
						*tile++ = Y[raster_adr + 1];
						*tile++ = Y[raster_adr + 2];
						*tile++ = Y[raster_adr + 3];
						raster_adr += width;
					}
				}
			}
		}
	}

	///////////////////////////////////UV///////////////////////////////////////
	u_offset = width*height;
	width = width >> 1;
	height = height >> 1;

	for (j=0; j<mb_height; ++j)
	{
		for (i=0; i<mb_width; ++i)
		{
			mb_idx = j * mb_width + i;
			for (k=0; k<2; ++k)
			{
				for (l=0; l<2; ++l)
				{
					blk_idx = (k<<1) | l;

					//U
					raster_adr = ((j<<3) + (k<<2))*width + (i<<3) + (l<<2);
					for (m=0; m<4; ++m)
					{
						*tile++ = U[raster_adr + 0];
						*tile++ = U[raster_adr + 1];
						*tile++ = U[raster_adr + 2];
						*tile++ = U[raster_adr + 3];
						raster_adr += width;
					}

					//V
					raster_adr = ((j<<3) + (k<<2))*width + (i<<3) + (l<<2);
					for (m=0; m<4; ++m)
					{
						*tile++ = V[raster_adr + 0];
						*tile++ = V[raster_adr + 1];
						*tile++ = V[raster_adr + 2];
						*tile++ = V[raster_adr + 3];
						raster_adr += width;
					}
				}
			}
		}
	}
}

void tile2raster(UNSG8 *Y, UNSG8 *U, UNSG8 *V, UNSG8 *tile_buf, UNSG32 width, UNSG32 height)
{
	UNSG32 i,j,k,l,m;
	UNSG32 mb_idx, blk_idx;
	UNSG32 mb_width = (width+15) >> 4;
	UNSG32 mb_height = (height+15) >> 4;
	UNSG32 raster_adr;
	UNSG32 u_offset;
	UNSG8 *tile = tile_buf;

	//////////////////////////////////Y///////////////////////////////////////
	for (j=0; j<mb_height; ++j)
	{
		for (i=0; i<mb_width; ++i)
		{
			mb_idx = j * mb_width + i;
			for (k=0; k<4; ++k)
			{
				for (l=0; l<4; ++l)
				{
					blk_idx = (k<<2) | l;

					raster_adr = ((j<<4) + (k<<2))*width + (i<<4) + (l<<2);

					for (m=0; m<4; ++m)
					{
						Y[raster_adr + 0] = *tile++ ;
						Y[raster_adr + 1] = *tile++ ;
						Y[raster_adr + 2] = *tile++ ;
						Y[raster_adr + 3] = *tile++ ;
						raster_adr += width;
					}
				}
			}
		}
	}

	///////////////////////////////////UV///////////////////////////////////////
	u_offset = width*height;
	width = width >> 1;
	height = height >> 1;

	for (j=0; j<mb_height; ++j)
	{
		for (i=0; i<mb_width; ++i)
		{
			mb_idx = j * mb_width + i;
			for (k=0; k<2; ++k)
			{
				for (l=0; l<2; ++l)
				{
					blk_idx = (k<<1) | l;

					//U
					raster_adr = ((j<<3) + (k<<2))*width + (i<<3) + (l<<2);
					for (m=0; m<4; ++m)
					{
						U[raster_adr + 0] = *tile++ ;
						U[raster_adr + 1] = *tile++ ;
						U[raster_adr + 2] = *tile++ ;
						U[raster_adr + 3] = *tile++ ;
						raster_adr += width;
					}

					//V
					raster_adr = ((j<<3) + (k<<2))*width + (i<<3) + (l<<2);
					for (m=0; m<4; ++m)
					{
						V[raster_adr + 0] = *tile++ ;
						V[raster_adr + 1] = *tile++ ;
						V[raster_adr + 2] = *tile++ ;
						V[raster_adr + 3] = *tile++ ;
						raster_adr += width;
					}
				}
			}
		}
	}
}

void yuv420_to_uyvy422(UNSG8 *Y, UNSG8 *U, UNSG8 *V,  UNSG8 *uyvy422, UNSG16 width, UNSG16 height)
{
	int i,j;
	int uyvy422_w = width<<1;

	for (j = 0; j < height; j++)
	{
		for (i = 0; i < width; i++)
		{
			uyvy422[j*uyvy422_w + (i<<1) + 1] = Y[j*width + i]; //Y component

			if((i&0x1) == 0)
				uyvy422[j*uyvy422_w + (i<<1)] = U[(j>>1)*(width>>1) + (i>>1)]; //U component;
			else
				uyvy422[j*uyvy422_w + (i<<1)] = V[(j>>1)*(width>>1) + (i>>1)]; //V component;
		}
	}
}


int yuv422toRGB(UNSG8 *uyvy422, UNSG8 *rgb, UNSG32 width, UNSG32 height)
{
	UNSG32  i,  j;
	SIGN32  y, u, v;
	SIGN32  r, g, b;

	for(j = 0; j < height; j++) {
		for (i = 0; i < width; i++) {
			y = *(uyvy422 + j*width*2 + (i << 1) + 1);
			u = *(uyvy422 + j*width*2 + (((i >> 1) << 1) << 1));
			v = *(uyvy422 + j*width*2 + (((((i >> 1) << 1) + 1) << 1)));

			r = clip((y*1192 + 1634*v- 228262) >> 10);
			g = clip((y*1192 - 400*u -833*v + 138739) >> 10);
			b = clip((y*1192 + 2066*u - 283574) >> 10);

			//r = SATURATE(r, 0, 255);
			//g = SATURATE(g, 0, 255);
			//b = SATURATE(b, 0, 255);

			//*(rgb + ((height - j - 1) * width + i) * 3) = b;
			//*(rgb + ((height - j - 1) * width + i) * 3 + 1) = g;
			//*(rgb + ((height - j - 1) * width + i) * 3 + 2) = r;
			*(rgb + (j * width + i) * 3) = r;
			*(rgb + (j * width + i) * 3 + 1) = g;
			*(rgb + (j * width + i) * 3 + 2) = b;

		}
	}
	return 0;
}

void usage()
{
    printf("Usage: inputData outputData width height format\n \ 
            Support format:\n \ 
                UYVY2YUV422,\n \
                RASTER2TILE,\n \
                TILE2RASTER,\n \
                YUV4202UYVY422,\n \
                YUV4222RGB\n");
}

SIGN32 main(int argc, char *argv[])
{
    UNSG32   width, height  ;
    UNSG32   fmt            ;
    UNSG8    inputFile[256] ;
    UNSG8    outputFile[256];

    UNSG8    *inBuf, *outBuf;
    UNSG32   pic_size;

    if ( argc != 6 ) {
        printf("Error input parameters!\n");
        usage();
        return -1;
    }

    //initial parameters
    strcpy(inputFile, argv[1]);
    strcpy(outputFile, argv[2]);
    width       = atoi(argv[3]);
    height      = atoi(argv[4]);

    pic_size = width * height;
    //inBuf = (UNSG8 *)malloc()
    

}
