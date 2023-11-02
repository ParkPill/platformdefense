//
//  Trap.cpp
//  AlmostLegendary
//
//  Created by SUNG PILL PACK on 13. 7. 15..
//
//

#include "Trap.h"

using namespace cocos2d;

// on "init" you need to initialize your instance
bool Trap::init()
{
    bool bRet = false;
    do
    {
        CC_BREAK_IF(! Sprite::init());
		PriceValue = 0;
        bRet = true;
    } while (0);
	
    return bRet;
}
void Trap::init(const char* normalSprite, float coolTime, int type, int category)
{
    this->initWithSpriteFrameName(normalSprite);
	this->setTag(type);
	this->schedule(schedule_selector(Trap::update), 0.1f);
	this->TrapType = type;
	this->TrapCategory = category;
	this->Cooltime = coolTime;
	this->IsReady = false;
	this->CooltimeLeft = coolTime + (cocos2d::random()%10)*0.1f;
	this->EffectTimeLeft = 0;
	this->Level = 0;
	this->IsRolling = false;
}
Rect Trap::GetTargetRect(){
	return TargetRect;
}
void Trap::OnFire(){
	CooltimeLeft = Cooltime;
	IsReady = false;
}
void Trap::SetAttackRect(int x, int y, int width, int height){
	TargetRect = Rect(x, y, width, height);
}
void Trap::Update(float dt){
	if (EffectTimeLeft > 0){
		EffectTimeLeft -= dt;
	}
	if (CooltimeLeft > 0){
		CooltimeLeft -= dt;
		if (CooltimeLeft <= 0)
		{
			IsReady = true;
		}
	}
}
int Trap::GetPower(){
	return 1;
}
void Trap::OnPlaced(){
	this->runAction(Sequence::create(EaseOut::create(ScaleTo::create(0.2f, this->getScaleX()*1.3f, this->getScaleY()*1.3f), 2),
		EaseIn::create(ScaleTo::create(0.5f, this->getScaleX() * 1, this->getScaleY()), 2),
		CallFunc::create(CC_CALLBACK_0(Droppable::addLandingDirt, this)), NULL));
}
// GasBurner
GasBurner* GasBurner::create(const char* normalSprite, float coolTime){
	GasBurner* trap = new GasBurner();
	trap->init(normalSprite, coolTime, TRAP_GASBURNER, TRAPTYPE_FLOOR);
	trap->AddFire();
	return trap;
}
void GasBurner::AddFire(){
	_fire0 = Droppable::createDroppable(0, 1, 0, "fire0_0.png");
	_fire0->runAnimation("fire", true);
	this->addChild(_fire0);
	_fire0->setAnchorPoint(Point(0.5f, 0));
	_fire0->setPosition(Point(this->getContentSize().width / 2, this->getContentSize().height / 2 + TILE_SIZE / 2 - 4));

	_fire1 = Droppable::createDroppable(0, 1, 0, "fire0_0.png");
	_fire1->runAnimation("fire", true);
	this->addChild(_fire1);
	_fire1->setAnchorPoint(Point(0.5f, 0));
	_fire1->setPosition(Point(this->getContentSize().width / 2 - 5, this->getContentSize().height / 2 + TILE_SIZE / 2 - 4));

	_fire2 = Droppable::createDroppable(0, 1, 0, "fire0_0.png");
	_fire2->runAnimation("fire", true);
	this->addChild(_fire2);
	_fire2->setAnchorPoint(Point(0.5f, 0));
	_fire2->setPosition(Point(this->getContentSize().width / 2 + 5, this->getContentSize().height / 2 + TILE_SIZE / 2 - 4));
}

void GasBurner::OnPlaced(){
	Trap::OnPlaced();
	_fire0->runAnimation("fire", true);
	_fire1->runAnimation("fire", true);
	_fire2->runAnimation("fire", true);
}

// Freezer
Freezer* Freezer::create(const char* normalSprite, float coolTime){
	Freezer* trap = new Freezer();
	trap->init(normalSprite, coolTime, TRAP_FREEZER, TRAPTYPE_BEHIND);
	trap->EffectTime = 1.5f;
	return trap;
}

void Freezer::OnPlaced(){
	Trap::OnPlaced();
}
void Freezer::OnFire(){
	this->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("fridgeOpen.png"));
	this->scheduleOnce(schedule_selector(Freezer::ReturnToNormal), EffectTime);
	CooltimeLeft = Cooltime;
	EffectTimeLeft = EffectTime;
	for (int i = 0; i < 10; i++){
		Sprite* sptIcePart = Sprite::createWithSpriteFrameName("whiteExplode.png");
		sptIcePart->setColor(Color3B(237, 234, 255));
		this->getParent()->addChild(sptIcePart);
		sptIcePart->setPosition(this->getPosition() + Point(this->getContentSize().width / 2 + cocos2d::random() % 7 - 14, 5 + cocos2d::random() % 3 - 4));
		sptIcePart->setScale(0.3f);
		sptIcePart->setOpacity(50);
		sptIcePart->runAction(Sequence::create(DelayTime::create(i*0.08f), ScaleTo::create(1, 0.6f), FadeOut::create(1.0f), CallFuncN::create(CC_CALLBACK_1(Droppable::spriteMoveDone, this)), NULL));
		//sptIcePart->runAction(FadeIn::create(1));
		
		sptIcePart->runAction(Sequence::create(DelayTime::create(i*0.08f), EaseIn::create(MoveBy::create(3.5f, Point(cocos2d::random() % 10 - 19, cocos2d::random() % 3 - 30)), 2), NULL));
	}
}
void Freezer::ReturnToNormal(float dt){
	this->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("fridge.png"));
	IsReady = false;
}

// Fire
Fire* Fire::create(const char* normalSprite, float coolTime){
	Fire* trap = new Fire();
	trap->init(normalSprite, coolTime, TRAP_FIRE, TRAPTYPE_BEHIND);
	trap->AddFire();
	return trap;
}
void Fire::AddFire(){
	this->setScaleY(2);
	this->setScaleX(1.2f);
	this->setAnchorPoint(Point(0.5, 0));
	_fire1 = Droppable::createDroppable(0, 1, 0, "fire0_0.png");
	this->addChild(_fire1);
	_fire1->setAnchorPoint(Point(0.5f, 0));

	_fire2 = Droppable::createDroppable(0, 1, 0, "fire0_0.png");
	this->addChild(_fire2);
	_fire2->setAnchorPoint(Point(0.5f, 0));
}

void Fire::OnPlaced(){
	Trap::OnPlaced();
	this->runAnimation("fire", true);
	this->setPosition(this->getPosition() + Point(0, -TILE_SIZE / 2 - 2));
	_fire1->runAnimation("fire", true);
	_fire1->setPosition(Point(this->getContentSize().width / 2 - 3, -1));
	_fire2->runAnimation("fire", true);
	_fire2->setPosition(Point(this->getContentSize().width / 2 + 3, -1));
}
// Spike
Spike* Spike::create(const char* normalSprite, float coolTime){
	Spike* trap = new Spike();
	trap->init(normalSprite, coolTime, TRAP_SPIKE, TRAPTYPE_BEHIND);
	trap->IsReady = true;
	return trap;
}

void Spike::OnPlaced(){
	Trap::OnPlaced();
}
// TopSpike
TopSpike* TopSpike::create(const char* normalSprite, float coolTime){
	TopSpike* trap = new TopSpike();
	trap->init(normalSprite, coolTime, TRAP_TOPSPIKE, TRAPTYPE_CEILING);
	return trap;
}
void TopSpike::OnFire(){
	CooltimeLeft = Cooltime;
	IsReady = false;
	this->scheduleOnce(schedule_selector(TopSpike::ShowSpikeAgain), 0.6f);
	this->setVisible(false);
}
void TopSpike::ShowSpikeAgain(float dt){
	this->setVisible(true);
}
// IceBall
IceBall* IceBall::create(const char* normalSprite, float coolTime){
	IceBall* trap = new IceBall();
	trap->init(normalSprite, coolTime, TRAP_ICEBALL, TRAPTYPE_CEILING);
	
	float x;float y;Sprite* spt;
	float duration = 0.8f;
	for (int i = 0; i < 10; i++){
		x = cocos2d::random() % 10 - 4;
		y = cocos2d::random() % 10 - 4 + 4;
		spt = Sprite::createWithSpriteFrameName("iceBall.png");
		trap->addChild(spt);
		Point pos = Point(trap->getContentSize().width / 2 + x, trap->getContentSize().height / 2 + y);
		spt->setPosition(pos);
		spt->setOpacity(200);
		duration = 0.5f + (cocos2d::random() % 10)*0.1f;
		if (i % 2 == 0){
			spt->runAction(RepeatForever::create(Sequence::create(
				EaseOut::create(MoveBy::create(duration, Point(TILE_SIZE / 4, 0)), 2),
				EaseIn::create(MoveBy::create(duration, Point(-TILE_SIZE / 4, 0)), 2),
				EaseOut::create(MoveBy::create(duration, Point(-TILE_SIZE / 4, 0)), 2),
				EaseIn::create(MoveBy::create(duration, Point(TILE_SIZE / 4, 0)), 2), NULL)));
			spt->runAction(RepeatForever::create(Sequence::create(
				EaseIn::create(MoveBy::create(duration, Point(0, -TILE_SIZE / 4)), 2),
				EaseOut::create(MoveBy::create(duration, Point(0, -TILE_SIZE / 4)), 2),
				EaseIn::create(MoveBy::create(duration, Point(0, TILE_SIZE / 4)), 2),
				EaseOut::create(MoveBy::create(duration, Point(0, TILE_SIZE / 4)), 2), NULL)));
		}
		else{
			spt->runAction(RepeatForever::create(Sequence::create(
				EaseOut::create(MoveBy::create(duration, Point(-TILE_SIZE / 4, 0)), 2),
				EaseIn::create(MoveBy::create(duration, Point(TILE_SIZE / 4, 0)), 2),
				EaseOut::create(MoveBy::create(duration, Point(TILE_SIZE / 4, 0)), 2),
				EaseIn::create(MoveBy::create(duration, Point(-TILE_SIZE / 4, 0)), 2), NULL)));
			spt->runAction(RepeatForever::create(Sequence::create(
				EaseIn::create(MoveBy::create(duration, Point(0, -TILE_SIZE / 4)), 2),
				EaseOut::create(MoveBy::create(duration, Point(0, -TILE_SIZE / 4)), 2),
				EaseIn::create(MoveBy::create(duration, Point(0, TILE_SIZE / 4)), 2),
				EaseOut::create(MoveBy::create(duration, Point(0, TILE_SIZE / 4)), 2), NULL)));
		}
	}
	return trap;
}


// Bomb
Bomb* Bomb::create(const char* normalSprite, float coolTime){
	Bomb* trap = new Bomb();
	trap->init(normalSprite, coolTime, TRAP_BOMB, TRAPTYPE_BEHIND);
	trap->CooltimeLeft = -1;
	trap->EffectTimeLeft = 1000.2f;
	trap->EffectTime = 1.2f;
	trap->IsReady = true;
	
	return trap;
}

void Bomb::OnFire(){
	runAnimation("bomb", true);
	this->EffectTimeLeft = EffectTime;
}
// MaceBall
MaceBall* MaceBall::create(const char* normalSprite, float coolTime){
	MaceBall* trap = new MaceBall();
	trap->init(normalSprite, coolTime, TRAP_MACEBALL, TRAPTYPE_CORNER);
	trap->IsReady = true;
	return trap;
}

Rect MaceBall::GetTargetRect(){
	if (IsRolling){
		return this->collectBoundingBox();
	}
	else{
		return TargetRect;
	}
}
// Flask
Flask* Flask::create(const char* normalSprite, float coolTime){
	Flask* trap = new Flask();
	trap->init(normalSprite, coolTime, TRAP_FLASK, TRAPTYPE_CEILING);
	trap->energy = trap->GetPower();
	return trap;
}

// Wall
Wall* Wall::create(const char* normalSprite, float coolTime){
	Wall* trap = new Wall();
	trap->init(normalSprite, coolTime, TRAP_WALL, TRAPTYPE_BEHIND);
	trap->energy = 30;
	trap->maxEnergy = trap->energy;
	return trap;
}

// Hand
Hand* Hand::create(const char* normalSprite, float coolTime){
	Hand* trap = new Hand();
	trap->init(normalSprite, coolTime, TRAP_HAND, TRAPTYPE_BEHIND);
	trap->EffectTime = 0.3f;
	return trap;
}
void Hand::OnFire(){
	this->runAction(Sequence::create(ScaleTo::create(0.1f, 1.3f), DelayTime::create(0.9f), ScaleTo::create(0.6f, 1), NULL));
	CooltimeLeft = Cooltime;
	EffectTimeLeft = EffectTime;
	runAnimation("hand", false);
}

// WallSpike
WallSpike* WallSpike::create(const char* normalSprite, float coolTime){
	WallSpike* trap = new WallSpike();
	trap->init(normalSprite, coolTime, TRAP_WALLSPIKE, TRAPTYPE_CORNER);

trap->EffectTime = 0.5f;
	trap->SptLegs = Sprite::createWithSpriteFrameName("wallSpikeLegs.png");
	trap->SptLegs->setAnchorPoint(Point(0, 0.5));
	trap->SptLegs->setPosition(Point(trap->getContentSize().width / 2 - TILE_SIZE / 2, trap->getContentSize().height / 2));
	trap->addChild(trap->SptLegs);

	trap->SptSpike = Sprite::createWithSpriteFrameName("wallSpike.png");
	trap->SptSpike->setAnchorPoint(Point(0, 0.5));
	trap->SptSpike->setPosition(Point(trap->getContentSize().width / 2 - TILE_SIZE / 2, trap->getContentSize().height / 2));
	trap->addChild(trap->SptSpike);
	return trap;
}
void WallSpike::SetDirection(bool isLeft){
	IsLeft = isLeft;
	float gap = 3;
	SptLegs->setAnchorPoint(Point(isLeft?0:1, 0.5));
	SptLegs->setPosition(Point(getContentSize().width / 2 + (TILE_SIZE / 2)*(isLeft?-1:1), getContentSize().height / 2));
	float retractScale = gap / SptLegs->getContentSize().width;
	SptLegs->setScale(retractScale, 1);
	//TargetRect = 
	SptSpike->setAnchorPoint(Point(isLeft ? 0 : 1, 0.5));
	SptSpike->setPosition(Point(getContentSize().width / 2 + (TILE_SIZE / 2)*(isLeft ? -1 : 1) + (isLeft ? gap : -gap), getContentSize().height / 2));
	if (isLeft){
		SptSpike->setFlippedX(true);
	}
}
void WallSpike::OnFire(){
	float gap = 3;
	SptSpike->runAction(Sequence::create(MoveTo::create(0.1f, Point(getContentSize().width / 2 + (TILE_SIZE / 2)*(IsLeft ? -1 : 1)  + SptLegs->getContentSize().width*(IsLeft ? 1 : -1), getContentSize().height / 2)),
										DelayTime::create(0.7f), 
										MoveTo::create(1.4f, Point(getContentSize().width / 2 + (TILE_SIZE / 2)*(IsLeft ? -1 : 1) + (IsLeft ? gap : -gap), getContentSize().height / 2)), NULL));
	float retractScale = gap / SptLegs->getContentSize().width;
	SptLegs->setScale(retractScale);
	SptLegs->runAction(Sequence::create(ScaleTo::create(0.1f, 1, 1),
										DelayTime::create(0.7f),
										ScaleTo::create(1.4f, retractScale, 1), NULL));
	CooltimeLeft = Cooltime;
	EffectTimeLeft = EffectTime;
}
void Flask::OnPlaced(){

}

// Cactus
Cactus* Cactus::create(const char* normalSprite, float coolTime){
    Cactus* trap = new Cactus();
    trap->init(normalSprite, coolTime, TRAP_CACTUS, TRAPTYPE_BEHIND);
    trap->IsReady = true;
    return trap;
}



// Power
int GasBurner::GetPower(){
	return Level + 2;
}
int Freezer::GetPower(){
	return 1 + Level * 2;
}
int Fire::GetPower(){
	return Level * 1.5f + 3;
}
int Spike::GetPower(){
	return Level * 2.5f + 3;
}
int TopSpike::GetPower(){
	return Level * 1.5f + 2;
}
int IceBall::GetPower(){
	return Level * 1.0f + 1;
}
int Bomb::GetPower(){
	return (Level + 1) * 6.3f + 3;
}
int Flask::GetPower(){
	return (Level * 10 + 1) * 16.3f + 3;
}
int Hand::GetPower(){
	return (Level + 1) * 1.3f + 3;
}
int WallSpike::GetPower(){
	return Level * 3.3f + 8;
}
int MaceBall::GetPower(){
	return (Level + 1) * 2.3f + 3;
}
int Wall::GetPower(){
	return (Level + 1);
}
int Cactus::GetPower(){
    return (Level + 10);
}