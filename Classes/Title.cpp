
#include "LanguageManager.h"
#include "HudLayer.h"
#include "Title.h"
#include "GameManager.h"
#include "HelloWorldScene.h"
#include "MyMessageBox.h"
#include "LanguageManager.h"
#include "ui/UIHelper.h"
#include "ShopLayer.h"
#include "NativeInterface.h"
#include "SettingLayer.h"
#include "PluginIAP/PluginIAP.h"
#include "TimeManager.h"
#include "Firework.h"
#include "PluginChartboost/PluginChartboost.h"

bool Title::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}
	size = Director::getInstance()->getWinSize();
	_listener = EventListenerKeyboard::create();
	_listener->onKeyPressed = CC_CALLBACK_2(Title::onKeyPressed, this);
	_listener->onKeyReleased = CC_CALLBACK_2(Title::onKeyReleased, this);
	_clickCount = 0;
    _loadingNode = NULL;
    gameQuitPopup = NULL;
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(_listener, this);
    GameManager::getInstance()->titleLayer = this;

	Node* titleNode = (Widget*)CSLoader::createNode("TitleScene.csb");
		// cocosstudio animation
	cocostudio::timeline::ActionTimeline* action = CSLoader::createTimeline("TitleScene.csb");
	titleNode->runAction(action);
	action->gotoFrameAndPlay(0, true);
	titleNode->setContentSize(size);
	ui::Helper::doLayout(titleNode);
	_isLoggedIn = false;

	this->addChild(titleNode);

	Sprite* sptBackground = (Sprite*)titleNode->getChildByName("background");
	float backHeight = size.height - sptBackground->getPositionY();
	sptBackground->setScaleY(backHeight / sptBackground->getContentSize().height);
    sptBackground->setScaleX(size.width/(sptBackground->getContentSize().width));
    
    Sprite* sptTitleBackground = (Sprite*)titleNode->getChildByName("titleBackground_1");
    sptTitleBackground->setScaleX(size.width/(sptTitleBackground->getContentSize().width));
    
	Button* btn = (Button*)titleNode->getChildByName("setting");
	btn->addClickEventListener(CC_CALLBACK_0(Title::ShowSetting, this));
    btn = (Button*)titleNode->getChildByName("share");
    btn->addClickEventListener(CC_CALLBACK_0(Title::shareGame, this));
    btn = (Button*)titleNode->getChildByName("achievement");
    btn->addClickEventListener(CC_CALLBACK_0(Title::OnAchievementClick, this));
    btn = (Button*)titleNode->getChildByName("btnMine");
    btn->addClickEventListener(CC_CALLBACK_0(Title::showMine, this));

	btn = (Button*)titleNode->getChildByName("touchToPlay");
	btn->addClickEventListener(CC_CALLBACK_0(Title::TouchToPlay, this));
	btn = (Button*)titleNode->getChildByName("btnShop");
	btn->addClickEventListener(CC_CALLBACK_0(Title::OnShopClick, this));

	_rankingPopup = titleNode->getChildByName("rankingPopup");
	btn = (Button*)_rankingPopup->getChildByName("close");
	btn->addClickEventListener(CC_CALLBACK_0(Title::CloseRanking, this));
	
    btn = (Button*)_rankingPopup->getChildByName("btnLeaderboard");
	btn->addClickEventListener(CC_CALLBACK_0(Title::ShowLeaderboard, this));
    
    btn = (Button*)_rankingPopup->getChildByName("btnWaveRanking");
    btn->addClickEventListener(CC_CALLBACK_0(Title::ShowWaveRanking, this));
    
	btn = (Button*)_rankingPopup->getChildByName("btnTotal");
	btn->addClickEventListener(CC_CALLBACK_0(Title::ShowEnemyKillRanking, this));

    // test
    /*
    char buf[30];
    sprintf(buf, KEY_ACHIEVEMENT_PROGRESS_FORMAT, 0);
    UserDefault::getInstance()->setIntegerForKey(buf, 9);
    sprintf(buf, KEY_ACHIEVEMENT_PROGRESS_FORMAT, 1);
    UserDefault::getInstance()->setIntegerForKey(buf, 90);
    sprintf(buf, KEY_ACHIEVEMENT_PROGRESS_FORMAT, 2);
    UserDefault::getInstance()->setIntegerForKey(buf, 90);
    sprintf(buf, KEY_ACHIEVEMENT_PROGRESS_FORMAT, 3);
    UserDefault::getInstance()->setIntegerForKey(buf, 100000);

    char levelBuf[30];
    sprintf(levelBuf, KEY_FRIEND_LEVEL_FORMAT, 0);
    UserDefault::getInstance()->setIntegerForKey(levelBuf, 99);
    sprintf(levelBuf, KEY_FRIEND_LEVEL_FORMAT, 1);
    UserDefault::getInstance()->setIntegerForKey(levelBuf, 10);
    sprintf(levelBuf, KEY_FRIEND_LEVEL_FORMAT, 2);
    UserDefault::getInstance()->setIntegerForKey(levelBuf, 20);
    sprintf(levelBuf, KEY_FRIEND_LEVEL_FORMAT, 3);
    UserDefault::getInstance()->setIntegerForKey(levelBuf, 30);
    */
    

    _isLoggedIn = false;
	
    _heroPopup = (Widget*)CSLoader::createNode("HeroSelectScene.csb");
    _heroPopup->setContentSize(size);
    ui::Helper::doLayout(_heroPopup);
    Node* panel = (Node*)_heroPopup->getChildByName("panel");
    char btnName[8];
    for(int i = 0;i<5;i++){
        sprintf(btnName, "btn%d", i);
        btn = (Button*)panel->getChildByName(btnName);
        btn->addClickEventListener(CC_CALLBACK_1(Title::OnHeroButtonClick, this));
        btn->setTag(i);
    }
    
    btn = (Button*)_heroPopup->getChildByName("block");
    btn->addClickEventListener(CC_CALLBACK_0(Title::OnHeroPopupClose, this));
    btn = (Button*)panel->getChildByName("ok");
    btn->addClickEventListener(CC_CALLBACK_0(Title::OnHeroPopupClose, this));
    this->addChild(_heroPopup);
    _heroPopup->setVisible(false);
    
    _friendsPopup = (Widget*)CSLoader::createNode("friendsScene.csb");
    _friendsPopup->setContentSize(size);
    ui::Helper::doLayout(_friendsPopup);
    panel = (Node*)_friendsPopup->getChildByName("panel");
    for(int i = 0;i<5;i++){
        sprintf(btnName, "btn%d", i);
        btn = (Button*)panel->getChildByName(btnName);
        btn->addClickEventListener(CC_CALLBACK_1(Title::OnFriendsButtonClick, this));
        btn->setTag(i);
    }
    
//    btn = (Button*)_friendsPopup->getChildByName("block");
//    btn->addClickEventListener(CC_CALLBACK_0(Title::OnFriendsClose, this));
    btn = (Button*)panel->getChildByName("ok");
    btn->addClickEventListener(CC_CALLBACK_0(Title::OnFriendsClose, this));
    btn = (Button*)panel->getChildByName("btnUnlockAll");
    btn->addClickEventListener(CC_CALLBACK_0(Title::OnUnlockFriends, this));
    this->addChild(_friendsPopup);
    _friendsPopup->setVisible(false);

    
    _achievementPopup = (Widget*)CSLoader::createNode("AchievementScene.csb");
    _achievementPopup->setContentSize(size);
    ui::Helper::doLayout(_achievementPopup);
    panel = (Node*)_achievementPopup->getChildByName("panel");
    for(int i = 0; i < 5; i++){
        sprintf(btnName, "btn%d", i);
        btn = (Button*)panel->getChildByName(btnName);
        btn->addClickEventListener(CC_CALLBACK_1(Title::OnAchievementButtonClick, this));
        btn->setTag(i);
    }
    btn = (Button*)_achievementPopup->getChildByName("panel")->getChildByName("btnAchievement");
    btn->addClickEventListener(CC_CALLBACK_0(Title::OnAchievementPopupClose, this));
    if (GameManager::getInstance()->market == MARKET_PLAYSTORE_PAID ||
        GameManager::getInstance()->market == MARKET_PLAYSTORE_FREE ||
        GameManager::getInstance()->market == MARKET_TSTORE_PAID ||
        GameManager::getInstance()->market == MARKET_NAVER_PAID) {
        btn->setVisible(true);
        btn->addClickEventListener(CC_CALLBACK_0(Title::showAchievement, this));
    }else if (GameManager::getInstance()->market == MARKET_APPSTORE_PAID ||
              GameManager::getInstance()->market == MARKET_APPSTORE_FREE) {
        btn->setVisible(true);
        btn->loadTextureNormal("gamecenter-icon.png");
        btn->loadTexturePressed("gamecenter-icon.png");
        btn->addClickEventListener(CC_CALLBACK_0(Title::showAchievement, this));
    }else{
        btn->setVisible(false);
    }
    
//    btn = (Button*)_achievementPopup->getChildByName("block");
//    btn->addClickEventListener(CC_CALLBACK_0(Title::OnAchievementPopupClose, this));
    btn = (Button*)panel->getChildByName("ok");
    btn->addClickEventListener(CC_CALLBACK_0(Title::OnAchievementPopupClose, this));
    this->addChild(_achievementPopup);
    _achievementPopup->setVisible(false);

	_startPopup = titleNode->getChildByName("startPopup");

	Sprite* spt = (Sprite*)_startPopup->getChildByName("playLinker");
	spt->setVisible(false);
	btn = (Button*)_startPopup->getChildByName("play");
	btn->addClickEventListener(CC_CALLBACK_0(Title::Play, this));
	btn = (Button*)_startPopup->getChildByName("playContinue");
	btn->setEnabled(UserDefault::getInstance()->getBoolForKey(KEY_CONTINUE_AVAILABLE, false)); // check previous data
	
	btn->addClickEventListener(CC_CALLBACK_0(Title::PlayContinue, this));
	btn = (Button*)_startPopup->getChildByName("playNew");
	btn->addClickEventListener(CC_CALLBACK_0(Title::PlayNew, this));
    
    btn = (Button*)_startPopup->getChildByName("ranking");
    btn->addClickEventListener(CC_CALLBACK_0(Title::ShowRanking, this));
    
    btn = (Button*)_startPopup->getChildByName("hero");
    btn->addClickEventListener(CC_CALLBACK_0(Title::OnHeroClick, this));
    
    btn = (Button*)_startPopup->getChildByName("friends");
    btn->addClickEventListener(CC_CALLBACK_0(Title::OnFriendsClick, this));

    btn = (Button*)_startPopup->getChildByName("close");
    btn->addClickEventListener(CC_CALLBACK_0(Title::CloseStartPopup, this));
    btn = (Button*)_startPopup->getChildByName("block");
    btn->addClickEventListener(CC_CALLBACK_0(Title::CloseStartPopup, this));

    

	Node* node = titleNode->getChildByName("lblTouchToPlay");
	node->runAction(RepeatForever::create(Sequence::create(FadeOut::create(0.1), DelayTime::create(0.3), FadeIn::create(0.1), DelayTime::create(0.4), NULL)));

	_startPopup = titleNode->getChildByName("startPopup");
	/*
	Label* lblDemage = Label::createWithBMFont("boldPixelFont.fnt", "AABBCCDDABCD\nPLAY_A\₩!@#$%^&*()_+BCDE\nFGHIJ\nKLMNOP\nQRSTU\nVWX\\YZ");
	lblDemage->getFontAtlas()->setAliasTexParameters();
	this->addChild(lblDemage, 100);
	lblDemage->setScale(4);
	lblDemage->setPosition(300, 400);*/

	if (UserDefault::getInstance()->getBoolForKey("FirstRun", true)){
		UserDefault::getInstance()->setBoolForKey("FirstRun", false);
		AskShowTutorial();
	}
	
	GameManager::getInstance()->playSoundEffect(SOUND_BGM_BRIGHT);
    
    this->schedule(schedule_selector(Title::onDailyRewardCheck), 0.5f);
//    UserDefault::getInstance()->setIntegerForKey(KEY_COIN_LEFT, 23443);
    
    const char* secondRunKey = "SecondRunKey";
    bool isSecondRun = UserDefault::getInstance()->getBoolForKey(secondRunKey, false);
    if (isSecondRun) {
        sdkbox::PluginChartboost::show("Default");
    }
    UserDefault::getInstance()->setBoolForKey(secondRunKey, true);
    
	return true;
}
void Title::OnFriendsClose(){
    GameManager::getInstance()->playSoundEffect(SOUND_DDALKACK);
    _friendsPopup->setVisible(false);
    
    GameManager::getInstance()->currentScene = SCENE_TOUCH_TO_PLAY;
}
void Title::OnFriendsClick(){
    GameManager::getInstance()->playSoundEffect(SOUND_DDALKACK);
    showPlaySubMenus(false);
    
    _friendsPopup->setVisible(true);
    UpdateFriendsPopup();
    
    GameManager::getInstance()->currentScene = SCENE_FRIENDS;
}
void Title::OnFriendsButtonClick(cocos2d::Ref *pSender){
    Button* btn = (Button*)pSender;
    const char* key;
    const char* item;
    if(btn->getTag() == 0){
        key = KEY_BREE;
        item = "bree";
    }else if(btn->getTag() == 1){
        key = KEY_TOM;
        item = "tom";
    }else if(btn->getTag() == 2){
        key = KEY_KAREN;
        item = "karen";
    }else if(btn->getTag() == 3){
        key = KEY_KARL;
        item = "karl";
    }else if(btn->getTag() == 4){
        key = KEY_ANDREW;
        item = "andrew";
    }
    bool unlocked = isAchievementUnlocked(btn->getTag()) || UserDefault::getInstance()->getBoolForKey(key, false);
    
    if(unlocked){
        int price = getFriendUpgradePrice(btn->getTag());
        int coinLeft = UserDefault::getInstance()->getIntegerForKey(KEY_COIN_LEFT, 0);
        if(coinLeft >= price){
            coinLeft -= price;
            UserDefault::getInstance()->setIntegerForKey(KEY_COIN_LEFT, coinLeft);
            int level = getFriendLevel(btn->getTag());
            level++;
            char levelBuf[30];
            sprintf(levelBuf, KEY_FRIEND_LEVEL_FORMAT, btn->getTag());
            UserDefault::getInstance()->setIntegerForKey(levelBuf, level);
            
            UserDefault::getInstance()->flush();
            UpdateFriendsPopup();
        }else{
            GameManager::getInstance()->playSoundEffect(SOUND_ROCKEY_LAUNCH);
        }
        UpdateFriendsPopup();
    }else{
        GameManager::getInstance()->playSoundEffect(SOUND_DDALKACK);
        ShowIndicator();
        
        sdkbox::IAP::purchase(item);
    }
}

void Title::HideIndicator(){
    if(_indicator != NULL){
        _indicator->removeFromParentAndCleanup(true);
        _indicator = NULL;
    }
}
void Title::ShowIndicator(){
    _indicator = CSLoader::createNode("Indicator.csb");
    _indicator->setContentSize(size);
    ui::Helper::doLayout(_indicator);
    this->addChild(_indicator);
    cocostudio::timeline::ActionTimeline* action = CSLoader::createTimeline("Indicator.csb");
    _indicator->runAction(action);
    action->gotoFrameAndPlay(0, true);
}
void Title::OnUnlockFriends(){
    GameManager::getInstance()->playSoundEffect(SOUND_DDALKACK);
    ShowIndicator();
    sdkbox::IAP::purchase("unlockfriends");
}
void Title::UpdateFriendsPopup(){
    HideIndicator();
    char buf[30];
    bool unlocked = false;
    Sprite* spt;
    TextBMFont* text;
    Node* panel;
    Button* btn;
    const char* key;
    panel =_friendsPopup->getChildByName("panel");
    Text* lbl;
    btn =(Button*)panel->getChildByName("btnUnlockAll");
    lbl = (Text*)btn->getChildByName("lblUnlockPrice");
    if (UserDefault::getInstance()->getBoolForKey(KEY_UNLOCK_FRIENDS_ALL, false)) {
        lbl->setString("GOT");
    }else{
        lbl->setString(GameManager::getInstance()->allFriendsPriceText);
    }
    TextBMFont* lblCoinLeft = (TextBMFont*)panel->getChildByName("lblCoinLeft");
    lblCoinLeft->setString(cocos2d::Value(UserDefault::getInstance()->getIntegerForKey(KEY_COIN_LEFT, 0)).asString());
    
    for (int i = 0; i < 5; i++) {
        sprintf(buf, "btn%d", i);
        btn =(Button*)panel->getChildByName(buf);
        if(btn->getTag() == 0){
            key = KEY_BREE;
        }else if(btn->getTag() == 1){
            key = KEY_TOM;
        }else if(btn->getTag() == 2){
            key = KEY_KAREN;
        }else if(btn->getTag() == 3){
            key = KEY_KARL;
        }else if(btn->getTag() == 4){
            key = KEY_ANDREW;
        }
        if (canUnlockAchievement(i)) {
            sprintf(buf, KEY_ACHIEVEMENT_UNLOCKED_FORMAT, i);
            UserDefault::getInstance()->setBoolForKey(buf, true);
        }
        
        unlocked = isAchievementUnlocked(i) || UserDefault::getInstance()->getBoolForKey(key, false);
        
        sprintf(buf, "hero%d", i);
        Sprite* sptHero = (Sprite*)panel->getChildByName(buf);
        sptHero->setVisible(unlocked);
        
        sprintf(buf, "lblName%d", i);
        text = (TextBMFont*)panel->getChildByName(buf);
        text->setVisible(unlocked);
        
        sprintf(buf, "lblWeapon%d", i);
        text = (TextBMFont*)panel->getChildByName(buf);
        text->setVisible(unlocked);
        
        sprintf(buf, "lblCondition_%d", i);
        Node* temp = panel->getChildByName(buf);
        Text* lblCondition = (Text*)temp;
        lblCondition->setVisible(!unlocked);
        ((Label*)lblCondition)->setLineHeight(10);
        lblCondition->setFontName(LanguageManager::getInstance()->getStringForKey(STR_font));
        int strKey = STR_friend_condition_0;
        lblCondition->setString(LanguageManager::getInstance()->getStringForKey(strKey + i));
        
        Node* node = btn->getChildByName("lblUnlockPrice");
        lbl = (Text*)node;
        lbl->setVisible(!unlocked);
        if(i == 0){
            lbl->setString(GameManager::getInstance()->breePriceText);
        }else if(i == 1){
            lbl->setString(GameManager::getInstance()->tomPriceText);
        }else if(i == 2){
            lbl->setString(GameManager::getInstance()->karenPriceText);
        }else if(i == 3){
            lbl->setString(GameManager::getInstance()->karlPriceText);
        }else if(i == 4){
            lbl->setString(GameManager::getInstance()->andrewPriceText);
        }
//        lbl->enableOutline(Color4B::WHITE, 3);
        
        char levelBuf[30];
        sprintf(levelBuf, KEY_FRIEND_LEVEL_FORMAT, btn->getTag());
        int level = UserDefault::getInstance()->getIntegerForKey(levelBuf);
        if(i == 0){
            text->setString(cocos2d::__String::createWithFormat("W: Turner(%d)", level)->getCString());
        }else if(i == 1){
            text->setString(cocos2d::__String::createWithFormat("W: Shovel(%d)", level)->getCString());
        }else if(i == 2){
            text->setString(cocos2d::__String::createWithFormat("W: Pan(%d)", level)->getCString());
        }else if(i == 3){
            text->setString(cocos2d::__String::createWithFormat("W: Stick(%d)", level)->getCString());
        }else if(i == 4){
            text->setString(cocos2d::__String::createWithFormat("W: Tennis(%d)", level)->getCString());
        }
        
        sprintf(buf, "heroBackground%d", i);
        spt = (Sprite*)panel->getChildByName(buf);
        spt->setTexture(unlocked?"unlockedHero.png":"lockedAchievement.png");
        
        text = (TextBMFont*)btn->getChildByName("lblPrice");
        if(unlocked){
            int price = getFriendUpgradePrice(btn->getTag());
            text->setString(cocos2d::Value(price).asString());
            text->setVisible(true);
            
            sprintf(buf, "lblOr%d", i);
            text = (TextBMFont*)panel->getChildByName(buf);
            text->setVisible(false);
        }else{
            text->setVisible(false);
        }
    }
}
void Title::showAchievement(){
    NativeInterface::NativeInterface::showAchievements();
}
void Title::showPlaySubMenus(bool show){
    Sprite* spt = (Sprite*)_startPopup->getChildByName("playLinker");
    spt->setVisible(show);
    Button* btn = (Button*)_startPopup->getChildByName("playContinue");
    btn->setVisible(show);
    btn->setEnabled(UserDefault::getInstance()->getBoolForKey(KEY_CONTINUE_AVAILABLE, false));
    btn = (Button*)_startPopup->getChildByName("playNew");
    btn->setVisible(show);
    GameManager::getInstance()->currentScene = SCENE_TOUCH_TO_PLAY;
}
void Title::OnHeroClick(){
    GameManager::getInstance()->playSoundEffect(SOUND_DDALKACK);
    showPlaySubMenus(false);

    _heroPopup->setVisible(true);
    UpdateHeroPopup();
    
    GameManager::getInstance()->currentScene = SCENE_HERO;
}
void Title::OnHeroButtonClick(cocos2d::Ref *pSender){
    GameManager::getInstance()->playSoundEffect(SOUND_DDALKACK);
    Button* btn = (Button*)pSender;
    bool unlocked = isHeroUnlocked(btn->getTag());
    
    if(unlocked){
        bool wasSelected = UserDefault::getInstance()->getIntegerForKey(KEY_SELECTED_HERO, 0) == btn->getTag();
        UserDefault::getInstance()->setIntegerForKey(KEY_SELECTED_HERO, btn->getTag());
        if (wasSelected) {
            if(btn->getTag() == 2){
                
                int price = getAokiziLevelUpPrice();
                int coinLeft = UserDefault::getInstance()->getIntegerForKey(KEY_COIN_LEFT, 0);
                if(coinLeft >= price){
                    coinLeft -= price;
                    UserDefault::getInstance()->setIntegerForKey(KEY_COIN_LEFT, coinLeft);
                    int level = UserDefault::getInstance()->getIntegerForKey(KEY_AOKIZI_LEVEL, 0);
                    level++;
                    UserDefault::getInstance()->setIntegerForKey(KEY_AOKIZI_LEVEL, level);
                    UserDefault::getInstance()->flush();
                    UpdateHeroPopup();
                }else{
                    GameManager::getInstance()->playSoundEffect(SOUND_ROCKEY_LAUNCH);
                }
            }else if(btn->getTag() == 4){
                int price = getDaryLevelUpPrice();
                int coinLeft = UserDefault::getInstance()->getIntegerForKey(KEY_COIN_LEFT, 0);
                if(coinLeft >= price){
                    coinLeft -= price;
                    UserDefault::getInstance()->setIntegerForKey(KEY_COIN_LEFT, coinLeft);
                    int level = UserDefault::getInstance()->getIntegerForKey(KEY_DARY_LEVEL, 0);
                    level++;
                    UserDefault::getInstance()->setIntegerForKey(KEY_DARY_LEVEL, level);
                    UserDefault::getInstance()->flush();
                    UpdateHeroPopup();
                }else{
                    GameManager::getInstance()->playSoundEffect(SOUND_ROCKEY_LAUNCH);
                }
            }
        }
        UpdateHeroPopup();
        GameManager::getInstance()->playSoundEffect(SOUND_DDALKACK);
    }else{
        if(btn->getTag() == 3){
            int coinLeft = UserDefault::getInstance()->getIntegerForKey(KEY_COIN_LEFT, 0);
            if(coinLeft >= 10000){
                coinLeft -= 10000;
                UserDefault::getInstance()->setIntegerForKey(KEY_COIN_LEFT, coinLeft);
                UserDefault::getInstance()->setBoolForKey(KEY_HERMIONE_UNLOCKED, true);
                UserDefault::getInstance()->flush();
                UserDefault::getInstance()->setIntegerForKey(KEY_SELECTED_HERO, btn->getTag());
                UpdateHeroPopup();
            }else{
                GameManager::getInstance()->playSoundEffect(SOUND_ROCKEY_LAUNCH);
            }
        }else{
            GameManager::getInstance()->playSoundEffect(SOUND_ROCKEY_LAUNCH);
        }
    }
}
bool Title::isHeroUnlocked(int i){
    bool unlocked = false;
    if(i == 0){
        unlocked = true;
    }else if(i == 1){
        unlocked = UserDefault::getInstance()->getIntegerForKey(KEY_CLEAR_COUNT, 0) > 0;
        if (unlocked || UserDefault::getInstance()->getBoolForKey(KEY_ALL_HEROES, false)) {
            if (GameManager::getInstance()->market == MARKET_PLAYSTORE_PAID ||
                GameManager::getInstance()->market == MARKET_PLAYSTORE_FREE) {
                NativeInterface::NativeInterface::unlockAchievement("CgkIut_jhL8VEAIQBQ");
            }
        }
    }else if(i == 2){
        int starLeft = UserDefault::getInstance()->getIntegerForKey(KEY_STAR_COUNT, 0);
        unlocked = starLeft >= 2000 || UserDefault::getInstance()->getBoolForKey(KEY_AOKIZI_UNLOCKED, false);
        if (unlocked || UserDefault::getInstance()->getBoolForKey(KEY_ALL_HEROES, false)) {
            if (GameManager::getInstance()->market == MARKET_PLAYSTORE_PAID ||
                GameManager::getInstance()->market == MARKET_PLAYSTORE_FREE) {
                NativeInterface::NativeInterface::unlockAchievement("CgkIut_jhL8VEAIQBg");
            }
        }
    }else if(i == 3){
        unlocked = UserDefault::getInstance()->getBoolForKey(KEY_HERMIONE_UNLOCKED, false);
        if (unlocked || UserDefault::getInstance()->getBoolForKey(KEY_ALL_HEROES, false)) {
            if (GameManager::getInstance()->market == MARKET_PLAYSTORE_PAID ||
                GameManager::getInstance()->market == MARKET_PLAYSTORE_FREE) {
                NativeInterface::NativeInterface::unlockAchievement("CgkIut_jhL8VEAIQBw");
            }
        }
    }else if(i == 4){
        unlocked = UserDefault::getInstance()->getIntegerForKey(KEY_CLEAR_COUNT, 0) > 4;
        if (unlocked || UserDefault::getInstance()->getBoolForKey(KEY_ALL_HEROES, false)) {
            if (GameManager::getInstance()->market == MARKET_PLAYSTORE_PAID ||
                GameManager::getInstance()->market == MARKET_PLAYSTORE_FREE) {
                NativeInterface::NativeInterface::unlockAchievement("CgkIut_jhL8VEAIQCA");
            }
        }
    }

    return unlocked || UserDefault::getInstance()->getBoolForKey(KEY_ALL_HEROES, false);
}

void Title::UpdateHeroPopup(){
    char buf[30];
    bool unlocked = false;
    Sprite* spt;
    Sprite* sptCheck;
    TextBMFont* text;
    Node* panel;
    Button* btn;
    for (int i = 0; i < 5; i++) {
        panel =_heroPopup->getChildByName("panel");
        sprintf(buf, "btn%d", i);
        btn =(Button*)panel->getChildByName(buf);
        unlocked = isHeroUnlocked(i);
        if(i == 0){
        }else if(i == 1){
            text = (TextBMFont*)btn->getChildByName("lblCoin");
            text->setString(cocos2d::Value(UserDefault::getInstance()->getIntegerForKey(KEY_CLEAR_COUNT, 0)).asString());
        }else if(i == 2){
            text = (TextBMFont*)btn->getChildByName("lblCoin");
            spt = (Sprite*)btn->getChildByName("star");
            if (unlocked) {
                if(UserDefault::getInstance()->getIntegerForKey(KEY_SELECTED_HERO, 0) == i){
                    spt->setVisible(true);
                    spt->setSpriteFrame("coin0.png");
                    spt->setScale(0.6, 0.6);
                }else{
                    spt->setVisible(false);
                }
                text->setString(cocos2d::Value(UserDefault::getInstance()->getIntegerForKey(KEY_COIN_LEFT, 0)).asString());
            }else{
                spt->setSpriteFrame("blueStar.png");
                text->setString(cocos2d::Value(UserDefault::getInstance()->getIntegerForKey(KEY_STAR_COUNT, 0)).asString());
            }
        }else if(i == 3){
            if(unlocked){
                spt = (Sprite*)btn->getChildByName("coin");
                spt->setVisible(false);
            }
            
            text = (TextBMFont*)btn->getChildByName("lblCoin");
            text->setString(cocos2d::Value(UserDefault::getInstance()->getIntegerForKey(KEY_COIN_LEFT, 0)).asString());
        }else if(i == 4){
            text = (TextBMFont*)btn->getChildByName("lblCoin");
            text->setString(cocos2d::Value(UserDefault::getInstance()->getIntegerForKey(KEY_CLEAR_COUNT, 0)).asString());
        }
        if(i > 0){
            if(unlocked){
                if (i != 2) {
                    text->setVisible(false);
                }else{
                    Sprite* sptCoin = (Sprite*)text->getChildByName("coin");
                    sptCoin->setSpriteFrame("coin0.png");
                    sptCoin->setScale(0.6, 0.6);
                }
            }
        }
        sprintf(buf, "hero%d", i);
        spt = (Sprite*)panel->getChildByName(buf);
        spt->setVisible(unlocked);
        
        sprintf(buf, "check%d", i);
        spt = (Sprite*)panel->getChildByName(buf);
        spt->setVisible(UserDefault::getInstance()->getIntegerForKey(KEY_SELECTED_HERO, 0) == i);
        
        sprintf(buf, "lblName%d", i);
        text = (TextBMFont*)panel->getChildByName(buf);
        text->setVisible(unlocked);
        
        sprintf(buf, "lblWeapon%d", i);
        text = (TextBMFont*)panel->getChildByName(buf);
        text->setVisible(unlocked);
        if(i == 2){
            int level = UserDefault::getInstance()->getIntegerForKey(KEY_AOKIZI_LEVEL, 0);
            text->setString(cocos2d::__String::createWithFormat("W: Ice(lv.%d)", level)->getCString());
        }else if(i == 4){
            int level = UserDefault::getInstance()->getIntegerForKey(KEY_DARY_LEVEL, 0);
            text->setString(cocos2d::__String::createWithFormat("W: Rnd(lv.%d)", level)->getCString());
        }
        
        sprintf(buf, "check%d", i);
        sptCheck = (Sprite*)panel->getChildByName(buf);
        sptCheck->setVisible(UserDefault::getInstance()->getIntegerForKey(KEY_SELECTED_HERO, 0) == i);
        
        sprintf(buf, "heroBackground%d", i);
        spt = (Sprite*)panel->getChildByName(buf);
        spt->setTexture(unlocked?"unlockedHero.png":"lockedHero.png");
        
        text = (TextBMFont*)btn->getChildByName("lblPrice");
        if(UserDefault::getInstance()->getIntegerForKey(KEY_SELECTED_HERO, 0) == i){
            if (i == 2) {
                int price = getAokiziLevelUpPrice();
                text->setString(cocos2d::Value(price).asString());
            }else if(i == 4){
                int price = getDaryLevelUpPrice();
                text->setString(cocos2d::Value(price).asString());
                text->getChildByName("coin")->setVisible(true);
            }else{
                text->setString("Selected");
                text->setScale(0.8f, 0.8f);
                text->setPosition(Point(23.82f, 9.30f));
                GameManager::getInstance()->showParticleExplosion(sptCheck->getParent(), "check.png", sptCheck->getPosition(), 30, 0.5f, Widget::TextureResType::LOCAL);
            }
        }else{
            if(unlocked){
                text->setString("Select");
                text->setScale(1);
                text->setPosition(Point(23.82f, 9.30f));
                if(i == 4){
                    text->getChildByName("coin")->setVisible(false);
                }
            }else{
                if(i == 1){
                    text->setString("Game Clear\n     1 Time");
                    text->setScale(0.7f, 0.7f);
                }else if(i == 2){
                    
                }else if(i == 3){
                    
                }else if(i == 4){
                    text->getChildByName("coin")->setVisible(false);
                    text->setString("Game Clear\n     5 Times");
                    text->setScale(0.7f, 0.7f);
                }
            }
        }
    }
}
int Title::getAokiziLevelUpPrice(){
    int level = UserDefault::getInstance()->getIntegerForKey(KEY_AOKIZI_LEVEL, 0);
    int price = 100;
    for (int i = 0; i < level + 1; i++) {
        price += 10;
    }
    return price;
}
int Title::getDaryLevelUpPrice(){
    int level = UserDefault::getInstance()->getIntegerForKey(KEY_DARY_LEVEL, 0);
    int price = 120;
    for (int i = 0; i < level + 1; i++) {
        price += 70;
    }
    return price;
}

void Title::OnAchievementClick(){
    GameManager::getInstance()->playSoundEffect(SOUND_DDALKACK);
    
    _achievementPopup->setVisible(true);
    UpdateAchievementPopup();
    
    GameManager::getInstance()->currentScene = SCENE_ACHIEVEMENT;
}

void Title::showMine(){
    
    // mine
    _minePopup = (Widget*)CSLoader::createNode("Mine.csb");
    _minePopup->setContentSize(size);
    ui::Helper::doLayout(_achievementPopup);
    Node* panel = (Node*)_minePopup->getChildByName("panel");
    Button* btn = (Button*)_minePopup->getChildByName("panel")->getChildByName("btnWork");
    btn->addClickEventListener(CC_CALLBACK_0(Title::onWorkHarder, this));
    btn = (Button*)panel->getChildByName("ok");
    btn->addClickEventListener(CC_CALLBACK_0(Title::onMineClosed, this));
    this->addChild(_minePopup);
    
    GameManager::getInstance()->playSoundEffect(SOUND_DDALKACK);
    mineCoin = 0;

    int wave = UserDefault::getInstance()->getIntegerForKey(KEY_CHECK_POINT, 0);
    bool unlocked = wave > 55;
    if(unlocked){
        cocostudio::timeline::ActionTimeline* action = CSLoader::createTimeline("Mine.csb");
        action->gotoFrameAndPlay(0, true);
        _minePopup->runAction(action);
        unlocked = true;
        mineTime = 2;
        this->schedule(schedule_selector(Title::mineSchedule), 0.1f);
    }
    panel = _minePopup->getChildByName("panel");
    panel->getChildByName("lblCondition")->setVisible(!unlocked);
    panel->getChildByName("lblCoin")->setVisible(unlocked);
    panel->getChildByName("btnWork")->setVisible(unlocked);
    ((TextBMFont*)panel->getChildByName("lblCoin"))->setString(__String::createWithFormat("%d/%d", mineCoin, MAX_MINE_COIN)->getCString());
    UpdateAchievementPopup();
    
    GameManager::getInstance()->currentScene = SCENE_MINE;
}
void Title::onWorkHarder(){
    mineTime -= 0.7f;
    _minePopup->stopAllActions();
    cocostudio::timeline::ActionTimeline* action = CSLoader::createTimeline("Mine.csb");
    action->gotoFrameAndPlay(0, true);
    _minePopup->runAction(action);
    GameManager::getInstance()->playSoundEffect(SOUND_MACHINE_GUN_1);
}
void Title::mineSchedule(float dt){
    mineTime -= dt;
    if (mineTime < 0 && mineCoin < MAX_MINE_COIN) {
        mineTime = 2.3f;
        mineCoin++;
        Sprite* sptCoin = Sprite::create("coin0.png");
        Node* panel =_minePopup->getChildByName("panel");
        Node* mine = panel->getChildByName("sptMine");
        panel->addChild(sptCoin);
        ((TextBMFont*)panel->getChildByName("lblCoin"))->setString(__String::createWithFormat("%d/%d", mineCoin, MAX_MINE_COIN)->getCString());
        
        sptCoin->setPosition(mine->getPosition());
        sptCoin->runAction(Sequence::create(EaseOut::create(MoveBy::create(1, Point(0, 40)), 2), FadeOut::create(0.3f), CallFuncN::create(CC_CALLBACK_1(Title::spriteMoveDone, this)), NULL));
        GameManager::getInstance()->playSoundEffect(SOUND_COIN);
    }
}
void Title::spriteMoveDone(Node* node){
    node->removeFromParentAndCleanup(true);
}
int Title::getAchievementTotal(int i){
    int count = 0;
    if (i == 0) {
        count = 5;
    }else if(i == 1){
        count = 70;
    }else if(i == 2){
        count = 50;
    }else if(i == 3){
        count = 100000;
    }else if(i == 4){
        count = 15;
    }
    return count;
}
int Title::getCurrentAchievementCount(int i){
    char buf[30];
    sprintf(buf, KEY_ACHIEVEMENT_PROGRESS_FORMAT, i);
    int achievementCount = UserDefault::getInstance()->getIntegerForKey(buf, 0);
    return achievementCount;
}
bool Title::canUnlockAchievement(int i){
    int achievementCount = getCurrentAchievementCount(i);
    bool unlocked = achievementCount >= getAchievementTotal(i);
    return unlocked;
}
bool Title::isAchievementUnlocked(int i){
    char buf[30];
    sprintf(buf, KEY_ACHIEVEMENT_UNLOCKED_FORMAT, i);
    return UserDefault::getInstance()->getBoolForKey(buf);
}
int Title::getFriendLevel(int i){
    char levelBuf[30];
    sprintf(levelBuf, KEY_FRIEND_LEVEL_FORMAT, i);
    int level = UserDefault::getInstance()->getIntegerForKey(levelBuf);
    return level;
}
int Title::getFriendUpgradePrice(int i){
    int level = getFriendLevel(i);
    int price = 100 + level * 80;
    return price;
}
void Title::OnAchievementButtonClick(cocos2d::Ref *pSender){
    GameManager::getInstance()->playSoundEffect(SOUND_DDALKACK);
    Button* btn = (Button*)pSender;
    bool unlocked = isAchievementUnlocked(btn->getTag());
    
    if(unlocked && canUnlockAchievement(btn->getTag())){
        int price = getFriendUpgradePrice(btn->getTag());
        int coinLeft = UserDefault::getInstance()->getIntegerForKey(KEY_COIN_LEFT, 0);
        if(coinLeft >= price){
            coinLeft -= price;
            UserDefault::getInstance()->setIntegerForKey(KEY_COIN_LEFT, coinLeft);
            int level = getFriendLevel(btn->getTag());
            level++;
            char levelBuf[30];
            sprintf(levelBuf, KEY_FRIEND_LEVEL_FORMAT, btn->getTag());
            UserDefault::getInstance()->setIntegerForKey(levelBuf, level);
            
            UserDefault::getInstance()->flush();
            UpdateAchievementPopup();
        }else{
            GameManager::getInstance()->playSoundEffect(SOUND_ROCKEY_LAUNCH);
        }
        UpdateAchievementPopup();
    }else{
        if(canUnlockAchievement(btn->getTag())){
            char buf[30];
            sprintf(buf, KEY_ACHIEVEMENT_UNLOCKED_FORMAT, btn->getTag());
            UserDefault::getInstance()->setBoolForKey(buf, true);
            GameManager::getInstance()->showParticleExplosion(btn->getParent(), "coin0.png", btn->getPosition(), 30, 0.5f, Widget::TextureResType::LOCAL);
            UpdateAchievementPopup();
        }else{
            GameManager::getInstance()->playSoundEffect(SOUND_ROCKEY_LAUNCH);
        }
    }
}
void Title::onMineClosed(){
    _minePopup->stopAllActions();
    this->unschedule(schedule_selector(Title::mineSchedule));
    GameManager::getInstance()->playSoundEffect(SOUND_DDALKACK);
    _minePopup->setVisible(false);
    int coinLeft = UserDefault::getInstance()->getIntegerForKey(KEY_COIN_LEFT, 0);
    coinLeft += mineCoin;
    UserDefault::getInstance()->setIntegerForKey(KEY_COIN_LEFT, coinLeft);
    mineCoin = 0;
    _minePopup->removeFromParentAndCleanup(true);
    GameManager::getInstance()->currentScene = SCENE_TITLE;
}
void Title::OnAchievementPopupClose(){
    GameManager::getInstance()->playSoundEffect(SOUND_DDALKACK);
    _achievementPopup->setVisible(false);
    
    GameManager::getInstance()->currentScene = SCENE_TITLE;
}
void Title::UpdateAchievementPopup(){
    char buf[30];
    bool unlocked = false;
    Sprite* spt;
    TextBMFont* text;
    Node* panel;
    Button* btn;
    panel =_achievementPopup->getChildByName("panel");
    TextBMFont* lblCoinLeft = (TextBMFont*)panel->getChildByName("lblCoinLeft");
    lblCoinLeft->setString(cocos2d::Value(UserDefault::getInstance()->getIntegerForKey(KEY_COIN_LEFT, 0)).asString());
    for (int i = 0; i < 5; i++) {
        sprintf(buf, "btn%d", i);
        btn =(Button*)panel->getChildByName(buf);
        unlocked = isAchievementUnlocked(i) && canUnlockAchievement(i);
        
        sprintf(buf, "hero%d", i);
        Sprite* sptHero = (Sprite*)panel->getChildByName(buf);
        sptHero->setVisible(unlocked);
        
        sprintf(buf, "check%d", i);
        spt = (Sprite*)panel->getChildByName(buf);
        spt->setVisible(unlocked);
        
        sprintf(buf, "lblName%d", i);
        text = (TextBMFont*)panel->getChildByName(buf);
        text->setVisible(unlocked);
        
        sprintf(buf, "lblWeapon%d", i);
        text = (TextBMFont*)panel->getChildByName(buf);
        text->setVisible(unlocked);
        
        sprintf(buf, "lblCondition_%d", i);
        Node* temp = panel->getChildByName(buf);
        Text* lblCondition = (Text*)temp;
        lblCondition->setVisible(!unlocked);
        ((Label*)lblCondition)->setLineHeight(10);
        lblCondition->setFontName(LanguageManager::getInstance()->getStringForKey(STR_font));
        char keyBuf[50];
//        sprintf(keyBuf, KEY_ACHIEVEMENT_CONDITION_FORMAT, i);
        int strKey = STR_friend_condition_0;
        lblCondition->setString(LanguageManager::getInstance()->getStringForKey(strKey + i));
        
        char levelBuf[30];
        sprintf(levelBuf, KEY_FRIEND_LEVEL_FORMAT, btn->getTag());
        int level = UserDefault::getInstance()->getIntegerForKey(levelBuf);
        if(i == 0){
            text->setString(cocos2d::__String::createWithFormat("W: Turner(%d)", level)->getCString());
        }else if(i == 1){
            text->setString(cocos2d::__String::createWithFormat("W: Shovel(%d)", level)->getCString());
        }else if(i == 2){
            text->setString(cocos2d::__String::createWithFormat("W: Pan(%d)", level)->getCString());
        }else if(i == 3){
            text->setString(cocos2d::__String::createWithFormat("W: Stick(%d)", level)->getCString());
        }else if(i == 4){
            text->setString(cocos2d::__String::createWithFormat("W: Tennis(%d)", level)->getCString());
        }
        
        sprintf(buf, "heroBackground%d", i);
        spt = (Sprite*)panel->getChildByName(buf);
        spt->setTexture(unlocked?"unlockedHero.png":"lockedAchievement.png");
        
        text = (TextBMFont*)btn->getChildByName("lblPrice");
        if(unlocked){
            int price = getFriendUpgradePrice(btn->getTag());
            text->setString(cocos2d::Value(price).asString());
            text->getChildByName("coin")->setVisible(true);
        }else{
            if (canUnlockAchievement(i)) {
                text->setString("UNLOCK");
            }else{
                int current = getCurrentAchievementCount(i);
                int total = getAchievementTotal(i);
                char conditionBuf[30];
                if (total > 10000) {
                    sprintf(conditionBuf, "%d\n/%d", current, total);
                }else{
                    sprintf(conditionBuf, "%d/%d", current, total);
                }
                text->setString(conditionBuf);
            }
        }
    }
}

void Title::OnHeroPopupClose(){
    GameManager::getInstance()->playSoundEffect(SOUND_DDALKACK);
    _heroPopup->setVisible(false);
    
    GameManager::getInstance()->currentScene = SCENE_TOUCH_TO_PLAY;
}
void Title::OnShopClick(){
    GameManager::getInstance()->playSoundEffect(SOUND_DDALKACK);
	ShopLayer* shopLayer = ShopLayer::create();
    GameManager::getInstance()->currentScene = SCENE_TITLE_SHOP;
	this->addChild(shopLayer);
}
// Implementation of the keyboard event callback function prototype
void Title::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
	log("Key with keycode %d pressed", keyCode);

}

void Title::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
	log("Key with keycode %d released", keyCode);
    if (GameManager::getInstance()->currentScene == SCENE_TITLE) {
        /*if (gameQuitPopup == NULL) {
            gameQuitPopup = CSLoader::createNode("MessageBoxScene.csb");
            gameQuitPopup->setContentSize(size);
            ui::Helper::doLayout(gameQuitPopup);
            Node* panel = gameQuitPopup->getChildByName("panel");
            this->addChild(gameQuitPopup);
            Button* btn = (Button*)gameQuitPopup->getChildByName("block");
            btn->addClickEventListener(CC_CALLBACK_0(Title::onGameQuitPopupCancel, this));
            btn = (Button*)panel->getChildByName("close");
            btn->addClickEventListener(CC_CALLBACK_0(Title::onGameQuitPopupCancel, this));
            btn = (Button*)panel->getChildByName("ok");
            btn->addClickEventListener(CC_CALLBACK_0(Title::onGameQuitPopupCancel, this));
            Text* lbl = (Text*)gameQuitPopup->getChildByName("label");
            lbl->setString(LanguageManager::getInstance()->getStringForKey("back_key").c_str());
        }else*/{
            Director::getInstance()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
            exit(0);
#endif
        }
    }else if(GameManager::getInstance()->currentScene == SCENE_TOUCH_TO_PLAY) {
        CloseStartPopup();
    }else if(GameManager::getInstance()->currentScene == SCENE_PLAY) {
        showPlaySubMenus(false);
    }else if(GameManager::getInstance()->currentScene == SCENE_HERO) {
        OnHeroPopupClose();
    }else if(GameManager::getInstance()->currentScene == SCENE_ACHIEVEMENT) {
        OnAchievementPopupClose();
    }else if(GameManager::getInstance()->currentScene == SCENE_LOADING) {
        
    }else if(GameManager::getInstance()->currentScene == SCENE_RANKING) {
        CloseRanking();
    }else if(GameManager::getInstance()->currentScene == SCENE_ASK_NEW_GAME) {
        PlayNewCancel();
    }else if(GameManager::getInstance()->currentScene == SCENE_TITLE_SHOP){
        if (!GameManager::getInstance()->shopLayer->isWaiting) {
            GameManager::getInstance()->currentScene = SCENE_TITLE;
        }
    }else if(GameManager::getInstance()->currentScene == SCENE_SETTING){
        GameManager::getInstance()->currentScene = SCENE_TITLE;
        ((SettingLayer*)GameManager::getInstance()->settingLayer)->CloseSettingPopup();
    }else if(GameManager::getInstance()->currentScene == SCENE_SAVE_WARN ||
             GameManager::getInstance()->currentScene == SCENE_LOAD_WARN){
        ((SettingLayer*)GameManager::getInstance()->settingLayer)->onLoadWarnCancel();
    }else if(GameManager::getInstance()->currentScene == SCENE_SHARE_GAME){
        shareGameOk();
    }else if(GameManager::getInstance()->currentScene == SCENE_DAILY_REWARD){
        onDailyRewardClosed();
    }else if(GameManager::getInstance()->currentScene == SCENE_FRIENDS){
        OnFriendsClose();
    }
}
void Title::shareGame(){
    NativeInterface::NativeInterface::openUrl("https://game.nanoo.so/PlatformDefense");
    
    /*
    GameManager::getInstance()->playSoundEffect(SOUND_DDALKACK);
    GameManager::getInstance()->currentScene = SCENE_SHARE_GAME;
    
    _messageBox = CSLoader::createNode("MessageBoxScene.csb");
    Node* panel = _messageBox->getChildByName("panel");
    this->addChild(_messageBox, 100);
    Button* btn = (Button*)_messageBox->getChildByName("block");
    btn->addClickEventListener(CC_CALLBACK_0(Title::shareGameOk, this));
    btn = (Button*)panel->getChildByName("close");
    btn->addClickEventListener(CC_CALLBACK_0(Title::shareGameOk, this));
    btn = (Button*)panel->getChildByName("ok");
    btn->addClickEventListener(CC_CALLBACK_0(Title::shareGameOk, this));
    Text* lbl = (Text*)_messageBox->getChildByName("label");
    lbl->setFontName(LanguageManager::getInstance()->getStringForKey("font"));
    lbl->setString(LanguageManager::getInstance()->getStringForKey("share_game"));
     */
}
void Title::shareGameOk(){
    GameManager::getInstance()->currentScene = SCENE_TITLE;
    NativeInterface::NativeInterface::shareLink("Platform Defense", "https://play.google.com/store/apps/details?id=com.magmon.PlatformDefense");
    _messageBox->removeFromParentAndCleanup(true);
    _messageBox = NULL;
}
void Title::ShowSetting(){
    GameManager::getInstance()->playSoundEffect(SOUND_DDALKACK);
    SettingLayer* setting = SettingLayer::create();
    this->addChild(setting);
    GameManager::getInstance()->currentScene = SCENE_SETTING;
}

void Title::CloseRanking(){
    GameManager::getInstance()->playSoundEffect(SOUND_DDALKACK);
	_rankingPopup->setVisible(false);
    
    GameManager::getInstance()->currentScene = SCENE_TOUCH_TO_PLAY;
}
void Title::ShowRanking(){
    GameManager::getInstance()->playSoundEffect(SOUND_DDALKACK);
    showPlaySubMenus(false);
	_rankingPopup->setVisible(true);
	
	auto lbl = (TextBMFont*)_rankingPopup->getChildByName("totalEnemyKill");
	lbl->setString(cocos2d::Value(UserDefault::getInstance()->getIntegerForKey(KEY_TOTAL_ENEMY_KILL, 0)).asString());
    
    GameManager::getInstance()->currentScene = SCENE_RANKING;
}
void Title::ShowWaveRanking(){
    GameManager::getInstance()->playSoundEffect(SOUND_DDALKACK);
    const char* leaderboardID;
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    leaderboardID = "platform_defense_wave_ranking";
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    leaderboardID = "CgkIut_jhL8VEAIQCQ";
#endif
    log("ShowWaveRankning: %s", leaderboardID);
    NativeInterface::NativeInterface::showLeaderboard(leaderboardID);
}
void Title::ShowLeaderboard(){
    GameManager::getInstance()->playSoundEffect(SOUND_DDALKACK);
    const char* leaderboardID;
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    leaderboardID = "platform_defense_ranking";
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    leaderboardID = "CgkIut_jhL8VEAIQAg";
#endif
    log("ShowStandardRankning: %s", leaderboardID);
    NativeInterface::NativeInterface::showLeaderboard(leaderboardID);
}
void Title::ShowEnemyKillRanking(){
    GameManager::getInstance()->playSoundEffect(SOUND_DDALKACK);
    const char* leaderboardID;
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    leaderboardID = "platform_defense_total_enemy_kill";
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    leaderboardID = "CgkIut_jhL8VEAIQAQ";
#endif
    log("ShowStandardRankning: %s", leaderboardID);
    NativeInterface::NativeInterface::showLeaderboard(leaderboardID);
}
void Title::AskShowTutorial(){
    return;
	_messageBox = CSLoader::createNode("MessageBoxScene.csb");
	Node* panel = _messageBox->getChildByName("panel");
	this->addChild(_messageBox);
	Button* btn = (Button*)_messageBox->getChildByName("block");
	btn->addClickEventListener(CC_CALLBACK_0(Title::ShowTutorialCancel, this));
	btn = (Button*)panel->getChildByName("close");
	btn->addClickEventListener(CC_CALLBACK_0(Title::ShowTutorialCancel, this));
	btn = (Button*)panel->getChildByName("ok");
	btn->addClickEventListener(CC_CALLBACK_0(Title::ShowTutorial, this));
	Text* lbl = (Text*)_messageBox->getChildByName("label");
	lbl->setFontName(LanguageManager::getInstance()->getStringForKey(STR_font));
}
void Title::ShowTutorial(){
	_messageBox->removeFromParentAndCleanup(true);
	_messageBox = CSLoader::createNode("TutorialScene.csb");
	this->addChild(_messageBox);
	Button* btn = (Button*)_messageBox->getChildByName("button");
	btn->addClickEventListener(CC_CALLBACK_0(Title::OnTutorialNext, this));
	
	_tutorial = cocos2d::CSLoader::createTimeline("TutorialScene.csb");
	_messageBox->runAction(_tutorial);

	_lblNext = _messageBox->getChildByName("lblNext");
	_lblTutorial = (Text*)_messageBox->getChildByName("label");
	Node* bree = _messageBox->getChildByName("bree");
	_tutorialIndex = -1;
	_lblNext->runAction(RepeatForever::create(Sequence::create(FadeOut::create(0.1), DelayTime::create(0.3), FadeIn::create(0.1), DelayTime::create(0.4), NULL)));
	
	_tutorialLabelWidth = bree->getPositionX();// -_lblTutorial->getPositionX();
	
	_lblTutorial->setFontName(LanguageManager::getInstance()->getStringForKey(STR_font));
//	_lblTutorial->setString(LanguageManager::getInstance()->getStringForKey(STR_tutorial_start));
	_tutorial->play("jump", false); //not looping
}
void Title::ShowTutorialCancel(){
	_messageBox->removeFromParentAndCleanup(true);
}
void Title::OnTutorialNext(){
	_tutorialIndex++;
	if (_tutorialIndex >= 7){
		_messageBox->removeFromParentAndCleanup(true);
		return;
	}

	char buf[10];
//	sprintf(buf, "tutorial%d", _tutorialIndex);
//	_lblTutorial->setString(LanguageManager::getInstance()->getStringForKey(buf));
	_tutorial->play(buf, false);
}
void Title::TouchToPlay(){
    GameManager::getInstance()->playSoundEffect(SOUND_DDALKACK);
	_startPopup->setVisible(true);
    showPlaySubMenus(false);

    GameManager::getInstance()->currentScene = SCENE_TOUCH_TO_PLAY;
}
void Title::Play(){
    GameManager::getInstance()->playSoundEffect(SOUND_DDALKACK);
	showPlaySubMenus(true);
    GameManager::getInstance()->currentScene = SCENE_PLAY;
}
void Title::ShowLoadingScene(){
	_loadingNode = CSLoader::createNode("LoadingScene.csb");
    _loadingNode->setContentSize(size);
    ui::Helper::doLayout(_loadingNode);

    Button* btn = (Button*)_loadingNode->getChildByName("Button_1");
    btn->setScaleX(size.width/btn->getContentSize().width);
    
	auto lbl = (Text*)_loadingNode->getChildByName("lblTip");
	std::string tip = "";
	int key = cocos2d::random() % 12;
//	char keyBuf[12];
//	sprintf(keyBuf, "tip%d", key);
    int strKey = STR_tip0;

//	char buf[1000];
//	sprintf(buf, "TIP: %s",LanguageManager::getInstance()->getStringForKey(keyBuf).c_str());
//	lbl->setString(buf);
    
	lbl->setFontName(LanguageManager::getInstance()->getStringForKey(STR_font));
    lbl->setString(LanguageManager::getInstance()->getStringForKey(strKey + key));
	this->addChild(_loadingNode);

    this->scheduleOnce(schedule_selector(Title::LoadSceneLater), 2.1f);
    GameManager::getInstance()->currentScene = SCENE_LOADING;
}
void Title::PlayContinue(){
    GameManager::getInstance()->playSoundEffect(SOUND_DDALKACK);
//    UserDefault::getInstance()->setIntegerForKey(KEY_COIN_LEFT, 1000000);
    GameManager::getInstance()->IsNewGame = false;
	ShowLoadingScene();
	_stageToPlay = UserDefault::getInstance()->getIntegerForKey(KEY_CHECK_POINT, 1);
    if (_stageToPlay%100 == 0) {
        _stageToPlay -= 10;
    }
    _stageToPlay -= 1;
}

void Title::PlayNew(){
    GameManager::getInstance()->playSoundEffect(SOUND_DDALKACK);
    
	_messageBox = CSLoader::createNode("MessageBoxScene.csb");
	Node* panel = _messageBox->getChildByName("panel");
	panel->setPosition(_startPopup->getPosition()+ Point(-panel->getContentSize().width*panel->getScaleX()/2, -panel->getContentSize().height*panel->getScaleY()/2));
	this->addChild(_messageBox);
	Button* btn = (Button*)_messageBox->getChildByName("block");
	btn->addClickEventListener(CC_CALLBACK_0(Title::PlayNewCancel, this));
	btn = (Button*)panel->getChildByName("close");
	btn->addClickEventListener(CC_CALLBACK_0(Title::PlayNewCancel, this));
	btn = (Button*)panel->getChildByName("ok");
	btn->addClickEventListener(CC_CALLBACK_0(Title::PlayNewOK, this));
	Text* lbl = (Text*)_messageBox->getChildByName("label");
	lbl->setFontName(LanguageManager::getInstance()->getStringForKey(STR_font));
	lbl->setString(LanguageManager::getInstance()->getStringForKey(STR_new_game));
    
    GameManager::getInstance()->currentScene = SCENE_ASK_NEW_GAME;
}
void Title::PlayNewOK(){
    GameManager::getInstance()->playSoundEffect(SOUND_DDALKACK);
    UserDefault::getInstance()->setIntegerForKey(KEY_TOTAL_COIN_SPENT, 0);
    UserDefault::getInstance()->setBoolForKey(KEY_CONTINUED_GAME, false);
    
//    UserDefault::getInstance()->setIntegerForKey(KEY_COIN_LEFT, 2000000);
    
	_messageBox->removeFromParentAndCleanup(true);
	GameManager::getInstance()->IsNewGame = true;
    UserDefault::getInstance()->setBoolForKey(KEY_CLEAR_COUNT_READY_TO_ADD, true);
	UserDefault::getInstance()->setBoolForKey(KEY_CONTINUE_AVAILABLE, true);
    int wave = UserDefault::getInstance()->getIntegerForKey(KEY_CHECK_POINT, 1);
    if (wave > 45) {
        UserDefault::getInstance()->setIntegerForKey(KEY_CHECK_POINT, 1);
        char buf[30];
        sprintf(buf, KEY_ACHIEVEMENT_PROGRESS_FORMAT, 0);
        int count = UserDefault::getInstance()->getIntegerForKey(buf, 0);
        count++;
        UserDefault::getInstance()->setIntegerForKey(buf, count);
        
        if(GameManager::getInstance()->market == MARKET_APPSTORE_PAID){
            float percent = count*100.0f/getAchievementTotal(0);
            NativeInterface::NativeInterface::sendAchievement("pd_achievement_0", percent);
        }else{
            NativeInterface::NativeInterface::sendAchievement("CgkIut_jhL8VEAIQCg", count);
        }
    }
    UserDefault::getInstance()->setBoolForKey(KEY_ANTIAIR_USED, false);
    UserDefault::getInstance()->setBoolForKey(KEY_TRAP_USED, false);
	CloseStartPopup();
    ShowLoadingScene();
	_stageToPlay = 0;
}
void Title::PlayNewCancel(){
    GameManager::getInstance()->playSoundEffect(SOUND_DDALKACK);
	_messageBox->removeFromParentAndCleanup(true);
    
    GameManager::getInstance()->currentScene = SCENE_PLAY;
}
void Title::LoadSceneLater(float dt){
	_loadingNode->removeFromParentAndCleanup(true);
    _loadingNode = NULL;
    Director::getInstance()->pushScene(HelloWorld::scene(_stageToPlay));
}

void Title::CloseStartPopup(){
    GameManager::getInstance()->playSoundEffect(SOUND_DDALKACK);
	_startPopup->setVisible(false);
    GameManager::getInstance()->currentScene = SCENE_TITLE;
}
void Title::onGameQuitPopupCancel(){
    gameQuitPopup->removeFromParentAndCleanup(true);
    gameQuitPopup = NULL;
}

void Title::onDailyRewardCheck(float dt){
    if(!TimeManager::getInstance()->timeEstablished){
        if(!TimeManager::getInstance()->isTrying){
            TimeManager::getInstance()->getHttpTime();
        }
        return;
    }
    
    int lastDay = UserDefault::getInstance()->getIntegerForKey(KEY_DAILY_REWARD_DAY, 0);
    int lastMonth = UserDefault::getInstance()->getIntegerForKey(KEY_DAILY_REWARD_MONTH, 0);
    int lastYear = UserDefault::getInstance()->getIntegerForKey(KEY_DAILY_REWARD_YEAR, 0);
    
    int today = TimeManager::getInstance()->getDay();
    int month = TimeManager::getInstance()->getMonth();
    int year = TimeManager::getInstance()->getYear();
    
    bool differentDay = lastDay != today || lastMonth != month || lastYear != year;
    
    if (differentDay && UserDefault::getInstance()->getBoolForKey(KEY_CONTINUE_AVAILABLE, false) && (GameManager::getInstance()->currentScene == SCENE_TITLE ||GameManager::getInstance()->currentScene == SCENE_PLAY)) {
        UserDefault::getInstance()->setIntegerForKey(KEY_DAILY_REWARD_DAY, today);
        UserDefault::getInstance()->setIntegerForKey(KEY_DAILY_REWARD_MONTH, month);
        UserDefault::getInstance()->setIntegerForKey(KEY_DAILY_REWARD_YEAR, year);
        
        int yesterdayDay = TimeManager::getInstance()->getYesterdayDay();
        int yesterdayMonth = TimeManager::getInstance()->getYesterdayMonth();
        int yesterdayYear = TimeManager::getInstance()->getYesterdayYear();
        bool continued = lastDay == yesterdayDay &&
                        lastMonth == yesterdayMonth &&
                        lastYear == yesterdayYear;
        log("last day %d/%d/%d", lastDay, lastMonth, lastYear);
        log("today %d/%d/%d", today ,month, year);
        log("yesterday %d/%d/%d", yesterdayDay, yesterdayMonth, yesterdayYear);
        int howManyDays = UserDefault::getInstance()->getIntegerForKey(KEY_DAY_COUNT, 1);
        if (continued) {
            howManyDays++;
        }else{
            howManyDays = 1;
        }
        char buf[30];
        sprintf(buf, KEY_ACHIEVEMENT_PROGRESS_FORMAT, 4);
        int count = UserDefault::getInstance()->getIntegerForKey(buf, 0);
        if (count < howManyDays) {
            UserDefault::getInstance()->setIntegerForKey(buf, howManyDays);
            
            if(GameManager::getInstance()->market == MARKET_APPSTORE_PAID){
                float percent = howManyDays*100.0f/getAchievementTotal(4);
                NativeInterface::NativeInterface::sendAchievement("pd_achievement_4", percent);
            }else {
                NativeInterface::NativeInterface::sendAchievement("CgkIut_jhL8VEAIQDg", howManyDays);
            }
        }
        
        UserDefault::getInstance()->setIntegerForKey(KEY_DAY_COUNT, howManyDays);
        
        int rewardUnit = 120;
        int reward = 0;
        int rewardTomorrow = 0;
        for (int i = 0; i < howManyDays + 1; i++) {
            if (i < howManyDays) {
                reward += rewardUnit;
            }
            rewardTomorrow += rewardUnit;
            
            rewardUnit -= 10;
            if (rewardUnit < 10) {
                rewardUnit = 10;
            }
        }
        
        if (reward > 1050) {
            reward = 1050;
        }
        if (rewardTomorrow > 1050) {
            rewardTomorrow = 1050;
        }
        
        
        _messageBox = CSLoader::createNode("DailyRewardScene.csb");
        _messageBox->setContentSize(size);
        ui::Helper::doLayout(_messageBox);
        Node* panel = _messageBox->getChildByName("panel");
        this->addChild(_messageBox);
//        Button* btn = (Button*)_messageBox->getChildByName("block");
//        btn->addClickEventListener(CC_CALLBACK_0(Title::onDailyRewardClosed, this));
        Button* btn = (Button*)panel->getChildByName("close");
        btn->addClickEventListener(CC_CALLBACK_0(Title::onDailyRewardClosed, this));
        btn = (Button*)panel->getChildByName("ok");
        btn->addClickEventListener(CC_CALLBACK_0(Title::onDailyRewardClosed, this));
        Text* lbl = (Text*)panel->getChildByName("label");
        lbl->setString(LanguageManager::getInstance()->getStringForKey(STR_daily_reward).c_str());
        
        // days
        lbl = (Text*)panel->getChildByName("lblToday");
        lbl->setString(__String::createWithFormat("Day %d", howManyDays)->getCString());
        lbl = (Text*)panel->getChildByName("lblTomorrow");
        lbl->setString(__String::createWithFormat("Day %d", howManyDays + 1)->getCString());

        // rewards
        lbl = (Text*)panel->getChildByName("lblRewardToday");
        GameManager::getInstance()->makeItSiluk(lbl);
        lbl->setString(__String::createWithFormat("%d", reward)->getCString());
        lbl = (Text*)panel->getChildByName("lblRewardTomorrow");
        lbl->setString(__String::createWithFormat("%d", rewardTomorrow)->getCString());

        Sprite* sptDary = (Sprite*)panel->getChildByName("dary");
        int moveHeight = 20;
        sptDary->runAction(MoveBy::create(0, Point(0, -moveHeight/2)));
        sptDary->runAction(RepeatForever::create(Sequence::create(MoveBy::create(0.9f, Point(0, moveHeight)), MoveBy::create(0.9f, Point(0, -moveHeight)), NULL)));
        
        Sprite* sptCheck = (Sprite*)panel->getChildByName("sptCheck");
        sptCheck->runAction(Sequence::create(DelayTime::create(1), CallFunc::create(CC_CALLBACK_0(Title::playCoinSound, this)),FadeIn::create(0.1f), NULL));
        
        Firework* firework = Firework::create("coin0.png", 4, false);
        _messageBox->addChild(firework, -10);
        if (GameManager::getInstance()->currentScene == SCENE_TOUCH_TO_PLAY) {
            CloseStartPopup();
        }
        GameManager::getInstance()->currentScene = SCENE_DAILY_REWARD;
        
        int coinLeft = UserDefault::getInstance()->getIntegerForKey(KEY_COIN_LEFT, 0);
        coinLeft += reward;
        UserDefault::getInstance()->setIntegerForKey(KEY_COIN_LEFT, coinLeft);
        
        UserDefault::getInstance()->flush();
    }
}
void Title::playCoinSound(){
    GameManager::getInstance()->playSoundEffect(SOUND_COIN);
}
void Title::onDailyRewardClosed(){
    _messageBox->removeFromParentAndCleanup(true);
    GameManager::getInstance()->currentScene = SCENE_TITLE;
}