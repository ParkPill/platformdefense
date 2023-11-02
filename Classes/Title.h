//
//  _TITLE_H_.h
//  AlmostLegendary
//
//  Created by SUNG PILL PACK on 13. 6. 29..
//
//

#ifndef _TITLE_H_
#define _TITLE_H_

#include "cocos2d.h"

#include "ui/UISlider.h"
#include "Droppable.h"
#include "base/CCEventListenerController.h"
#include "base/CCEventController.h"
#include "base/CCController.h"
#include "cocostudio/CocoStudio.h"

#define MAX_MINE_COIN 500
using namespace cocos2d;

using namespace cocos2d::ui;
using namespace cocostudio;
class Title : public Layer
{
private:
    Size size;
    EventListenerKeyboard* _listener;
    void onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event);
    void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event);
    int _clickCount;
    Node* _startPopup;
    Node* _loadingNode;
    Node* _rankingPopup;
    Node* _messageBox;
    Node* _lblNext;
    Node* _heroPopup;
    Node* _friendsPopup;
    Node* _achievementPopup;
    Node* _minePopup;
    Node* _indicator;
    Text* _lblTutorial;
    float _tutorialLabelWidth;
    cocostudio::timeline::ActionTimeline* _tutorial;
    int _tutorialIndex = 0;
    Slider* _musicBar;
    Slider* _soundBar;
    bool _isLoggedIn;
    int _stageToPlay;
    void showPlaySubMenus(bool show);
    void ShowLoadingScene();
    Node* gameQuitPopup;
    int getAokiziLevelUpPrice();
    int getDaryLevelUpPrice();
    float mineTime;
    int mineCoin;

public:
    virtual bool init();
    CREATE_FUNC(Title);
    
    void AskShowTutorial();
    void ShowTutorial();
    void ShowTutorialCancel();
    void OnTutorialNext();
    
    void ShowSetting();
    void shareGame();
    void shareGameOk();
    
    void ShowRanking();
    void ShowLeaderboard();
    void ShowWaveRanking();
    void ShowEnemyKillRanking();
    void CloseRanking();
    
    void TouchToPlay();
    void Play();
    void PlayContinue();
    void PlayNew();
    void PlayNewOK();
    void PlayNewCancel();
    void OnHeroClick();
    void ShowIndicator();
    void HideIndicator();
    void showMine();
    void mineSchedule(float dt);
    void onMineClosed();
    void onWorkHarder();
    void spriteMoveDone(Node* node);
    void OnFriendsButtonClick(cocos2d::Ref *pSender);
    void OnFriendsClose();
    void OnFriendsClick();
    void UpdateFriendsPopup();
    void OnHeroButtonClick(cocos2d::Ref *pSender);
    void OnHeroPopupClose();
    void OnUnlockFriends();
    void UpdateHeroPopup();
    bool isHeroUnlocked(int i);
    void OnAchievementClick();
    void OnAchievementButtonClick(cocos2d::Ref *pSender);
    void OnAchievementPopupClose();
    void UpdateAchievementPopup();
    void showAchievement();
    bool isAchievementUnlocked(int i);
    bool canUnlockAchievement(int i);
    int getFriendUpgradePrice(int i);
    int getFriendLevel(int i);
    int getAchievementTotal(int i);
    int getCurrentAchievementCount(int i);
    void CloseStartPopup();
    void OnShopClick();
    
    void LoadSceneLater(float dt);
    
    void onGameQuitPopupCancel();
    
    void onDailyRewardCheck(float dt);
    void onDailyRewardClosed();
    void playCoinSound();
};
#endif
