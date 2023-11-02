//
//  HudLayer.cpp
//  AlmostLegendary
//
//  Created by SUNG PILL PACK on 13. 6. 29..
//
//

#include "HudLayer.h"
#include "GameManager.h"
#include "HelloWorldScene.h"
#include "MyMessageBox.h"
#include "NativeInterface.h"
#include "LanguageManager.h"
#include "ShopLayer.h"
#include "Applause.h"
#include "Firework.h"
#include "ui/UIHelper.h"
bool HudLayer::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
	_lastUnlockedIndex = 0;
    weaponIndex = 0;
    size = Director::getInstance()->getWinSize();
    isRecording = false;
    listener = EventListenerKeyboard::create();
    listener->onKeyPressed = CC_CALLBACK_2(HudLayer::onKeyPressed, this);
    listener->onKeyReleased = CC_CALLBACK_2(HudLayer::onKeyReleased, this);
    isSharing = false;
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
    
    
    _tutorialNode = NULL;
    talkLayer = NULL;
    talkLabel = NULL;
    isSlowedBySystem = false;
    gameOverLayer = NULL;
    _pauseLayer = NULL;
    gameClearLayer = NULL;
    
    GameManager::getInstance()->firePressed = false;
    GameManager::getInstance()->jumpPressed = false;
    GameManager::getInstance()->leftPressed = false;
    GameManager::getInstance()->rightPressed = false;
	
	int gap = 55;
    pauseItem = MenuItemImage::create("pause.png", "pause.png", CC_CALLBACK_0(HudLayer::showPauseLayer, this));
	pauseItem->setVisible(false);
	pauseItem->setPosition(Point(size.width - gap, size.height - gap));

    Menu* menu =  Menu::create(pauseItem,  NULL);
    menu->setPosition(Point(0, 0));
    this->addChild(menu, 10);
    
    blackScreen = Sprite::create("whiteSquare.png");
    this->addChild(blackScreen);
	blackScreen->setColor(Color3B::BLACK);
    blackScreen->setScaleX(size.width);
    blackScreen->setScaleY(size.height);
    blackScreen->setPosition(Point(size.width/2, size.height/2));
    blackScreen->setVisible(false);
    
    registerControllerListener();
    
	_sptChooseThis = Droppable::createDroppable(UNIT_COIN, 1, 0, "blueRect.png");
	this->addChild(_sptChooseThis);
	_sptChooseThis->setScale(2);
	_sptChooseThis->setPosition(Point(size.width / 2, size.height + _sptChooseThis->getContentSize().height*_sptChooseThis->getScaleY()));
	Sprite* sptCircle = Sprite::create("whiteSquare.png");
	_sptChooseThis->addChild(sptCircle, -1);
	sptCircle->setScale(90, 22);
	sptCircle->setPosition(-15, 8.1f);
	sptCircle->setOpacity(100);
	sptCircle->setColor(Color3B(0,0,0));
	Label* lblChooseThis = Label::createWithBMFont("boldPixelFont.fnt", "CHOOSE");
	_sptChooseThis->addChild(lblChooseThis);
	lblChooseThis->setPosition(Point( - lblChooseThis->getContentSize().width / 2 - 10 ,
		_sptChooseThis->getContentSize().height/2));

    return true;
}
void HudLayer::ShowWrongRect(){
	_sptChooseThis->stopAllActions();
	_sptChooseThis->runAnimation("blueRect", true);
	_sptChooseThis->setScale(_notEnoughCoinsScale);
	_sptChooseThis->runAction(Sequence::create(EaseOut::create(ScaleTo::create(0.2f, _notEnoughCoinsScale*1.2f), 2), DelayTime::create(0.1f), EaseIn::create(ScaleTo::create(0.2f, _notEnoughCoinsScale), 2), NULL));
	_sptChooseThis->runAction(Sequence::create(EaseOut::create(MoveTo::create(0.3f, cocos2d::Point(_sptChooseThis->getPosition().x, size.height - _sptChooseThis->getContentSize().height*_sptChooseThis->getScaleY() - 50)), 5),
		DelayTime::create(2.2f),
		EaseIn::create(MoveTo::create(0.3f, cocos2d::Point(_sptChooseThis->getPosition().x, size.height + _sptChooseThis->getContentSize().height*_sptChooseThis->getScaleY())), 5), NULL));
    
    HideTrapInfo();
}
void HudLayer::changeSpeed()
{
    GameManager::getInstance()->playSoundEffect(SOUND_DDALKACK);
	IsFastMode = !IsFastMode;
	if (IsFastMode){
		_btnFast->loadTextures("2xPlay.png", "2xPlay.png", "2xPlay.png", Widget::TextureResType::PLIST);
		Director::getInstance()->SetTimeScale(FAST_SPEED);
	}
	else{
		_btnFast->loadTextures("1xPlay.png", "1xPlay.png", "1xPlay.png", Widget::TextureResType::PLIST);
		Director::getInstance()->SetTimeScale(1);
	}
	GameManager::getInstance()->getCurrentStageLayer()->changeSpeed();
}
void HudLayer::trapButtonClicked(cocos2d::Ref *pSender){
	Button* btn = (Button*)pSender;
    
    GameManager::getInstance()->playSoundEffect(SOUND_DDALKACK);
	int price = GameManager::getInstance()->GetTrapPrice(btn->getTag());
	if (GameManager::getInstance()->GetCoin() >= price || GameManager::getInstance()->developer){
		GameManager::getInstance()->getCurrentStageLayer()->showGuide(btn->getTag());
        GameManager::getInstance()->getCurrentStageLayer()->pauseLayer();
        if(_tutorialNode != NULL){
            _tutorialNode->removeFromParentAndCleanup(true);
            _tutorialNode = NULL;
        }
        GameManager::getInstance()->currentScene = SCENE_GAME_PLACE_TRAP;
        
        Trap* trap;
        int trapType = btn->getTag();
        if (trapType == TRAP_GASBURNER){
            trap = GasBurner::create("gasBurner.png", 0.3f);
        }
        else if (trapType == TRAP_FREEZER){
            trap = Freezer::create("fridge.png", 2.3f);
        }
        else if (trapType == TRAP_FIRE){
            trap = Fire::create("fire0_0.png", 0.3f);
        }
        else if (trapType == TRAP_SPIKE){
            trap = Spike::create("spike.png", 1.0f);
        }
        else if (trapType == TRAP_TOPSPIKE){
            trap = TopSpike::create("topSpike.png", 2.0f);
        }
        else if (trapType == TRAP_ICEBALL){
            trap = IceBall::create("iceBall.png", 1.5f);
        }
        else if (trapType == TRAP_BOMB){
            trap = Bomb::create("bomb2.png", 1.0f);
        }
        else if (trapType == TRAP_MACEBALL){
            trap = MaceBall::create("maceBall.png", 1.0f);
        }
        else if (trapType == TRAP_FLASK){
            trap = Flask::create("flask.png", 1.0f);
        }
        else if (trapType == TRAP_WALL){
            trap = Wall::create("wall.png", 1.0f);
        }
        else if (trapType == TRAP_HAND){
            trap = Hand::create("hand4.png", 3.3f);
        }
        else if (trapType == TRAP_WALLSPIKE){
            trap = WallSpike::create("spike.png", 2.5f);
        }
        else if (trapType == TRAP_CACTUS){
            trap = Cactus::create("heroWeapon18_0.png", 2.5f);
        }

        ShowTrapInfo(trap);
	}
	else{
		ShowNotEnoughCoinLabel();
	}
}
void HudLayer::ShowNewTrap(int index){
	Node* node = CSLoader::createNode("NewTrapScene.csb");
	this->addChild(node);
	
	Sprite* sptBox = (Sprite*)node->getChildByName("box");
	float moveY = sptBox->getContentSize().height*sptBox->getScaleY();
	node->setPosition(0, size.height
		);

	Sprite* sptTrap = (Sprite*)node->getChildByName("trap");

	sptTrap->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(getIconName(index)));
	float moveTime = 0.55f;
	node->runAction(Sequence::create(
		EaseOut::create(MoveTo::create(moveTime, cocos2d::Point(0, size.height - moveY)), 5),
		DelayTime::create(2.0f),
		EaseIn::create(MoveTo::create(moveTime, cocos2d::Point(0, size.height)), 5),
		CallFuncN::create(CC_CALLBACK_1(HudLayer::spriteMoveDone, this)),
		NULL));
	cocostudio::timeline::ActionTimeline* action = CSLoader::createTimeline("NewTrapScene.csb");
	node->runAction(action);
	action->gotoFrameAndPlay(0, true);
}
void HudLayer::afterCaptured(bool succeed, const std::string& outputFile)
{
    if (succeed)
    {
        filePath = outputFile;
        captureSuccess = true;
    }
    else
    {
        log("Capture screen failed.");
    }
}
void HudLayer::capture(float dt)
{
    captureSuccess = false;
    char buf[40];
    sprintf(buf, "screenShot_%d.png", waveToShow);
    utils::captureScreen(CC_CALLBACK_2(HudLayer::afterCaptured, this),buf);
}
void HudLayer::askShare(){
    if(!captureSuccess) return;
    
    shareDialog = CSLoader::createNode("ShareDialog.csb");
    shareDialog->setContentSize(size);
    ui::Helper::doLayout(shareDialog);
    Node* panel = shareDialog->getChildByName("panel");
    this->addChild(shareDialog);
    Button* btn = (Button*)shareDialog->getChildByName("block");
    btn = (Button*)panel->getChildByName("close");
    btn->addClickEventListener(CC_CALLBACK_0(HudLayer::onShareCancel, this));
    btn = (Button*)panel->getChildByName("ok");
    btn->addClickEventListener(CC_CALLBACK_0(HudLayer::onShareOk, this));
    Text* lbl = (Text*)shareDialog->getChildByName("label");
    char buf[14];
//    sprintf(buf, "askShare%d", cocos2d::random()%5);
    int strKey = STR_askShare0;
    lbl->setString(LanguageManager::getInstance()->getStringForKey(strKey + cocos2d::random()%5).c_str());
    
    // show screenshot
    auto sp = Sprite::create(filePath);
    sp->setAnchorPoint(Point::ZERO);
    // setup stencil shape
    DrawNode* shape = DrawNode::create();
    cocos2d::Point pts[6];
//    int offsetY = 250;
//    int offsetX = 50;
    int width = panel->getContentSize().width*panel->getScaleX() - 40;
    int height = panel->getContentSize().height*panel->getScaleY()*5/8;
    int gapY = height/8;
    Point centerPoint = panel->getPosition() + Point(0, 20);
    pts[0] = cocos2d::Point( centerPoint.x - width/2, centerPoint.y + height/2 - gapY );
    pts[1] = cocos2d::Point( centerPoint.x, centerPoint.y + height/2 );
    pts[2] = cocos2d::Point( centerPoint.x + width/2, centerPoint.y + height/2 - gapY);
    pts[3] = cocos2d::Point( centerPoint.x + width/2, centerPoint.y - height/2 + gapY);
    pts[4] = cocos2d::Point( centerPoint.x, centerPoint.y - height/2 );
    pts[5] = cocos2d::Point( centerPoint.x - width/2, centerPoint.y - height/2 + gapY );
//    pts[0] = cocos2d::Point( offsetX, size.height/2 + offsetY );
//    pts[1] = cocos2d::Point( sp->getContentSize().width/2, size.height/2 + (offsetY + 100) );
//    pts[2] = cocos2d::Point( sp->getContentSize().width - offsetX, size.height/2 + offsetY );
//    pts[3] = cocos2d::Point( sp->getContentSize().width - offsetX, size.height/2 - offsetY );
//    pts[4] = cocos2d::Point( sp->getContentSize().width/2, size.height/2 - (offsetY + 100) );
//    pts[5] = cocos2d::Point( offsetX, size.height/2 - offsetY );
    
    shape->drawPolygon(pts, 6, Color4F(1, 1, 1, 1), 3, Color4F(1, 0, 0, 1));
    
    // add shape in stencil
    ClippingNode* clip = ClippingNode::create();
    clip->setAnchorPoint(Point(0.5, 0.5));
    clip->setStencil(shape);
    clip->addChild(sp);
//    clip->setScale(0.4f);
//    cocos2d::Point localPos = cocos2d::Point(size.width*clip->getScale() / 2, size.height*clip->getScale() / 2);
//    cocos2d::Point pos = panel->getPosition() - localPos + cocos2d::Point(0, 30);

//    clip->setPosition(pos);
    shareDialog->addChild(clip);
    
//    if(waveToShow < 95 && waveToShow < 105){
        autoCloseCount = 5;
        sharePopupAutoClose(0);
        this->schedule(schedule_selector(HudLayer::sharePopupAutoClose), 1);
//    }
}

void HudLayer::sharePopupAutoClose(float dt){
    if(shareDialog == NULL) return;
    Label* lblNum = Label::createWithBMFont("boldPixelFont.fnt", cocos2d::Value(autoCloseCount).asString());
    lblNum->setScale(2);
    Node* panel = shareDialog->getChildByName("panel");
    Button* btn = (Button*)panel->getChildByName("close");
    btn->addChild(lblNum);
    lblNum->setPosition(Point(btn->getContentSize().width/2, + 24));
    lblNum->runAction(Sequence::create(ScaleTo::create(1, 0.4), CallFuncN::create(CC_CALLBACK_1(HudLayer::spriteMoveDone, this)), NULL));
    
    autoCloseCount--;
    if(autoCloseCount < 0){
        this->unschedule(schedule_selector(HudLayer::sharePopupAutoClose));
        onShareCancel();
    }
}
void HudLayer::onShareCancel(){
    GameManager::getInstance()->playSoundEffect(SOUND_DDALKACK);
    this->unschedule(schedule_selector(HudLayer::sharePopupAutoClose));
    shareDialog->removeFromParentAndCleanup(true);
    shareDialog = NULL;
    if(!GameManager::getInstance()->getCurrentStageLayer()->isGameOver){
        GameManager::getInstance()->getCurrentStageLayer()->resumeLayer();
        GameManager::getInstance()->getCurrentStageLayer()->loadWaveSchedule();
        GameManager::getInstance()->getCurrentStageLayer()->startWaveSchedule();
        ShowWave(waveToShow, true);
        GameManager::getInstance()->playSoundEffect(SOUND_BGM_WHOS_BOSS);
    }
    if (isSlowedBySystem) {
        changeSpeed();
        isSlowedBySystem = false;
    }
}

void HudLayer::onShareOk(){
    if (isSlowedBySystem) {
        changeSpeed();
        isSlowedBySystem = false;
    }
    GameManager::getInstance()->playSoundEffect(SOUND_DDALKACK);
    this->unschedule(schedule_selector(HudLayer::sharePopupAutoClose));
    shareDialog->removeFromParentAndCleanup(true);
    shareDialog = NULL;
    char buf[150];
    sprintf(buf, "Wave %d on Platform Defense!\nBest Defense Game Ever! \n\n", waveToShow);//iOS:goo.gl/OzKfaJ \nAndroid:goo.gl/OzKfaJ
    NativeInterface::NativeInterface::sharePhoto(filePath.c_str(), buf);
    isSharing = true;
    if (waveToShow != 100) {
        showPauseLayer();
    }
}

void HudLayer::rateUs(){
    rateDialog = CSLoader::createNode("ReviewBox.csb");
    rateDialog->setContentSize(size);
    ui::Helper::doLayout(rateDialog);
    Node* panel = rateDialog->getChildByName("panel");
    this->addChild(rateDialog);
    Button* btn = (Button*)rateDialog->getChildByName("block");
    btn = (Button*)panel->getChildByName("close");
    btn->addClickEventListener(CC_CALLBACK_0(HudLayer::onRateCancel, this));
    btn = (Button*)panel->getChildByName("ok");
    btn->addClickEventListener(CC_CALLBACK_0(HudLayer::onRateOk, this));
    Text* lbl = (Text*)panel->getChildByName("lblText");
    lbl->setString(LanguageManager::getInstance()->getStringForKey(STR_rate_us).c_str());
}

void HudLayer::onRateCancel(){
    GameManager::getInstance()->playSoundEffect(SOUND_DDALKACK);
    rateDialog->removeFromParentAndCleanup(true);
    rateDialog = NULL;
    if(!GameManager::getInstance()->getCurrentStageLayer()->isGameOver){
        GameManager::getInstance()->getCurrentStageLayer()->resumeLayer();
        GameManager::getInstance()->getCurrentStageLayer()->loadWaveSchedule();
        GameManager::getInstance()->getCurrentStageLayer()->startWaveSchedule();
        ShowWave(waveToShow, true);
        GameManager::getInstance()->playSoundEffect(SOUND_BGM_WHOS_BOSS);
    }
    if (isSlowedBySystem) {
        changeSpeed();
        isSlowedBySystem = false;
    }
}

void HudLayer::onRateOk(){
    if (isSlowedBySystem) {
        changeSpeed();
        isSlowedBySystem = false;
    }
    GameManager::getInstance()->playSoundEffect(SOUND_DDALKACK);
    rateDialog->removeFromParentAndCleanup(true);
    rateDialog = NULL;
    isSharing = true;
    
    if (waveToShow != 100) {
        showPauseLayer();
    }
    
    if (GameManager::getInstance()->market == MARKET_PLAYSTORE_FREE) {
        NativeInterface::NativeInterface::openUrl("http://play.google.com/store/apps/details?id=com.magmon.PlatformDefenseF");
    }else if(GameManager::getInstance()->market == MARKET_PLAYSTORE_PAID){
        NativeInterface::NativeInterface::openUrl("http://play.google.com/store/apps/details?id=com.magmon.PlatformDefense");
    }else if(GameManager::getInstance()->market == MARKET_APPSTORE_PAID){
        NativeInterface::NativeInterface::openUrl("itms-apps://itunes.apple.com/app/id1046407019");
    }else{
        return;
    }
}


void HudLayer::ShowNewMonster(int index, float delayTime){
//    if(IsFastMode){
//        changeSpeed();
//    }
	Node* node = CSLoader::createNode("NewMonsterScene.csb");
	node->setPosition(0, -size.height);
	this->addChild(node);
	Sprite* spt = (Sprite*)node->getChildByName("monster");
	char buf[20];
	sprintf(buf, "monster%d_0.png", index);
	spt->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(buf));
	float moveTime = 0.55f;
	node->runAction(Sequence::create(DelayTime::create(delayTime),
		EaseOut::create(MoveTo::create(moveTime, cocos2d::Point(0, 0)), 5),
		DelayTime::create(2.0f),
		EaseIn::create(MoveTo::create(moveTime, cocos2d::Point(0, size.height)), 5),
		CallFuncN::create(CC_CALLBACK_1(HudLayer::spriteMoveDone, this)),
		NULL));
	cocostudio::timeline::ActionTimeline* action = CSLoader::createTimeline("NewMonsterScene.csb");
	node->runAction(action);
	action->gotoFrameAndPlay(0, true);
	Text* lblDescription = (Text*)node->getChildByName("description");
	lblDescription->setFontName(LanguageManager::getInstance()->getStringForKey(STR_font));
	switch (GameManager::getInstance()->GetSpecialSkill(index))
	{
	case SPECIAL_NORMAL:
		lblDescription->setString(LanguageManager::getInstance()->getStringForKey(STR_ability_normal));
		//lblDescription->removeFromParentAndCleanup(true);
		break;
	case SPECIAL_SLIM:
		lblDescription->setString(LanguageManager::getInstance()->getStringForKey(STR_ability_slim));
		break;
	case SPECIAL_HARD:
		lblDescription->setString(LanguageManager::getInstance()->getStringForKey(STR_ability_hard));
		break;
	case SPECIAL_HEALTHY:
		lblDescription->setString(LanguageManager::getInstance()->getStringForKey(STR_ability_healthy));
		break;
	case SPECIAL_FAT:
		lblDescription->setString(LanguageManager::getInstance()->getStringForKey(STR_ability_fat));
		break;
	case SPECIAL_FIRE:
		lblDescription->setString(LanguageManager::getInstance()->getStringForKey(STR_ability_fire));
		break;
	case SPECIAL_FAST:
		lblDescription->setString(LanguageManager::getInstance()->getStringForKey(STR_ability_fast));
		break;
	case SPECIAL_ICE:
		lblDescription->setString(LanguageManager::getInstance()->getStringForKey(STR_ability_ice));
		break;
	case SPECIAL_FLY:
		lblDescription->setString(LanguageManager::getInstance()->getStringForKey(STR_ability_fly));
		break;
	case SPECIAL_BOSS:
		lblDescription->setString(LanguageManager::getInstance()->getStringForKey(STR_ability_boss));
		break;
	}
}
void HudLayer::ShowWave(int wave, bool checkPoint){
	const char* sceneName;
	if (checkPoint){
		sceneName = "WaveScene.csb";
	}
	else{
		sceneName = "WaveWithoutCheckPointScene.csb";
	}

	Node* node = CSLoader::createNode(sceneName);
	node->setPosition(0, -size.height);
	this->addChild(node);
	TextBMFont* lbl = (TextBMFont*)node->getChildByName("lblWave");
	lbl->setString(cocos2d::Value(wave).asString());
	float moveTime = 0.55f;
	float delayTime = 1.0f;
	node->runAction(Sequence::create(EaseOut::create(MoveTo::create(moveTime, cocos2d::Point(0, 0)), 5), 
		DelayTime::create(delayTime),
		EaseIn::create(MoveTo::create(moveTime, cocos2d::Point(0, size.height)), 5), 
		CallFuncN::create(CC_CALLBACK_1(HudLayer::spriteMoveDone, this)), 
		NULL));
	cocostudio::timeline::ActionTimeline* action = CSLoader::createTimeline(sceneName);
	node->runAction(action);
    action->gotoFrameAndPlay(0, true);
}

void HudLayer::ShowCheckPoint(int wave){
    if(IsFastMode) {
        changeSpeed();
        isSlowedBySystem = true;
    }
    this->scheduleOnce(schedule_selector(HudLayer::capture), 2.0f);
    GameManager::getInstance()->getCurrentStageLayer()->pauseLayer();
    waveToShow = wave;
    
    const char* sceneName = "CheckPointScene.csb";
    
    Node* node = CSLoader::createNode(sceneName);
    node->setPosition(0, -size.height);
    this->addChild(node);
    
    TextBMFont* lbl = (TextBMFont*)node->getChildByName("lblWave");
    lbl->setString(cocos2d::Value(wave).asString());
    lbl = (TextBMFont*)node->getChildByName("lblCurrentWave");
    lbl->setString(cocos2d::Value(wave).asString());
    float moveTime = 0.55f;
    float delayTime = 5.0f;
    int highScore = UserDefault::getInstance()->getIntegerForKey(KEY_HIGH_SCORE, 0);
    Action* afterAction;
    if (highScore < waveToShow) {
        afterAction = DelayTime::create(0);
        UserDefault::getInstance()->setIntegerForKey(KEY_HIGH_SCORE, waveToShow);
    }else{
        afterAction = CallFunc::create(CC_CALLBACK_0(HudLayer::closeAskShare, this));
    }
    CallFunc* call;
    const char* rateKey = "rateUsPlease";
    bool isRated = UserDefault::getInstance()->getBoolForKey(rateKey, false);
    if (wave == 20 && !isRated) {
        UserDefault::getInstance()->setBoolForKey(rateKey, true);
        call = CallFunc::create(CC_CALLBACK_0(HudLayer::rateUs, this));
    }else{
        call = CallFunc::create(CC_CALLBACK_0(HudLayer::askShare, this));
    }
    node->runAction(Sequence::create(EaseOut::create(MoveTo::create(moveTime, cocos2d::Point(0, 0)), 5),
                                     DelayTime::create(delayTime),
                                     EaseIn::create(MoveTo::create(moveTime, cocos2d::Point(0, size.height)), 5),
                                     call,
                                     afterAction,
                                     CallFuncN::create(CC_CALLBACK_1(HudLayer::spriteMoveDone, this)),
                                     NULL));
    cocostudio::timeline::ActionTimeline* action = CSLoader::createTimeline(sceneName);
    node->runAction(action);
    action->gotoFrameAndPlay(0, true);
    node->addChild(Applause::create());
    node->addChild(Firework::create());
}
void HudLayer::closeAskShare(){
    autoCloseCount = -1;
    this->sharePopupAutoClose(0);
}
void HudLayer::ShowWaveLater(){
    ShowWave(waveToShow, true);
}

void HudLayer::ShowNotEnoughCoinLabel(){
	_lblNotEnoughCoin->stopAllActions();
	_lblNotEnoughCoin->setScale(_notEnoughCoinsScale);
	_lblNotEnoughCoin->runAction(Sequence::create(EaseOut::create(ScaleTo::create(0.2f, _notEnoughCoinsScale*1.2f), 2), DelayTime::create(0.1f), EaseIn::create(ScaleTo::create(0.2f, _notEnoughCoinsScale), 2), NULL));
	_lblNotEnoughCoin->runAction(Sequence::create(EaseOut::create(MoveTo::create(0.3f, cocos2d::Point(_lblNotEnoughCoin->getPosition().x, size.height - _lblNotEnoughCoin->getContentSize().height*_lblNotEnoughCoin->getScaleY() - 50)), 5),
		DelayTime::create(0.2f),
		EaseIn::create(MoveTo::create(0.3f, cocos2d::Point(_lblNotEnoughCoin->getPosition().x, size.height + _lblNotEnoughCoin->getContentSize().height*_lblNotEnoughCoin->getScaleY())), 5), NULL));
   
    if(UserDefault::getInstance()->getIntegerForKey(KEY_NOT_ENOUGH_COIN_COUNT, 0) > 0){
        GameManager::getInstance()->getCurrentStageLayer()->pauseLayer();
        
        int currentCoin = GameManager::getInstance()->getCurrentStageLayer()->CollectedCoinCount;
        UserDefault::getInstance()->setIntegerForKey(KEY_COIN_LEFT, currentCoin);
        ShopLayer* shopLayer = ShopLayer::create();
        this->addChild(shopLayer, 1001);
        if (GameManager::getInstance()->currentScene != SCENE_GAME_UPGRADE_TRAP) {
            showPauseLayer();
        }
        
    }else{
        int count = UserDefault::getInstance()->getIntegerForKey(KEY_NOT_ENOUGH_COIN_COUNT, 0);
        count++;
        UserDefault::getInstance()->setIntegerForKey(KEY_NOT_ENOUGH_COIN_COUNT, count);
    }
    
}
void HudLayer::registerControllerListener()
{
    bool okay = true;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    float version = 0;//GoogleBilling::Billing::getSystemVersion();
    log("** iOS Version: %f", version);
    if (version < 7) {
        okay = false;
    }
#endif
    if (okay) {
        
        //Activate the listener into the event dispatcher
        //Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(_listener, this);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        //This function should be called for iOS platform
        Controller::startDiscoveryController();
#endif
        
    }
}

// Implementation of the keyboard event callback function prototype
void HudLayer::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
    log("Key with keycode %d pressed", (int)keyCode);
    

}

void HudLayer::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
    if (keyCode == EventKeyboard::KeyCode::KEY_BACK)
    {
        if (GameManager::getInstance()->currentScene == SCENE_IN_GAME) {
            showPauseLayer();
        }else if (GameManager::getInstance()->currentScene == SCENE_GAME_CLEAR) {
            onCloseGameClear();
        }else if (GameManager::getInstance()->currentScene == SCENE_GAME_ENDING) {
            // wait
        }else if (GameManager::getInstance()->currentScene == SCENE_GAME_OVER) {
//            OnQuit();
        }else if (GameManager::getInstance()->currentScene == SCENE_GAME_PLACE_TRAP) {
            GameManager::getInstance()->getCurrentStageLayer()->touchBegan(Point::ZERO);
        }else if (GameManager::getInstance()->currentScene == SCENE_GAME_UPGRADE_TRAP) {
            CloseUpgradeLayer();
        }else if (GameManager::getInstance()->currentScene == SCENE_PAUSE) {
            resumeGame();
        }else if(GameManager::getInstance()->currentScene == SCENE_GAME_TUTORIAL){
            GameManager::getInstance()->getCurrentStageLayer()->onTutorialClosed();
        }
    }
    log("Key with keycode %d released", (int)keyCode);
}

void HudLayer::keyDown(int key) {
    switch (key) {
        case 6: // Z
            GameManager::getInstance()->firePressed = true;
            break;
        case 7: // X
            GameManager::getInstance()->jumpPressed = true;
            break;
        case 8: // C
            GameManager::getInstance()->cPressed = true;
            break;
        case 123:
            GameManager::getInstance()->leftPressed = true;
            break;
        case 124:
            GameManager::getInstance()->rightPressed = true;
            break;
        case 125:
            GameManager::getInstance()->downPressed = true;
            break;
        case 126:
            GameManager::getInstance()->upPressed = true;
            break;
    }
//    log("%d pressed", key);
}

void HudLayer::keyUp(int key) {
    switch (key) {
        case 6: // Z
            GameManager::getInstance()->firePressed = false;
            break;
        case 7: // X
            GameManager::getInstance()->jumpPressed = false;
            break;
        case 8: // C
            GameManager::getInstance()->cPressed = false;
            break;
        case 123:
            GameManager::getInstance()->leftPressed = false;
            break;
        case 124:
            GameManager::getInstance()->rightPressed = false;
            break;
        case 125:
            GameManager::getInstance()->downPressed = false;
            break;
        case 126:
            GameManager::getInstance()->upPressed = false;
            break;
    }
//    log("%d released", key);
}
void HudLayer::showTalk(const char* talk){
    if (talkLabel == NULL) {
//        talkLayer = Layer::create();
        talkLabel = Label::createWithTTF(talk, LanguageManager::getInstance()->getStringForKey(STR_font).c_str(), 30);//Label::createWithSystemFont(talk, GameManager::getInstance()->getFont(FONT_ARIAL), 30);
        
//        talkLayer->addChild(talkLabel);
//        talkLabel->enableOutline(Color4B(80,80,80,255), 7);
        talkLabel->enableShadow();
//        talkLabel->enableGlow(Color4B(200,200,200,255));
//        talkLabel->setColor(Color3B::BLACK);
        this->addChild(talkLabel);
//        talkLabel->setVisible(false);
        
    }else{
//        hideTalk();
    }
//    hideTalk();
//    log("showTalk: %s", talk);
    talkLabel->stopAllActions();
    talkLabel->setString(talk);
    talkLabel->setPosition(Point(size.width/2, size.height/2 + 75));
    talkLabel->setScale(0.1);
//    talkLabel->runAction(Sequence::create(CCFadeIn::create(0.4), DelayTime::create(2), CCFadeOut::create(0.4), NULL));
    talkLabel->runAction(Sequence::create(EaseOut::create(ScaleTo::create(0.4, 1), 8), DelayTime::create(2), EaseIn::create(ScaleTo::create(0.4, 0.1), 8), NULL));
    talkLabel->runAction(Sequence::create(EaseOut::create(MoveBy::create(0.4, cocos2d::Point(0, 50)), 8), DelayTime::create(2), EaseIn::create(MoveBy::create(0.4, cocos2d::Point(0, -50)), 8), MoveBy::create(0.01, cocos2d::Point(0, 1000)),NULL));
//    talkLabel->runAction(Sequence::create(MoveBy::create(0.4, cocos2d::Point(0, 50)), DelayTime::create(2), MoveBy::create(0.4, cocos2d::Point(0, 50)),MoveBy::create(0.01, cocos2d::Point(0, 500)), NULL));
}
void HudLayer::hideTalk(){
    talkLabel->runAction(FadeOut::create(0.5));
    talkLabel->runAction(MoveBy::create(0.5, cocos2d::Point(0, 40)));
}
void HudLayer::showCoinShopLayer(){
    showShopLayer();
//    ((ShopLayer*)GameManager::getInstance()->getShopLayer())->openShop();
}

void HudLayer::showAchievementComplete(const char* text){
    log("show achievement");
    Sprite* achieveNode = Sprite::create("achievementBar.png");
    Sprite* spt = Sprite::create("achievementIconEnabled.png");
    achieveNode->addChild(spt);
    spt->setPosition(Point(80, achieveNode->getContentSize().height/2));
    this->addChild(achieveNode, 10000);
    
    Label* lblText = Label::createWithSystemFont(text, "Arial", 30);
    lblText->setAnchorPoint(Point(0, 0.5));
    lblText->setPosition(Point(spt->getPosition().x + 80, achieveNode->getContentSize().height/2));
    lblText->setTag(722);
    achieveNode->addChild(lblText);
    
    ((Label*)achieveNode->getChildByTag(722))->setString(text);
    achieveNode->setPosition(Point(size.width/2 + 15, size.height + (achieveNode->getContentSize().height/2)));
    
    achieveNode->runAction(Sequence::create(EaseIn::create(MoveBy::create(0.5f, cocos2d::Point(0, -achieveNode->getContentSize().height)),0.2f),
                                            CallFuncN::create(CC_CALLBACK_1(HudLayer::achievementEffect, this)),
                                            DelayTime::create(1),
                                            EaseIn::create(MoveBy::create(0.5f, cocos2d::Point(0, achieveNode->getContentSize().height)),4),
                                            CallFuncN::create(CC_CALLBACK_1(HudLayer::nodeMoveDone, this)),
                                            NULL));
}

void HudLayer::achievementEffect(Node* node){
    Sprite* achieveNode = (Sprite*)node;
    Sprite* checkSprite = Sprite::create("checkImage.png");
    checkSprite->setPosition(Point(achieveNode->getContentSize().width - 80, achieveNode->getContentSize().height/2));
    
    GameManager::getInstance()->playSoundEffect(SOUND_POWER_UP_45);
    
    ParticleExplosion* particle = ParticleExplosion::create();
    particle->setLife(0.4);                          // 지속시간
    particle->setLifeVar(0.3);
    particle->setPosition(checkSprite->getPosition());        // 위치
    particle->setSpeed(70);                      // 속도
    particle->setTotalParticles(15);
    //    particle->setScale(0.3);                          // 크기
    particle->setStartColor(Color4F(205, 205, 0, 155));
    particle->setEndColor(Color4F(205, 205, 0, 0));
    particle->setTexture(Director::getInstance()->getTextureCache()->addImage("checkImage.png"));          // 이미지 변경
    achieveNode->addChild(particle);
    
    achieveNode->addChild(checkSprite);
}
void HudLayer::nodeMoveDone(Ref* obj)
{
    if (obj == NULL) {
        return;
    }
    Node* node = (Node*)obj;
    node->stopAllActions();
    node->removeFromParentAndCleanup(true);
}

void HudLayer::setButtonSize(float siz) // 0 ~ 1
{
   
}
void HudLayer::enableJoystick(bool enable)
{
  
}
void HudLayer::addEnergyEffect(){
    int effectCount = 15;
    float dur = 0.5;
    float x = 10 + 47*(lifeArray.size() + 1);
    float y = size.height - 28;
    float interval = 0.05f;
    Sprite* spt;
    int spread = 80;
    for (int i = 0; i < effectCount; i++) {
        spt = Sprite::createWithSpriteFrameName("particle.png");
        spt->setPosition(Point(size.width/2, size.height/2) + cocos2d::Point(cocos2d::random()%spread - spread/2, cocos2d::random()%spread - spread/2));
        cocos2d::Point pos = spt->getPosition();
        spt->setScale(3 + (cocos2d::random()%10)*0.4);
        this->addChild(spt, 100);
        spt->runAction(FadeOut::create(0));
        spt->runAction(Sequence::create(DelayTime::create(i*interval), FadeIn::create(interval), NULL));
        spt->runAction(Sequence::create(DelayTime::create(i*interval), EaseIn::create(MoveTo::create(dur, cocos2d::Point(x, y)), 3), CallFuncN::create(CC_CALLBACK_1(HudLayer::spriteMoveDone, this)), NULL));
    }
    this->scheduleOnce(schedule_selector(HudLayer::addEnergyDone), dur + interval*effectCount);
}
void HudLayer::addEnergyDone(float dt){
    setLife((int)lifeArray.size() + 1);
    float x = 10 + 47*(lifeArray.size() + 1);
    float y = size.height - 28;
    Sprite* spt = Sprite::create("hudHeart.png");
    this->addChild(spt);
    spt->setPosition(Point(x, y));
    float dur =0.5;
    spt->runAction(ScaleTo::create(dur, 3));
    spt->runAction(Sequence::create(FadeOut::create(dur), CallFuncN::create(CC_CALLBACK_1(HudLayer::spriteMoveDone, this)), NULL));
    GameManager::getInstance()->playSoundEffect(SOUND_MEDICINE);
    GameManager::getInstance()->getCurrentStageLayer()->player->energy = (int)lifeArray.size();
    GameManager::getInstance()->getCurrentStageLayer()->player->maxEnergy = (int)lifeArray.size();
}
void HudLayer::initializeSreen(int lifeCount)
{
	_lastUnlockedIndex = UserDefault::getInstance()->getIntegerForKey(KEY_NEW_TRAP_INDEX, 0);
	int uiScale = 5;
	Sprite* sptRail = Sprite::createWithSpriteFrameName("rail.png");
	
	for (int i = 0; i < size.height; i += sptRail->getContentSize().height*3){
		sptRail->setAnchorPoint(Point(0, 1));
		sptRail->setPosition(-14, size.height - i);
		sptRail->setScale(3);
		this->addChild(sptRail);

		if (i + sptRail->getContentSize().height*3 > size.height){
			break;
		}
		sptRail = Sprite::createWithSpriteFrameName("rail.png");
	}

	Sprite* sptWoodRail = Sprite::createWithSpriteFrameName("woodRailHead.png");

	for (int i = 0; i < size.height; i += sptRail->getContentSize().height*uiScale){
		sptWoodRail->setAnchorPoint(Point(1, 1));
		sptWoodRail->setPosition(size.width, size.height - i);
		sptWoodRail->setScale(uiScale*0.9f, uiScale);
		this->addChild(sptWoodRail);

		if (i + sptWoodRail->getContentSize().height*uiScale > size.height){
			break;
		}
		sptWoodRail = Sprite::createWithSpriteFrameName("woodRailBody.png");
	}

	Button* btnPause = Button::create("pause.png", "pause.png", "pause.png", ui::Widget::TextureResType::PLIST);
	btnPause->setScale(uiScale);
	btnPause->addClickEventListener(CC_CALLBACK_0(HudLayer::showPauseLayer, this));
	this->addChild(btnPause);
	btnPause->setPosition(Point(size.width - btnPause->getContentSize().width*uiScale / 2 - uiScale*2, size.height - btnPause->getContentSize().height*uiScale / 2 - 2));

	_btnFast = Button::create("1xPlay.png", "1xPlay.png", "1xPlayGray.png", ui::Widget::TextureResType::PLIST);
	_btnFast->addClickEventListener(CC_CALLBACK_0(HudLayer::changeSpeed, this));
	this->addChild(_btnFast);
	_btnFast->setScale(uiScale);
	_btnFast->setPosition(Point(size.width - _btnFast->getContentSize().width*uiScale / 2 - uiScale*2, _btnFast->getContentSize().height*uiScale / 2 + 2));
    if (GameManager::getInstance()->market == MARKET_PLAYSTORE_FREE) {
        _btnFast->setEnabled(false);
    }
	IsFastMode = false;

	// list view
	_trapListview = ListView::create();
	_trapListview->setDirection(ui::ScrollView::Direction::VERTICAL);
	_listViewWidth = 54*uiScale/3;
	_trapListview->setPosition(Point(size.width - _listViewWidth, _btnFast->getContentSize().height*uiScale + 5));
	_trapListview->setContentSize(Size(_listViewWidth, size.height - _btnFast->getContentSize().height*uiScale - btnPause->getContentSize().height*uiScale - 10));
	_trapListview->setColor(Color3B::BLUE);
	_trapListview->setItemsMargin(70*uiScale/3);

	_lblNotEnoughCoin = Label::createWithBMFont("boldPixelFont.fnt", "NOT ENOUGH COINS!");
	_notEnoughCoinsScale = 3;
	_lblNotEnoughCoin->setScale(_notEnoughCoinsScale, _notEnoughCoinsScale);
	this->addChild(_lblNotEnoughCoin,1000);
	_lblNotEnoughCoin->setPosition((size.width - _listViewWidth) / 2, size.height + _lblNotEnoughCoin->getContentSize().height*_lblNotEnoughCoin->getScaleY());

	// test
//	std::string testKey = GameManager::getInstance()->GetSlotKey(0);
//	UserDefault::getInstance()->setIntegerForKey(testKey.c_str(), 1);
//	int what = UserDefault::getInstance()->getIntegerForKey(testKey.c_str());
//	UserDefault::getInstance()->flush();

	char buf[20];
	int topHeight = 48;
//	int trapIndex = -1;
//	int slotIndex = 0;
    int fontSize = 40;
	Point extraPos;
    int rotation = 0;
	TrapButtonList.clear();
	for (int i = 0; i < TOTAL_TRAP_COUNT; i++)
	{
		Button* btn = Button::create("ironRect.png", "ironRect.png", "grayRect.png", ui::Widget::TextureResType::PLIST);
		btn->setAnchorPoint(Point(0, 1));
		btn->setScale(uiScale);
		btn->addClickEventListener(CC_CALLBACK_1(HudLayer::trapButtonClicked, this));
		_trapListview->pushBackCustomItem(btn);
		TrapButtonList.pushBack(btn);
		btn->setTag(i);
		if (i == TRAP_GASBURNER){
			sprintf(buf, "gasBurner.png");
			extraPos = cocos2d::Point(0, -1.5f);
		}
		else if (i == TRAP_FREEZER){
			sprintf(buf, "fridge.png");
			extraPos = cocos2d::Point(0.5f, 0);
		}
		else if (i == TRAP_FIRE){
			sprintf(buf, "fireIcon.png");
			extraPos = cocos2d::Point(0, -1);
		}
		else if (i == TRAP_SPIKE){
			sprintf(buf, "spike.png");
			extraPos = cocos2d::Point(0.0f, -0.5f);
		}
		else if (i == TRAP_TOPSPIKE){
			sprintf(buf, "topSpike.png");
			extraPos = cocos2d::Point(0.0f, 1.0f);
		}
		else if (i == TRAP_ICEBALL){
			sprintf(buf, "iceBall.png");
			extraPos = cocos2d::Point(0.0f, -0.0f);
		}
		else if (i == TRAP_BOMB){
			sprintf(buf, "bomb.png");
			extraPos = cocos2d::Point(0.0f, 2.0f);
		}
		else if (i == TRAP_MACEBALL){
			sprintf(buf, "maceBallIcon.png");
			extraPos = cocos2d::Point(0.0f, 0.0f);
		}
		else if (i == TRAP_FLASK){
			sprintf(buf, "flask.png");
			extraPos = cocos2d::Point(0.0f, 2.0f);
		}
		else if (i == TRAP_WALL){
			sprintf(buf, "wallIcon.png");
			extraPos = cocos2d::Point(0.0f, 0.0f);
		}
		else if (i == TRAP_HAND){
			sprintf(buf, "hand.png");
			extraPos = cocos2d::Point(0.0f, 0.0f);
		}
		else if (i == TRAP_WALLSPIKE){
			sprintf(buf, "wallSpike.png");
			extraPos = cocos2d::Point(0.0f, 0.0f);
        }
        else if (i == TRAP_CACTUS){
            sprintf(buf, "heroWeapon18_0.png");
            extraPos = cocos2d::Point(0.0f, 0.0f);
            rotation = -90;
        }
		
		Sprite* sptIcon = Sprite::createWithSpriteFrameName(buf);
		btn->addChild(sptIcon);
		sptIcon->setName("sptIcon");
		sptIcon->setPosition(Point(btn->getContentSize().width/2, btn->getContentSize().height/2 ) + extraPos);
        sptIcon->setRotation(rotation);

		Sprite* sptCoin = Sprite::createWithSpriteFrameName("coin0.png");
		sptCoin->setScale(0.3);
		btn->addChild(sptCoin);
		sptCoin->setName("sptCoin");

		sprintf(buf, "%d", GameManager::getInstance()->GetTrapPrice(i));
		Label* lblCoin = Label::createWithTTF(buf, "bitdust2.ttf", fontSize);
		lblCoin->setScale(0.15f);
		lblCoin->setName("lblCoin");
		btn->addChild(lblCoin);
		lblCoin->setPosition(9, -3.5f);
		sptCoin->setPosition(lblCoin->getPosition().x - lblCoin->getContentSize().width*lblCoin->getScale() / 2 - 1.5f, lblCoin->getPosition().y - 0.5f);

		Sprite* sptCoinBox = Sprite::createWithSpriteFrameName("coinBox.png");
		btn->addChild(sptCoinBox);
		sptCoinBox->setPosition(lblCoin->getPosition() + cocos2d::Point(-1.2f,-0.5f));//Point(lblCoin->getContentSize().width*lblCoin->getScale(), lblCoin->getContentSize().height*lblCoin->getScale() / 2));
		sptCoinBox->setLocalZOrder(-1);
	}
	
	_trapListview->pushBackCustomItem(Button::create());

	this->addChild(_trapListview);
	_trapListview->setClippingEnabled(true);
	_trapListview->setBounceEnabled(true);
	_trapListview->setClippingType(Layout::ClippingType::SCISSOR);


	sprintf(buf, "WAVE %d", 1);

	LblWaveCount = Label::createWithTTF(buf, "bitdust2.ttf", fontSize);
	this->addChild(LblWaveCount);
	LblWaveCount->setPosition(Point(10, size.height - LblWaveCount->getContentSize().height / 2));
	LblWaveCount->setAnchorPoint(Point(0, 0.5));
	LblWaveCount->setTextColor(Color4B(253, 240, 128, 255));
	LblWaveCount->enableOutline(Color4B(82, 102, 101, 255), 3);

	LblCoinCount = Label::createWithTTF(cocos2d::Value(GameManager::getInstance()->GetCoin()).asString(), "bitdust2.ttf", fontSize);
	this->addChild(LblCoinCount);
	LblCoinCount->setPosition(Point(size.width / 2, size.height - LblCoinCount->getContentSize().height / 2));
	LblCoinCount->setTextColor(Color4B(253, 240, 128, 255));
	LblCoinCount->enableOutline(Color4B(82, 102, 101, 255), 3);

	Sprite* sptCoin = Sprite::createWithSpriteFrameName("coin0.png");
	LblCoinCount->addChild(sptCoin);
	sptCoin->setAnchorPoint(Point(1, 0.5));
	sptCoin->setName("sptCoin");
	int coinCount = 0;
	sprintf(buf, "%d", coinCount);
	LblCoinCount->setString(buf);
	sptCoin->setPosition(-5, LblCoinCount->getContentSize().height / 2 - 3);
	sptCoin->setScale(2);

	LblStarCount = Label::createWithTTF(cocos2d::Value(GameManager::getInstance()->GetStarCount()).asString(), "bitdust2.ttf", fontSize);
	this->addChild(LblStarCount);
	LblStarCount->setAnchorPoint(Point(1, 0.5));
	LblStarCount->setPosition(Point(size.width - _listViewWidth - 10, size.height - LblStarCount->getContentSize().height / 2));
	LblStarCount->setTextColor(Color4B(253, 240, 128, 255));
	LblStarCount->enableOutline(Color4B(82, 102, 101, 255), 3);

	Sprite* sptStar = Sprite::createWithSpriteFrameName("blueStar.png");
	LblStarCount->addChild(sptStar);
	sptStar->setAnchorPoint(Point(1, 0.5));
	sptStar->setName("sptStar");
	
	sptStar->setPosition(-5, LblStarCount->getContentSize().height / 2 - 3);
	sptStar->setScale(3.5);

	Sprite* whiteRect = Sprite::create("whiteSquare.png");
	whiteRect->setPosition(Point(0, size.height - topHeight));
	this->addChild(whiteRect, -1);
	whiteRect->setAnchorPoint(Point(0, 0));
	whiteRect->setScale(size.width, topHeight);
	whiteRect->setOpacity(120);

    setLife(lifeCount);

    _pauseLayer = CSLoader::createNode("PauseScene.csb");
	Button* btn = (Button*)_pauseLayer->getChildByName("block");
	btn->addClickEventListener(CC_CALLBACK_0(HudLayer::resumeGame, this));

	Node* panel = _pauseLayer->getChildByName("panel");
	btn = (Button*)panel->getChildByName("resume");
	btn->addClickEventListener(CC_CALLBACK_0(HudLayer::resumeGame, this));
	btn = (Button*)panel->getChildByName("close");
	btn->addClickEventListener(CC_CALLBACK_0(HudLayer::resumeGame, this));
	btn = (Button*)_pauseLayer->getChildByName("block");
	btn->addClickEventListener(CC_CALLBACK_0(HudLayer::resumeGame, this));
	btn = (Button*)panel->getChildByName("home");
	btn->addClickEventListener(CC_CALLBACK_0(HudLayer::toMainByPauseLayer, this));

	_pauseLayer->setPosition(Point(size.width / 2 - _pauseLayer->getContentSize().width / 2, 0));
	this->addChild(_pauseLayer, 1000);
	_pauseLayer->setVisible(false);


	_whiteLine = Sprite::create("whiteSquare.png");
	this->addChild(_whiteLine);
	_whiteLine->setVisible(false);

	_whiteRect = Sprite::create("whiteSquare.png");
	this->addChild(_whiteRect);
	_whiteRect->setVisible(false);
	_whiteRect->setOpacity(100);

	
	_upgradeScene = CSLoader::createNode("UpgradeScene.csb");
	panel = _upgradeScene->getChildByName("panel");

	btn = (Button*)_upgradeScene->getChildByName("block");
	btn->addClickEventListener(CC_CALLBACK_0(HudLayer::CloseUpgradeLayer, this));
	btn = (Button*)panel->getChildByName("upgrade");
	btn->addClickEventListener(CC_CALLBACK_0(HudLayer::UpgradeTrapClick, this));
	btn = (Button*)panel->getChildByName("sell");
	btn->addClickEventListener(CC_CALLBACK_0(HudLayer::SellTrapClick, this));
	this->addChild(_upgradeScene);
	_upgradeScene->setVisible(false);

	_trapInfoScene = CSLoader::createNode("TrapInfo.csb");
	this->addChild(_trapInfoScene);
    _trapInfoScene->setContentSize(size);
    ui::Helper::doLayout(_trapInfoScene);
	_trapInfoScene->setVisible(false);
    
	UpdateTrapButton();
}
void HudLayer::continueGame(){
//    blackLayer->removeFromParentAndCleanup(true);
    GameManager::getInstance()->playSoundEffect(SOUND_BGM_WHOS_BOSS);
    GameManager::getInstance()->getCurrentStageLayer()->continueGame();
//    endingNode->removeFromParentAndCleanup(true);
}
void HudLayer::addStar(){
    Sprite* sptStar = Sprite::create("blueStar.png");
    sptStar->setScale(6);
    sptStar->setPosition(Point(size.width/2, - 50));
    sptStar->setRotation(-5);
    sptStar->runAction(RotateBy::create(10, 50));
    GameManager::getInstance()->addYellowTurningBackground(sptStar, sptStar->getContentSize().width*2);
    Node* star= LblStarCount->getChildByName("sptStar");
    sptStar->runAction(Sequence::create(EaseOut::create(MoveTo::create(0.6f, cocos2d::Point(size.width/2, size.height*2/3)), 3), DelayTime::create(0.4f), EaseIn::create(MoveTo::create(0.3f, LblStarCount->getPosition() + star->getPosition()), 2), CallFunc::create(CC_CALLBACK_0(HudLayer::showStarParticle, this)),CallFunc::create(CC_CALLBACK_0(HudLayer::addStarLater, this)), CallFuncN::create(CC_CALLBACK_1(HudLayer::spriteMoveDone, this)), NULL));
    sptStar->runAction(Sequence::create(DelayTime::create(1.0f), ScaleTo::create(0.3f, 3), NULL));
    this->addChild(sptStar);
}
void HudLayer::HideTutrorialHelper(){
    if(_tutorialNode != NULL){
        _tutorialNode->removeFromParentAndCleanup(true);
        _tutorialNode = NULL;
    }
}
void HudLayer::ShowTutorialHelper(int index, cocos2d::Point pos){
    HideTutrorialHelper();
    _tutorialNode = CSLoader::createNode("TutorialHelperScene.csb");
    cocostudio::timeline::ActionTimeline* action = CSLoader::createTimeline("TutorialHelperScene.csb");
    _tutorialNode->runAction(action);
    action->gotoFrameAndPlay(0, true);
    
    
    if(index == 0)  // trap button
    {
        _trapListview->setInnerContainerPosition(Point(0, _trapListview->getContentSize().height-_trapListview->getInnerContainerSize().height));
        pos = _trapListview->getPosition() + cocos2d::Point(40, _trapListview->getContentSize().height - 40);
        GameManager::getInstance()->getCurrentStageLayer()->pauseLayer();
        this->addChild(_tutorialNode, 999);
        _tutorialNode->setPosition(pos);
        return;
    }
    else if(index == 1)    // trap
    {
//        HelloWorld* layer = GameManager::getInstance()->getCurrentStageLayer();
//        layer->
    }
    else   // upgrade
    {
        
    }
    this->addChild(_tutorialNode, 999);
    _tutorialNode->setPosition(pos);
}
void HudLayer::showStarParticle(){
    Node* star= LblStarCount->getChildByName("sptStar");
    GameManager::getInstance()->showParticleExplosion(LblStarCount, "blueStar.png", star->getPosition() + cocos2d::Point(-star->getContentSize().width*star->getScaleX()/2, 0), 50, 2, Widget::TextureResType::PLIST);
}
void HudLayer::addStarLater(){
    GameManager::getInstance()->AddStarCount(1);
    UpdateTrapButton();
}
int HudLayer::GetUnlockStarCount(int index){
	switch (index){
	case 0:
		return 0;
	case 1:
		return 4;
	case 2:
		return 7;
	case 3:
		return 10;
	case 4:
		return 20;
	case 5:
		return 40;
	case 6:
		return 70;
	case 7:
		return 100;
	case 8:
		return 110;
	case 9:
		return 200;
	case 10:
		return 400;
	case 11:
		return 700;
	case 12:
		return 1000;
	case 13:
		return 1500;
	}
	return 50;
}
void HudLayer::HideTrapInfo(){

	_trapInfoScene->setVisible(false);
}
void HudLayer::ShowTrapInfo(Point tilePos){
    Trap* trap = GameManager::getInstance()->getCurrentStageLayer()->GetTrap(tilePos);
    ShowTrapInfo(trap);
}
void HudLayer::ShowTrapInfo(Trap* trap){
	_trapInfoScene->setVisible(true);
	TextBMFont* lblLevel = (TextBMFont*)_trapInfoScene->getChildByName("level");
	TextBMFont* lblAtt = (TextBMFont*)_trapInfoScene->getChildByName("attack");
	Text* lblDescription = (Text*)_trapInfoScene->getChildByName("description");
	Sprite* ironBoard = (Sprite*)_trapInfoScene->getChildByName("ironBoard");
//	char buf[50];
    std::string description;
    int rotation = 0;
	const char* iconName;
	switch (trap->TrapType){
	case TRAP_GASBURNER:
        description = LanguageManager::getInstance()->getStringForKey(STR_TRAP_GASBURNER);
        iconName = "gasBurner.png";
		break;
    case TRAP_FREEZER:
        description = LanguageManager::getInstance()->getStringForKey(STR_TRAP_FREEZER);
		iconName = "fridge.png";
		break;
    case TRAP_WALL:
        description = LanguageManager::getInstance()->getStringForKey(STR_TRAP_WALL);
		iconName = "wallIcon.png";
		break;
    case TRAP_FLASK:
        description = LanguageManager::getInstance()->getStringForKey(STR_TRAP_FLASK);
		iconName = "flask.png";
		break;
    case TRAP_MACEBALL:
        description = LanguageManager::getInstance()->getStringForKey(STR_TRAP_MACEBALL);
		iconName = "maceBallIcon.png";
		break;
    case TRAP_BOMB:
        description = LanguageManager::getInstance()->getStringForKey(STR_TRAP_BOMB);
		iconName = "bomb.png";
		break;
    case TRAP_FIRE:
        description = LanguageManager::getInstance()->getStringForKey(STR_TRAP_FIRE);
		iconName = "fireIcon.png";
		break;
    case TRAP_SPIKE:
        description = LanguageManager::getInstance()->getStringForKey(STR_TRAP_SPIKE);
		iconName = "spike.png";
		break;
    case TRAP_TOPSPIKE:
        description = LanguageManager::getInstance()->getStringForKey(STR_TRAP_TOPSPIKE);
		iconName = "topSpike.png";
		break;
    case TRAP_ICEBALL:
        description = LanguageManager::getInstance()->getStringForKey(STR_TRAP_ICEBALL);
		iconName = "iceBall.png";
		break;
    case TRAP_HAND:
        description = LanguageManager::getInstance()->getStringForKey(STR_TRAP_HAND);
		iconName = "hand.png";
		break;
    case TRAP_WALLSPIKE:
        description = LanguageManager::getInstance()->getStringForKey(STR_TRAP_WALLSPIKE);
		iconName = "wallSpike.png";
        break;
    case TRAP_CACTUS:
        description = LanguageManager::getInstance()->getStringForKey(STR_TRAP_CACTUS);
        iconName = "heroWeapon18_0.png";
        rotation = -90;
        break;
	}

	lblLevel->setString(cocos2d::Value(trap->Level).asString());
	lblAtt->setString(cocos2d::Value(trap->GetPower()).asString());
    log("description: %s", description.c_str());
    lblDescription->stopAllActions();
    lblDescription->runAction(RepeatForever::create(Sequence::create(ScaleTo::create(0.4, 1.0, 1.0), ScaleTo::create(0.4, 1.0, 1.1), NULL)));
    lblDescription->setFontName(LanguageManager::getInstance()->getStringForKey(STR_font));
    lblDescription->setString(description);
//	GameManager::getInstance()->SetStringWithAutoLineBreak(lblDescription, description, ironBoard->getContentSize().width*ironBoard->getScaleX());

	Sprite* sptIcon = (Sprite*)_trapInfoScene->getChildByName("trap");
	sptIcon->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(iconName));
    sptIcon->setRotation(rotation);

}
void HudLayer::ShowUpgradeLayer(Point screenPos, cocos2d::Point tilePos, cocos2d::Point tileCenter){
	GameManager::getInstance()->getCurrentStageLayer()->pauseLayer();
	ShowTrapInfo(tilePos);
	
	_clickedTrapPos = tilePos;
	_clickedTrapPosInScreen = tileCenter;

	Node* panel = _upgradeScene->getChildByName("panel");
	Point endPos = GameManager::getInstance()->getCurrentStageLayer()->EndPos;
	float extraY = tilePos.y - endPos.y< 2 ? -100 : 100;
	tileCenter += cocos2d::Point(0, extraY);
	float half = panel->getContentSize().width*panel->getScaleX() / 2;
	float x = clampf(tileCenter.x, half, size.width - _listViewWidth - half);
	half = panel->getContentSize().height*panel->getScaleY() / 2;
	float y = clampf(tileCenter.y, half, size.height + half);
	_upgradeScene->setVisible(true);
	_upgradeScene->setPosition(x - _upgradeScene->getContentSize().width / 2, y - _upgradeScene->getContentSize().height / 2);
	
	TextBMFont* lbl = (TextBMFont*)panel->getChildByName("price");

	int price = GameManager::getInstance()->GetUpgradePrice(tilePos);
	lbl->setString(cocos2d::Value(price).asString());

	_whiteLine->setVisible(true);
	_whiteLine->setAnchorPoint(Point(0.5, 0));
	_whiteLine->setPosition(tileCenter + cocos2d::Point(0, -extraY));
	_whiteLine->setScale(2 / _whiteLine->getContentSize().width, extraY / _whiteLine->getContentSize().height);

	_whiteRect->setVisible(true);
    _whiteRect->setScale(TILE_SIZE * GameManager::getInstance()->getCurrentStageLayer()->getScale() / _whiteRect->getContentSize().width);
	_whiteRect->setPosition(tileCenter + cocos2d::Point(0, -extraY));
    
    if (_tutorialNode != NULL) {
        HideTutrorialHelper();
        Button* btn = (Button*)panel->getChildByName("upgrade");
        _tutorialNode = CSLoader::createNode("TutorialHelperScene.csb");
        _tutorialNode->setScale(1/3.0f);
        btn->getParent()->addChild(_tutorialNode);
        _tutorialNode->setPosition(btn->getPosition());
        
        cocostudio::timeline::ActionTimeline* action = CSLoader::createTimeline("TutorialHelperScene.csb");
        _tutorialNode->runAction(action);
        action->gotoFrameAndPlay(0, true);
    }
    GameManager::getInstance()->currentScene = SCENE_GAME_UPGRADE_TRAP;
}

const char* HudLayer::getIconName(int index){
	const char* iconName;
	
	switch (index){
	case TRAP_GASBURNER:
		iconName = "gasBurner.png";
		break;
	case TRAP_FREEZER:
		iconName = "fridge.png";
		break;
	case TRAP_WALL:
		iconName = "wallIcon.png";
		break;
	case TRAP_FLASK:
		iconName = "flask.png";
		break;
	case TRAP_MACEBALL:
		iconName = "maceBallIcon.png";
		break;
	case TRAP_BOMB:
		iconName = "bomb.png";
		break;
	case TRAP_FIRE:
		iconName = "fireIcon.png";
		break;
	case TRAP_SPIKE:
		iconName = "spike.png";
		break;
	case TRAP_TOPSPIKE:
		iconName = "topSpike.png";
		break;
	case TRAP_ICEBALL:
		iconName = "iceBall.png";
		break;
	case TRAP_HAND:
		iconName = "hand.png";
		break;
	case TRAP_WALLSPIKE:
		iconName = "wallSpike.png";
        break;
    case TRAP_CACTUS:
        iconName = "heroWeapon18_0.png";
        break;
	}
	return iconName;
}
void HudLayer::CloseUpgradeLayer(){
	GameManager::getInstance()->getCurrentStageLayer()->resumeLayer();
	_upgradeScene->setVisible(false);
	_whiteLine->setVisible(false);
	_whiteRect->setVisible(false);
    _trapInfoScene->setVisible(false);
    
    GameManager::getInstance()->currentScene = SCENE_IN_GAME;
}
void HudLayer::UpgradeTrapClick(){
    if(_tutorialNode != NULL){
        _tutorialNode->removeFromParentAndCleanup(true);
        _tutorialNode = NULL;
    }
	int price = GameManager::getInstance()->GetUpgradePrice(_clickedTrapPos);
	if (GameManager::getInstance()->GetCoin() >= price){
		Node* panel = _upgradeScene->getChildByName("panel");
		TextBMFont* lbl = (TextBMFont*)panel->getChildByName("price");
		GameManager::getInstance()->AddCoin(-price);
        GameManager::getInstance()->getCurrentStageLayer()->coinSpent += price;
		GameManager::getInstance()->getCurrentStageLayer()->UpgradeTrap(_clickedTrapPos);

		LblCoinCount->setString(cocos2d::Value(GameManager::getInstance()->GetCoin()).asString());

		lbl->setString(cocos2d::Value(GameManager::getInstance()->GetUpgradePrice(_clickedTrapPos)).asString());
		
		TextBMFont* lblLevel = (TextBMFont*)_trapInfoScene->getChildByName("level");
		TextBMFont* lblAtt = (TextBMFont*)_trapInfoScene->getChildByName("attack");
		Trap* trap = GameManager::getInstance()->getCurrentStageLayer()->GetTrap(_clickedTrapPos);
		
		lblLevel->setString(cocos2d::Value(trap->Level).asString());
		lblAtt->setString(cocos2d::Value(trap->GetPower()).asString());

		GameManager::getInstance()->showParticleExplosion(this, "coin0.png", _clickedTrapPosInScreen, 100, 10, Widget::TextureResType::PLIST);
		GameManager::getInstance()->playSoundEffect(SOUND_DDALKACK);
		GameManager::getInstance()->playSoundEffect(SOUND_COIN);
	}
	else{
		GameManager::getInstance()->playSoundEffect(SOUND_ROCKEY_LAUNCH);
		ShowNotEnoughCoinLabel();
	}
}
void HudLayer::SellTrapClick(){
	_messageBox = CSLoader::createNode("MessageBoxScene.csb");
    _messageBox->setContentSize(size);
    ui::Helper::doLayout(_messageBox);
	Node* panel = _messageBox->getChildByName("panel");
	this->addChild(_messageBox);
	Button* btn = (Button*)_messageBox->getChildByName("block");
	btn->addClickEventListener(CC_CALLBACK_0(HudLayer::SellTrapCancel, this));
	btn = (Button*)panel->getChildByName("close");
	btn->addClickEventListener(CC_CALLBACK_0(HudLayer::SellTrapCancel, this));
	btn = (Button*)panel->getChildByName("ok");
	btn->addClickEventListener(CC_CALLBACK_0(HudLayer::SellTrapOk, this));
	Text* lbl = (Text*)_messageBox->getChildByName("label");
	Trap* trap = GameManager::getInstance()->getCurrentStageLayer()->GetTrap(_clickedTrapPos);
	char buf[100];
	sprintf(buf, "Do you want to sell it for %d coins?", trap->PriceValue*60/100);
	lbl->setString(buf);
	//GameManager::getInstance()->SetStringWithAutoLineBreak(lbl, buf, panel->getContentSize().width);
}
void HudLayer::SellTrapOk(){
	Trap* trap = GameManager::getInstance()->getCurrentStageLayer()->GetTrap(_clickedTrapPos);
	char buf[5];
	int price = trap->PriceValue * 60 / 100;
	sprintf(buf, "+%d", price);
	Label* lbl = Label::createWithBMFont("boldPixelFont.fnt", buf);
	lbl->setScale(4);
	lbl->setPosition(_clickedTrapPosInScreen);
	lbl->runAction(Sequence::create(MoveBy::create(1, cocos2d::Point(0, 50)),
		DelayTime::create(0.5f), 
		CallFuncN::create(CC_CALLBACK_1(HudLayer::spriteMoveDone, this)), NULL));
	this->addChild(lbl); 
	lbl->setColor(Color3B(255, 236, 79));
	Sprite* sptCoin = Sprite::createWithSpriteFrameName("coin0.png");
	lbl->addChild(sptCoin);
	sptCoin->setPosition(-sptCoin->getContentSize().width/2 + 3, lbl->getContentSize().height / 2);
	sptCoin->setScale(0.7f);
	GameManager::getInstance()->getCurrentStageLayer()->SellTrap(_clickedTrapPos);
	GameManager::getInstance()->AddCoin(price);
	LblCoinCount->setString(cocos2d::Value(GameManager::getInstance()->GetCoin()).asString());
	_upgradeScene->setVisible(false);
	_messageBox->removeFromParentAndCleanup(true);
	_whiteLine->setVisible(false);
	_whiteRect->setVisible(false);
	_trapInfoScene->setVisible(false);
	UpdateTrapButton();
	GameManager::getInstance()->getCurrentStageLayer()->SaveTrap(_clickedTrapPos, -1, 0);
	GameManager::getInstance()->getCurrentStageLayer()->resumeLayer();
}
void HudLayer::SellTrapCancel(){
	_upgradeScene->setVisible(false);
	_messageBox->removeFromParentAndCleanup(true);
	_whiteLine->setVisible(false);
	_whiteRect->setVisible(false);
	_trapInfoScene->setVisible(false);
	GameManager::getInstance()->getCurrentStageLayer()->resumeLayer();
}
void HudLayer::UpdateTrapButton(){
	Label* lblCoin;
	Sprite* sptIcon;
	Sprite* sptCoin;
	Button* btn;
	bool unlocked;
	int price;
	bool isNew = false;
	for (int i = 0; i < TrapButtonList.size(); i++){
		btn = TrapButtonList.at(i);
		lblCoin = (Label*)btn->getChildByName("lblCoin");
		sptCoin = (Sprite*)btn->getChildByName("sptCoin");
		sptIcon = (Sprite*)btn->getChildByName("sptIcon");

		price = GetUnlockStarCount(i);
		unlocked = price <= GameManager::getInstance()->GetStarCount() || GameManager::getInstance()->developer;
		if (unlocked){
			lblCoin->setString(cocos2d::Value(GameManager::getInstance()->GetTrapPrice(i)).asString());
			sptCoin->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("coin0.png"));
			sptCoin->setScale(0.3);
			sptIcon->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(getIconName(i)));
			if (_lastUnlockedIndex < i){
				isNew = true;
				UserDefault::getInstance()->setIntegerForKey(KEY_NEW_TRAP_INDEX, i);
				_lastUnlockedIndex = i;
			}
		}
		else{
			lblCoin->setString(cocos2d::Value(price).asString());
			sptCoin->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("blueStar.png"));
			sptCoin->setScale(0.7);
			sptIcon->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("lockIcon.png"));
		}
		btn->setEnabled(unlocked);
		sptCoin->setPosition(lblCoin->getPosition().x - lblCoin->getContentSize().width*lblCoin->getScale() / 2 - sptCoin->getContentSize().width*sptCoin->getScaleX()/2, lblCoin->getPosition().y - 0.5f);
	}
	if (isNew){
		ShowNewTrap(_lastUnlockedIndex);
	}
}
void HudLayer::reduceLife()
{
    Sprite* sprite = (Sprite*)lifeArray.at(lifeArray.size()-1);
    lifeArray.eraseObject(sprite);
    sprite->removeFromParentAndCleanup(true);
}

void HudLayer::setLife(int eng)
{
    for(auto life:lifeArray)
    {
        life->removeFromParentAndCleanup(true);
    }
    lifeArray.clear();
}

void HudLayer::keyCollected()
{
    key->setTexture("hudGoldKey.png");

}
void HudLayer::starCollected(int count)
{
    cocos2d::Point pos;
    if (count == 1) {
        star1->setTexture("hudGoldStar.png");
        pos = star1->getPosition();
    }else if(count == 2){
        star2->setTexture("hudGoldStar.png");
        pos = star2->getPosition();
    }else if(count == 3){
        star3->setTexture("hudGoldStar.png");
        pos = star3->getPosition();
    }
    
    starCount = count;
}

void HudLayer::starCollectedFancy(int count, cocos2d::Point collectedPos)
{
    Sprite* sprite = Sprite::createWithSpriteFrameName("goldStar.png");
    sprite->setPosition(collectedPos);
//    sprite->runAction(EaseIn::create(RotateBy::create(0.5f, 720), 0.1f));
    sprite->runAction(Sequence::create(ScaleTo::create(0.2, 4.2),
                                         ScaleTo::create(0.1, 3.8),
                                         ScaleTo::create(0.1, 4),
                                         DelayTime::create(0.2),
                                         ScaleTo::create(0.5, 2), NULL));
    
    cocos2d::Point pos;
    if (count == 1) {
        pos = star1->getPosition();
    }else if(count == 2){
        pos = star2->getPosition();
    }else if(count == 3){
        pos = star3->getPosition();
    }
    
    sprite->runAction(Sequence::create(DelayTime::create(0.6),
                                         EaseIn::create(MoveTo::create(0.5, pos), 7),
                                         CallFuncN::create(CC_CALLBACK_1(HudLayer::spriteMoveDone, this)),NULL));
    addChild(sprite, 1);
    starCount = count;
    this->scheduleOnce(schedule_selector(HudLayer::starChange), 1.1);
}

void HudLayer::starChange(float dt)
{
    cocos2d::Point pos;
    if (starCount == 1) {
        star1->setTexture(Director::getInstance()->getTextureCache()->addImage("hudGoldStar.png"));
        pos = star1->getPosition();
    }else if(starCount == 2){
        star2->setTexture(Director::getInstance()->getTextureCache()->addImage("hudGoldStar.png"));
        pos = star2->getPosition();
    }else if(starCount == 3){
        star3->setTexture(Director::getInstance()->getTextureCache()->addImage("hudGoldStar.png"));
        pos = star3->getPosition();
    }
    
}

void HudLayer::spriteMoveDone(Node* sprite)
{
    sprite->removeFromParentAndCleanup(true);
}

void HudLayer::resumeGame(){
    if(_pauseLayer == NULL) return;
    if (isSharing) {
        GameManager::getInstance()->getCurrentStageLayer()->loadWaveSchedule();
        GameManager::getInstance()->getCurrentStageLayer()->startWaveSchedule();
        ShowWave(waveToShow, true);
        GameManager::getInstance()->playSoundEffect(SOUND_BGM_WHOS_BOSS);
        isSharing = false;
    }
    GameManager::getInstance()->playSoundEffect(SOUND_DDALKACK);
	_pauseLayer->setVisible(false);
    
    GameManager::getInstance()->isInMiddleOfGame = true;
    
    GameManager::getInstance()->getCurrentStageLayer()->resumeLayer();
    
    GameManager::getInstance()->currentScene = SCENE_IN_GAME;
}

void HudLayer::startRecord(){
    isRecording = true;
    recordItem->setVisible(false);
    if (GameManager::getInstance()->market == MARKET_CHINA_NO_IAP) {
        return;
    }
    //GoogleBilling::Billing::startRecording();
}

void HudLayer::showPauseLayer()
{
    if(_pauseLayer == NULL) return;
    GameManager::getInstance()->playSoundEffect(SOUND_DDALKACK);
	UserDefault::getInstance()->flush();
	_pauseLayer->setVisible(true);
    GameManager::getInstance()->firePressed = false;
    
//    GoogleBilling::Billing::showRectAds();
    GameManager::getInstance()->isInMiddleOfGame = false;
    
    GameManager::getInstance()->saveCoin();
    
    GameManager::getInstance()->getCurrentStageLayer()->pauseLayer();
    
    GameManager::getInstance()->currentScene = SCENE_PAUSE;
}
void HudLayer::updateCoinCount(){
    int coinLeft = UserDefault::getInstance()->getIntegerForKey(KEY_COIN_LEFT, 0);
    LblCoinCount->setString(cocos2d::Value(coinLeft).asString());
    if (GameManager::getInstance()->getCurrentStageLayer() != NULL) {
        GameManager::getInstance()->getCurrentStageLayer()->CollectedCoinCount = coinLeft;
    }
}
void HudLayer::showShopLayer()
{
    showPauseLayer();
    enableButtons(false);
    Layer* layer = GameManager::getInstance()->getShopLayer();
    if (layer->getParent()) {
        layer->removeFromParentAndCleanup(false);
    }
	this->addChild(layer, 1001);
    
//    Director::getInstance()->pause();
    GameManager::getInstance()->getCurrentStageLayer()->pauseLayer();
}

void HudLayer::OnQuit(){
    toMain();
}
void HudLayer::OnRestartFromCheckPoint(){
    GameManager::getInstance()->playSoundEffect(SOUND_DDALKACK);
	GameManager::getInstance()->IsNewGame = false;
	GameManager::getInstance()->getCurrentStageLayer()->Dispose();
	int checkPoint = UserDefault::getInstance()->getIntegerForKey(KEY_CHECK_POINT, 1);
	Director::getInstance()->replaceScene(HelloWorld::scene(checkPoint-1));
}
void HudLayer::ShowArriveDestination(){
    _messageBox = CSLoader::createNode("MessageBoxScene.csb");
    Node* panel = _messageBox->getChildByName("panel");
    this->addChild(_messageBox, 1000);
    
    Button* btn = (Button*)panel->getChildByName("close");
    btn->addClickEventListener(CC_CALLBACK_0(HudLayer::onArriveDestinationClosed, this));
    btn = (Button*)panel->getChildByName("ok");
    btn->addClickEventListener(CC_CALLBACK_0(HudLayer::onArriveDestinationClosed, this));
    Text* lbl = (Text*)_messageBox->getChildByName("label");
    lbl->setFontName(LanguageManager::getInstance()->getStringForKey(STR_font));
//    lbl->setString(LanguageManager::getInstance()->getStringForKey(STR_destination));
}
void HudLayer::onArriveDestinationClosed(){
    toMain();
}
void HudLayer::ShowGameClearLayer(int from, int to, float time){
    if(IsFastMode){
        changeSpeed();
    }
    waveToShow = 100;
    GameManager::getInstance()->playSoundEffect(SOUND_WIN);
    
    bool isReadyToAdd = UserDefault::getInstance()->getBoolForKey(KEY_CLEAR_COUNT_READY_TO_ADD, false);
    if(isReadyToAdd){
        int clearCount = UserDefault::getInstance()->getIntegerForKey(KEY_CLEAR_COUNT, 0);
        clearCount++;
        UserDefault::getInstance()->setIntegerForKey(KEY_CLEAR_COUNT, clearCount);
    }
    UserDefault::getInstance()->setBoolForKey(KEY_CLEAR_COUNT_READY_TO_ADD, false);
    UserDefault::getInstance()->flush();
    waveScore = 0;
    coinScore = 0;
    totalScore = 0;
    gameClearNode = CSLoader::createNode("GameClearScene.csb");
    gameClearNode->setPosition(0, -size.height);
    this->addChild(gameClearNode);
    
    Node* panel = gameClearNode->getChildByName("panel");
    
    TextBMFont* lbl = (TextBMFont*)panel->getChildByName("lblFrom");
    lbl->setString(cocos2d::Value(from).asString());
    
    lbl = (TextBMFont*)panel->getChildByName("lblTo");
    lbl->setString(cocos2d::Value(to).asString());
    
    lbl = (TextBMFont*)panel->getChildByName("lblSpent");
    int coinSpent = UserDefault::getInstance()->getIntegerForKey(KEY_TOTAL_COIN_SPENT, 1000);
    lbl->setString(cocos2d::Value(coinSpent).asString());
    
    waveScoreTotal = (to - from)*100000;
    coinScoreTotal = coinSpent;
    totalScoreTotal = waveScoreTotal - coinScoreTotal;

#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    NativeInterface::NativeInterface::postScore(totalScoreTotal, "platform_defense_ranking");
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    NativeInterface::NativeInterface::postScore(totalScoreTotal, "CgkIut_jhL8VEAIQAg");
#endif

    
    waveScore = 0;
    coinScore = 0;
    totalScore = 0;
    
    this->schedule(schedule_selector(HudLayer::gameClearScoring), 0.1f);
    
    Button* btn = (Button*)panel->getChildByName("ok");
    btn->addClickEventListener(CC_CALLBACK_0(HudLayer::onCloseGameClear, this));
    btn->setVisible(false);
    
    float moveTime = 0.55f;
    gameClearNode->runAction(EaseOut::create(MoveTo::create(moveTime, cocos2d::Point(0, 0)), 5));
    
    cocostudio::timeline::ActionTimeline* action = CSLoader::createTimeline("GameClearScene.csb");
    gameClearNode->runAction(action);
    action->gotoFrameAndPlay(0, true);
    
    gameClearNode->addChild(Applause::create());
    gameClearNode->addChild(Firework::create());

}
void HudLayer::startEndingAnimation(){
    cocostudio::timeline::ActionTimeline* action = CSLoader::createTimeline("EndingScene.csb");
    endingNode->runAction(action);
    action->gotoFrameAndPlay(0, false);
}
void HudLayer::gameClearScoring(float dt){
    GameManager::getInstance()->playSoundEffect(SOUND_FA);
    TextBMFont* lbl;
    Node* panel = gameClearNode->getChildByName("panel");
    if(waveScore < waveScoreTotal){
        waveScore += waveScoreTotal/20;
        if(waveScore >= waveScoreTotal){
            waveScore = waveScoreTotal;
        }
        lbl = (TextBMFont*)panel->getChildByName("lblWaveScore");
        lbl->setString(cocos2d::Value(waveScore).asString());
        return;
    }
    
    if(coinScore < coinScoreTotal){
        coinScore += coinScoreTotal/20;
        if(coinScore >= coinScoreTotal){
            coinScore = coinScoreTotal;
        }
        lbl = (TextBMFont*)panel->getChildByName("lblCoinScore");
        lbl->setString(cocos2d::Value(-coinScore).asString());
        return;
    }
    
    if(totalScore < totalScoreTotal){
        totalScore += totalScoreTotal/20;
        if(totalScore >= totalScoreTotal){
            totalScore = totalScoreTotal;
        }
        lbl = (TextBMFont*)panel->getChildByName("lblTotalScore");
        lbl->setString(cocos2d::Value(totalScore).asString());
        return;
    }
    
    capture(0);
    this->runAction(Sequence::create(DelayTime::create(1.2f), CallFunc::create(CC_CALLBACK_0(HudLayer::askShare, this)), NULL));
    
    this->unschedule(schedule_selector(HudLayer::gameClearScoring));
    
    Button* btn = (Button*)panel->getChildByName("ok");
    btn->setVisible(true);
    btn->setOpacity(0);
    btn->setScale(btn->getScale()*1.2f);
    btn->runAction(Sequence::create(DelayTime::create(2), FadeIn::create(0.1f), ScaleTo::create(0.1f, btn->getScale()*1.3f), ScaleTo::create(0.1f, btn->getScale()), NULL));
}
void HudLayer::onCloseGameClear(){
    float moveTime = 0.95f;
    gameClearNode->runAction(Sequence::create(EaseIn::create(MoveTo::create(moveTime, cocos2d::Point(0, size.height)), 5),
                                              CallFuncN::create(CC_CALLBACK_1(HudLayer::spriteMoveDone, this)),
                                              CallFunc::create(CC_CALLBACK_0(HudLayer::continueGame, this)),
                                              NULL));
}
void HudLayer::showEnding(){
    if (IsFastMode) {
        changeSpeed();
    }
    GameManager::getInstance()->playSoundEffect(SOUND_BGM_ALMOST_BRIGHT);
    endingNode = CSLoader::createNode("EndingScene.csb");
    endingNode->setPosition(0, -size.height);
    endingNode->setContentSize(size);
    ui::Helper::doLayout(endingNode);
    this->addChild(endingNode, 100);
    
    Sprite* sptBackground = (Sprite*)endingNode->getChildByName("background");
    float backHeight = size.height - sptBackground->getPositionY();
    sptBackground->setScaleY(backHeight / sptBackground->getContentSize().height);
    sptBackground->setScaleX(size.width/(sptBackground->getContentSize().width));
    
    Sprite* sptTitleBackground = (Sprite*)endingNode->getChildByName("titleBackground_1");
    sptTitleBackground->setScaleX(size.width/(sptTitleBackground->getContentSize().width));
    
    
    
    float endingTime = 530/15;
    float fadeInTime = 1.0f;
    float changeTime = 3.0f;

    // blackout
    Sprite* sptBlack = Sprite::create("whiteSquare.png");
    sptBlack->setScale(size.width/sptBlack->getContentSize().width, size.height/sptBlack->getContentSize().height);
    this->addChild(sptBlack, 200);
    sptBlack->setOpacity(1);
    sptBlack->setPosition(cocos2d::Point(size.width/2, size.height/2));
    sptBlack->runAction(Sequence::create(FadeIn::create(fadeInTime), FadeOut::create(fadeInTime), NULL));
    
    Label* label = Label::createWithBMFont("boldPixelFont.fnt", "TO BE CONTINUED...");
    blackLayer = LayerColor::create(Color4B::BLACK);
    label->setOpacity(0);
    blackLayer->addChild(label, 90);
    blackLayer->setContentSize(size);
    label->setScale(6);
    sptBlack->setOpacity(1);
    label->setPosition(cocos2d::Point(size.width/2, size.height/2));
//    label->runAction(FadeIn::create(10));
    label->runAction(Sequence::create(DelayTime::create(fadeInTime*2 + endingTime+ changeTime/2), FadeIn::create(changeTime/2), DelayTime::create(changeTime+2), FadeOut::create(changeTime), NULL));
    blackLayer->runAction(Sequence::create(FadeOut::create(0),DelayTime::create(fadeInTime*2 + endingTime ), FadeIn::create(changeTime/2), DelayTime::create(changeTime), NULL));
    this->addChild(blackLayer, 300);
    
    endingNode->runAction(Sequence::create(DelayTime::create(fadeInTime),MoveTo::create(0, cocos2d::Point::ZERO),DelayTime::create(fadeInTime),CallFunc::create(CC_CALLBACK_0(HudLayer::startEndingAnimation, this)),DelayTime::create(endingTime + changeTime*3 + 2.5f), CallFunc::create(CC_CALLBACK_0(HudLayer::toMain, this)), NULL));
}
void HudLayer::ShowGameOverLayer(int from, int to, int earned, int spent, int kill, float time){
    if (IsFastMode) changeSpeed();
    GameManager::getInstance()->playSoundEffect(SOUND_DDALKACK);
	const char* sceneName = "GameOverScene.csb";
	Node* node = CSLoader::createNode(sceneName);
	node->setPosition(0, -size.height);
    node->setContentSize(size);
    ui::Helper::doLayout(node);
    
    // star for video ads is also added when game over
    int starForAds = UserDefault::getInstance()->getIntegerForKey(KEY_STAR_TO_SEE_VIDEO_ADS, VIDEO_ADS_ALLOW_STAR_COUNT);
    starForAds += 1;
    UserDefault::getInstance()->setIntegerForKey(KEY_STAR_TO_SEE_VIDEO_ADS, starForAds);
    
    
    // tip
    std::string tip = "";
    int key = cocos2d::random() % 12;
    char keyBuf[12];
//    sprintf(keyBuf, "tip%d", key);
    int strKey = STR_tip0;
    auto lblTip = Label::createWithTTF(LanguageManager::getInstance()->getStringForKey(strKey + key), LanguageManager::getInstance()->getStringForKey(STR_font), 30);
    node->addChild(lblTip);
    lblTip->setAnchorPoint(Point(0, 0));
    lblTip->setWidth(size.width - 20 - _listViewWidth);
    lblTip->setPosition(Point(10,10));
    
	this->addChild(node);
	
	Node* panel = node->getChildByName("panel");

	TextBMFont* lbl = (TextBMFont*)panel->getChildByName("lblFrom");
	lbl->setString(cocos2d::Value(from).asString());

	lbl = (TextBMFont*)panel->getChildByName("lblTo");
	lbl->setString(cocos2d::Value(to).asString());

	lbl = (TextBMFont*)panel->getChildByName("lblFrom");
	lbl->setString(cocos2d::Value(from).asString());

	lbl = (TextBMFont*)panel->getChildByName("lblEnemy");
	lbl->setString(cocos2d::Value(kill).asString());

	lbl = (TextBMFont*)panel->getChildByName("lblEarned");
	lbl->setString(cocos2d::Value(earned).asString());

	lbl = (TextBMFont*)panel->getChildByName("lblSpent");
	lbl->setString(cocos2d::Value(spent).asString());

	lbl = (TextBMFont*)panel->getChildByName("lblTime");
	char timeBuf[16];
	sprintf(timeBuf, "%02d:%02d:%02d", ((int)time) / 60, ((int)time) % 60, (int)(time * 100) % 100);
	lbl->setString(timeBuf);

	int checkPoint = UserDefault::getInstance()->getIntegerForKey(KEY_CHECK_POINT, 1);
	lbl = (TextBMFont*)panel->getChildByName("lblWave");
	char buf[10];
	sprintf(buf, "WAVE %d", checkPoint);
	lbl->setString(buf);

	Button* btn = (Button*)panel->getChildByName("quit");
	btn->addClickEventListener(CC_CALLBACK_0(HudLayer::OnQuit, this));
	btn = (Button*)panel->getChildByName("restart");
	btn->addClickEventListener(CC_CALLBACK_0(HudLayer::OnRestartFromCheckPoint, this));

	float moveTime = 0.55f;
	node->runAction(EaseOut::create(MoveTo::create(moveTime, cocos2d::Point(0, 0)), 5));

	cocostudio::timeline::ActionTimeline* action = CSLoader::createTimeline(sceneName);
	node->runAction(action);
	action->gotoFrameAndPlay(0, true);

	btn = (Button*)node->getChildByName("btnVideo");
	btn->addClickEventListener(CC_CALLBACK_0(HudLayer::OnVideoClick, this));
    
    btnVideo = btn;
    
    int starCount = UserDefault::getInstance()->getIntegerForKey(KEY_STAR_TO_SEE_VIDEO_ADS, VIDEO_ADS_ALLOW_STAR_COUNT);
    if( starCount >= VIDEO_ADS_ALLOW_STAR_COUNT){
        btn->setEnabled(true);
    }else{
        btn->setEnabled(false);
        TextBMFont* label = (TextBMFont*)btn->getChildByName("label");
        char buf[20];
        sprintf(buf, "%d to go", VIDEO_ADS_ALLOW_STAR_COUNT - starCount);
        label->setString(buf);
        Sprite* sprite = (Sprite*)label->getChildByName("sprite");
        sprite->setSpriteFrame("blueStar.png");
        sprite->setScale(1);
    }
    
	btn = (Button*)node->getChildByName("btnShop");
	btn->addClickEventListener(CC_CALLBACK_0(HudLayer::OnShopClick, this));
    
    GameManager::getInstance()->currentScene = SCENE_GAME_OVER;
}
void HudLayer::OnVideoClick(){
    GameManager::getInstance()->playSoundEffect(SOUND_DDALKACK);
    NativeInterface::NativeInterface::showUnityAdsVideo();
}
void HudLayer::OnShopClick(){
    GameManager::getInstance()->playSoundEffect(SOUND_DDALKACK);
	ShopLayer* shopLayer = ShopLayer::create();
    this->addChild(shopLayer);
    GameManager::getInstance()->currentScene = SCENE_GAME_OVER_SHOP;
}
void HudLayer::OnVideoEnded(){
    log("Hudlyaer::OnVideoEnded");
    UserDefault::getInstance()->setIntegerForKey(KEY_STAR_TO_SEE_VIDEO_ADS, 0);
    Sprite* sptCoin = Sprite::create("coin0.png");
    btnVideo->getParent()->addChild(sptCoin);
    sptCoin->setPosition(btnVideo->getPosition() + Point(-btnVideo->getContentSize().width/2,  - 15));
    int coinEarned = 300 + cocos2d::random()%200;
    int coinLeft = UserDefault::getInstance()->getIntegerForKey(KEY_COIN_LEFT, 0);
    coinLeft += coinEarned;
    UserDefault::getInstance()->setIntegerForKey(KEY_COIN_LEFT, coinLeft);
    GameManager::getInstance()->getCurrentStageLayer()->CollectedCoinCount = coinLeft;
    updateCoinCount();
    
    Label* lblCoin = Label::createWithBMFont("boldPixelFont.fnt", cocos2d::Value(coinEarned).asString());
    sptCoin->addChild(lblCoin);
    lblCoin->setAnchorPoint(Point(0, 0.5));
    
    lblCoin->setPosition(sptCoin->getContentSize().width + 5, sptCoin->getContentSize().height/2);
    sptCoin->setScale(3);
    sptCoin->runAction(MoveBy::create(3, Point(0, 30)));
    btnVideo->removeFromParentAndCleanup(true);
    
    LblCoinCount->setString(cocos2d::Value(coinLeft).asString());
}
void HudLayer::onFreeCoin(Ref* pSender, ui::Widget::TouchEventType eEventType){
    if(TouchEventType::TOUCH_EVENT_ENDED != (TouchEventType)eEventType) return;
    if (GameManager::getInstance()->market == MARKET_CHINA_NO_IAP) {
        MyMessageBox::getInstance()->showDialog(this->getParent(), "NOT AVAILABLE");
        return;
    }
    GameManager::getInstance()->showVideo();
}
void HudLayer::videoDone(){
    if(GameManager::getInstance()->getHudLayer()){
        GameManager::getInstance()->getHudLayer()->scheduleOnce(schedule_selector(HudLayer::closeVideoDone), 0.05);
    }
}
void HudLayer::videoFailed(){
    if(GameManager::getInstance()->getHudLayer()){
        GameManager::getInstance()->getHudLayer()->scheduleOnce(schedule_selector(HudLayer::closeVideoFailed), 0.1);
    }
}
void HudLayer::closeVideoDone(float dt){
    if (btnFreeCoin == NULL) {
        return;
    }
    Sprite* sptCoin = Sprite::create("coin.png");
    btnFreeCoin->getParent()->addChild(sptCoin);
    sptCoin->setPosition(btnFreeCoin->getPosition() + cocos2d::Point(-40, 0));
    
    Label* lbl = Label::createWithTTF("+100", "bitdust2.ttf", 30);
    sptCoin->addChild(lbl);
    lbl->setPosition(Point(80, 19));
    
    btnFreeCoin->removeFromParent();
    btnFreeCoin = NULL;
    GameManager::getInstance()->saveCoin();
}

void HudLayer::closeVideoFailed(float dt){
    if (btnFreeCoin == NULL) {
        return;
    }
    Label* lbl = Label::createWithTTF("ADS FAILED", "bitdust2.ttf", 30);
    btnFreeCoin->getParent()->addChild(lbl);
    lbl->setPosition(btnFreeCoin->getPosition());
    
    btnFreeCoin->removeFromParent();
    btnFreeCoin = NULL;
}


void HudLayer::shareFacebook(){
    this->scheduleOnce(schedule_selector(HudLayer::facebookSafe), 0.1);
}

void HudLayer::facebookSafe(float dt){
    const char* link = "http://www.facebook.com/FifteenSixKorea";
    if (GameManager::getInstance()->market == MARKET_PLAYSTORE_PAID ||
        GameManager::getInstance()->market == MARKET_NAVER_PAID ||
        GameManager::getInstance()->market == MARKET_TSTORE_PAID) {
        link = "https://play.google.com/store/apps/details?id=com.magmon.LegendDaryTwo";
    }
    //GoogleBilling::Billing::postFacebookStatus(LanguageManager::getInstance()->getText(STR_APP_DESC), link, LanguageManager::getInstance()->getText(STR_APP_NAME));
}
void HudLayer::playCoinSoundLater(){
    GameManager::getInstance()->playSoundEffect(SOUND_COIN);
}
void HudLayer::closeWaitMessageBoxLater(){
    this->scheduleOnce(schedule_selector(HudLayer::closeWaitMessageBox), 0.01);
}
void HudLayer::closeWaitMessageBox(float dt){
    if(MyMessageBox::getInstance()->messageBox){
        MyMessageBox::getInstance()->messageBox->removeFromParentAndCleanup(true);
        MyMessageBox::getInstance()->messageBox = NULL;
    }
}
void HudLayer::closeWaitMessageBoxLaterAndShowSucess(){
    this->scheduleOnce(schedule_selector(HudLayer::closeWaitMessageBoxAndShowSuccess), 0.01);
}
void HudLayer::closeWaitMessageBoxAndShowSuccess(float dt){
    if(MyMessageBox::getInstance()->messageBox){
        MyMessageBox::getInstance()->messageBox->removeFromParentAndCleanup(true);
        MyMessageBox::getInstance()->messageBox = NULL;
    }
//    MyMessageBox::getInstance()->showDialog(this, "COIN PURCHASED!");
}
void HudLayer::buyGemClosed(Ref* obj){
    if(this->getTag() == MESSAGE_BOX_RESULT_BUTTON_2){
        // buy gem
        log("buy gem!");
    }else{
        
    }
}
void HudLayer::removeUsedAssets(){
    GameManager::getInstance()->getCurrentStageLayer()->removeUsedAssets();
    
    _eventDispatcher->removeEventListener(touchListener);
    _eventDispatcher->removeEventListener(listener);
    /*if (gameOverLayer) {
        gameOverLayer->removeFromParentAndCleanup(true);
    }
    if (reviveLayer) {
        reviveLayer->removeFromParentAndCleanup(true);
    }
    if (pauseLayer) {
        pauseLayer->removeFromParentAndCleanup(true);
    }
    if (gameClearLayer) {
        gameClearLayer->removeFromParentAndCleanup(true);
    }
    this->removeFromParentAndCleanup(true);
    
    GameManager::getInstance()->getCurrentStageLayer()->removeUsedAssets();*/
}
void HudLayer::toMainByPauseLayer(){
    if (GameManager::getInstance()->market == MARKET_PLAYSTORE_FREE &&
        !UserDefault::getInstance()->getBoolForKey(KEY_PAID_USER, false)) {
        NativeInterface::NativeInterface::showAds();
    }
    GameManager::getInstance()->getCurrentStageLayer()->saveTotalEnemyKill();
    toMain();
}
void HudLayer::toMain(){
	if (IsFastMode) changeSpeed();
    
    GameManager::getInstance()->playSoundEffect(SOUND_DDALKACK);
    //TimeManager::getInstance()->getHttpTime();
    //this->scheduleOnce(schedule_selector(HudLayer::closeSchedule), 0.01);
    GameManager::getInstance()->playSoundEffect(SOUND_BGM_BRIGHT);
	UserDefault::getInstance()->setIntegerForKey(KEY_COIN_LEFT, GameManager::getInstance()->getCurrentStageLayer()->CollectedCoinCount);
	GameManager::getInstance()->getCurrentStageLayer()->Dispose();
	Director::getInstance()->popScene();
    
    GameManager::getInstance()->currentScene = SCENE_PLAY;
    GameManager::getInstance()->setCurrentStageLayer(NULL);
}
void HudLayer::toStageSelect(Ref* pSender, ui::Widget::TouchEventType eEventType){
    if(TouchEventType::TOUCH_EVENT_ENDED != (TouchEventType)eEventType) return;
    
   // TimeManager::getInstance()->getHttpTime();
    
    whereToGo = CLOSE_TO_STAGES;
    this->scheduleOnce(schedule_selector(HudLayer::closeSchedule), 0.01);
}
void HudLayer::closeSchedule(float dt){
//    Director::getInstance()->popScene();
    removeUsedAssets();
//    ((TitleLayer*)GameManager::getInstance()->titleLayer)->closeTo(whereToGo);
    int toWhat = whereToGo;
    if(toWhat == CLOSE_TO_NEXT){
        if(GameManager::getInstance()->theme*24 + GameManager::getInstance()->currentStageIndex == GameManager::getInstance()->totalStage - 1){
            Director::getInstance()->popScene();
            return;
        }

        if(GameManager::getInstance()->currentStageIndex == 23 ){
            Director::getInstance()->popScene();
           
		}else{
            Director::getInstance()->popScene();
//            int stage = GameManager::getInstance()->currentStageIndex + 1;

        }
    }else if(toWhat == CLOSE_TO_RETRY){
        Director::getInstance()->popScene();
    }else if(toWhat == CLOSE_TO_STAGES){
        Director::getInstance()->popScene();
    }else if(toWhat == CLOSE_TO_MAIN){
        Director::getInstance()->popScene();
    }
}

void HudLayer::nextStage(Ref* pSender, ui::Widget::TouchEventType eEventType){
    if(TouchEventType::TOUCH_EVENT_ENDED != (TouchEventType)eEventType) return;
    
    whereToGo = CLOSE_TO_NEXT;
    this->scheduleOnce(schedule_selector(HudLayer::closeSchedule), 0.01);
}
void HudLayer::retryStage(Ref* pSender, ui::Widget::TouchEventType eEventType){
    if(TouchEventType::TOUCH_EVENT_ENDED != (TouchEventType)eEventType) return;
    
    whereToGo = CLOSE_TO_RETRY;
    this->scheduleOnce(schedule_selector(HudLayer::closeSchedule), 0.01);
}
void HudLayer::onReplayVideo(Ref* pSender, ui::Widget::TouchEventType eEventType){
    if(TouchEventType::TOUCH_EVENT_ENDED != (TouchEventType)eEventType) return;
    if (GameManager::getInstance()->market == MARKET_CHINA_NO_IAP) {
        MyMessageBox::getInstance()->showDialog(this->getParent(), "NOT AVAILABLE");
        return;
    }
    //GoogleBilling::Billing::showRecording();
}

void HudLayer::enableButtons(bool enb)
{
    
//    enableLayerAndMenus(enb);
}

void HudLayer::displayBossEnergy(const char* name)
{
//    bossEnergyBackground = Sprite::create("dialogBox.png");
//    this->addChild(bossEnergyBackground);
//    bossEnergyBackground->setPosition(Point(size.width/2, size.height/2 - 60));
//    bossEnergyBackground->setScaleY(0.5);
    bossEnergyBarWidth = 600;
    
    bossName = Label::createWithTTF(name, "bitdust2.ttf", 30);
    bossName->setColor(Color3B(255, 255, 255));
    bossName->enableShadow();
    bossName->getFontAtlas()->setAliasTexParameters();
    this->addChild(bossName);
    bossName->setAnchorPoint(Point(0,0));
    bossName->setPosition(Point(size.width/2 - bossEnergyBarWidth/2, size.height/2 + 188));
    
    bossEnergyBarRed = Sprite::createWithSpriteFrameName("bossEnergyRed.png");
    this->addChild(bossEnergyBarRed);
    bossEnergyBarRed->setPosition(Point(size.width/2 - bossEnergyBarWidth/2, size.height/2 + 180));
    bossEnergyBarRed->setAnchorPoint(Point(0, 0.5f));
    bossEnergyBarRed->setScaleX(bossEnergyBarWidth);
    bossEnergyBarRed->setScaleY(2);
    
    bossEnergyBarYellow = Sprite::createWithSpriteFrameName("bossEnergyGreen.png");
    this->addChild(bossEnergyBarYellow);
    bossEnergyBarYellow->setPosition(Point(size.width/2 - bossEnergyBarWidth/2, size.height/2 + 180));
    bossEnergyBarYellow->setAnchorPoint(Point(0, 0.5f));
    bossEnergyBarYellow->setScaleX(bossEnergyBarWidth);
    bossEnergyBarYellow->setScaleY(2);
}

void HudLayer::setBossEnergy(int percent)
{
    bossEnergyBarYellow->setScaleX(bossEnergyBarWidth*percent/100.0f);
}
void HudLayer::addListener(){
    touchListener = cocos2d::EventListenerTouchOneByOne::create();
    touchListener->setSwallowTouches(true);
    
    touchListener->onTouchBegan = [&](cocos2d::Touch* touch, cocos2d::Event* event)
    {
        cocos2d::Point p = touch->getLocation();
        cocos2d::Point location = p;
        GameManager::getInstance()->getCurrentStageLayer()->isTouchStarted = true;
        
        
        return true; // we did not consume this event, pass thru.
    };
    
    touchListener->onTouchMoved = [=](cocos2d::Touch* touch, cocos2d::Event* event)
    {
        cocos2d::Point location = touch->getLocationInView();
        location = Director::getInstance()->convertToGL(location);
        
        if (!GameManager::getInstance()->getCurrentStageLayer()->isTouchStarted) {
            return;
        }

    };
    
    touchListener->onTouchEnded = [=](cocos2d::Touch* touch, cocos2d::Event* event)
    {
        GameManager::getInstance()->getCurrentStageLayer()->isTouchStarted = false;
    };
    
    cocos2d::Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(touchListener, 30);
}

void HudLayer::showDialog(const char* message, const char* btn1, const char* btn2)
{
    CallFuncN* call = CallFuncN::create(CC_CALLBACK_1(HudLayer::messageBoxClosed, this));
    MyMessageBox::getInstance()->showDialog(this, call, message, btn1, btn2);
    GameManager::getInstance()->getHudLayer()->enableJoystick(false);

//    this->setTouchEnabled(false);
    
    ((HelloWorld*)GameManager::getInstance()->getCurrentStageLayer())->pauseLayer();
}

void HudLayer::messageBoxClosed(Node* node)
{
    ((HelloWorld*)GameManager::getInstance()->getCurrentStageLayer())->resumeLayer();
//	this->setTouchEnabled(true);
    //	_dialogBox->setVisible(false);
	
    GameManager::getInstance()->getHudLayer()->enableJoystick(true);
	
    if (this->getTag() == 0) {
//        Director::getInstance()->replaceScene(StageSelectScene::scene(GameManager::getInstance()->theme));
    }
}
