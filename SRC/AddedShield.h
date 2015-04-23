#ifndef __ADDEDSHIELD_H__
#define __ADDEDSHIELD_H__

#include "GameObject.h"

class Spaceship;

class AddedShield : public GameObject
{
public:
	AddedShield(shared_ptr<Spaceship>objectspaceship);
	~AddedShield(void);

	bool CollisionTest(shared_ptr<GameObject> o);
	void OnCollision(const GameObjectList& objects);
	void Update(int t);
private:
	shared_ptr<Spaceship> spaceship;
};

#endif
