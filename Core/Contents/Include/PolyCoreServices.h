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
#include "PolyEventDispatcher.h"
#include "PolyMaterialManager.h"
#include <map>

namespace Polycode {

	class PolycodeModule;
	class Renderer;
	class Config;
	class FontManager;
	class SceneManager;
	class ScreenManager;
	class TimerManager;
	class TweenManager;
	class ResourceManager;
	class SoundManager;
	class Core;
	class CoreMutex;
	
	/**
	* Global services singleton. CoreServices instantiates and provides global Singleton access to all of the main manager classes in Polycode as well as the Renderer and Config classes.
	*/
	class _PolyExport CoreServices : public EventDispatcher {
		public:
		
			/**
			* Returns the singleton instance. NOTE: The singleton instance is unique to each thread and currently Polycode does not support multithreaded access to the core services. The reason for this is being able to run multiple cores in the same application and still have global singleton access to these services.
			*/ 
			static CoreServices *getInstance();		
			static void setInstance(CoreServices *_instance);
			static CoreMutex *getRenderMutex();
			
			void setRenderer(Renderer *renderer);

			/**
			* Returns the main renderer.
			* @return The main renderer.
			* @see Renderer
			*/			
			Renderer *getRenderer();
			
			void Update(int elapsed);
			
			void setCore(Core *core);
			
			/**
			* Returns the core. 
			* @return The core.
			* @see Core
			*/																														
			Core *getCore();
			
			void handleEvent(Event *event);
		
			/**
			* Installs a plugin module at runtime.
			* @param module Plugin module to install. See PolygonModule for more details on modules.
			@see PolycodeModule
			*/
			void installModule(PolycodeModule *module);
			
			/**
			* Returns the material manager. The material manager is responsible for loading and managing textures, shaders and materials.
			* @return Material manager.
			* @see MaterialManager
			*/
			MaterialManager *getMaterialManager();
			
			/**
			* Returns the screen manager. The screen manager is responsible for maintaining and rendering 2D screens.
			* @return Screen Manager
			* @see ScreenManager
			*/			
			ScreenManager *getScreenManager();
			
			/**
			* Returns the scene manager. The screen manager is responsible for maintaining and rendering 3D scenes.
			* @return Scene Manager
			* @see SceneManager
			*/						
			SceneManager *getSceneManager();
			
			/**
			* Returns the timer manager. The timer manager is responsible for updating timers in the framework.
			* @return Timer Manager
			* @see TimerManager
			*/									
			TimerManager *getTimerManager();
			
			/**
			* Returns the tween manager. The tween manager is responsible for updating animated tweens in the framework.
			* @return Tween Manager
			* @see TweenManager
			*/												
			TweenManager *getTweenManager();
			
			/**
			* Returns the resource manager. The resource manager is responsible for loading and unloading resources.
			* @return Resource Manager
			* @see ResourceManager
			*/																					
			ResourceManager *getResourceManager();
			
			/**
			* Returns the sound manager. The sound manager is responsible for loading and playing sounds.
			* @return Sound Manager
			* @see SoundManager
			*/																								
			SoundManager *getSoundManager();
			
			/**
			* Returns the font manager. The font manager is responsible for loading and managing fonts.
			* @return Font Manager
			* @see FontManager
			*/																											
			FontManager *getFontManager();

			/**
			* Returns the config. The config loads and saves data to disk.
			* @return Config manager.
			* @see Config
			*/																													
			Config *getConfig();
		
			/**
			 * Provides the current width, height, and refresh rate of the screen.
			 * @param width If non-NULL, current screen width will be written here (or 0 if unknown).
			 * @param hight If non-NULL, current screen height will be written here (or 0 if unknown).
			 * @param hz If non-NULL, current screen refresh rate will be written here (or 0 if unknown).
			 */
			void getScreenInfo(int *width, int *height, int *hz);
		
			virtual ~CoreServices();
		
		protected:
		
			CoreServices();
					
		private:
		
			static CoreServices* overrideInstance;
			static std::map <long, CoreServices*> instanceMap;
			static CoreMutex *renderMutex;
		
			std::vector<PolycodeModule*> modules;
			std::vector<PolycodeModule*> updateModules;
					
			Core *core;
			Config *config;
			MaterialManager *materialManager;
			ScreenManager *screenManager;		
			SceneManager *sceneManager;
			TimerManager *timerManager;
			TweenManager *tweenManager;
			ResourceManager *resourceManager;
			SoundManager *soundManager;
			FontManager *fontManager;
			Renderer *renderer;
	};
}
