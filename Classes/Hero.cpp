//
//  Hero.cpp
//  LegendDaryTwo
//
//  Created by SUNG PILL PACK on 14. 7. 9..
//
//

#include "Hero.h"
#include "GameManager.h"
Hero* Hero::createHero(int unit, int eng, float extraSpd){
    Hero* hero = new Hero();
    hero->autorelease();
	hero->init(unit, eng, extraSpd, "hero15_0.png");
    hero->getTexture()->setAliasTexParameters();
    hero->energy = eng;
    hero->maxEnergy = eng;
    hero->setTag(unit);
    hero->extraSpeed = extraSpd;
    hero->weight = 1;
    hero->fireCoolTime = 0;
    hero->isFootStepStarted = false;
    hero->shieldCount = 0;
    hero->sptShield = NULL;
	hero->setAnchorPoint(Point(0.5f, 0));
	hero->gunFlipPos = Point(15, 5);
	hero->gunPos = Point(3, 5);
    if(unit == HERO_DARY){
        hero->changedWeapon = 0;
        hero->schedule(schedule_selector(Hero::changeWeapon), 0.2f);
    }
    return hero;
}
void Hero::updatePosition(float dt)
{
    Droppable::updatePosition(dt);
    if (onGround) {
        tumbled = false;
    }
}
void Hero::onJumped(){
    GameManager::getInstance()->playSoundEffect(SOUND_FOOT_STEP);
    GameManager::getInstance()->playSoundEffect(SOUND_FOOT_STEP);

    isHatUp = false;
   
    stopAndStand();
    addJumpingDirt();

   
}
void Hero::startFootStep(){
    if (!isFootStepStarted) {
        isFootStepStarted = true;
        this->schedule(schedule_selector(Hero::stepSound), 0.2, kRepeatForever, 0.08);
    }
}
void Hero::stopFootStep(){
    if(isFootStepStarted){
        isFootStepStarted = false;
        this->unschedule(schedule_selector(Hero::stepSound));
    }
}

void Hero::stepSoundOnce(float dt){
    GameManager::getInstance()->playSoundEffect(SOUND_FOOT_STEP);
}
void Hero::stepSound(float dt){
    GameManager::getInstance()->playSoundEffect(SOUND_FOOT_STEP);
    addDirt();
}
void Hero::hatCheck(float dt){
    if (!isHatUp && velocity.y < -1.5) {
        isHatUp = true;
    }
}
void Hero::setShield(){
    shieldCount = 2;
    sptShield = Sprite::createWithSpriteFrameName("shield_circle0.png");
    Animation* animation = AnimationCache::getInstance()->getAnimation("shield");
    Animate* animate = Animate::create(animation);
    RepeatForever* forever =RepeatForever::create(animate);
    forever->setTag(ACTION_TAG_ANIMATION);
    sptShield->runAction(forever);
    
    this->addChild(sptShield, 100);
    sptShield->setPosition(Point(5, 13));
}
void Hero::demageShield(){
    shieldCount--;
    if (shieldCount == 1) {
        sptShield->runAction(RepeatForever::create(Sequence::create(EaseIn::create(FadeOut::create(0.4), 2), EaseOut::create(FadeIn::create(0.4), 2), NULL)));
    }
    if (shieldCount <= 0) {
        if (sptShield) {
            sptShield->stopAllActions();
            sptShield->removeFromParent();
            sptShield = NULL;
        }
    }
}
void Hero::onLanded(){
    stopAnimationAndStand();
    oneWayCancelY = 0;
    Droppable::onLanded();
    
    isHatUp = false;
     
    addLandingDirt();
    if(velocity.x != 0){
        startWalkingAnimation();
    }
    GameManager::getInstance()->playSoundEffect(SOUND_FOOT_STEP);
    GameManager::getInstance()->playSoundEffect(SOUND_FOOT_STEP);
    this->scheduleOnce(schedule_selector(Hero::stepSoundOnce), 0.04);
}
void Hero::getInTheVehicle(int vType, Sprite* vhc)
{
    //    Size size = Director::getInstance()->getWinSize();
    if (isTowardLeft) {
        vhc->setFlippedX(isTowardLeft);
    }
}
void Hero::getOutTheVehicle()
{
}

void Hero::updatePositionForSuperBoard(float dt, bool jump, bool left, bool right)
{
    Point position = this->getPosition();
    if (left && !isTowardLeft) {
        isTowardLeft = true;
        this->setFlippedX(isTowardLeft);
    }else if(right && isTowardLeft){
        isTowardLeft = false;
        this->setFlippedX(isTowardLeft);
    }
    
    Point gravity = Point(0.0, inWater?-waterAccelYN:-850.0);
    if (onGround) {
//        gravity = Point(0.0, 0);
        exitingVehicle = false;
//        if (state == STATE_JUMPING) {
//            state = STATE_STANDING;
//            stopAnimationAndStand();
//            landed();
//        }

        jumped = false;
    }
    
    Point gravityStep = gravity * dt;
    
    Point vlct = this->velocity + gravityStep;
    
    
    float jumpCutoff = 150.0;
    
    if (jump) {
		if (wantsToJump && !jumped && (onGround || inWater)) {
			Point jumpForce = Point(0.0, inWater ? (onGround ? waterAccelYP + 10 : waterAccelYP) : 1850.0);
			vlct = vlct + jumpForce;

			wantsToJump = false;
			if (currentPlatform) {
				currentPlatform->player = NULL;
				currentPlatform = NULL;
			}
		}
        onGround = false;
        jumped = true;
        if (currentPlatform) {
            currentPlatform->player = NULL;
            currentPlatform = NULL;
        }
    } else if (!jump && vlct.y > jumpCutoff) {
        
        // sppark: This allows you to jump to different height depending how long a player hold the jump button.
        //this->velocity = Point(this->velocity.x, jumpCutoff);
    }
//    if(jumped && !jump) jumped = false;
	if (left) {
		Point backwardMove = Point(inWater ? -waterAccelX : -2800.0, 0.0);
		Point backwardStep = backwardMove * dt; //1
		vlct = (vlct + backwardStep);
	}
	else if (right){
		Point forwardMove = Point(inWater ? waterAccelX : 2800.0, 0.0);
		Point forwardStep = forwardMove * dt; //1
		vlct = vlct + forwardStep;
	}
    
    Point minMovement;
    Point maxMovement;
	minMovement = Point(inWater ? -waterMaxX : -50.0 - extraSpeed, inWater ? -waterMinY : -180.0 - extraJump);
	maxMovement = Point(inWater ? waterMaxX : 50.0 + extraSpeed, 180.0 + extraJump);
    
    // Clamp allows you limit a hero's speed while Moving
    vlct = vlct.getClampPoint(minMovement, maxMovement);
    Point stepVelocity = vlct * dt;
    
    //this->setPosition(ccpAdd(getPosition(), stepVelocity));
    desiredPosition = getPosition() + stepVelocity;
    this->velocity = vlct;
}
void Hero::setRestOfBodyPart(){
   

}
void Hero::runHatCheck(){
    this->schedule(schedule_selector(Hero::hatCheck), 0.1);
}
void Hero::changeWeapon(float dt){
    const char* weaponName = "heroWeapon0_0.png";
    changedWeapon++;
    if(changedWeapon >= 5){
        changedWeapon = 0;
    }
    int weaponType = changedWeapon;
    if(weaponType == HERO_INDIAN){
        gunFlipPos = Point(15, 4);
        gunPos = Point(3, 4);
    }else if(weaponType == HERO_JONNY){
        weaponName = "spike.png";
        gunFlipPos = Point(12, 3);
        gunPos = Point(5, 5);
    }else if(weaponType == HERO_AOKIZI){
        weaponName = "iceBall.png";
        gunFlipPos = Point(11, 4);
        gunPos = Point(6, 5);
    }else if(weaponType == HERO_HERMIONE){
        weaponName = "fire0_0.png";
        gunFlipPos = Point(10, 4);
        gunPos = Point(7, 5);
    }else if(weaponType == HERO_DARY){
        
    }
    gun->setTag(weaponType);
    setWeaponPos();
    //    SpriteFrame* frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(name);
    gun->setSpriteFrame(weaponName);
    
//    gun->setRotation(0);
    gun->stopAllActions();
    if(weaponType == HERO_INDIAN){
        
    }else if(weaponType == HERO_JONNY){
//        gun->setRotation(180);
    }else if(weaponType == HERO_HERMIONE){
        gun->runAnimation("fire", true);
    }
    
}
void Hero::setWeaponPos(){
    int weaponType = gun->getTag();
    if(weaponType == HERO_INDIAN){
        gunFlipPos = Point(16, 4);
        gunPos = Point(4, 4);
    }else if(weaponType == HERO_JONNY){
        gunFlipPos = Point(12, 4);
        gunPos = Point(5, 4);
    }else if(weaponType == HERO_AOKIZI){
        gunFlipPos = Point(18, 4);
        gunPos = Point(6, 4);
    }else if(weaponType == HERO_HERMIONE){
        gunFlipPos = Point(13, 7);
        gunPos = Point(8, 7);
    }else if(weaponType == HERO_DARY){
        
    }
}
void Hero::setWeapon(int weaponType){
    const char* weaponName = "heroWeapon0_0.png";
    if(weaponType == HERO_INDIAN){
    }else if(weaponType == HERO_JONNY){
        weaponName = "spike.png";
    }else if(weaponType == HERO_AOKIZI){
        weaponName = "iceBall.png";
    }else if(weaponType == HERO_HERMIONE){
        weaponName = "fire0_0.png";
    }else if(weaponType == HERO_DARY){
        
    }else if(weaponType == HERO_BREE){
        weaponName = "heroWeapon2_0.png";
    }else if(weaponType == HERO_TOM){
        weaponName = "heroWeapon13_0.png";
    }else if(weaponType == HERO_KAREN){
        weaponName = "heroWeapon0_0.png";
    }else if(weaponType == HERO_KARL){
        weaponName = "heroWeapon3_0.png";
    }else if(weaponType == HERO_ANDREW){
        weaponName = "heroWeapon11_0.png";
    }
//    SpriteFrame* frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(name);
    gun = Droppable::createDroppable(0, 0, 0, weaponName);
    gun->setTag(weaponType);
	this->addChild(gun);
    
    setWeaponPos();
    
    if(weaponType == HERO_INDIAN){
    }else if(weaponType == HERO_JONNY){
//        gun->setRotation(180);
    }else if(weaponType == HERO_HERMIONE){
        gun->runAnimation("fire", true);
    }
    
//    if (frame) {
//        gun->setSpriteFrame(frame);
//    }else{
//        log("Weapon image NULL!");
//    }
}

void Hero::setFlippedX(bool flip){
//    this->armatureBody->setScaleX(flip?-1:1);
    Droppable::setFlippedX(flip);
//    armatureBody->setPosition(Point(flip?5:6, 5));
    gun->setAnchorPoint(Point(flip?1:0, 0.5));
   
//	gun->setFlippedX(flip);
    
    if(flip){
        
        gun->setPosition(gunFlipPos);
    }else{
       
        gun->setPosition(gunPos);
    }
    if(flip){
//        gunFlipPos.x -= 1;
//        gunPos.x -= 1;
    }else{
//        gunFlipPos.x += 1;
//        gunPos.x += 1;
    }
    setWeaponPos();
}
void Hero::setGun(int gunType)
{
    
}

void Hero::tumbleAnimation()
{
    state = STATE_JUMPING;
	
	this->runAction(RotateBy::create(0.5, isTowardLeft ? -360 : 360));
	GameManager::getInstance()->playSoundEffect(SOUND_SPRING);
}
Point Hero::getGravityPosition()
{
    return this->Droppable::getGravityPosition();
}
Rect Hero::demageBoundingBox()
{
    return Rect(getPosition().x, getPosition().y, 10, 20);
    //    Rect bounding = boundingBox();
    Point pos = getPosition();
   
    Rect bounding = Rect(getPosition().x - 5, getPosition().y - 5, 10, 25);
//    Rect collisionBox = RectInset(Rect(bounding.origin.x + 2, bounding.origin.y + 4, 12, 18), 1, 0);
//    Point diff = desiredPosition - getPosition();
//    Rect returnBoundingBox = RectOffset(collisionBox, diff.x, diff.y);
//    return returnBoundingBox;
    return bounding;
}
void Hero::setAppleWorm(){
  
}
void Hero::appleWormBlink(float dt){
   
}
void Hero::appleWormDone(float dt){
   
}
float Hero::getCurrentY(){
   
    return getPositionY();
}
Rect Hero::collisionBoundingBox()
{
    return Rect(desiredPosition.x-4, desiredPosition.y, 10, TILE_SIZE*1.5);
    /*
    Rect bounding = this->getBoundingBox();
    if (vehicleType == VEHICLE_GOLIATH) {
        Point pos = getPosition();
        Rect collisionBox = Rect(pos.x - TILE_SIZE, pos.y - 24, TILE_SIZE*2, TILE_SIZE*2);
        Point diff = desiredPosition - getPosition();
        Rect returnBoundingBox = RectOffset(collisionBox, diff.x, diff.y);
        return returnBoundingBox;
    }else if (vehicleType == CHARACTER_BROOM) {
        Point pos = getPosition();
        Rect collisionBox = Rect(pos.x - TILE_SIZE + 2, pos.y - TILE_SIZE + 3, TILE_SIZE*2 - 4, TILE_SIZE*2 - 4);
        Point diff = desiredPosition - getPosition();
        Rect returnBoundingBox = RectOffset(collisionBox, diff.x, diff.y);
        return returnBoundingBox;
    }
    
//    Rect collisionBox = RectInset(Rect(bounding.origin.x, bounding.origin.y, 16, 26), 1, 0);
    Rect collisionBox = RectInset(bounding, 2, 1);
    Point diff = desiredPosition - getPosition();
    Rect returnBoundingBox = RectOffset(collisionBox, diff.x, diff.y);
    return returnBoundingBox;*/
}
void Hero::runFireAnimation()
{
    gun->stopAllActions();
    if(gun->isFlippedX()){
        gun->setPosition(gunFlipPos);
        gun->runAction(Sequence::create(MoveBy::create(0.02, Point(1, 0)), MoveBy::create(0.1, Point(-1, 0)), NULL));
    }else{
        gun->setPosition(gunPos);
        gun->runAction(Sequence::create(MoveBy::create(0.02, Point(-1, 0)), MoveBy::create(0.1, Point(1, 0)), NULL));
    }
}
void Hero::startWalkingAnimation()
{
  
    this->stopAnimationAndStand();
     state = STATE_WALKING;
    
  
}
void Hero::stopAnimationAndStand()
{
    
    state = STATE_STANDING;
}

void Hero::stopAndStand(){
    
    wantsToGoLeft = false;
    wantsToGoRight = false;
    stopAnimationAndStand();
}
void Hero::goLeft(){
    wantsToGoLeft = true;
}
void Hero::goRight(){
    wantsToGoRight = true;
}
void Hero::jump(){
    wantsToJump = true;
}
void Hero::setColorTo(Color3B color){
}