#ifndef __POWERUPSHIELD_H__
#define __POWERUPSHIELD_H__

#include "GameObject.h"

class PowerUpShield : public GameObject
{
public:
	PowerUpShield(void);
	~PowerUpShield(void);

	bool CollisionTest(shared_ptr<GameObject> o);
	void OnCollision(const GameObjectList& objects);
};

#endif
