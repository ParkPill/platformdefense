//
//  Droppable.h
//  AlmostLegendary
//
//  Created by SUNG PILL PACK on 13. 6. 26..
//
//

#ifndef AlmostLegendary_Droppable_h
#define AlmostLegendary_Droppable_h

#include "cocos2d.h"
#include "MovingPlatform.h"

#define TILE_SIZE 16
#define TILE_WIDTH 16
#define TILE_HEIGHT 16
using namespace cocos2d;

class Droppable : public cocos2d::Sprite
{
private:
    
public:
    
    float waterAccelX ;
    float waterMaxX ;
    float waterAccelYN;
    float waterAccelYP;
    float waterMinY;
    
    Droppable();
    ~Droppable(){}

    cocos2d::Point centerPosition;
    cocos2d::Point velocity;
    cocos2d::Point desiredPosition;
    cocos2d::Rect demageRect;
    Droppable* target;
    MovingPlatform* currentPlatform;
    
    virtual void setFlippedX(bool flip);
    
    int oneWayCancelY;
    int energy;
    int maxEnergy;
    int ySpeed;
    int xSpeed;
    int state;
    bool freezed;
        
    int extraSpeed;
    int extraJump;
    int effectType;
    int movingAngle;
    float weight;
    

    int goldPossiblity;
    float fireGuage;
    float blinkingTime;
    bool isMissile;
    bool isTowardLeft;
    bool exitingVehicle;
    bool inWater;
    bool untouchable;
    bool immortal;
    bool wantsToJump;
    bool wantsToGoLeft;
    bool wantsToGoRight;
    bool wantsToStop;
    int monitoringDistance;
    bool isFregile;
    bool ignoreGravity;
	bool IsRolling;

    bool onGround;
//    bool encounteredWall;
//    bool encounteredCliff;
    bool jumped;
//    void initialize();
    void turnIntoEnemy();
    void setInWater(bool inOrOut);
	virtual void updatePosition(float dt);
	void updatePositionWithoutAccel(float dt);
    void updatePositionForStraight(float dt);
    void updatePositionForBouncing(float dt);
    void updatePositionToFlyToFollowTarget(float dt);
    virtual float getCurrentY();
    void setTarget(Droppable* t);
    bool isTargetFound;
    float findTarget(Droppable* tar, float dist);
    void runAnimation(const char* name, bool repeat);
    void runFireAnimation();
    void fireAnimationDone(Node* nd);
    virtual void onLanded();
    virtual void onJumped();
    void jumpAnimation();
    int secondTag;
    virtual void encounteredWall();
    virtual void encounteredCliff();
    virtual cocos2d::Rect demageBoundingBox();
    virtual cocos2d::Rect collisionBoundingBox();
    virtual cocos2d::Rect collectBoundingBox();

	void addDirt();
	void addLandingDirt();
	void addJumpingDirt();
	void spriteMoveDone(Ref* obj);
    cocos2d::Rect RectOffset(cocos2d::Rect rect, float x, float y);
    cocos2d::Rect RectInset(cocos2d::Rect rect, float x, float y);
    
    void init(int unit, int eng, float extraSpd, const char* sptName);
    static Droppable* createDroppable(int unit, int eng, float extraSpd, const char* sptName);
    static Droppable* createDroppable(int unit, int eng, float extraSpd, const char* sptName, const char* animationName);
    static Droppable* createDroppable(int unit, int eng, float extraSpd, const char* sptName, const char* animationName, bool repeat);

    virtual cocos2d::Point getGravityPosition();
    void blinkForAWhile();
    void blinking(float dt);
	virtual void runSchedule();
	Sprite* energyBar;
	Sprite* energyBarContent;
	void showEnergy();
};

#endif
