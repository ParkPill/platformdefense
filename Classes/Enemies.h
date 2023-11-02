//
//  Enemies.h
//  LegendDaryTheTower
//
//  Created by SUNG PILL PACK on 14. 6. 13..
//
//

#ifndef __LegendDaryTheTower__Enemies__
#define __LegendDaryTheTower__Enemies__


#include "cocos2d.h"
#include "Droppable.h"
using namespace cocos2d;

///////////////////////////
// EnemyBase
///////////////////////////
class EnemyBase : public Droppable
{
private:
    
public:
    virtual void resumeWalking(float dt);
    virtual void movingSchedule(float dt);
    EnemyBase();
    ~EnemyBase();
    void stopSchedule();
    int orderAction;
    int currentAction;
	int ability;
    int currentMoveType;
    int moveTypeBeforeFindHero;
    int moveTypeAfterFindHero;
    int actionTypeBeforeFindHero;
    int actionTypeAfterFindHero;

	int FireDemage;
	float FireTimeLeft;
	float FreezeTimeLeft;
	float PressedTimeLeft;
	float SpikeTimeLeft;
	float HandTimeLeft;
	float WallLeftTime;
	Droppable* SptSideEffect;
	void UpdateSideEffect();
	
    cocos2d::Point wanderStart;
    cocos2d::Point wanderEnd;
    
    int rewards;
//    int effectType;
    void initEnemy(int unit, int eng, float extraSpd, const char* sptName, int rewardsCoinCount, int goldCoinPossiblity, int howToMoveBeforeFindHero, int howToMoveAfterFindHero, int actionBeforeFindHero, int actionAfterFindHero, int monitorDistance);
    virtual void runSchedule();
    
    static EnemyBase* createEnemy(int unit, int eng, float extraSpd, const char* sptName, int rewardsCoinCount, int goldCoinPossiblity, int howToMoveBeforeFindHero, int howToMoveAfterFindHero, int monitorDistance);
    static EnemyBase* createEnemy(int unit, int eng, float extraSpd, const char* sptName, int rewardsCoinCount, int goldCoinPossiblity, int howToMoveBeforeFindHero, int howToMoveAfterFindHero, int actionBeforeFindHero, int actionAfterFindHero, int monitorDistance);
};

///////////////////////////
// FireableBase
///////////////////////////
class FireableBase : public EnemyBase
{
private:
    
public:
    FireableBase();
    ~FireableBase();
    
    float fireCoolTime;
    float fireCoolTimeMax;
    Droppable* missile;
    float missileAngle;
};
///////////////////////////
// WalkableOnPlatformBase
///////////////////////////
class WalkableOnPlatformBase : public EnemyBase
{
private:
    
public:
    WalkableOnPlatformBase();
    ~WalkableOnPlatformBase();
    virtual void encounteredWall();
    virtual void encounteredCliff();
};

///////////////////////////
// WalkableCrossPlatformBase
///////////////////////////
class WalkableCrossPlatformBase : public EnemyBase
{
private:
    
public:
    WalkableCrossPlatformBase();
    ~WalkableCrossPlatformBase();
    virtual void encounteredWall();
};
///////////////////////////
// FireableOnPlatformBase
///////////////////////////
class FireableOnPlatformBase : public FireableBase
{
private:
    
public:
    FireableOnPlatformBase();
    ~FireableOnPlatformBase();
    virtual void encounteredWall();
    virtual void encounteredCliff();
};

///////////////////////////
// FireableCrossPlatformBase
///////////////////////////
class FireableCrossPlatformBase : public FireableBase
{
private:
    
public:
    FireableCrossPlatformBase();
    ~FireableCrossPlatformBase();
    virtual void encounteredWall();
};


///////////////////////////
// BlueWorm
///////////////////////////
class BlueWorm : public WalkableOnPlatformBase
{
private:
    virtual void movingSchedule(float dt);
public:
    virtual void runSchedule();
    static BlueWorm* create(int unit, int eng, float extraSpd, const char* sptName, int rewardsCoinCount, int goldCoinPossiblity, int howToMoveBeforeFindHero, int howToMoveAfterFindHero, int monitorDistance);
	virtual void encounteredWall();
	virtual void encounteredCliff();
};

class Missile : public EnemyBase
{
public:
    Vector<Droppable*> victimArray;
    static Missile* createMissile(int unit, int eng, int angle, float extraSpd, const char* sptName);
    static Missile* createMissile(int unit, int eng, int angle, float extraSpd, const char* sptName, bool destructable);
};
#endif /* defined(__LegendDaryTheTower__Enemies__) */
