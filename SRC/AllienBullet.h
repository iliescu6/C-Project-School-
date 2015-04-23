#ifndef __ALLIENBULLET_H__
#define __ALLIENBULLET_H__

#include "GameUtil.h"
#include "GameObject.h"

class AllienBullet : public GameObject
{
public:
	AllienBullet();
	AllienBullet(GLVector3f p, GLVector3f v, GLVector3f a, GLfloat h, GLfloat r, int ttl);
	AllienBullet(const AllienBullet& b);
	virtual ~AllienBullet(void);

	virtual void Update(int t);

	void SetTimeToLive(int ttl) { mTimeToLive = ttl; }
	int GetTimeToLive(void) { return mTimeToLive; }

	bool CollisionTest(shared_ptr<GameObject> o);
	void OnCollision(const GameObjectList& objects);

protected:
	int mTimeToLive;
};

#endif