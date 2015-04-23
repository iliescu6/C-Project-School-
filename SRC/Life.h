#ifndef __LIFE_H__
#define __LIFE_H__

#include "GameUtil.h"
#include "GameObject.h"

class Life : public GameObject
{
public:
	Life();
	Life(GLVector3f p, GLVector3f v, GLfloat h, GLfloat r);
	Life(const Life& e);
	virtual ~Life(void);

	virtual void Update(int t);
	bool CollisionTest(shared_ptr<GameObject> o);
	void OnCollision(const GameObjectList& objects);
};

#endif
