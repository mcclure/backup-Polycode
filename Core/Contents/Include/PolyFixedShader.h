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
#include "PolyString.h"
#include "PolyLogger.h"
#include "PolyGlobals.h"
#include "PolyShader.h"
#include "PolyTexture.h"

namespace Polycode {

	class _PolyExport FixedShader : public Shader {
		public:
			FixedShader();
			virtual ~FixedShader();
			
			ShaderBinding *createBinding();

		protected:
			
	};

	class _PolyExport FixedShaderBinding : public ShaderBinding {
		public:
			FixedShaderBinding(FixedShader *shader);
			virtual ~FixedShaderBinding();
			
			void addTexture(const String& name, Texture *texture);
			void addCubemap(const String& name, Cubemap *cubemap);
			void addParam(const String& type, const String& name, const String& value);
			
			Texture *getDiffuseTexture();
			
		protected:

			std::vector<Texture*> textures;
			std::vector<Cubemap*> cubemaps;
			FixedShader *fixedShader;
	};

}
