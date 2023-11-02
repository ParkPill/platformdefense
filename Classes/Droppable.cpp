//
//  Droppable.cpp
//  AlmostLegendary
//
//  Created by SUNG PILL PACK on 13. 6. 26..
//
//

#include "Droppable.h"
#include "GameManager.h"

//bool Droppable::init(){}
Droppable::Droppable()
{
    ignoreGravity = false;
    waterAccelX = 100;
    waterMaxX = 50;
    waterAccelYN = 100;
    waterAccelYP = 90;
    waterMinY = 60;
    inWater = false;
    isTowardLeft = false;
    isFregile = false;
    exitingVehicle = false;
    state = STATE_STANDING;
    xSpeed = 0;
    ySpeed = 0;
    velocity = Point(0,-100);
    jumped = false;
    blinkingTime = 0;
    freezed = false;
    immortal = false;
    centerPosition = Point(0,0);
    currentPlatform = NULL;
    untouchable = false;
    onGround = false;
    wantsToStop = false;
    wantsToGoLeft = false;
    wantsToGoRight = false;
    wantsToJump = false;
    weight = 1;
    target = NULL;
    effectType = 0;
	IsRolling = false;
	energyBar = NULL;
	energyBarContent = NULL;
	extraJump = 0;
//    legAniName = NULL;
//    isTowardLeft = true;
}

void Droppable::init(int unit, int eng, float extraSpd, const char* sptName)
{
//    this->autorelease();
    this->Sprite::initWithSpriteFrameName(sptName);
//    SpriteFrame *frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(sptName);
//    this->setDisplayFrame(frame);
    this->getTexture()->setAliasTexParameters();
    this->energy = eng;
    this->maxEnergy = eng;
    this->setTag(unit);
    this->extraSpeed = extraSpd;
    this->weight = 1;
    this->secondTag = -1;
}

float Droppable::getCurrentY(){
    return getPositionY();
}
Droppable* Droppable::createDroppable(int unit, int eng, float extraSpd, const char* sptName)
{
    Droppable* drop = new Droppable();
    drop->init(unit, eng, extraSpd, sptName);
    return drop;
}

Droppable* Droppable::createDroppable(int unit, int eng, float extraSpd, const char* sptName, const char* animationName)
{
    return Droppable::createDroppable(unit, eng, extraSpd, sptName, animationName, false);
}

Droppable* Droppable::createDroppable(int unit, int eng, float extraSpd, const char* sptName, const char* animationName, bool repeat)
{
    Droppable* drop = Droppable::createDroppable(unit, eng, extraSpd, sptName);
    
    drop->runAnimation(animationName, repeat);
    return drop;
}

void Droppable::showEnergy(){
	if (energyBar == NULL){
		energyBar = Sprite::createWithSpriteFrameName("energyBar.png");
		this->addChild(energyBar);
        if(this->getTag() == TRAP_WALL){
            energyBar->setPosition(Point(this->getContentSize().width / 2, this->getContentSize().height));
        }else{
            energyBar->setPosition(Point(this->getContentSize().width / 2, this->getContentSize().height + 10));
        }
		energyBarContent = Sprite::createWithSpriteFrameName("energyBarGreen.png");
		energyBar->addChild(energyBarContent);
		energyBarContent->setPosition(Point(1, energyBar->getContentSize().height / 2));
		energyBarContent->setAnchorPoint(Point(0, 0.5));
	}
	int length = 8;
	float rate = (float)energy / maxEnergy;
	energyBarContent->setScaleX(length*rate);
	if (rate < 0.3){
		energyBarContent->setSpriteFrame("energyBarRed.png");
	}
	else if (rate < 0.7){
		energyBarContent->setSpriteFrame("energyBarYellow.png");
	}
	else{
		energyBarContent->setSpriteFrame("energyBarGreen.png");
	}
}

void Droppable::setFlippedX(bool flip){
    //Sprite::setFlippedX(flip);
	this->setScaleX((flip ? -1 : 1)*std::abs(this->getScaleX()));
	if (energyBar != NULL){
		energyBar->setScaleX((flip ? -1 : 1)*std::abs(energyBar->getScaleX()));
	}
    isTowardLeft = flip;
}
void Droppable::encounteredCliff(){}
void Droppable::encounteredWall(){}
void Droppable::onLanded(){
    onGround = true;
	oneWayCancelY = 0;
}
void Droppable::updatePosition(float dt)
{
    if (GameManager::getInstance()->getHudLayer()->IsFastMode) {
        dt *= 0.6;
    }
    cocos2d::Point gravity = Point(0.0, inWater?-waterAccelYN:(-850.0 * weight));
    bool toLeft = wantsToStop?false:wantsToGoLeft;
    bool toRight = wantsToStop?false:wantsToGoRight;
    
    if (onGround) {
        exitingVehicle = false;
        jumped = false;
        if (state == STATE_JUMPING) {
            state = STATE_STANDING;
            onLanded();
            return;
        }
    }
    
    if (wantsToGoLeft && !isTowardLeft) {
        setFlippedX(true);
    }else if(wantsToGoRight && isTowardLeft){
        setFlippedX(false);
    }
    
    cocos2d::Point gravityStep = gravity * dt;
    
    if (!wantsToJump && jumped) {
        jumped = false;
//        tumbled = false;
    }
    
   cocos2d::Point vlct = this->velocity + gravityStep;
    
    
    {
        if (wantsToJump && !jumped && onGround) {
			Point jumpForce = Point(0.0, inWater ? (onGround ? waterAccelYP + 10 : waterAccelYP) : 300.0);
            vlct = vlct + jumpForce;
            jumped = true;
            onGround = false;
            wantsToJump = false;
            onJumped();
            if (currentPlatform) {
                currentPlatform->player = NULL;
                currentPlatform = NULL;
            }
        }
        if (this->freezed) {
            vlct = Point(inWater?vlct.x*0.9:vlct.x * (freezed?0.95:0.30), vlct.y);
//            log("jump force: %f", vlct.y);
        }else{
            if (wantsToStop){// && !wantsToGoLeft && !wantsToGoRight) {
                vlct = Point(0, vlct.y);//Point(inWater?vlct.x*0.9:vlct.x * (freezed?0.95:0.30), vlct.y); //2
            }else{
                if (toLeft) {
                    cocos2d::Point backwardMove = Point(inWater?-waterAccelX:-2800.0, 0.0);
                    cocos2d::Point backwardStep = backwardMove * dt; //1
                    vlct = vlct + backwardStep;
                }else if(toRight){
                    cocos2d::Point forwardMove = Point(inWater?waterAccelX:2800.0, 0.0);
                    cocos2d::Point forwardStep = forwardMove * dt; //1
                    vlct = vlct + forwardStep;
                }
            }
        }
    }
//    float jumpCutoff = 150.0;
//    log("On ground: %d", onGround);
    bool fast = GameManager::getInstance()->getHudLayer()->IsFastMode;
    cocos2d::Point minMovement = Point((inWater?-waterMaxX:-16.0-extraSpeed)*(fast?1.5f:1), inWater?-waterMinY:-300.0 - extraJump);
   cocos2d::Point maxMovement = Point((inWater?waterMaxX:16.0+extraSpeed)*(fast?1.5f:1), 300.0 + extraJump);

    vlct = vlct.getClampPoint(minMovement, maxMovement);
    cocos2d::Point stepVelocity = vlct * dt;
    //this->setPosition(ccpAdd(getPosition(), stepVelocity));
    desiredPosition = getPosition() + stepVelocity;
    this->velocity = vlct;
}
void Droppable::updatePositionWithoutAccel(float dt){
	Point gravity = Point(0.0, inWater ? -waterAccelYN : (-850.0 * weight));
	bool toLeft = wantsToStop ? false : wantsToGoLeft;
	bool toRight = wantsToStop ? false : wantsToGoRight;

	if (onGround) {
		exitingVehicle = false;
		jumped = false;
		if (state == STATE_JUMPING) {
			state = STATE_STANDING;
			onLanded();
			return;
		}
	}

	if (wantsToGoLeft && !isTowardLeft) {
		setFlippedX(true);
	}
	else if (wantsToGoRight && isTowardLeft){
		setFlippedX(false);
	}

	Point gravityStep = gravity * dt;

	if (!wantsToJump && jumped) {
		jumped = false;
		//        tumbled = false;
	}

	Point vlct = this->velocity + gravityStep;

	float horizontal = 0;
	if (this->freezed) {
		vlct = Point(inWater ? vlct.x*0.9 : vlct.x * (freezed ? 0.95 : 0.30), vlct.y);
	}
	else{
		if (wantsToJump && !jumped && (onGround)) {
			Point jumpForce = Point(0.0, inWater ? (onGround ? waterAccelYP + 10 : waterAccelYP) : 300.0);
			vlct = vlct + jumpForce;
			jumped = true;
			onGround = false;
			wantsToJump = false;
			onJumped();
			if (currentPlatform) {
				currentPlatform->player = NULL;
				currentPlatform = NULL;
			}
		}

		if (wantsToStop){// && !wantsToGoLeft && !wantsToGoRight) {
			vlct = Point(0, vlct.y);//Point(inWater?vlct.x*0.9:vlct.x * (freezed?0.95:0.30), vlct.y); //2
		}
		else{
			if (toLeft) {
				horizontal = (-50 - extraSpeed)*dt;
			}
			else if (toRight){
				horizontal = (50 + extraSpeed)*dt;
			}
		}
	}
	//    float jumpCutoff = 150.0;
	//    log("On ground: %d", onGround);


	Point minMovement = Point(0, inWater ? -waterMinY : -280.0 - extraJump);
	Point maxMovement = Point(0, 280.0 + extraJump);

	// Clamp allows you limit a hero's speed while Moving
	vlct = vlct.getClampPoint(minMovement, maxMovement);
	Point stepVelocity = vlct * dt;
	desiredPosition = Point(getPosition().x + horizontal, getPosition().y + stepVelocity.y);
	this->velocity = vlct;
}
void Droppable::onJumped(){
    
}
void Droppable::updatePositionForStraight(float dt)
{
   cocos2d::Point vlct;
    float baseSpeed = 50;
    
    vlct = Point((baseSpeed+extraSpeed)*cos(movingAngle*3.14/180),
                 (baseSpeed+extraSpeed)*sin(movingAngle*3.14/180));
    
    cocos2d::Point minMovement = Point(-baseSpeed-extraSpeed, -280.0);
   cocos2d::Point maxMovement = Point(baseSpeed+extraSpeed, 280.0);
    
    // Clamp allows you limit a hero's speed while Moving
    vlct = vlct.getClampPoint(minMovement, maxMovement);
   cocos2d::Point stepVelocity = vlct * dt;

    //this->setPosition(ccpAdd(getPosition(), stepVelocity));
    desiredPosition = getPosition() + stepVelocity;
    this->velocity = vlct;
}

void Droppable::updatePositionToFlyToFollowTarget(float dt)
{
//    baseSpeed+extraSpeed
   cocos2d::Point tPos = target->getPosition();
   cocos2d::Point pos = getPosition();
    float diff = sqrtf((tPos.x - pos.x)*(tPos.x-pos.x)+(tPos.y - pos.y)*(tPos.y - pos.y));
    float toMove = dt*50 + dt*extraSpeed;
    
    float toX = (tPos.x - pos.x)*(toMove/diff);
    float toY = (tPos.y - pos.y)*(toMove/diff);
    desiredPosition = Point(pos.x + toX, pos.y + toY);
    velocity = Point(0, this->getPositionY() >= desiredPosition.y?-1:1);
}

void Droppable::updatePositionForBouncing(float dt)
{
   cocos2d::Point vlct;
    float baseSpeed = 50;
    
   cocos2d::Point gravity = Point(0.0, -850.0);
    if (onGround) {
        gravity = Point(0.0, 0);
        if (state == STATE_JUMPING) {
            state = STATE_STANDING;
        }
    }
   cocos2d::Point gravityStep = gravity * dt;
    
    vlct = this->velocity + gravityStep;
    
   cocos2d::Point minMovement = Point(-baseSpeed-extraSpeed, -280.0);
   cocos2d::Point maxMovement = Point(baseSpeed+extraSpeed, 280.0);
    
    vlct = vlct.getClampPoint(minMovement, maxMovement);
   cocos2d::Point stepVelocity = vlct * dt;
    
    desiredPosition = getPosition() + stepVelocity;
    this->velocity = vlct;
}
void Droppable::addDirt(){
	if (inWater) return;
	int index = cocos2d::random() % 3;
	char buf[20];
	sprintf(buf, "dirt%d.png", index);
	Sprite* sptDirt = Sprite::createWithSpriteFrameName(buf);
	this->getParent()->addChild(sptDirt, 100);
	float dur = 1;
	int diff = 20;
	sptDirt->runAction(Sequence::create(EaseIn::create(MoveBy::create(dur, Point(cocos2d::random() % diff - diff / 2, diff / 2)), 2), CallFuncN::create(CC_CALLBACK_1(Hero::spriteMoveDone, this)), NULL));
	sptDirt->runAction(Sequence::create(DelayTime::create(dur / 2), FadeOut::create(dur / 2), NULL));
	sptDirt->setPosition(this->getPosition() + Point(cocos2d::random() % 15, -1));
}
void Droppable::addJumpingDirt(){
	if (inWater) return;
	Sprite* sptDirt = Sprite::createWithSpriteFrameName("jumpEffect.png");
	int xDiff = 0;
	float dur = 0.5f;
	int angle = 0;
	if (velocity.x > 0){
		xDiff = -12;
		angle = 30;
	}
	else if (velocity.x == 0){
		xDiff = 0;
	}
	else{
		xDiff = 12;
		angle = -30;
	}
	this->getParent()->addChild(sptDirt);
	sptDirt->setRotation(angle);
	sptDirt->runAction(EaseOut::create(MoveBy::create(dur, Point(0, -12)), 3));
	sptDirt->runAction(Sequence::create(EaseOut::create(MoveBy::create(dur, Point(xDiff, 0)), 1.5), CallFuncN::create(CC_CALLBACK_1(Droppable::spriteMoveDone, this)), NULL));
	sptDirt->runAction(Sequence::create(DelayTime::create(dur / 2), FadeOut::create(dur / 2), NULL));
	sptDirt->setPosition(this->getPosition() + Point(5, 20));
}

void Droppable::addLandingDirt(){
	if (inWater) return;
	Sprite* sptDirt;
	for (int i = -20; i <= 20; i += 5) {
		if (i <= 10 && i >= -10) continue;
		if (i == -20 || i == 20) sptDirt = Sprite::createWithSpriteFrameName("dirt0.png");
		if (i == -15 || i == 15) sptDirt = Sprite::createWithSpriteFrameName("dirt1.png");
		this->getParent()->addChild(sptDirt, 100);
		float dur = 1;
		if (i == 20 || i == -20){
			sptDirt->runAction(EaseIn::create(MoveBy::create(dur, Point(0, 8)), 2));
		}
		else{
			sptDirt->runAction(EaseIn::create(MoveBy::create(dur, Point(0, 5)), 4));
		}

		sptDirt->runAction(Sequence::create(EaseOut::create(MoveBy::create(dur, Point(i, 0)), 4), CallFuncN::create(CC_CALLBACK_1(Droppable::spriteMoveDone, this)), NULL));
		sptDirt->runAction(Sequence::create(DelayTime::create(dur / 2), FadeOut::create(dur / 2), NULL));
		sptDirt->setPosition(this->getPosition());
	}
}
void Droppable::spriteMoveDone(Ref* obj){
	Sprite* spt = (Sprite*)obj;
	spt->removeFromParentAndCleanup(true);
}
Rect Droppable::collectBoundingBox()
{
    return Rect(desiredPosition.x - TILE_SIZE/2, desiredPosition.y - getContentSize().height/2, TILE_SIZE, TILE_SIZE*2);
}
Rect Droppable::collisionBoundingBox()
{
    float width = getContentSize().width/2;
    float height = getContentSize().height;
    if (width <= 10) {
        width = TILE_SIZE;
    }
    if (height > 32) {
        return Rect(desiredPosition.x - width/2, desiredPosition.y - height/2, width, 32);
    }
//    return Rect(desiredPosition.x - TILE_SIZE/2, desiredPosition.y - getContentSize().height/2, width, height);//TILE_SIZE, TILE_SIZE);
    return Rect(desiredPosition.x - width/2, desiredPosition.y - height/2, width, height);
	Rect bounding = getBoundingBox();
   
    if (bounding.size.width < TILE_SIZE*2 && bounding.size.height < TILE_SIZE*2) {
        cocos2d::Rect collisionBox = RectInset(bounding, 1, 0);
        cocos2d::Point diff = desiredPosition - getPosition();
        cocos2d::Rect returnBoundingBox = RectOffset(collisionBox, diff.x, diff.y);
        return returnBoundingBox;
    }else{
        cocos2d::Point pos = getPosition();
        cocos2d::Rect rect = Rect(pos.x - TILE_SIZE/2, pos.y - getContentSize().height/2, TILE_SIZE, getContentSize().height);
        cocos2d::Rect collisionBox = RectInset(rect, 0, 0);
        cocos2d::Point diff = desiredPosition - getPosition();
        cocos2d::Rect returnBoundingBox = RectOffset(collisionBox, diff.x, diff.y);
        return returnBoundingBox;
    }
}
Rect Droppable::demageBoundingBox()
{
    cocos2d::Rect bounding = getBoundingBox();
    if(bounding.size.width > 16){
        return Rect(bounding.origin.x + bounding.size.width/2 - 8, bounding.origin.y, 16, bounding.size.height > 32?32:bounding.size.height);
    }
    cocos2d::Rect collisionBox = RectInset(bounding, 3, 3);
//   cocos2d::Point diff = 0;//desiredPosition - getPosition();
//    cocos2d::Rect returnBoundingBox = RectOffset(collisionBox, diff.x, diff.y);
//    return returnBoundingBox;
    return collisionBox;
}

Point Droppable::getGravityPosition()
{
    return Point(getPosition().x, getPosition().y-getContentSize().height/2 + TILE_SIZE/2);
}

/*Rect Droppable::currentBoundingBox()
{
    cocos2d::Rect bounding = boundingBox();
    
    if (vehicleType == VEHICLE_GOLIATH) {
        
       cocos2d::Point pos = getPosition();
        cocos2d::Rect collisionBox = Rect(pos.x - TILE_SIZE, pos.y - 28, TILE_SIZE*2, TILE_SIZE*2);
        return collisionBox;
    }
    
    if (bounding.size.width < TILE_SIZE*2 && bounding.size.height < TILE_SIZE*2) {
        cocos2d::Rect collisionBox = RectInset(bounding, 0, 0);
//       cocos2d::Point diff = ccpSub(desiredPosition, getPosition());
//        cocos2d::Rect returnBoundingBox = RectOffset(collisionBox, diff.x, diff.y);
        return collisionBox;
    }else{
       cocos2d::Point pos = getPosition();
        cocos2d::Rect rect = Rect(pos.x - TILE_SIZE/2, pos.y - TILE_SIZE + 1, TILE_SIZE, TILE_SIZE*2 - 2);
        cocos2d::Rect collisionBox = RectInset(rect, 0, 0);
//       cocos2d::Point diff = ccpSub(desiredPosition, getPosition());
//        cocos2d::Rect returnBoundingBox = RectOffset(collisionBox, diff.x, diff.y);
        return collisionBox;
    }
}*/

Rect Droppable::RectOffset(Rect rect, float x, float y)
{
    return Rect(rect.origin.x + x, rect.origin.y + y, rect.size.width, rect.size.height);
}

Rect Droppable::RectInset(Rect rect, float x, float y)
{
    return Rect(rect.origin.x + x, rect.origin.y + y, rect.size.width - x*2, rect.size.height - y*2);
}

/*void Droppable::setOrder(int order)
{
    orderAction = order;
    if (currentAction != orderAction) {
        if (orderAction == ENEMY_ACTION_MOVE_TO_LEFT) {
            isTowardLeft = true;
            this->setFlippedX(isTowardLeft);
            
            if(unitType == UNIT_SNAIL_BALL){
                this->stopAllActions();
                this->runAction(RepeatForever::create(RotateBy::create(1, isTowardLeft?-360:360)));
            }
        }else if(orderAction == ENEMY_ACTION_MOVE_TO_RIGHT){
            isTowardLeft = false;
            this->setFlippedX(isTowardLeft);
            
            if(unitType == UNIT_SNAIL_BALL){
                this->stopAllActions();
                this->runAction(RepeatForever::create(RotateBy::create(1, isTowardLeft?-360:360)));
            }
        }
        
        switch (unitType) {
            case UNIT_LIZARD_MAN:
                //this->runAnimation("lizardMan");
                break;
                
            default:
                break;
        }
        currentAction = orderAction;
    }
}
*/
void Droppable::setTarget(Droppable* t)
{
    target = t;
}
float Droppable::findTarget(Droppable* tar, float dist)
{
   cocos2d::Point pos = this->getPosition();
   cocos2d::Point tPos = tar->getPosition();
    float distance = sqrtf((pos.x - tPos.x)*(pos.x - tPos.x) + (pos.y - tPos.y)*(pos.y - tPos.y));
    
    if (distance < dist){
        if(!isTargetFound)
            isTargetFound = true;
    }
//    else{
//        if(isTargetFound)
//            isTargetFound = false;
//    }
    return distance;
}

void Droppable::runAnimation(const char* name, bool repeat)
{
    this->stopActionByTag(ACTION_TAG_ANIMATION);
	Animation* animation = AnimationCache::getInstance()->getAnimation(name);
    Animate* animate = Animate::create(animation);
	
    if(repeat)
    {
        RepeatForever* forever =RepeatForever::create(animate);
        forever->setTag(ACTION_TAG_ANIMATION);
        this->runAction(forever);
    }
    else
    {
        animate->setTag(ACTION_TAG_ANIMATION);
        this->runAction(animate);
    }
}

void Droppable::fireAnimationDone(Node* nd)
{
    
}

void Droppable::jumpAnimation()
{
//    isStopped = false;
    state = STATE_JUMPING;
    
}

void Droppable::blinkForAWhile()
{
    if (blinkingTime <= 0) {
        blinkingTime = 0.6f;
		if (!freezed)
			this->schedule(schedule_selector(Droppable::blinking), 0.1f);
    }else{
        blinkingTime = 0.6f;
    }
}

void Droppable::blinking(float dt)
{
    if (this->getColor().g == 115) {
        this->setColor(Color3B(255, 255, 255));
    }else{
        this->setColor(Color3B(255, 115, 115));
    }
    
    blinkingTime -= dt;
    if (blinkingTime < 0) {
        this->unschedule(schedule_selector(Droppable::blinking));
        this->setColor(Color3B(255, 255, 255));
    }
}

void Droppable::setInWater(bool inOrOut)
{
    inWater = inOrOut;
}

void Droppable::runSchedule(){
//    log("Droppable runSchedule");
}

