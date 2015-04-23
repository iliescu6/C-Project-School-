#include "GameWorld.h"
#include "BoundingSphere.h"
#include "Life.h"

//Public instance contructors

//Constructor
Life::Life() :GameObject("Life"){
	mPosition.x = rand() / 2;
	mPosition.y = rand() / 2;
	mPosition.z = 0.0;
}

Life::Life(GLVector3f p, GLVector3f v, GLfloat h, GLfloat r)
	: GameObject("Life", p, v, GLVector3f(), h, r){

	mRotation = 0;
	mPosition.x = rand() / 2;
	mPosition.y = rand() / 2;
	mPosition.z = 0.0;
}

//Copy constructor
Life::Life(const Life& e) : GameObject(e) {}

//Destructor
Life::~Life(void){}

//Public intance methods

void Life::Update(int t)
{
	//Update position/velocity
	GameObject::Update(t);

	if (mSprite.get() == NULL) return;

}

bool Life::CollisionTest(shared_ptr<GameObject> o)
{
	if (o->GetType() != GameObjectType("Spaceship")) return false;
	if (GetType() == o->GetType()) return false;
	if (mBoundingShape.get() == NULL) return false;
	if (o->GetBoundingShape().get() == NULL) return false;
	return mBoundingShape->CollisionTest(o->GetBoundingShape());
}

void Life::OnCollision(const GameObjectList& objects)
{
	mWorld->FlagForRemoval(GetThisPtr());
}
