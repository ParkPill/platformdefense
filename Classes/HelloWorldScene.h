#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "Droppable.h"
#include "Hero.h"
#include "Enemies.h"
#include "MovingPlatform.h"
#include "HudLayer.h"
#include "Trap.h"

#define MAX_DROP_SPEED -10

#define HERO_INDIAN 0
#define HERO_JONNY 1
#define HERO_AOKIZI 2
#define HERO_HERMIONE 3
#define HERO_DARY 4

#define HERO_BREE 5
#define HERO_TOM 6
#define HERO_KAREN 7
#define HERO_KARL 8
#define HERO_ANDREW 9
using namespace cocos2d;

class HelloWorld : public cocos2d::Layer
{
private:
    Sprite* _sptTarget;
    float _targetGap;
    cocos2d::Point  _viewPoint;
    float _scale;
    bool _trapTurorial;
    int _checkPointStageCount;
    bool _isGamePaused;
	bool _isFastMode;
	int _guidingTrapType;
	bool _isDown;
	bool _isActoinDone;
    int _playerType;
	void ShootingForHold(float dt);
    void showTutorial(const char* msg);
    Node* _messageBox;
	HudLayer* _hudLayer;
    int _shootTutorialCount;
    bool _shootTutorialStarted;
    bool isTouchEnded;
    int shootFilter;
protected:
    void updateIndicators();
    Vector<Sprite*> indicatorArray;
    Vector<Droppable*> indicatedArray;

    
    float missileEffectCollapsedTime;
    Vector<Droppable*> enemyMissileArray;
    Vector<EnemyBase*> flyingSwimingEnemyArray;
    Vector<Droppable*> droppableArray;
    Vector<Droppable*> straightMovingArray;
    Vector<Droppable*> customMovingArray;
    Vector<Droppable*> chasingMissileArray;
    Vector<Droppable*> droppableMissileArray;
    Vector<Droppable*> droppableCoinArray;
    std::vector<int> _enemyToCreate;
    
    Vector<Sprite*> teleportArray;
    Vector<EnemyBase*> enemiesToRemove;
  

	PointArray* heroPositionArray;
    int extraCritical;
    int extraPower;
    int extraFruit;
    void sacrificeLater(float dt);
    void sacrificeDone(Ref* obj);
	void splashDemage(cocos2d::Point pos, int radius, int demage);
	bool IsOneWay(int id);
	bool IsBlock(int id);
	


	float _totalTime;
	int _earnedCoin;
	int _initCoinCount;
	
//    int getEpisodeIndex();
//    void episodeActionNeeded();
//    void episodeActionFinished();
//    int episodeActionIndex;
//    void showEpisode();
//    EpisodeLayer* episodeLayer;
//    void episodeDone();
//    Droppable* episodeNpc;
    
    Sprite* blackSquare;
    cocos2d::Size size;
    Vector<Droppable*> coinArray;
    Vector<Droppable*> starArray;
    Vector<EnemyBase*> enemyArray;
    Vector<Trap*> trapArray;
    Vector<Droppable*> heroMissileArray;
    Vector<MovingPlatform*> movingPlatformArray;
    Vector<Droppable*> helperArray;
    Vector<Droppable*> inWaterArray;
    Vector<FireableBase*> fireableArray;
    Vector<Droppable*> spiderArray;
    
    Vector<Droppable*> coinToRemove;
    Sprite* nearBack;
    Sprite* farBack;
    Sprite* furtherBack;
	
	Droppable* theDoor;
    Droppable* theKey;
    Vector<Droppable*>talkArray;
    Vector<Label*> talkLabelArray;
    float indestructableTime;
    int playerWeaponType;
	
//    int playerMissileDemage;
    
    int currentTalkIndex;
    int starCount;
    float coinDelay;
    float otherDelay;
    bool bulletWasted;
    cocos2d::Point  deadPoint;
    bool guidedMissile;
    Droppable* findTarget(int distance);
    Sprite* activityIndicator;
    int criticalLevel;
    int furtherWidth;
    int furtherHeight;
    int farWidth;
    int farHeight;
    int nearWidth;
    int nearHeight;

	int _waveCount;
	float _nextCallWaitTime;
	bool _isEnemyCreated;
	bool _isClear;
	cocos2d::Point _spawnPoint;
	float _nextEnemyTime;
	int _weapon;
	int _stageMode;
	int _stageNumber;
	int _lastStrongestEnemy;

	void Shoot(cocos2d::Point location);
	void CreatingEnemies(float dt);
	void WaveSchedule(float dt);
    void solveCollision(Droppable* p, cocos2d::Point  pos, cocos2d::Rect rect);
    void dungeonWaveSchedule(float dt);
public:
    int coinSpent;
	int CollectedCoinCount;
	cocos2d::Point StartPos;
	cocos2d::Point EndPos;
    cocos2d::Point  _touchStart;
	cocos2d::Point _touchPoint;
    bool isTouchStarted;
    float playerFireCoolTime;
    // Method 'init' in cocos2d-x returns bool, instead of 'id' in cocos2d-iphone (an object pointer)
    virtual bool init();
    Hero* player;
    Vector<Hero*> friends;
//    bool leftPressed;
//    bool rightPressed;
//    bool firePressed;
//    bool jumpPressed;
	int killedEnemyCount;
    bool isGameOver;
    bool keyCollected;
    int mastery;
	Node* CreateNumberSprite(int number);
	void SaveTrap(cocos2d::Point pos, int trapType, int level);
	Trap* LoadTrap(cocos2d::Point pos);
    void angelReviveLater(float dt);
    void cloudSchedule(float dt);
    Vector<Sprite*> cloudArray;
    int enemyKilledCount;
    int enemyMaxCount;
    void rateResult(Node* node);
    void revive();
	float checkBottom(Droppable* p);
	float checkTop(Droppable* p);
    bool isSetStageDone;
	void AddTrap(cocos2d::Point pos, int trapType, int trapLevel, bool isFree);
    void saveTotalEnemyKill();
    cocos2d::Point  positionToTeleport;
    void teleportLater(float dt);
    void teleportLaterForThemeThree(float dt);
    bool isTeleporting;
    bool playerIgnoreGravity;
    Sprite* getTeleport(int teleport);
    // there's no 'id' in cpp, so we recommend to return the class instance pointer
    static cocos2d::Scene* scene(int stage);
    SpriteBatchNode* spriteBatch;
//    SpriteBatchNode* coinSpriteBatch;
    SpriteBatchNode* backgroundSpriteBatch;
    // a selector callback
    void menuCloseCallback(Ref* pSender);
	
    void removeUsedAssets();
    EventListenerTouchOneByOne* listener;
    EventListenerTouchAllAtOnce* multitouchlistener;
    HelloWorld();
//    ~HelloWorld();
    // preprocessor macro for "static create()" constructor ( node() deprecated )
    CREATE_FUNC(HelloWorld);
    void unscheduleAll();
	void setViewPointCenter(cocos2d::Point position);
	void setViewPoint(float x, float y);
	void registerScriptAccelerateHandler(int handler);
    double getAngle(cocos2d::Point pos1, cocos2d::Point  pos2);
    void setPlayerPosition(cocos2d::Point position);
    void gravityUpdate(float dt);
    void gravityUpdateForCoins(float dt);
    void gravityUpdateForStraight(float dt);
    void gravityUpdateForCustomMoving(float dt);
    bool hitDary();
    void useBomb();
    void usePotion();
    int demagingUnit;
    void doTheBombToMissiles(float dt);
    void doTheBombToEnemies(float dt);
    Vector<Droppable*> bombTargetMissileArray;
    int bombTargetEnemyIndex;
    Vector<EnemyBase*> bombTargetEnemyArray;
//    void gravityUpdateForOthers(float dt);
//    void gravityUpdateForOnlyCheckCollision(float dt);
    void gravityUpdateForFlyingOrSwimingEnemies(float dt);
    void gravityUpdateForWaterEnemies(float dt);
    void chasingMissileUpdate(float dt);
    void missileEffectUpdate(float dt);
    void coinUpdate(float dt);
    void enemyUpdate(float dt);
    void missileUpdate(float dt);
    void talkUpdate(float dt);
    
    void helperAndTrapUpdate(float dt);
    void createRewards(EnemyBase* enemy);
    bool keyExist(ValueMap map, const char* key);
    
    float powerTestTime;
    void powerTestSchedule(float dt);
    float powerPerSec;
    float totalPower;
    void blinkForeLayer(float dt);
    void coinMagnet();
	bool isPaused;
    bool isVideoRewardAttack;
    bool everySecond;
	
	void SellTrap(cocos2d::Point tilePos);
	int GetTrapLevel(cocos2d::Point tilePos);
	int GetTrapType(cocos2d::Point tilePos);
	Trap* GetTrap(cocos2d::Point tilePos);
	void UpgradeTrap(cocos2d::Point tilePos);
	void OnTrapPlaced(Sprite* spt);

    cocos2d::TMXTiledMap *tileMap;
    cocos2d::TMXLayer *stageLayer;
    cocos2d::TMXLayer *unitLayer;
	cocos2d::TMXLayer *guideLayer;
    cocos2d::Point  tileCoordForPosition(cocos2d::Point position);
    cocos2d::Rect tileRectFromTileCoords(cocos2d::Point tileCoords);
    int getEnergyForWave(int wave, int enemyIndex);
    void continueGame();
    void onContinueGameClosed();
    void checkForAndResolveCollisions(Droppable* p);
    void checkForAndResolveCollisionsForOthers(Droppable* p);
    void checkForAndResolveCollisionsForWater(Droppable* p);
    void checkForAndResolveCollisionsForBouncing(Droppable* p);
    Droppable* checkForAndResolveCollisionsForMissile(Droppable* p);
    bool checkDroppableMissileCollision(Droppable* p);
    void changePlayerState(int state);
    void setStage(int stageNumber);
    
    void fire();
    void enemyFireLoop(float dt);

//    void fireLoop(float dt);
    void missileMoveDone(Ref* obj);
    void spriteMoveDone(Ref* obj);
    void coinWaitDone(Ref* obj);
    void enemyBaseMoveDone(Ref* obj);
    void fireBulletMoveDone(Ref* drop);
    void nodeMoveDone(Ref* obj);
    void runEffect(int effect, cocos2d::Point  point);
    void addGlowEffect(Sprite* sprite,const Color3B& colour, const cocos2d::Size& size);
    void gameClearAnimationDone(Ref* obj);
    void gameOver();
    void shakeScreen();
    void shakeScreenEnd(float dt);
    void setPlayerInfo();
    void setWeaponInfo(int weapon);
    void jumpBySpring(float dt);
    float blinkingTime;
    void blinkingPlayer(float dt);
    void immortalForAWhile(float dt);
    cocos2d::Rect RectInset(cocos2d::Rect rect, float x, float y);
    void removeEnemy(EnemyBase* spt);
    void unfreezePlayer(float dt);
    void stepBackForHit(float dt);
    void pauseLayer();
    void resumeLayer();
    void refreshPlayerInfo(int costum, int weapon);
    void smokeForVehicle();
    cocos2d::Rect intersection(cocos2d::Rect source, cocos2d::Rect rect);
//    virtual void draw();
    void dialogFinished(Ref* obj);
    virtual void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* unused_event);
    cocos2d::Point  waterBoyPos;
    bool isWaterBoy;
    bool isWaterBoyFlip;
    float waterBoyAngle;
    Sprite* background;
    void showCoinCount(cocos2d::Point pos, int count);
    void onUpdateResultComplete();
    void onUpdateResultErrorComplete(char const *status, char const *error);
    void onUpdateUserComplete();
    void onUpdateUserErrorComplete(char const *status, char const *error);
    bool containsTouchLocation(Touch* touch);
    void touchBegan(cocos2d::Point location);
    void touchMoved(cocos2d::Point location);
    void touchEnded(cocos2d::Point location);
    virtual bool onTouchBegan(Touch *touch, Event *unused_event);
    virtual void onTouchMoved(Touch *touch, Event *unused_event);
    virtual void onTouchEnded(Touch *touch, Event *unused_event);
    void onTouchesBegan(const std::vector<Touch*>& touches, Event *unused_event);
    void onTouchesMoved(const std::vector<Touch*>& touches, Event *unused_event);
    void onTouchesEnded(const std::vector<Touch*>& touches, Event *unused_event);
    void loadWaveSchedule();
    void startWaveSchedule();
    void resetTouchStart(float dt);
    void addListener();
	void showGuide(int type);
	void changeSpeed();
	void RemoveTrapRecord(cocos2d::Point tilePos);
	bool HitEnemy(EnemyBase* enemy, int demage, int criticalPercent, bool bounceBack);
	void BlinkGuideLayer(float dt);
	bool IsOccupied(int category, cocos2d::Point  pos);
	std::string GetCoordinateKey(cocos2d::Point pos);
	void LoadPreviousTraps();
	void ResetPreviousTraps();
    
    void onTutorialClosed();

    int getAchievementTotal(int i);
	void Dispose();
};

#endif // __HELLOWORLD_SCENE_H__
