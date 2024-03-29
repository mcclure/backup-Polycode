/*
Copyright (C) 2011 by Ivan Safrin

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

#pragma once
#include "PolyGlobals.h"
#include "PolyColor.h"

namespace Polycode {

	class String;

	/**
	* An image in memory. Basic RGB or RGBA images stored in memory. Can be loaded from PNG files, created into textures and written to file.
	*/
	class _PolyExport Image {
		public:
		
			/**
			* Create image from file name.
			* @param fileName Path to image file to load.
			*/ 
			Image(const String& fileName);
			
			/**
			* Create a blank image of specified size and type.
			* @param width Width of the image to create.
			* @param height Height of the image to create.			
			* @param type Type of image to create. Can be IMAGE_RGBA or IMAGE_RGB.
			*/ 			
			Image(int width, int height, int type = IMAGE_RGBA);
			
			/**
			* Create an image of specified size and type and set its contents from the specified buffer.
			* @param data Data buffer to set contents from.
			* @param width Width of the image to create.
			* @param height Height of the image to create.			
			* @param type Type of image to create. Can be IMAGE_RGBA or IMAGE_RGB.
			*/ 						
			Image(char *data, int width, int height, int type = IMAGE_RGBA);
			
			/**
			* Create an image from another image.
			* @param copyImage The image to copy.
			*/ 						
			Image(Image *copyImage);		
			Image();			
			virtual ~Image();

			/**
			* Load an image from a file
			* @param fileName Path to image file to load.
			* @return True if successfully loaded, false otherwise.
			*/ 			
			bool loadImage(const String& fileName);
			bool loadPNG(const String& fileName);
			
			/**
			* Saves the image to a file. Currently only PNG files are supported.
			* @param fileName Path to image file to load.	
			* @return True if successfully loaded, false otherwise.	
			*/
			bool saveImage(const String &fileName);
			bool savePNG(const String &fileName);
			
			
			/**
			* Recreate the image as an empty image of specified size. The image type stays the same.
			* @param width Width of the image to create.
			* @param height Height of the image to create.			
			*/ 						
			void createEmpty(unsigned int width, unsigned int height);
			
			/**
			* Fills the image with the specified color values.
			* @param r Red value 0-1.
			* @param g Green value 0-1
			* @param b Blue value 0-1
			* @param a Alpha value 0-1									
			*/ 									
			void fill(Number r, Number g, Number b, Number a);
			
			/**
			* Sets a pixel at specified coordinates to specified color.
			* @param x X position of pixel.
			* @param y Y position of pixel.			
			* @param r Red value 0-1.
			* @param g Green value 0-1
			* @param b Blue value 0-1
			* @param a Alpha value 0-1												
			*/
			void setPixel(int x, int y, Number r, Number g, Number b, Number a);
			
			/**
			* Sets a pixel at specified coordinates to specified color.
			* @param x X position of pixel.
			* @param y Y position of pixel.			
			* @param col Color to set.
			*/			
			void setPixel(int x, int y, Color col);
			
			/**
			* Returns the pixel color at specified position.
			* @param x X position of pixel.
			* @param y Y position of pixel.						
			* @return Pixel color at specified position.
			*/			
			Color getPixel(int x, int y);
						   
			void swap(int *v1, int *v2);
			
			/**
			* Draws a line with specified color.
			* @param x0 Starting x position.
			* @param y0 Starting y position.
			* @param x1 Ending x position.
			* @param y1 Ending y position.
			* @param col Color to use.						
			*/						
			void line(int x0, int y0, int x1, int y1, Color col);

			/**
			* Moves brush to specified position
			* @param x New brush position X
			* @param y New brush position Y
			*/									
			void moveTo(int x, int y);
			
			/**
			* Translates brush a specified amount relative to its current position.
			* @param x Amount to translate on X axis
			* @param y Amount to translate on Y axis
			*/												
			void move(int x, int y);
			
			/**
			* Draws a line to specified position.
			* @param x Ending x position.
			* @param y Ending y position.
			* @param col Color to use.						
			*/												
			void lineTo(int x, int y, Color col);
			
			/**
			* Draws a rectangle with specified color.
			* @param x Rectangle x position.
			* @param y Rectangle y position.
			* @param w Rectangle width.
			* @param h Rectangle height.
			* @param col Color to use.						
			*/									
			void drawRect(int x, int y, int w, int h, Color col);
			
			/**
			* Draws perlin noise in the image
			* @param seed Seed for the noise
			* @param alpha If true, affects alpha, if false only affects the color.
			*/									
			void perlinNoise(int seed, bool alpha);
			
			/**
			* Blurs the image using box blur.
			* @param blurSize Size of the blur in pixels.
			*/												
			void fastBlur(int blurSize);
			void fastBlurVert(int blurSize);
			void fastBlurHor(int blurSize);
			
			// What are these??? I wrote them way too long ago.
			void darken(Number amt, bool color, bool alpha);
			void lighten(Number amt, bool color, bool alpha);
			void multiply(Number amt, bool color, bool alpha);
						
			/**
			* Returns an area of the image buffer. The area can go outside of image bounds, in which case the pixels not within the image are zeroed out. This method allocates new memory for the returned buffer and you must free it manually.
			* @param x X position of the area to return.
			* @param y Y position of the area to return.
			* @param width Width of the area to return.
			* @param height Height of the area to return.					
			*/			
			char *getPixelsInRect(unsigned int x, unsigned int y, unsigned int width, unsigned int height);
			
			/**
			* Returns the x position of the brush.
			*/
			int getBrushX() const;
			
			/**
			* Returns the y position of the brush.
			*/			
			int getBrushY() const;
		
			bool isLoaded() const;
		
			int getType() const { return imageType; }
			
			void writeBMP(const String& fileName) const;
			
			/**
			* Returns the width of the image.
			*/			
			unsigned int getWidth() const;
			
			/**
			* Returns the height of the image.
			*/						
			unsigned int getHeight() const;
			
			/**
			* Returns the raw image data
			* @return Pointer to raw image data.
			*/						
			char *getPixels();
		
			static const int IMAGE_RGB = 0;
			static const int IMAGE_RGBA = 1;
			static const int IMAGE_FP16 = 2;
		
		protected:
		
			void setPixelType(int type);		
		
		int imageType;
		int pixelSize;
		
		bool loaded;
		
		Color strokeColor;
		int brushPosX;
		int brushPosY;
		
		char *imageData;
		unsigned int width;
		unsigned int height;
	};

}
