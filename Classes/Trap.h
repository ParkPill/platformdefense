#ifndef __TRAP_H__
#define __TRAP_H__

#include "cocos2d.h"
#include "Enemies.h"
#define TRAPTYPE_FLOOR 0
#define TRAPTYPE_BEHIND 1
#define TRAPTYPE_CEILING 2
#define TRAPTYPE_CORNER 3


#define TRAP_SPIKE 0
#define TRAP_FREEZER 1
#define TRAP_WALL 2
#define TRAP_WALLSPIKE 3
#define TRAP_FLASK 4 
#define TRAP_GASBURNER 5
#define TRAP_TOPSPIKE 6
#define TRAP_BOMB 7
#define TRAP_ICEBALL 8
#define TRAP_MACEBALL 9
#define TRAP_FIRE 10
#define TRAP_HAND 11
#define TRAP_CACTUS 12

#define MISSILE_INDIAN 912
#define MISSILE_JONNY 913
#define MISSILE_AOKIZI 914
#define MISSILE_HERMIONE 915

#define EFFECT_FIRE 0
#define EFFECT_POISON 1
#define EFFECT_ICE 2
#define EFFECT_STUN 3
#define EFFECT_SLOW 4
#define MONITOR_HORIZONTAL 0
#define MONITOR_VERTICAL 1
#define MONITOR_CUSTOM 2

using namespace cocos2d;
class Trap : public Droppable
{
private:
    
public:
	virtual cocos2d::Rect GetTargetRect();
	cocos2d::Point TilePos;
    cocos2d::Rect TargetRect;
	int Level;
	int PriceValue;
	float Cooltime;
	float CooltimeLeft;
	float EffectTime;
	float EffectTimeLeft;
	bool IsReady;
	int TrapType;
	int TrapCategory;
	EnemyBase* Target;
    virtual bool init();
	CREATE_FUNC(Trap);

	virtual void OnFire();

	void SetAttackRect(int x, int y, int width, int height);
    void init(const char* normalSprite, float coolTime, int type, int category);
	virtual void OnPlaced();
	virtual void Update(float dt);
	virtual int GetPower();
	void spriteMoveDone(Sprite* spt);
};

class GasBurner : public Trap
{
private:
	Droppable* _fire0;
	Droppable* _fire1;
	Droppable* _fire2;
public:
	static GasBurner* create(const char* normalSprite, float coolTime);
	void AddFire();
	virtual void OnPlaced();
	virtual int GetPower();
};

class Freezer : public Trap
{
private:
public:
	static Freezer* create(const char* normalSprite, float coolTime);
	virtual void OnPlaced();
	virtual void OnFire();
	virtual int GetPower();
	void ReturnToNormal(float dt);
};

class Fire : public Trap
{
private:
	Droppable* _fire1;
	Droppable* _fire2;
public:
	static Fire* create(const char* normalSprite, float coolTime);
	void AddFire();
	virtual void OnPlaced();
	virtual int GetPower();
};


class Spike : public Trap
{
private:
	
public:
	static Spike* create(const char* normalSprite, float coolTime);
	virtual void OnPlaced();
	virtual int GetPower();
};

class TopSpike : public Trap
{
private:

public:
	static TopSpike* create(const char* normalSprite, float coolTime);
	virtual int GetPower();
	virtual void OnFire();
	void ShowSpikeAgain(float dt);
};
class IceBall : public Trap
{
private:

public:
	static IceBall* create(const char* normalSprite, float coolTime);
	virtual int GetPower();
};
class Bomb : public Trap
{
private:

public:
	static Bomb* create(const char* normalSprite, float coolTime);
	void OnFire();
	virtual int GetPower();
};
class MaceBall : public Trap
{
private:

public:
	virtual cocos2d::Rect GetTargetRect();
	static MaceBall* create(const char* normalSprite, float coolTime);
	virtual int GetPower();
};
class Flask : public Trap
{
private:

public:
	static Flask* create(const char* normalSprite, float coolTime);
	virtual void OnPlaced();
	virtual int GetPower();
};
class Wall : public Trap
{
private:

public:
	static Wall* create(const char* normalSprite, float coolTime);
	virtual int GetPower();
};
class Hand : public Trap
{
private:

public:
	static Hand* create(const char* normalSprite, float coolTime);
	virtual void OnFire();
	virtual int GetPower();
};
class WallSpike : public Trap
{
private:
public:
	bool IsLeft;
	Sprite* SptSpike;
	Sprite* SptLegs;
	static WallSpike* create(const char* normalSprite, float coolTime);
	virtual void OnFire();
	virtual int GetPower();
	void SetDirection(bool isLeft);
};
class Cactus : public Trap
{
private:
public:
    static Cactus* create(const char* normalSprite, float coolTime);
    virtual int GetPower();
};
#endif  // __TRAP_H__