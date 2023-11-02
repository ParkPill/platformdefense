//
//  HudLayer.h
//  AlmostLegendary
//
//  Created by SUNG PILL PACK on 13. 6. 29..
//
//

#ifndef AlmostLegendary_HudLayer_h
#define AlmostLegendary_HudLayer_h
#include "cocos2d.h"
#include "ui/UIButton.h"
#include "ui/UIListView.h"
#include "ui/UIText.h"
#include "ui/UIImageView.h"
#include "ui/UILoadingBar.h"
#include "Droppable.h"
#include "cocostudio/CocoStudio.h"
#include "base/CCEventListenerController.h"
#include "base/CCEventController.h"
#include "base/CCController.h"
#include "Trap.h"

using namespace cocos2d;
using namespace cocos2d::ui;
using namespace cocostudio;
class HudLayer : public Layer
{
private:
	int _listViewWidth;
    int weaponIndex;
    bool toChar;
	int _lastUnlockedIndex;
	
	cocos2d::Point _clickedTrapPos;
	cocos2d::Point _clickedTrapPosInScreen;
    //Constructor
    Vector<Sprite*> lifeArray;
//    Sprite* coin;
//    Label* lblStage;
    int starCount;
    Label* talkLabel;
	Layer* talkLayer;
	Node* _pauseLayer;
	Node* _upgradeScene;
	Node* _trapInfoScene;
	Node* _messageBox;
    Node* shareDialog;
    Node* rateDialog;
    Node* gameClearNode;
    Node* endingNode;
	Sprite* _whiteLine;
	Sprite* _whiteRect;
    MenuItemImage* pauseItem;
    MenuItemImage* recordItem;
    MenuItemImage* bombItem;
    MenuItemImage* potionItem;
    Button* _btnFast;
    Button* btnVideo;
    Node* _tutorialNode;
    ListView* _trapListview;
    void closeWaitMessageBox(float dt);
    void closeWaitMessageBoxAndShowSuccess(float dt);
    void onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event);
	void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event);
	void trapButtonClicked(cocos2d::Ref *pSender);
    void changeSpeed();
    void addStarLater();
    void showStarParticle();
	Droppable* _sptChooseThis;
public:
	bool IsFastMode;
    bool isSlowedBySystem;
	float FastModeSpeed;
	Label* LblWaveCount;
	Label* LblCoinCount;
	Label* LblStarCount;
    void facebookSafe(float dt);
    EventListenerKeyboard* listener;
    EventListenerTouchOneByOne* touchListener;
    Sprite* bossEnergyBackground;
    Label* bossName;
    Sprite* bossEnergyBarYellow;
    Sprite* bossEnergyBarRed;
    int bossEnergyBarWidth;
    bool isRecording;
    void addStar();
    void addEnergyEffect();
    void addEnergyDone(float dt);
    Layer* blackLayer;
    Sprite* star1;
    Sprite* star2;
    Sprite* star3;
    Sprite* key;
//    Label* coinLabel;
   cocos2d::Size  size;
	Label* _lblNotEnoughCoin;
	float _notEnoughCoinsScale;
    Layout* gameOverLayer;
    Layout* gameClearLayer;
    
    void HideTutrorialHelper();
    void ShowTutorialHelper(int index, cocos2d::Point pos);
	void ShowWrongRect();
    void tryGameContinue(Ref* pSender, ui::Widget::TouchEventType eEventType);
    
    void buyGemClosed(Ref* obj);
    virtual bool init();
    CREATE_FUNC(HudLayer);
    void showTalk(const char* talk);

    void hideTalk();
    Layer* bombLayer;
    void closeWaitMessageBoxLater();
	void closeWaitMessageBoxLaterAndShowSucess();

	int GetUnlockStarCount(int index);
	void ShowNewMonster(int index, float delayTime);
	void ShowNewTrap(int index);
    void ShowWave(int wave, bool checkPoint);
    void ShowCheckPoint(int wave);
    int waveToShow;
    void ShowWaveLater();
	void ShowTrapInfo(cocos2d::Point tilePos);
    void ShowTrapInfo(Trap* trap);
	void HideTrapInfo();
	const char* getIconName(int index);
	void ShowNotEnoughCoinLabel();
	void ShowUpgradeLayer(cocos2d::Point screenPos, cocos2d::Point tilePos, cocos2d::Point tileCenter);
	void ShowGameOverLayer(int from, int to, int earned, int spent, int kill, float time);
    void updateCoinCount();
    void ShowGameClearLayer(int from, int to, float time);
    void ShowArriveDestination();
    void onArriveDestinationClosed();
    void gameClearScoring(float dt);
    int waveScoreTotal;
    int coinScoreTotal;
    int totalScoreTotal;
    int waveScore;
    int coinScore;
    int totalScore;
    void onCloseGameClear();
    void showEnding();
    void startEndingAnimation();
    void continueGame();
    
	void OnQuit();
	void OnRestartFromCheckPoint();
	void CloseUpgradeLayer();
	void UpgradeTrapClick();
	void SellTrapClick();
	void SellTrapOk();
	void SellTrapCancel();
	void notEnoughGemClosed(Ref* obj);
	void showPauseLayer();
	void resumeGame();
	void toMain();
    void closeAskShare();
    void toMainByPauseLayer();
	void toStageSelect(Ref* pSender, ui::Widget::TouchEventType eEventType);
	void nextStage(Ref* pSender, ui::Widget::TouchEventType eEventType);
	void retryStage(Ref* pSender, ui::Widget::TouchEventType eEventType);
	void onReplayVideo(Ref* pSender, ui::Widget::TouchEventType eEventType);

    // screenshot
    void afterCaptured(bool succeed, const std::string& outputFile);
    void capture(float dt);
    bool captureSuccess;
    void onShareOk();
    void onShareCancel();
    std::string filePath;
    void askShare();
    void rateUs();
    void onRateOk();
    void onRateCancel();
    void sharePopupAutoClose(float dt);
    int autoCloseCount;
    bool isSharing;

//    void hideItems();
    Sprite* blackScreen;

    void closePopupLayer();
    void enableJoystick(bool enable);
    void initializeSreen(int lifeCount);
    void reduceLife();
    void setLife(int eng);
    void keyCollected();
    void starCollectedFancy(int count, cocos2d::Point collectedPos);
    void starCollected(int count);
    void starChange(float dt);
    void startRecord();
    void showShopLayer();
    void showCoinShopLayer();
    

	void UpdateTrapButton();
    void showStageClearLayer(int coin, int enemyKilled, int enemyMax);
	Vector<Button*> TrapButtonList;
    LoadingBar* pbExp;
    ImageView* sptMaxTalkBalloon;
    Text* lblPerfect;
    Text* lblNext;
    ImageView* sptBackLight0;
    ImageView* sptBackLight1;
    ImageView* sptBackLight2;
    ImageView* sptStar0;
    ImageView* sptStar1;
    ImageView* sptStar2;
    Button* btnNext;
    Button* btnRetry;
    Button* btnHome;
    int increasingCoinCount;
    float increasingEnemyCount;
    int increasingExpCount;
    int increasingCounter;
    int increasingCounterMax;
    float countingInterval;
    int expCount;
    int maxExpCount;
    EventListenerController* _listener;
    void registerControllerListener();
    //Controller is the obejects of the Controller，keyCode means the keycode of the controller you click down
    void onKeyDown(cocos2d::Controller *controller, int keyCode, cocos2d::Event *event);
    void onKeyUp(cocos2d::Controller *controller, int keyCode, cocos2d::Event *event);
    
    //The axis includes X-axis and Y-axis and its range is from -1 to 1. X-axis is start from left to right and Y-axis is bottom to top.
    void onAxisEvent(cocos2d::Controller* controller, int keyCode, cocos2d::Event* event);
    void onConnectController(Controller* controller, Event* event);
    void onDisconnectedController(Controller* controller, Event* event_);
    void playCoinSoundLater();
    void removeUsedAssets();
    void testCharacter(int character);
    int testingCharacter;
    void closeTestCharacterBuyPopup();
    void closeTestCharacterBuyPopupLater(float dt);
    void closeTestCharacterBuyPopupFail();
    void closeTestCharacterBuyPopupLaterFail(float dt);
    void testCharacterDone(Ref* pSender, ui::Widget::TouchEventType eEventType);
    void testCharacterBuyDone(Ref* pSender, ui::Widget::TouchEventType eEventType);
    void onFreeCoin(Ref* pSender, ui::Widget::TouchEventType eEventType);
    Button* btnFreeCoin;
    void videoDone();
    void videoFailed();
    void closeVideoDone(float dt);
    void closeVideoFailed(float dt);
    Layout* messageBox;
    int coinCount;
    int enemyKilledCount;
    int enemyMaxCount;
    void askBuyTestCharacter(float dt);
    void spriteMoveDone(Node* sprite);
    void coinWaitDone(Node* sprite);
    void displayBossEnergy(const char* name);
    void setBossEnergy(int percent);
    void setButtonSize(float siz);
    void enableButtons(bool enb);
    void showAchievementComplete(const char* text);
    void achievementEffect(Node* node);
    void nodeMoveDone(Ref* obj);
    void addListener();
    int whereToGo;
    void closeSchedule(float dt);
    void shareFacebook();
    void showDialog(const char* message, const char* btn1, const char* btn2);
    void messageBoxClosed(Node* sender);
    void recommandCoinShopClosed(Node* sender);
    
//    virtual void keyBackClicked();
//    virtual void draw();
    virtual void keyDown(int key);
    virtual void keyUp(int key);
	void OnVideoClick();
	void OnShopClick();
    void OnVideoEnded();
};
#endif
