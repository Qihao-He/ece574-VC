#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>
#include <math.h>

#include <jpeglib.h>

#include "papi.h"

static int sobel_x_filter[3][3]={{-1,0,+1},{-2,0,+2},{-1,0,+1}};
static int sobel_y_filter[3][3]={{-1,-2,-1},{0,0,0},{1,2,+1}};

struct image_t {
	int x;
	int y;
	int depth;	/* bytes */
	unsigned char *pixels;
};

/*~~ Function transform the 3D x,y,color to the 1D array index ~~*/
static int transform(int x, int y,int color,int xsize){
	return	y*xsize*3+x*3+color;//index #	
}


static void generic_convolve(struct image_t *input_image,
				struct image_t *output_image,
				int filter[3][3]) {

	/* Look at the above image_t definition			*/
	/* You can use input_image->x (width)  			*/
	/*             input_image->y (height) 			*/
	/*         and input_image->depth (bytes per pixel)	*/
	/* input_image->pixels points to the RGB values		*/

	/******************/
	/* Your code here */
	/******************/
	int x,y,color,sum;
	for(x=1;x<input_image->x-1;x++){
		for(y=1;y<input_image->y-1;y++){
			for(color=0;color<3;color++){
			/*~~~~~ Convolute ~~~~~~*/
				sum=0;	
				sum+=filter[0][0]*input_image->pixels[transform(x-1,y-1,color,input_image->x)]; 
				sum+=filter[1][0]*input_image->pixels[transform(x,y-1,color,input_image->x)]; 
				sum+=filter[2][0]*input_image->pixels[transform(x+1,y-1,color,input_image->x)]; 
				sum+=filter[0][1]*input_image->pixels[transform(x-1,y,color,input_image->x)]; 
				sum+=filter[1][1]*input_image->pixels[transform(x,y,color,input_image->x)]; 
				sum+=filter[2][1]*input_image->pixels[transform(x+1,y,color,input_image->x)]; 
				sum+=filter[0][2]*input_image->pixels[transform(x-1,y+1,color,input_image->x)]; 
				sum+=filter[1][2]*input_image->pixels[transform(x,y+1,color,input_image->x)];
				sum+=filter[2][2]*input_image->pixels[transform(x+1,y+1,color,input_image->x)];	

				/* Saturate if necessary */
				/* Make sure stays in 0 to 255 range */
				if(sum<0){
					sum=0;
				}
				else if (sum>255){
					sum=255;
				}

				/* Set the new value */
				output_image->pixels[transform(x,y,color,output_image->x)]=sum;				
			}
		}
	}
}



static int load_jpeg(char *filename, struct image_t *image) {

	FILE *fff;
	struct jpeg_decompress_struct cinfo;
	struct jpeg_error_mgr jerr;
	JSAMPROW output_data;
	unsigned int scanline_len;
	int scanline_count=0;

	fff=fopen(filename,"rb");
	if (fff==NULL) {
		fprintf(stderr, "Could not load %s: %s\n",
			filename, strerror(errno));
		return -1;
	}

	/* set up jpeg error routines */
	cinfo.err = jpeg_std_error(&jerr);

	/* Initialize cinfo */
	jpeg_create_decompress(&cinfo);

	/* Set input file */
	jpeg_stdio_src(&cinfo, fff);

	/* read header */
	jpeg_read_header(&cinfo, TRUE);

	/* Start decompressor */
	jpeg_start_decompress(&cinfo);

	printf("output_width=%d, output_height=%d, output_components=%d\n",
		cinfo.output_width,
		cinfo.output_height,
		cinfo.output_components);

	image->x=cinfo.output_width;
	image->y=cinfo.output_height;
	image->depth=cinfo.output_components;

	scanline_len = cinfo.output_width * cinfo.output_components;
	image->pixels=malloc(cinfo.output_width * cinfo.output_height * cinfo.output_components);

	while (scanline_count < cinfo.output_height) {
		output_data = (image->pixels + (scanline_count * scanline_len));
		jpeg_read_scanlines(&cinfo, &output_data, 1);
		scanline_count++;
	}

	/* Finish decompressing */
	jpeg_finish_decompress(&cinfo);

	jpeg_destroy_decompress(&cinfo);

	fclose(fff);

	return 0;
}

static int store_jpeg(char *filename, struct image_t *image) {

	struct jpeg_compress_struct cinfo;
	struct jpeg_error_mgr jerr;
	int quality=90; /* % */
	int i;

	FILE *fff;

	JSAMPROW row_pointer[1];
	int row_stride;

	/* setup error handler */
	cinfo.err = jpeg_std_error(&jerr);

	/* initialize jpeg compression object */
	jpeg_create_compress(&cinfo);

	/* Open file */
	fff = fopen(filename, "wb");
	if (fff==NULL) {
		fprintf(stderr, "can't open %s: %s\n",
			filename,strerror(errno));
		return -1;
	}

	jpeg_stdio_dest(&cinfo, fff);

	/* Set compression parameters */
	cinfo.image_width = image->x;
	cinfo.image_height = image->y;
	cinfo.input_components = image->depth;
	cinfo.in_color_space = JCS_RGB;
	jpeg_set_defaults(&cinfo);
	jpeg_set_quality(&cinfo, quality, TRUE);

	/* start compressing */
	jpeg_start_compress(&cinfo, TRUE);

	row_stride=image->x*image->depth;

	for(i=0;i<image->y;i++) {
		row_pointer[0] = & image->pixels[i * row_stride];
		jpeg_write_scanlines(&cinfo, row_pointer, 1);
	}

	/* finish compressing */
	jpeg_finish_compress(&cinfo);

	/* close file */
	fclose(fff);

	/* clean up */
	jpeg_destroy_compress(&cinfo);

	return 0;
}

static int combine(struct image_t *sobel_x, struct image_t *sobel_y,
		struct image_t *new_image) {

	/******************/
	/* your code here */
	/******************/	
	int x,y,color;
	for(x=1;x<sobel_x->x-1;x++){
		for(y=1;y<sobel_x->y-1;y++){
			for(color=0;color<3;color++){
			/*~~~~~ normalizing the code ~~~~*/
			new_image->pixels[transform(x,y,color,sobel_x->x)]=\
				sqrt(pow(sobel_x->pixels[transform(x,y,color,sobel_x->x)],2)\
					+pow(sobel_y->pixels[transform(x,y,color,sobel_y->x)],2));
			}
		}
	}
	return 0;
}

int main(int argc, char **argv) {

	struct image_t image,sobel_x,sobel_y,new_image;
	int result;
	int eventset=PAPI_NULL;
	long long values[2];//measuring 2 event at once,gathered results are typically 64-bit integers

	/* Check command line usage */
	if (argc<2) {
		fprintf(stderr,"Usage: %s image_file\n",argv[0]);
		return -1;
	}
	
	result=PAPI_library_init(PAPI_VER_CURRENT);//Initialize
	if (result!=PAPI_VER_CURRENT) {//if the results does not match PAPI_VER_CURRENT
		fprintf(stderr,"Warning!  PAPI error %s\n",
			PAPI_strerror(result));
	}
	
	/* Load an image */
	load_jpeg(argv[1],&image);

	/* Allocate space for output image */
	new_image.x=image.x;
	new_image.y=image.y;
	new_image.depth=image.depth;
	new_image.pixels=malloc(image.x*image.y*image.depth*sizeof(char));

	/* Allocate space for output image */
	sobel_x.x=image.x;
	sobel_x.y=image.y;
	sobel_x.depth=image.depth;
	sobel_x.pixels=malloc(image.x*image.y*image.depth*sizeof(char));

	/* Allocate space for output image */
	sobel_y.x=image.x;
	sobel_y.y=image.y;
	sobel_y.depth=image.depth;
	sobel_y.pixels=malloc(image.x*image.y*image.depth*sizeof(char));

	/* convolution */
/*~~~~ PAPI measure the number of last level cache misses and cycles ~~~*/
	PAPI_create_eventset(&eventset);//create an eventset
	PAPI_add_named_event(eventset,"PAPI_L3_TCM");//add an event monitor L3 cache missess
	PAPI_add_named_event(eventset,"PAPI_TOT_CYC");//add an event monitor cycles

	PAPI_start(eventset);//Before the code of interest
	generic_convolve(&image,&sobel_x,sobel_x_filter);//sobel_x convolve
	PAPI_stop(eventset,values);//Afterward do,you can print the value or save it for later.
	printf("\nsobel_x L3 cache misses: %lld\nsobel_x cycles: %lld\n",values[0],values[1]);//printing L3 cache misses
	
	PAPI_start(eventset);//Before the code of interest
	generic_convolve(&image,&sobel_y,sobel_y_filter);
	PAPI_stop(eventset,values);//Afterward do,you can print the value or save it for later.
	printf("\nsobel_y L3 cache misses: %lld\nsobel_y cycles: %lld\n",values[0],values[1]);//printing L3 cache misses

	PAPI_start(eventset);//Before the code of interest
	combine(&sobel_x,&sobel_y,&new_image);
	PAPI_stop(eventset,values);//Afterward do,you can print the value or save it for later.
	printf("\ncombine L3 cache misses: %lld\ncombine cycles: %lld\n",values[0],values[1]);//printing L3 cache misses

	store_jpeg("out.jpg",&new_image);
//	store_jpeg("out_x.jpg",&sobel_x);//store the sobel_x
//	store_jpeg("out_y.jpg",&sobel_y);
	PAPI_shutdown();

	return 0;
}
