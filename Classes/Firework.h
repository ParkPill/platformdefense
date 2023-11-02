//
//  Firework.h
//  AlmostLegendary
//
//  Created by SUNG PILL PACK on 13. 6. 29..
//
//

#ifndef AlmostLegendary_Firework_h
#define AlmostLegendary_Firework_h

#include "cocos2d.h"

#include "Droppable.h"
#include "cocostudio/CocoStudio.h"
#include "base/CCEventListenerController.h"
#include "base/CCEventController.h"
#include "base/CCController.h"

using namespace cocos2d;

using namespace cocos2d::ui;
using namespace cocostudio;
class Firework : public Layer
{
private:
	SpriteBatchNode* spriteBatch;
	int nextFire;
	int minExplodePower;
	int maxExplodePower;
    bool isRandomColor;
    float fireScale;
	Point startPoint;
	Size size;
	float interval;
	float intervalVarious;
	float totalTime;
	int power;
	int powerVarious;
	Vector<Sprite*> firstFire;
	Vector<Sprite*> secondFire;
	Vector<Sprite*> thirdFire;
    const char* sptName;
public:
	

    virtual bool init();
    CREATE_FUNC(Firework);
    static Firework* create(const char* spriteName, float fireScale, bool isRandomColor);
	void fireworkSchedule(float dt);
	void spriteMoveDone(Node* node);
	
};
class FireParticle : public Sprite
{
public:
    const char* sptName;
	FireParticle();
	void initFire(const char* name);
	Point velocity;
	float weight;
	int minColor;
	static FireParticle* create(const char* name);
	void updatePosition(float dt);
	void spriteMoveDone(Node* node);
};
class FireMass : public FireParticle
{
private:

public:
	void initFireMass();
	static FireMass* create(const char* name);

	void creatingParticle(float dt);
	void spriteMoveDone(Node* node);
};

class FireRocket : public FireMass
{

private:

public:
    bool isRandomColor;
	int explodePower;
	int explodePowerVarious;
	int particleCount;
	int particleCountVarious;
	FireRocket();
	static FireRocket* create(const char* name);

	void setTimer(float time);
	void explodeFire(float dt);

};
#endif
