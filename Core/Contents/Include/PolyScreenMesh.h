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
#include "PolyScreenEntity.h"
#include "PolyMesh.h"

namespace Polycode {

	class Image;
	class Texture;

	/**
	* 2D Mesh. ScreenMesh is the base for most geometry-based screen entities. It's based aroudn a Mesh instance, like its 3D counterpart (SceneMesh), but currently has fewer options.
	* @see Mesh
	*/
	class _PolyExport ScreenMesh : public ScreenEntity {
		public:
		
			/**
			* Creates the screen mesh from existing Mesh.
			*/
			ScreenMesh(Mesh *mesh);
			
			/**
			* Creates the screen mesh and loads a mesh from a file name.
			*/
			ScreenMesh(const String& fileName);
			
			/**
			* Create an empty screen mesh of specified type. See Mesh for available mesh types.
			*/
			ScreenMesh(int meshType);
			virtual ~ScreenMesh();
			
			void Render();
			
			/**
			* Returns the mesh for this screen mesh.
			* @return The mesh.
			*/
			Mesh *getMesh() const;
			
			/**
			* Returns the texture associated with the mesh.
			*/
			Texture *getTexture() const;
			
			/**
			* Loads a texture from an image file.
			* @param fileName Path to the image file.
			*/
			void loadTexture(const String& fileName);
			
			/**
			* Loads a texture from an image instance.
			* @param image Image instance.
			*/			
			void loadTexture(Image *image);	
			
			/**
			* Applies a texture
			* @param Texture to apply.
			*/						
			void setTexture(Texture *texture);
			
			/**
			* If this is set to true, the lines in wireframe meshes will be anti-aliased if the support is available in the renderer.
			*/			
			bool lineSmooth;
			
			Number lineWidth;
			
			
		protected:
		
			Mesh *mesh;
			Texture *texture;
	};
}
