/*
 Copyright (C) 2012 by Ivan Safrin
 
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
#include "PolyUIVScrollBar.h"
#include "PolyUIHScrollBar.h"

namespace Polycode {
	
	class _PolyExport UIScrollContainer : public ScreenEntity {
	public:
		UIScrollContainer(ScreenEntity *scrolledEntity, bool hScroll, bool vScroll, Number width, Number height);
		virtual ~UIScrollContainer();
		
		void setContentSize(Number newContentWidth, Number newContentHeight);
		
		void Resize(int x, int y);
		
		void onMouseWheelDown(Number x, Number y);
		void onMouseWheelUp(Number x, Number y);		
		
		void handleEvent(Event *event);
		
	private:		
		
		Number defaultScrollSize;
		
		Number contentWidth;
		Number contentHeight;
		
		ScreenEntity *scrollChild;		
		ScreenShape *maskShape;		
		
		bool hasHScroll;
		bool hasVScroll;
		
		UIVScrollBar *vScrollBar;
		UIHScrollBar *hScrollBar;
	};
	
}