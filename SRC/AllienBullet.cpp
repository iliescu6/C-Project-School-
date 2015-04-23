#include "GameWorld.h"
#include "AllienBullet.h"
#include "BoundingSphere.h"

// PUBLIC INSTANCE CONSTRUCTORS ///////////////////////////////////////////////

/** Constructor. Bullets live for 2s by default. */
AllienBullet::AllienBullet()
	: GameObject("AllienBullet"), mTimeToLive(2000)
{
}

/** Construct a new bullet with given position, velocity, acceleration, angle, rotation and lifespan. */
AllienBullet::AllienBullet(GLVector3f p, GLVector3f v, GLVector3f a, GLfloat h, GLfloat r, int ttl)
	: GameObject("AllienBullet", p, v, a, h, r), mTimeToLive(ttl)
{
}

/** Copy constructor. */
AllienBullet::AllienBullet(const AllienBullet& b)
	: GameObject(b),
	mTimeToLive(b.mTimeToLive)
{
}

/** Destructor. */
AllienBullet::~AllienBullet(void)
{
}

// PUBLIC INSTANCE METHODS ////////////////////////////////////////////////////

/** Update bullet, removing it from game world if necessary. */
void AllienBullet::Update(int t)
{
	// Update position/velocity
	GameObject::Update(t);
	// Reduce time to live
	mTimeToLive = mTimeToLive - t;
	// Ensure time to live isn't negative
	if (mTimeToLive < 0) { mTimeToLive = 0; }
	// If time to live is zero then remove bullet from world
	if (mTimeToLive == 0) {
		if (mWorld) mWorld->FlagForRemoval(GetThisPtr());
	}

}

bool AllienBullet::CollisionTest(shared_ptr<GameObject> o)
{
	if (o->GetType() == GameObjectType("Life")) return false;
	if (o->GetType() == GameObjectType("PowerUpShield")) return false;
	if (o->GetType() == GameObjectType("AddedShield")) return false;
	if (o->GetType() == GameObjectType("Allien")) return false;
	if (mBoundingShape.get() == NULL) return false;
	if (o->GetBoundingShape().get() == NULL) return false;
	return mBoundingShape->CollisionTest(o->GetBoundingShape());
}

void AllienBullet::OnCollision(const GameObjectList& objects)
{
	mWorld->FlagForRemoval(GetThisPtr());
}
