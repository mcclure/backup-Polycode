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
#include "PolyString.h"
#include <vector>

namespace Polycode {

	class Font;

	class FontEntry {
	public:
		String fontName;
		Font *font;
	};
	
	/**
	* Manages fonts. The font manager shoudl only be accessed via the CoreServices singleton.
	*/
	class _PolyExport FontManager {
	public:
		FontManager();
		virtual ~FontManager();

		/**
		* Loads and registers a new font for use.
		* @param fontName Name to register the font as.
		* @param fontPath Filename of the font to load.
		*/
		void registerFont(const String& fontName, const String& fontPath);
		
		/**
		* Retuns a font based on the registerd font name.
		* @param fontName Name of registered font name.
		* @return The font instance associated with the font name or NULL if one doesn't exist.
		*/		
		Font *getFontByName(const String& fontName);
		
	private:
		
		std::vector <FontEntry> fonts;
		
	};
	
}
