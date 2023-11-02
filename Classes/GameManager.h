#ifndef GameManager_h
#define GameManager_h

#include "cocos2d.h"
#include "HudLayer.h"
#include "HelloWorldScene.h"
//#include "GoogleBilling.h"

#include "ui/UITextBMFont.h"
#include "ShopLayer.h"
#include "PluginIAP/PluginIAP.h"
#include "json/document.h"

using namespace cocos2d;
using namespace cocos2d::ui;


#define MAX_LEVEL 100

#define STATE_STANDING 0
#define STATE_WALKING 1
#define STATE_JUMPING 2
#define STATE_FALLING 3

#define ENEMY_MOVE_STAND 0
#define ENEMY_MOVE_WANDER_ON_A_PLATFORM 1
#define ENEMY_MOVE_WANDER_CROSS_PLATFORM 2
#define ENEMY_MOVE_WANDER_CROSS_PLATFORM_CRASH_BY_WALL 3
#define ENEMY_MOVE_WANDER_ON_CEILING 4
#define ENEMY_MOVE_WANDER_ON_WALL 5
#define ENEMY_MOVE_HANG 6
#define ENEMY_MOVE_DROP 7
#define ENEMY_MOVE_DROP_CRASH 8
#define ENEMY_MOVE_FLY_TO_HERO 9
#define ENEMY_MOVE_FLY_TO_HERO_HORIZONTALLY 10
#define ENEMY_MOVE_FLY_TO_HERO_FOLLOW 11
#define ENEMY_MOVE_FLY_TO_RIGHT 12
#define ENEMY_MOVE_FLY_TO_DOWN 13
#define ENEMY_MOVE_FLY_TO_LEFT 14
#define ENEMY_MOVE_FLY_TO_UP 15
#define ENEMY_MOVE_CUSTOM 16
#define ENEMY_MOVE_FLY_WANDER_STOP 17
#define ENEMY_MOVE_SWIM_TO_HERO_FOLLOW 18
#define ENEMY_MOVE_JUMP_STAND 19
#define ENEMY_MOVE_JUMP_WANDER 20

#define ENEMY_ACTION_NONE 0
#define ENEMY_ACTION_FIRE 1
#define ENEMY_ACTION_DEFENCE 2
#define ENEMY_ACTION_MOVE 3
#define ENEMY_ACTION_MOVE_TO_LEFT 4
#define ENEMY_ACTION_MOVE_TO_RIGHT 5


#define ENEMY_AFTER_FIND_HERO_STAND 0
#define ENEMY_AFTER_FIND_HERO_WANDER 1
#define ENEMY_AFTER_FIND_HERO_HANG 2
#define ENEMY_AFTER_FIND_HERO_FLY 3



#define SPECIAL_NORMAL 0
#define SPECIAL_SLIM 1
#define SPECIAL_HARD 2
#define SPECIAL_HEALTHY 3
#define SPECIAL_FAT 4
#define SPECIAL_FIRE 5
#define SPECIAL_FAST 6
#define SPECIAL_ICE 7
#define SPECIAL_FLY 8
#define SPECIAL_BOSS 9

#define UNIT_MISSILE_CHASING 0
#define UNIT_MISSILE_DROPPABLE 1
#define UNIT_MISSILE_CUSTOM 2
#define UNIT_MISSILE_STRAIGHT 3
#define UNIT_COIN 4

#define SOUND_BGM_BRIGHT 0
#define SOUND_BGM_EXCITING 1
#define SOUND_COIN 2
#define SOUND_FIRE_SMALL 3
#define SOUND_LASER 4
#define SOUND_MACHINE_GUN_1 5
#define SOUND_MACHINE_GUN_2 6
#define SOUND_MEDICINE 7
#define SOUND_SPRING 8
#define SOUND_DDALKACK 9
#define SOUND_JUMP 10
#define SOUND_POWER_UP_45 11
#define SOUND_ROCKEY_LAUNCH 12
#define SOUND_NOISE_37 13
#define SOUND_BGM_WHOS_BOSS 14
#define SOUND_BGM_MAYDAY 15
#define SOUND_FA 16
#define SOUND_MISOL 17
#define SOUND_GAMEOVER 18
#define SOUND_WIN 19
#define SOUND_REVIVE 20
#define SOUND_FOOT_STEP 21
#define SOUND_WATER_SPLASH 22
#define SOUND_SMALL_EXPLOSION 23
#define SOUND_BGM_ALMOST_BRIGHT 24
#define SOUND_CASTLE_DOOR 25


#define EFFECT_TWINKLE 0
#define EFFECT_EXPLODE_SMALL 1
#define EFFECT_EXPLODE_MIDDLE 2
#define EFFECT_EXPLODE_BIG 3
#define EFFECT_EXPLODE_HUGE 4
#define EFFECT_SMOKE 5
#define EFFECT_GREEN_SMOKE 6
#define EFFECT_RED_SMOKE 7


#define VEHICLE_NONE 0
#define VEHICLE_GOLIATH 1
#define VEHICLE_BROOM 2
#define VEHICLE_CART 3
#define VEHICLE_TOP 4
#define VEHICLE_BIRD 5
#define VEHICLE_BALLOON 6


#define ACTION_TAG_ANIMATION 0
#define ACTION_TAG_ROTATION 1

#define GUN_ORDINARY 0

#define MARKET_APPSTORE_PAID 0
#define MARKET_PLAYSTORE_PAID 1
#define MARKET_APPSTORE_FREE 2
#define MARKET_PLAYSTORE_FREE 3
#define MARKET_NAVER_PAID 4
#define MARKET_TSTORE_PAID 5
#define MARKET_SAMSUNG_APPS 6
#define MARKET_MAC 7
#define MARKET_FUNBOX 8
#define MARKET_OUYA 9
#define MARKET_WINDOWS 10
#define MARKET_CHINA_NO_IAP 11

#define FONT_DEFAULT 0
#define FONT_BITDUST_ONE 1
#define FONT_BITDUST_TWO 2
#define FONT_ARIAL 3

#define GOLD 0
#define GEM 1


#define MISSILE_EFFECT_NONE 0
#define MISSILE_EFFECT_SMOKE 1
#define MISSILE_EFFECT_RED_BALL 2
#define MISSILE_EFFECT_GREEN_SMOKE 3
#define MISSILE_EFFECT_RED_SMOKE 4

#define INVENTORY_MAX_COUNT 100


#define COIN_OFFSET 7973
#define GEM_OFFSET 9997
#define GAME_EXP_UNIT 37

#define KEY_TOTAL_ENEMY_KILL "TotalEnemyKill"
#define KEY_SOUND_VOLUMN "SoundVolumn"
#define KEY_MUSIC_VOLUMN "MusicVolumn"
#define KEY_NOTIFICATION_ON "NotificationOn"
#define KEY_CHECK_POINT "CheckPoint"
#define KEY_NEW_TRAP_INDEX "NewTrapIndex"
#define KEY_COIN_LEFT "CoinLeft"
#define KEY_ALL_HEROES "AllHeroes"
#define KEY_TOTAL_COIN_SPENT "TotalCoinSpent"
#define KEY_INSTALLED_TRAP_FORMAT "%d_%s_Type"
#define KEY_INSTALLED_TRAP_LEVEL_FORMAT "%d_%s_Level"
#define KEY_CONTINUE_AVAILABLE "Continue_Available"
#define KEY_STAR_COUNT "StarCount"
#define KEY_CLEAR_COUNT "TotalClearCount"
#define KEY_HERMIONE_UNLOCKED "HermioneUnlocked"
#define KEY_SELECTED_HERO "SelectedHero"
#define KEY_TUTORIAL_SHOOT "TutorialShoot"
#define KEY_TUTORIAL_TRAP "TutorialTrap"
#define KEY_TUTORIAL_UPGRADE "TutorialUpgrade"
#define KEY_TUTORIAL_GAMEOVER "TutorialGameOver"
#define KEY_TUTORIAL_CHECKPOINT "tutorialCheckpoint"
#define KEY_TUTORIAL_DONE "tutorialDone"
#define KEY_CLEAR_COUNT_READY_TO_ADD "ClearCountReadyToAdd"
#define KEY_STAR_TO_SEE_VIDEO_ADS "StarToSeeVideoAds"
#define KEY_NOT_ENOUGH_COIN_COUNT "NotEnoughCoinCount"
#define KEY_CONTINUED_GAME "ContinuedGame"
#define KEY_AOKIZI_UNLOCKED "AokiziUnlocked"
#define KEY_AOKIZI_LEVEL "AokiziLevel"
#define KEY_DARY_LEVEL "DaryLevel"
#define KEY_DAILY_REWARD_DAY "DailyRewardDay"
#define KEY_DAILY_REWARD_MONTH "DailyRewardMonth"
#define KEY_DAILY_REWARD_YEAR "DailyRewardYear"
#define KEY_DAY_COUNT "DayCountInARow"
#define KEY_ACHIEVEMENT_PROGRESS_FORMAT "Achievement_%d"
#define KEY_ACHIEVEMENT_UNLOCKED_FORMAT "Achievement_Unlocked_%d"
#define KEY_FRIEND_LEVEL_FORMAT "HeroLevel_%d"
#define KEY_ACHIEVEMENT_CONDITION_FORMAT "friend_condition_%d"
#define KEY_ANTIAIR_USED "Antiair_used"
#define KEY_TRAP_USED "Trap_used"
#define KEY_PAID_USER "IsPaidUser"
#define KEY_HIGH_SCORE "HighScore"
#define KEY_BREE "BreeUnlocked"
#define KEY_TOM "TomUnlocked"
#define KEY_KAREN "KarenUnlocked"
#define KEY_KARL "KarlUnlocked"
#define KEY_ANDREW "AndrewUnlocked"
#define KEY_UNLOCK_FRIENDS_ALL "AllFriendsUnlocked"

#define VIDEO_ADS_ALLOW_STAR_COUNT 10

#define ACHIEVEMENT_STAR_COLLECTER 0
#define ACHIEVEMENT_KEEPER 1
#define ACHIEVEMENT_FIRST_FRUIT 2
#define ACHIEVEMENT_RICH_MAN 3
#define ACHIEVEMENT_KILLER 4
#define ACHIEVEMENT_HOT_KETTLE 5
#define ACHIEVEMENT_WHOS_BOSS 6
#define ACHIEVEMENT_WEAPON_MASTER 7
#define ACHIEVEMENT_LEADERSHIP 8
#define ACHIEVEMENT_SOUL_LEGEND 9

#define PAGE_TITLE 0
#define PAGE_THEME_SELECT 1
#define PAGE_STAGE_SELECT 2
#define PAGE_EGG 3
#define PAGE_INVENTORY 4
#define PAGE_COLLECTION 5
#define PAGE_AVHIEVE 6
#define PAGE_SHOP 7
#define PAGE_SETTING 8

#define CLOSE_TO_NEXT 0
#define CLOSE_TO_RETRY 1
#define CLOSE_TO_STAGES 2
#define CLOSE_TO_MAIN 3
#define FAST_SPEED 2
#define DEFAULT_CRITICAL 5
#define TOTAL_TRAP_COUNT 13

#define SCENE_TITLE 0
#define SCENE_TITLE_SHOP 1
#define SCENE_SETTING 2
#define SCENE_TOUCH_TO_PLAY 3
#define SCENE_PLAY 4
#define SCENE_HERO 5
#define SCENE_RANKING 6
#define SCENE_LOADING 7
#define SCENE_PAUSE 8
#define SCENE_IN_GAME 9
#define SCENE_GAME_OVER 10
#define SCENE_GAME_CLEAR 11
#define SCENE_GAME_ENDING 12
#define SCENE_GAME_UPGRADE_TRAP 13
#define SCENE_GAME_PLACE_TRAP 14
#define SCENE_IN_GAME_SHOP 15
#define SCENE_GAME_OVER_SHOP 16
#define SCENE_ASK_NEW_GAME 17
#define SCENE_GAME_TUTORIAL 18
#define SCENE_SHARE_GAME 19
#define SCENE_DAILY_REWARD 20
#define SCENE_ACHIEVEMENT 21
#define SCENE_SAVE_WARN 22
#define SCENE_LOAD_WARN 23
#define SCENE_MINE 24
#define SCENE_FRIENDS 25
//static const Color3B greenColor = {5,126,45};
//static const Color3B yesColor = {241,74,1};
//static const Color3B noColor = {46,124,179};


typedef struct PetInfo
{
    int maxLevel;
    int maxAttack;
    int petNumber;
    int levelRequired;
    const char* name;
    const char* description;
    int starCount;
}PET;
using namespace cocos2d;

class GameManager : public Node, public sdkbox::IAPListener
{
private:
    int bulletType;
    
    //Constructor
    GameManager();
    
    //Instance of the singleton
    static GameManager* m_mySingleton;
    HelloWorld* stageLayer;
    HudLayer* hudLayer;
    Layer* pauseLayer;
    Layer* optionLayer;
    Layer* gameOverLayer;
    Layer* achievementLayer;
    
    Scene* gameOverScene;
	HelloWorld* currentStageLayer;
	std::vector<int> _trapCount;
	Node* LoadingScene;

    int currentBGM;
    std::string userDefaultData;
	int _starCount;
public:
    //Get instance of singleton
    static GameManager* getInstance();
    ShopLayer* shopLayer;
    std::string breePriceText;
    std::string tomPriceText;
    std::string karenPriceText;
    std::string karlPriceText;
    std::string andrewPriceText;
    std::string allFriendsPriceText;
    std::string SmallPriceText;
    std::string MiddlePriceText;
    std::string BigPriceText;
    std::string HugePriceText;
    std::string HeroPriceText;
	bool IsNewGame;
    int currentScene;
    bool isPlayServiceLogIn;
//    bool isShieldPurcahsed();
//    void setShield(long time);
//    long getShieldPurchasedTime();
	int StageMode;
	int GetUpgradePrice(cocos2d::Point pos);
	int GetTrapPrice(int trapType);
	void AddTrapCount(int trapType);
	void RemoveTrapCount(int trapType);
	void ResetTrapCount();
	int GetCoin();
	void AddCoin(int howMuch);
	void SetStringWithAutoLineBreak(TextBMFont* lbl, std::string str, float width);
    Layer* settingLayer;
    void onPlayServiceLogin(bool isLogin);

    void initGameManager();
    Layer* titleLayer;
    bool isGuestPlay;
    bool isVideoReady;
    bool isUsingController;
    const char* convertMyUserDefaultToCharArray();
    void convertCharArrayToMyUserDefault(const char* data);
    int totalThemeCount;
    int totalStage;
    bool gameStarted;
    bool initComplete;
	int GetSpecialSkill(int index);
	int GetStarCount();
	void SetStarCount(int star);
	void AddStarCount(int star);
    Sprite* myPhoto;
    
    void saveGameData();
    void loadGameData();
    void setLoadedData(const char* key, const char* value);
	void SpriteMoveDone(Node* node);
    cocos2d::Point getGemCountPosition();
    bool isPaidGame;
    bool isInMiddleOfGame;
    bool firstPlayed;
    const char* version;
    cocos2d::Size originalSize;
    bool isStageSetOnce;
    int currentStageIndex;
    bool leftPressed;
    bool rightPressed;
    bool jumpPressed;
    bool cPressed;
    bool firePressed;
    bool downPressed;
    bool upPressed;
    int market;
    int page;
    bool developer;
    bool canJumpForever;
    bool appAlreadyLaunched;
    int theme;
    const char* currentUserID;
    void showVideoDone();
    void showVideoFailed();
    void showVideo();
    void clickAdsDone();
    void clickAdsFailed();
    void exitGame();
    bool isVideoRewardEnergy;
    bool isVideoRewardAttack;
    bool isVideoRewardCoins;
    void setFontName(Label* lbl, const char* name, float fontSize);
    int getGemForCoin(int coinCount);
    //A function that returns zero "0"
    int ReturnZero(){return 0;}
    // another test function
    void runScene() { CCLOG("test");};
    void buttonDown(int buttonType);
    void buttonUp(int buttonType);
    void saveCoin();
    void setFontSize(Label* lbl, float size);
    int getGameLevel();
    int getCurrentExp();
    int getCurrentExpMax();
    std::string GetCoordinateKey(cocos2d::Point pos);


    bool checkDataSecure();
    void addGameExp(int exp);
    void addGameLevel();
    
    void preLoadAllSoundEffect();
    void playSoundEffect(int sound);
    void setMusicVolumn(float vol);
    void setSoundVolumn(float vol);
    void setNotificationOn(bool onOff);
    int getPotionCount();
    void setPotionCount(int bomb);
    float getMusicVolumn();
    float getSoundVolumn();
    bool getNotificationOn();
    double getAngle(cocos2d::Point pos1, cocos2d::Point pos2);
    void SendAchievement(const char* text, int count, int goal);
    const char* getPlayerInfoFileName(int character, int infoType);
    const char* getWeaponImageFileName(int weapon);
    void makeItSiluk(Node* node);
  
	std::string GetSlotKey(int index);
    void scrollTheLayer(ScrollView* scrollLayer, bool isLeft, bool isHorizontal, int howMuch);
//    HelloWorld* getStageLayer();
    HudLayer* getHudLayer();
    Scene* getStageScene(int stage);
    Scene* getTitleScene();
    Layer* getGameStartLayer();
    Scene* getGameStartScene();

    Layer* getTitleLayer();
    Layer* getShopLayer();
    Layer* getPauseLayer();
//    Layer* getGameOverLayer();
//    Scene* getGameOverScene();
    const char* getBulletName(int weaponType, int playerMissileDemage);
    Layer* getOptionLayer();
    Layer* getAchievementLayer();
    void setCurrentStageLayer(HelloWorld* layer);
    const char* getShortenedKoreanString(std::string str, int length);
    HelloWorld* getCurrentStageLayer();
    
//    void setStageLayer(Layer* layer);
    void setHudLayer(HudLayer* layer);
    void setStageScene(Scene* scene);
//    RepeatForever* getScaleUpDownAction(float scale);
    
   
    void pushLayer(Layer* parent, Layer* layer);
    void pushLayerWithoutDisable(Layer* parent, Layer* layer);
    void popLayer(Layer* layer);
    void popLayerWithoutAnimation(Layer* layer);
    void animateFadeIn(Node* layer, Node* parent);
    void animateFadeOut(Node* layer);
    void animationFadeInDone(Node* layer);
    void animationFadeOutDone(Node* layer);
    
//    void disableLayer(Layer* layer);
//    void enableLayer(Layer* layer);
    
	void showParticleExplosion(Node* prt, const char* sptName, cocos2d::Point pos, int distance, float scale, Widget::TextureResType type);
    void addYellowTurningBackground(Node* node, int backWidth);
    void scheduleLocalNotification(const char* title, const char* msg, int time);
    void scheduleLocalNotification();
    void unscheduleAllNotifications();
    long strToLong(const char* str);
    Sprite* getGrayScaleImage(const char* src);
    RenderTexture* createAdditiveBorder( Sprite* label, int size, Color3B color, GLubyte opacity );
    RenderTexture* createAdditive( Sprite* label, Color3B color, GLubyte opacity, int additiveCount);
    Sprite* getSpriteShapeSolidColorSprite(const char* src, Color3B color);
    Layer* getGridLayer();
    bool isBoss(int theme, int stage);
    
    void reset();
    void backup();
    
     void initAchievement();
    void setAchievementGoal(int achievementIndex, int goalCount);
    bool setAchievement(int achievementIndex, int countToAdd);
    const char* getAchievementId(int achievementIndex);
    void googleSignInOrOut(bool sign);
    bool isGoogleSigned;
    int getAchievementGoalCount(int achievementIndex);
    int getAchievementCurrentCount(int achievementIndex);
     
	int getIndexOf(std::vector<int> vec, int index);
     
    void nativeControllerButtonEvent(int controller, int button, bool isPressed, float value, bool isAnalog);
    
    void nativeControllerLAxisXEvent(int controller, float value);
    void nativeControllerLAxisYEvent(int controller, float value);
    
    Color3B getRandomColor();
    
    void iapFailed();
    void iapSuccess(int index);
    void iapSuccess();
    // iap methods
    virtual void onInitialized(bool ok) override;
    virtual void onSuccess(sdkbox::Product const& p) override;
    virtual void onFailure(sdkbox::Product const& p, const std::string &msg)
    override;
    virtual void onCanceled(sdkbox::Product const& p) override;
    virtual void onRestored(sdkbox::Product const& p) override;
    virtual void onProductRequestSuccess(std::vector<sdkbox::Product> const &products)
    override;
    virtual void onProductRequestFailure(const std::string &msg) override;
    virtual void onRestoreComplete(bool ok, const std::string &msg) override;
};

#endif