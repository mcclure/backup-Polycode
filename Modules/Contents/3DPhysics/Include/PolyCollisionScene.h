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
#include "PolyScene.h"
#include "PolyVector3.h"
#include <vector>

class btCollisionObject;
class btCollisionWorld;
class btDefaultCollisionConfiguration;
class btCollisionDispatcher;
class btAxisSweep3;

namespace Polycode {

class SceneEntity;
class CollisionSceneEntity;

/**
* Result of a collision test.
*/
struct CollisionResult {

	/**
	* True if collided.
	*/ 
	bool collided;
	
	/**
	* Collision normal.
	*/ 	
	Vector3 colNormal;
	
	/**
	* Collision distance.
	*/ 		
	Number colDist;
		
	bool setOldPosition;
	Vector3 newPos;
};
	
	/**
	* Result of a ray test.
	*/
	struct RayTestResult {
		/**
		* Entity returned.
		*/
		SceneEntity *entity;
		
		/**
		* Ray normal
		*/		
		Vector3 normal;
		
		/**
		* Ray position.
		*/				
		Vector3 position;
	};

	/**
	* A scene that tracks collisions between entities. The collision scene acts like a regular scene, only it automatically tracks collisions between its child entities.
	*/
	class _PolyExport CollisionScene : public Scene {
		public:
			/**
			* Creates a collision scene.
			*/
			CollisionScene();
			CollisionScene(bool virtualScene, bool deferInitCollision = false);		
			virtual ~CollisionScene();
		
			void initCollisionScene();
		
			virtual void Update();
			
			virtual void removeEntity(SceneEntity *entity);
						
			CollisionSceneEntity *getCollisionEntityByObject(btCollisionObject *collisionObject);		

			
			/** @name Collision scene
			*  Public methods
			*/
			//@{			
			
			RayTestResult getFirstEntityInRay(const Vector3 &origin,  const Vector3 &dest);			
			void enableCollision(SceneEntity *entity, bool val);											
			CollisionSceneEntity *getCollisionByScreenEntity(SceneEntity *ent);
			CollisionResult testCollision(SceneEntity *ent1, SceneEntity *ent2);
			CollisionResult testCollisionOnCollisionChild(CollisionSceneEntity *cEnt1, CollisionSceneEntity *cEnt2);				
			CollisionResult testCollisionOnCollisionChild_Convex(CollisionSceneEntity *cEnt1, CollisionSceneEntity *cEnt2);	
		
			virtual CollisionSceneEntity *addCollisionChild(SceneEntity *newEntity, int type=0, int group=1);
			CollisionSceneEntity *trackCollision(SceneEntity *newEntity, int type=0, int group=1);
			void removeCollision(SceneEntity *entity);
			void adjustForCollision(CollisionSceneEntity *collisionEntity);
			
			//@}
			// ----------------------------------------------------------------------------------------------------------------
			
		protected:
		
			std::vector<CollisionSceneEntity*> collisionChildren;
			btCollisionWorld *world;
		
			// Kept only to be deleted
			btDefaultCollisionConfiguration *collisionConfiguration;
			btCollisionDispatcher* dispatcher;
			btAxisSweep3*	axisSweep;
	};

}
