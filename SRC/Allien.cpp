#include "GameUtil.h"
#include "GameWorld.h"
#include "Allien.h"
#include "BoundingSphere.h"

using namespace std;

// PUBLIC INSTANCE CONSTRUCTORS ///////////////////////////////////////////////

/**  Default constructor. */
Allien::Allien()
	: GameObject("Allien")
{
	mPosition.x = rand() / 2;
	mPosition.y = rand() / 2;
	mPosition.z = 0.0;
}

/** Construct a spaceship with given position, velocity, acceleration, angle, and rotation. */
Allien::Allien(GLVector3f p, GLVector3f v, GLVector3f a, GLfloat h, GLfloat r)
	: GameObject("Allien", p, v, a, h, r), mThrust(0)
{
}

/** Copy constructor. */
Allien::Allien(const Allien& s)
	: GameObject(s), mThrust(0)
{
}

/** Destructor. */
Allien::~Allien(void)
{
}

// PUBLIC INSTANCE METHODS ////////////////////////////////////////////////////

/** Update this allien */
void Allien::Update(int t)
{
	int distance = ((allienShip->GetPosition().x - mPosition.x)*(allienShip->GetPosition().x - mPosition.x)) + ((allienShip->GetPosition().y - mPosition.y)*(allienShip->GetPosition().y - mPosition.y));
	distance = sqrt(distance);
	mAngle = (atan2f(mPosition.y - allienShip->GetPosition().y, mPosition.x - allienShip->GetPosition().x) * 180 / 3.14f) - 180.0f;
	mVelocity.x = 10.0*cos(DEG2RAD*mAngle);
	mVelocity.y = 10.0*sin(DEG2RAD*mAngle);
	mVelocity.z = 0;
	if (distance < 120){
		mVelocity.x = 20.0*cos(DEG2RAD*mAngle);
		mVelocity.y = 20.0*sin(DEG2RAD*mAngle);
		mVelocity.z = 0.0;
		AllienShoot();
	}
	if (distance < 60){
		mVelocity.x = 30.0*cos(DEG2RAD*mAngle);
		mVelocity.y = 30.0*sin(DEG2RAD*mAngle);
		mVelocity.z = 0.0;
		AllienShoot();
	}
	if (distance < 40){
		mVelocity.x = 40.0*cos(DEG2RAD*mAngle);
		mVelocity.y = 40.0*sin(DEG2RAD*mAngle);
		mVelocity.z = 0.0;
		AllienShoot();
	}
	// Call parent update function
	GameObject::Update(t);
}

/** Render this spaceship. */
void Allien::Render(void)
{
	if (mSpaceshipShape.get() != NULL) mSpaceshipShape->Render();

	// If ship is thrusting
	if ((mThrust > 0) && (mThrusterShape.get() != NULL)) {
		mThrusterShape->Render();
	}

	GameObject::Render();
}


/** Set the rotation. */
void Allien::Rotate(float r)
{
	mRotation = r;
}

/** Shoot a bullet. */
void Allien::AllienShoot(void)
{
	// Check the world exists
	if (!mWorld) return;
	// Construct a unit length vector in the direction the spaceship is headed
	GLVector3f spaceship_heading(cos(DEG2RAD*mAngle), sin(DEG2RAD*mAngle), 0);
	spaceship_heading.normalize();
	// Calculate the point at the node of the spaceship from position and heading
	GLVector3f bullet_position = mPosition + (spaceship_heading * 4);
	// Calculate how fast the bullet should travel
	float bullet_speed = 30;
	// Construct a vector for the bullet's velocity
	GLVector3f bullet_velocity = mVelocity + spaceship_heading * bullet_speed;

}

bool Allien::CollisionTest(shared_ptr<GameObject> o)
{
	if (o->GetType() == GameObjectType("Life")) return false;
	if (o->GetType() == GameObjectType("PowerUpShield")) return false;
	if (o->GetType() == GameObjectType("SmallAsteroid")) return false;
	if (o->GetType() == GameObjectType("Asteroid")) return false;
	if (o->GetType() == GameObjectType("AllienBullet")) return false;
	if (GetType() == o->GetType()) return false;
	if (mBoundingShape.get() == NULL) return false;
	if (o->GetBoundingShape().get() == NULL) return false;
	return mBoundingShape->CollisionTest(o->GetBoundingShape());


}

void Allien::OnCollision(const GameObjectList &objects)
{
	mWorld->FlagForRemoval(GetThisPtr());
}