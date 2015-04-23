#ifndef __ALLIEN_H__
#define __ALLIEN_H__

#include "GameUtil.h"
#include "GameObject.h"
#include "Shape.h"

class Allien : public GameObject
{
public:
	Allien();
	Allien(GLVector3f p, GLVector3f v, GLVector3f a, GLfloat h, GLfloat r);
	Allien(const Allien& s);
	virtual ~Allien(void);

	virtual void Update(int t);
	virtual void Render(void);
	virtual void Rotate(float r);
	virtual void AllienShoot(void);

	void SetAllienShape(shared_ptr<Shape> allien_ship) { mSpaceshipShape = allien_ship; }
	void SetBulletShape(shared_ptr<Shape> bullet_shape) { mBulletShape = bullet_shape; }
	void SetAllien(shared_ptr<GameObject>ptr){ allienShip = ptr; }

	bool CollisionTest(shared_ptr<GameObject> o);
	void OnCollision(const GameObjectList &objects);

private:
	float mThrust;

	shared_ptr<Shape> mSpaceshipShape;
	shared_ptr<Shape> mThrusterShape;
	shared_ptr<Shape> mBulletShape;
	shared_ptr<GameObject> allienShip;
};

#endif