#include <stdlib.h>
#include "GameUtil.h"
#include "AddedShield.h"
#include "Spaceship.h"
#include "BoundingShape.h"

AddedShield::AddedShield(shared_ptr<Spaceship> objectspaceship) : GameObject("AddedShield")
{
	spaceship = objectspaceship;
	mPosition = objectspaceship->GetPosition();

}

void AddedShield::Update(int t)
{
	mPosition = spaceship->GetPosition();
	GameObject::Update(t);
}

AddedShield::~AddedShield(void)
{
}

bool AddedShield::CollisionTest(shared_ptr<GameObject> o)
{
	if (o->GetType() != GameObjectType("Asteroid") && o->GetType() != GameObjectType("SmallAsteroid")
		&&o->GetType() != GameObjectType("Allien")&&o->GetType() != GameObjectType("AllienBullet")) return false;
	if (GetType() == o->GetType()) return false;
	if (mBoundingShape.get() == NULL) return false;
	if (o->GetBoundingShape().get() == NULL) return false;
	return mBoundingShape->CollisionTest(o->GetBoundingShape());
}

void AddedShield::OnCollision(const GameObjectList& objects)
{
	mWorld->FlagForRemoval(GetThisPtr());
}
