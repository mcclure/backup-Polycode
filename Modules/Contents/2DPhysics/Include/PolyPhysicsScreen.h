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
#include "PolyEvent.h"
#include "PolyScreen.h"
#include "PolyVector2.h"
#include "Box2D/Box2D.h"
#include <vector>

#define MAX_B2DCONTACTPOINTS 2048

namespace Polycode {

class ScreenEntity;
class PhysicsScreenEntity;
class Timer;

/**
* Event sent out by the PhysicsScreen class when collisions begin and end.
*/	
class _PolyExport PhysicsScreenEvent : public Event {
	public:	
		PhysicsScreenEvent();
		virtual ~PhysicsScreenEvent();
		
		/**
		* First colliding entity.
		*/
		ScreenEntity *entity1;	
		
		/**
		* Second colliding entity.
		*/		
		ScreenEntity *entity2;	

		/**
		* First colliding entity.
		*/
		ScreenEntity *getFirstEntity();	
		
		/**
		* Second colliding entity.
		*/		
		ScreenEntity *getSecondEntity();	

	
		/**
		* Local collision normal.
		*/
		Vector2 localCollisionNormal;

		/**
		* Collision normal in world space
		*/		
		Vector2 worldCollisionNormal;
		
		/**
		* Collision point in local space
		*/			
		Vector2 localCollisionPoint;

		/**
		* Collision point in world space
		*/			
		Vector2 worldCollisionPoint;

			
		/**
		* Strength of the collision impact.
		*/
		Number impactStrength;	
		
		/**
		* Friction strength of the impact
		*/		
		Number frictionStrength;	
							
		/**
		* Event sent out when a collision begins
		*/					
		static const int EVENT_NEW_SHAPE_COLLISION = 0;
		
		/**
		* Event sent out when a collision ends
		*/							
		static const int EVENT_END_SHAPE_COLLISION = 1;
		
		/**
		* Event sent out when a collision begins
		*/					
		static const int EVENT_SOLVE_SHAPE_COLLISION = 3;

		
};		


enum ContactState
{
	e_contactAdded = 0,
	e_contactPersisted = 1,
	e_contactRemoved = 2,
};
	
struct ContactPoint
{
	b2Shape* shape1;
	b2Shape* shape2;
	b2Vec2 normal;
	b2Vec2 position;
	b2Vec2 velocity;
	b2ContactID id;
	ContactState state;
};
	

class _PolyExport PhysicsJoint {
public:
	PhysicsJoint() {}
	virtual ~PhysicsJoint() {}
	b2Joint *box2DJoint;
	
};

/**
* A 2D Physics enabled screen. A PhysicsScreen acts like a normal screen, except that entities added to it with addPhysicsChild have physics automatically simulated. You can also use it to check collisions using addCollisionChild.
*/	
class _PolyExport PhysicsScreen : public Screen, b2ContactListener {

public:

	/**
	* Creates a new physics screen.
	*/ 
	PhysicsScreen(Number worldScale, Number freq);
	
	/**
	* Default constructor.
	*/
	PhysicsScreen();
	
	virtual ~PhysicsScreen();
	
	void Update();
	
	/**
	* Adds a ScreenEntity as a physics enabled child. 
	* @param newEntity Screen entity to add.
	* @param entType Physics entity type to add as. Possible values are PhysicsScreenEntity::ENTITY_RECT, PhysicsScreenEntity::ENTITY_CIRCLE and PhysicsScreenEntity::ENTITY_MESH. If the type is ENTITY_MESH, the ScreenEntity passed must be a ScreenMesh!
	* @param isStatic If this parameter is true, the body is static (doesn't move on its own).
	* @param friction Friction of the physics entity. Friction controls how entities drag along each other.
	* @param density Density of the physics entity. Density controls how heavy the entity is.
	* @param restitution Restitution of the physics entity. Restitution controls how bouncy the entity is.
	* @param isSensor If this is set to true, the entity won't collide with other entities, but its collision will register.
	* @param fixedRotation If this is set to true, the entity will always have a locked rotation.
	* @return The physics entity wrapper.
	*/
	PhysicsScreenEntity *addPhysicsChild(ScreenEntity *newEntity, int entType, bool isStatic, Number friction=0.1, Number density=1, Number restitution = 0, bool isSensor = false, bool fixedRotation = false);
	
	/**
	* Removes a physics child from the screen.
	* @param entityToRemove Entity to remove from the screen.
	*/
	void removePhysicsChild(ScreenEntity *entityToRemove);
	
	
	ScreenEntity* removeChild(ScreenEntity *entityToRemove);
	
	
	/**
	* Begins tracking collisions for a ScreenEntity.
	* @param newEntity Entity to track collisions for.
	* @param entType Physics shape of the entity. Possible values are PhysicsScreenEntity::ENTITY_RECT or PhysicsScreenEntity::ENTITY_CIRCLE.
	* @param entityToRemove Entity to remove from the screen.
	*/	
	PhysicsScreenEntity *addCollisionChild(ScreenEntity *newEntity, int entType);
	
	/**
	* Removes an existing joint.
	* @param joint Joint to remove.
	*/
	void destroyJoint(PhysicsJoint *joint);
	
	/**
	* Creates a new distance joint. Distance joints keep the two entities at a fixed distance.
	* @param ent1 First entity to join.
	* @param ent2 Second entity to join.	
	* @param collideConnected If set to true, both entities will collide with each other, if false, they will not.
	* @return Created physics joint.	
	*/ 
	PhysicsJoint *createDistanceJoint(ScreenEntity *ent1, ScreenEntity *ent2, bool collideConnected);
	
	/**
	* Creates a new prismatic joint. Prismatic joints provide one degree of freedom between two entities. 
	* @param ent1 First entity to join.
	* @param ent2 Second entity to join.	
	* @param collideConnected If set to true, both entities will collide with each other, if false, they will not.
	* @param ax Anchor point x (relative to first entity)
	* @param ay Anchor point y (relative to first entity)
	* @param enableLimit If true, the rotation will be limited to the specified values 
	* @param lowerTranslation If enableLimit is true, specifies the lower translation limit.
	* @param upperTranslation If enableLimit is true, specifies the upper translation limit.
	* @param motorEnabled If enabled, applies a constant motor to the rotation joint.
	* @param motorSpeed If motorEnabled is true, controls the speed at which the motor rotates.
	* @param maxTorque	If motorEnabled is true, specifies the maximum force applied.		
	* @param worldAxis Specifies the relative world axis for the prismatic joint.	
	* @return Created physics joint.	
	*/ 	
	PhysicsJoint *createPrismaticJoint(ScreenEntity *ent1, ScreenEntity *ent2, Vector2 worldAxis, Number ax, Number ay, bool collideConnected=false, Number lowerTranslation=0, Number upperTranslation=0, bool enableLimit=false, Number motorSpeed=0, Number motorForce=0, bool motorEnabled=false);

	/**
	* Creates a new revolute joint. Revolute joints enable one entity to rotate around a point on another entity.
	* @param ent1 Entity to anchor to.
	* @param ent2 Entity to anchor.
	* @param ax Anchor point x (relative to first entity)
	* @param ay Anchor point y (relative to first entity)
	* @param enableLimit If true, the rotation will be limited to the specified values 
	* @param lowerLimit If enableLimit is true, specifies the lower limit of the rotation in degrees.
	* @param upperLimit If enableLimit is true, specifies the upper limit of the rotation in degrees.				
	* @param motorEnabled If enabled, applies a constant torque to the rotation joint.
	* @param motorSpeed If motorEnabled is true, controls the speed at which the motor rotates.
	* @param maxTorque	If motorEnabled is true, specifies the maximum torque applied.		
	* @return Created physics joint.
	*/ 		
	PhysicsJoint *createRevoluteJoint(ScreenEntity *ent1, ScreenEntity *ent2, Number ax, Number ay, bool collideConnected=false, bool enableLimit=false, Number lowerLimit=0, Number upperLimit=0, bool motorEnabled=false, Number motorSpeed=0, Number maxTorque=0);
	
//	b2MouseJoint *createMouseJoint(ScreenEntity *ent1, Vector2 *mp);

	/**
	* Applies linear force to an entity.
	* @param ent Entity to apply force to.
	* @param fx X value of the force direction vector.
	* @param fy Y value of the force direction vector.		
	*/ 	
	void applyForce(ScreenEntity *ent, Number fx, Number fy);
	
	/**
	* Applies an impulse to an entity
	* @param ent Entity to apply force to.
	* @param fx X value of the impulse direction vector.
	* @param fy Y value of the impulse direction vector.		
	*/ 		
	void applyImpulse(ScreenEntity *ent, Number fx, Number fy);

	/**
	* Sets the gravity for this screen.
	* @param newGravity The new gravity vector.
	*/ 			
	void setGravity(Vector2 newGravity);

	/**
	* Warps an entity to the specified location and angle.
	* @param ent Entity to transform.
	* @param pos New position to set.
	* @param angle New angle to set.
	*/ 				
	void setTransform(ScreenEntity *ent, Vector2 pos, Number angle);

	
	PhysicsScreenEntity *getPhysicsEntityByShape(b2Shape *shape);
	PhysicsScreenEntity *getPhysicsEntityByFixture(b2Fixture *fixture);

	/**
	* Sets the linear velocity of an entity.
	* @param ent Entity to set velocity to.
	* @param fx X direction of velocity vector.
	* @param fy Y direction of velocity vector.
	*/ 					
	void setVelocity(ScreenEntity *ent, Number fx, Number fy);	

	/**
	* Sets the linear velocity of an entity on the X axis.
	* @param ent Entity to set velocity to.
	* @param fx X direction of velocity vector.
	*/ 						
	void setVelocityX(ScreenEntity *ent, Number fx);	
	
	/**
	* Sets the linear velocity of an entity on the Y axis.
	* @param ent Entity to set velocity to.
	* @param fy Y direction of velocity vector.
	*/ 							
	void setVelocityY(ScreenEntity *ent, Number fy);	
	
	/**
	* Sets the spin of an entity
	* @param ent Entity to apply spin to.
	* @param spin Spin value.
	*/ 								
	void setSpin(ScreenEntity *ent, Number spin);

	/**
	* Returns the velocity of an entity.
	* @param ent Entity to return velocity for
	* @return Velocity of the specified entity.
	*/ 									
	Vector2 getVelocity(ScreenEntity *ent);
			
	void BeginContact (b2Contact *contact);
	void EndContact (b2Contact *contact);	
	void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse);

	/**
	* Wake up a sleeping entity. The physics engine puts non-moving entities to sleep automatically. Use this to wake them up.
	* @param ent Entity to wake up.
	*/ 										
	void wakeUp(ScreenEntity *ent);
	
	void handleEvent(Event *event);

	/**
	* Returns the entity at the specified position.
	* @param x X position.
	* @param y Y position.
	* @return If there is a collision-tracked entity at the specified position, it will be returned, NULL if there isn't.
	*/ 													
	ScreenEntity *getEntityAtPosition(Number x, Number y);
	
	/**
	* Returns true if the specified entity is at the specified position.
	* @param ent Entity to test.
	* @param x X position.
	* @param y Y position.
	* @return If there specified entity overlaps the specified position, this returns true.
	*/ 														
	bool testEntityAtPosition(ScreenEntity *ent, Number x, Number y);

	/**
	* Tests collision between two entities
	*/
	bool testEntityCollision(ScreenEntity *ent1, ScreenEntity *ent2);
	
	void Shutdown();
	
	/**
	* Returns the physics entity for the specified screen entity. When you add ScreenEntities to the physics screen, these physics entities are created to track the physics status of the screen entities. You don't need to deal with these ever, but if you want, you can get them anyway.
	* @param ent ScreenEntity instance to return the physics entity for.
	* @return PhysicsEntity associated with the ScreenEntity.
	*/
	PhysicsScreenEntity *getPhysicsByScreenEntity(ScreenEntity *ent);
	void destroyMouseJoint(b2MouseJoint *mJoint);


protected:
	
	
	Number worldScale;
	
	void init(Number worldScale, Number physicsTimeStep, int physicsIterations, Vector2 physicsGravity);

	std::vector <PhysicsScreenEntity*> physicsChildren;
	std::vector<b2Contact*> contacts;
	b2World *world;
	Number timeStep;
	int32 iterations;
};


}
