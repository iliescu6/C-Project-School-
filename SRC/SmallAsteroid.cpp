#include <stdlib.h>
#include "GameUtil.h"
#include "SmallAsteroid.h"
#include "BoundingShape.h"

SmallAsteroid::SmallAsteroid(void) : GameObject("SmallAsteroid")
{
	mAngle = rand() % 360;
	mRotation = rand() % 90;
	mVelocity.x = 10.0 * cos(DEG2RAD*mAngle);
	mVelocity.y = 10.0 * sin(DEG2RAD*mAngle);
	mVelocity.z = 0.0;
}

SmallAsteroid::~SmallAsteroid(void)
{
}

bool SmallAsteroid::CollisionTest(shared_ptr<GameObject> o)
{
	if (o->GetType() == GameObjectType("Asteroid")) return false;
	if (o->GetType() == GameObjectType("PowerUpShield")) return false;
	if (o->GetType() == GameObjectType("Life")) return false;
	if (o->GetType() == GameObjectType("Allien")) return false;
	if (GetType() == o->GetType()) return false;
	if (mBoundingShape.get() == NULL) return false;
	if (o->GetBoundingShape().get() == NULL) return false;
	return mBoundingShape->CollisionTest(o->GetBoundingShape());
}

void SmallAsteroid::OnCollision(const GameObjectList& objects)
{
	mWorld->FlagForRemoval(GetThisPtr());
}
