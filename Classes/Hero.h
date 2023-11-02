//
//  Hero.h
//  LegendDaryTwo
//
//  Created by SUNG PILL PACK on 14. 7. 9..
//
//

#ifndef __LegendDaryTwo__Hero__
#define __LegendDaryTwo__Hero__

#include "cocos2d.h"
#include "Droppable.h"

using namespace cocos2d;

class Hero : public Droppable
{
public:
    cocos2d::Point leftArmFlipPos;
    cocos2d::Point rightArmFlipPos;
    cocos2d::Point faceFlipPos;
    cocos2d::Point gunFlipPos;
    cocos2d::Point leftArmPos;
    cocos2d::Point rightArmPos;
    cocos2d::Point leftLegPos;
    cocos2d::Point rightLegPos;
    cocos2d::Point bodyPos;
    cocos2d::Point bodyFlipPos;
    cocos2d::Point facePos;
    cocos2d::Point gunPos;
    Droppable* gun;
    float originalExtraSpeed;

    float fireCoolTime;
    cocos2d::Point bodyPoint;
    cocos2d::Point bodyPointFlip;
    bool doubleJump;
    bool tumbled;
    int weapon;
    bool isHatUp;
    void hatCheck(float dt);
    virtual void setFlippedX(bool flip);
    virtual void updatePosition(float dt);
    virtual float getCurrentY();
    virtual cocos2d::Rect collisionBoundingBox();
    virtual cocos2d::Rect demageBoundingBox();
    virtual cocos2d::Point getGravityPosition();
    void getInTheVehicle(int vType, Sprite* vhc);
    void getOutTheVehicle();
    void updatePositionForSuperBoard(float dt, bool jump, bool left, bool right);
    void setRestOfBodyPart();
    void setGun(int gunType);
    void tumbleAnimation();
    void runHatCheck();
    int shieldCount;
    Sprite* sptShield;
    void setShield();
    void demageShield();
    static Hero* createHero(int unit, int eng, float extraSpd);
    void runFireAnimation();
    void setAppleWorm();
    void appleWormBlink(float dt);
    void appleWormDone(float dt);
    bool isFootStepStarted;
    void startFootStep();
    void stopFootStep();
    void stepSoundOnce(float dt);
    void changeWeapon(float dt);
    int changedWeapon;
    void setWeaponPos();
    void stepSound(float dt);
    virtual void onJumped();
    virtual void onLanded();
    void startWalkingAnimation();
    void stopAnimationAndStand();
    void stopAndStand();
    void goLeft();
    void goRight();
    void jump();
    void setWeapon(int weaponType);
    void setColorTo(Color3B color);
};
#endif /* defined(__LegendDaryTwo__Hero__) */
