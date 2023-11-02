//
//  Enemies.cpp
//  LegendDaryTheTower
//
//  Created by SUNG PILL PACK on 14. 6. 13..
//
//

#include "Enemies.h"
#include "GameManager.h"
#include "HelloWorldScene.h"
///////////////////////////////////////
// EnemyBase
///////////////////////////////////////
EnemyBase::EnemyBase(){
//    effectType = EFFECT_NONE;
	FireDemage = 0;
	FireTimeLeft = 0;
	FreezeTimeLeft = 0;
	PressedTimeLeft = 0;
	SpikeTimeLeft = 0;
	HandTimeLeft = 0;
	WallLeftTime = 0;
}
void EnemyBase::UpdateSideEffect(){
	if (SptSideEffect->isVisible()){
		if (FireDemage <= 0){
			SptSideEffect->setVisible(false);
			FireTimeLeft = 0;
		}
	}
	else
	{
		if (FireDemage > 0){
			SptSideEffect->setVisible(true);
			SptSideEffect->runAnimation("fire", true);
			SptSideEffect->setScale(2*this->getContentSize().height / SptSideEffect->getContentSize().height);
			//freezed = false;
		}
	}
	
	// Freeze
	if (!freezed && this->getColor() != Color3B::WHITE){
		this->setColor(Color3B::WHITE);
		this->resume();
	}
	else if (freezed && this->getColor() == Color3B::WHITE && PressedTimeLeft <= 0)
	{
		this->setColor(Color3B(130, 120, 250));
		this->pause();
		
		FireDemage = 0;
		FireTimeLeft = 0;
		SptSideEffect->stopAllActions();
		SptSideEffect->setVisible(false);
	}

	// Pressed 
	if (PressedTimeLeft <= 0 && this->getScaleY() != 1){
		this->setScale(this->getScaleX() < 0 ? -1 : 1, 1);
		this->resume();
		this->freezed = false;
		this->setAnchorPoint(Point(0.5, 0.5));
		this->energyBar->setVisible(true);
	}
	else if (PressedTimeLeft > 0 && this->getScaleY() == 1){
		this->setScale(this->getScaleX() < 0 ? -3:3, 5 / this->getContentSize().height);
		this->setAnchorPoint(Point(0.5, 1.1));
		this->pause();
		this->freezed = true;
		FireDemage = 0;
		FireTimeLeft = 0;
		SptSideEffect->stopAllActions();
		SptSideEffect->setVisible(false);
		this->energyBar->setVisible(false);
	}
}
EnemyBase::~EnemyBase(){
    stopSchedule();
}
void EnemyBase::stopSchedule(){
    this->unschedule(schedule_selector(EnemyBase::movingSchedule));
}
void EnemyBase::resumeWalking(float dt){
    wantsToGoLeft = isTowardLeft;
    wantsToGoRight = !isTowardLeft;
}

void EnemyBase::runSchedule(){

    CCLOG("EnemyBase run schedule");
}
void EnemyBase::movingSchedule(float dt){
    
}
void EnemyBase::initEnemy(int unit, int eng, float extraSpd, const char* sptName, int rewardsCoinCount, int goldCoinPossiblity, int howToMoveBeforeFindHero, int howToMoveAfterFindHero, int actionBeforeFindHero, int actionAfterFindHero, int monitorDistance){
    this->init(unit, eng, extraSpd, sptName);
    this->moveTypeBeforeFindHero = howToMoveBeforeFindHero;
    this->moveTypeAfterFindHero = howToMoveAfterFindHero;
    this->currentMoveType = howToMoveBeforeFindHero;
    this->rewards = rewardsCoinCount;
    this->goldPossiblity = goldCoinPossiblity;
    this->monitoringDistance = monitorDistance;
    this->actionTypeBeforeFindHero = actionBeforeFindHero;
    this->actionTypeAfterFindHero = actionAfterFindHero;
    this->isTowardLeft = false;
    this->wantsToGoLeft = this->isTowardLeft;
    this->wantsToGoRight = !this->isTowardLeft;

	SptSideEffect = Droppable::createDroppable(0, 0, 0, "fire0_0.png");
	this->addChild(SptSideEffect);
	SptSideEffect->setVisible(false);
	SptSideEffect->setAnchorPoint(Point(0.5, 0));
	SptSideEffect->setPosition(Point(this->getContentSize().width / 2, 0));
	SptSideEffect->setOpacity(200);
    this->runSchedule();
}

EnemyBase* EnemyBase::createEnemy(int unit, int eng, float extraSpd, const char* sptName, int rewardsCoinCount, int goldCoinPossiblity, int howToMoveBeforeFindHero, int howToMoveAfterFindHero, int monitorDistance)
{
    EnemyBase* enemy = new EnemyBase();
    enemy->initEnemy(unit, eng, extraSpd, sptName, rewardsCoinCount, goldCoinPossiblity, howToMoveBeforeFindHero, howToMoveAfterFindHero, ENEMY_ACTION_NONE, ENEMY_ACTION_NONE, monitorDistance);
    return enemy;
}

EnemyBase* EnemyBase::createEnemy(int unit, int eng, float extraSpd, const char* sptName, int rewardsCoinCount, int goldCoinPossiblity, int howToMoveBeforeFindHero, int howToMoveAfterFindHero, int actionBeforeFindHero, int actionAfterFindHero, int monitorDistance)
{
    EnemyBase* enemy = new EnemyBase();
    enemy->initEnemy(unit, eng, extraSpd, sptName, rewardsCoinCount, goldCoinPossiblity, howToMoveBeforeFindHero, howToMoveAfterFindHero, actionBeforeFindHero, actionAfterFindHero, monitorDistance);
    return enemy;
}

///////////////////////////////////////
// FireableBase
///////////////////////////////////////
FireableBase::FireableBase(){
    fireCoolTimeMax = 1.5f;
    fireCoolTime = fireCoolTimeMax;
}
FireableBase::~FireableBase(){}

///////////////////////////////////////
// WalkableOnPlatformBase
///////////////////////////////////////
WalkableOnPlatformBase::WalkableOnPlatformBase(){}
WalkableOnPlatformBase::~WalkableOnPlatformBase(){}
void WalkableOnPlatformBase::encounteredCliff(){
    setFlippedX(!isFlippedX());
    wantsToGoLeft = isTowardLeft;
    wantsToGoRight = !isTowardLeft;
}
void WalkableOnPlatformBase::encounteredWall(){
    
    setFlippedX(!isFlippedX());
    wantsToGoLeft = isTowardLeft;
    wantsToGoRight = !isTowardLeft;
}

///////////////////////////////////////
// WalkableCrossPlatformBase
///////////////////////////////////////
WalkableCrossPlatformBase::WalkableCrossPlatformBase(){}
WalkableCrossPlatformBase::~WalkableCrossPlatformBase(){}
void WalkableCrossPlatformBase::encounteredWall(){
    setFlippedX(!isFlippedX());
    wantsToGoLeft = isTowardLeft;
    wantsToGoRight = !isTowardLeft;
}

///////////////////////////////////////
// FireableOnPlatformBase
///////////////////////////////////////
FireableOnPlatformBase::FireableOnPlatformBase(){}
FireableOnPlatformBase::~FireableOnPlatformBase(){}
void FireableOnPlatformBase::encounteredCliff(){
    setFlippedX(!isFlippedX());
    wantsToGoLeft = isTowardLeft;
    wantsToGoRight = !isTowardLeft;
}
void FireableOnPlatformBase::encounteredWall(){
    setFlippedX(!isFlippedX());
    wantsToGoLeft = isTowardLeft;
    wantsToGoRight = !isTowardLeft;
}

///////////////////////////////////////
// FireableCrossPlatformBase
///////////////////////////////////////
FireableCrossPlatformBase::FireableCrossPlatformBase(){}
FireableCrossPlatformBase::~FireableCrossPlatformBase(){}
void FireableCrossPlatformBase::encounteredWall(){
    setFlippedX(!isFlippedX());
    wantsToGoLeft = isTowardLeft;
    wantsToGoRight = !isTowardLeft;
}

///////////////////////////////////////
// BlueWorm
///////////////////////////////////////
void BlueWorm::movingSchedule(float dt){

}
void BlueWorm::runSchedule(){
	this->schedule(schedule_selector(BlueWorm::movingSchedule), 0.1);
}
BlueWorm* BlueWorm::create(int unit, int eng, float extraSpd, const char* sptName, int rewardsCoinCount, int goldCoinPossiblity, int howToMoveBeforeFindHero, int howToMoveAfterFindHero, int monitorDistance)
{
	BlueWorm* enemy = new BlueWorm();
	enemy->initEnemy(unit, eng, extraSpd, sptName, rewardsCoinCount, goldCoinPossiblity, howToMoveBeforeFindHero, howToMoveAfterFindHero, ENEMY_ACTION_MOVE, ENEMY_ACTION_MOVE, monitorDistance);
	enemy->wantsToStop = false;
	enemy->runAnimation(sptName, true);
	return enemy;
}
void BlueWorm::encounteredCliff(){
	setFlippedX(this->getScaleX() > 0);
	wantsToGoLeft = isTowardLeft;
	wantsToGoRight = !isTowardLeft;
}
void BlueWorm::encounteredWall(){
	setFlippedX(this->getScaleX() > 0);
	wantsToGoLeft = isTowardLeft;
	wantsToGoRight = !isTowardLeft;
	wantsToJump = true;
}

Missile* Missile::createMissile(int unit, int eng, int angle, float extraSpd, const char* sptName){
	return Missile::createMissile(unit, eng, angle, extraSpd, sptName, false);
}
Missile* Missile::createMissile(int unit, int eng, int angle, float extraSpd, const char* sptName, bool destructable){
	Missile* enemy = new Missile();
	enemy->init(unit, eng, extraSpd, sptName);
	enemy->movingAngle = angle;
	enemy->setRotation(-angle);
	enemy->immortal = !destructable;
	enemy->rewards = 0;
	enemy->goldPossiblity = 0;
	return enemy;
}