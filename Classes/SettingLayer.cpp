
#include "LanguageManager.h"
#include "HudLayer.h"
#include "SettingLayer.h"
#include "GameManager.h"
#include "HelloWorldScene.h"
#include "MyMessageBox.h"
#include "LanguageManager.h"
#include "ui/UIHelper.h"
#include "ShopLayer.h"
#include "NativeInterface.h"

bool SettingLayer::init()
{
    //////////////////////////////
    // 1. super init first
    if (!Layer::init())
    {
        return false;
    }
    size = Director::getInstance()->getWinSize();
    
    
    Node* settingScene = (Widget*)CSLoader::createNode("SettingScene.csb");
    settingScene->setContentSize(size);
    ui::Helper::doLayout(settingScene);
    this->addChild(settingScene);
    
    isPlayServiceLoggedIn = false;
    settingPopup = settingScene->getChildByName("settingPopup");
    btnLogin = (Button*)settingPopup->getChildByName("btnPlayServiceLogIn");
    btnLogin->addClickEventListener(CC_CALLBACK_0(SettingLayer::onPlayServiceLoginClick, this));
    if (GameManager::getInstance()->market == MARKET_PLAYSTORE_PAID ||
        GameManager::getInstance()->market == MARKET_PLAYSTORE_FREE) {
        btnLogin->setVisible(true);
    }else{
        btnLogin->setVisible(false);
    }
    checkingLogin(0);
    this->schedule(schedule_selector(SettingLayer::checkingLogin), 0.5f);
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    
#else
    
#endif
    Button* btn = (Button*)settingPopup->getChildByName("close");
    btn->addClickEventListener(CC_CALLBACK_0(SettingLayer::CloseSettingPopup, this));
    btn = (Button*)settingPopup->getChildByName("btnOk");
    btn->addClickEventListener(CC_CALLBACK_0(SettingLayer::CloseSettingPopup, this));
    btn = (Button*)settingPopup->getChildByName("btnLike");
    btn->addClickEventListener(CC_CALLBACK_0(SettingLayer::onLike, this));
    Button* btnSave = (Button*)settingPopup->getChildByName("btnSave");
    btnSave->addClickEventListener(CC_CALLBACK_0(SettingLayer::onSaveClick, this));
    Button* btnLoad = (Button*)settingPopup->getChildByName("btnLoad");
    btnLoad->addClickEventListener(CC_CALLBACK_0(SettingLayer::onLoadClick, this));
    if (GameManager::getInstance()->market == MARKET_PLAYSTORE_PAID ||
        GameManager::getInstance()->market == MARKET_APPSTORE_PAID) {
        btnLoad->setVisible(true);
        btnSave->setVisible(true);
        btnLoad->setEnabled(true);
        btnSave->setEnabled(true);
    }else{
        btnLoad->setVisible(false);
        btnSave->setVisible(false);
        btnLoad->setEnabled(false);
        btnSave->setEnabled(false);
    }

    musicBar = (Slider*)settingPopup->getChildByName("musicSlider");
    musicBar->addEventListener(CC_CALLBACK_2(SettingLayer::musicSliderEvent, this));
    musicBar->addTouchEventListener(CC_CALLBACK_2(SettingLayer::onMusicSliderTouch, this));
    
    soundBar = (Slider*)settingPopup->getChildByName("soundSlider");
    soundBar->addEventListener(CC_CALLBACK_2(SettingLayer::soundSliderEvent, this));
    soundBar->addTouchEventListener(CC_CALLBACK_2(SettingLayer::onSoundSliderTouch, this));
    
    GameManager::getInstance()->settingLayer = this;
    
    
    isPlayServiceLoggedIn = GameManager::getInstance()->isPlayServiceLogIn;
    onPlayServiceLogin(isPlayServiceLoggedIn);
    return true;
}

void SettingLayer::onSaveClick(){
    GameManager::getInstance()->currentScene = SCENE_SAVE_WARN;
    _messageBox = CSLoader::createNode("MessageBoxScene.csb");
    Node* panel = _messageBox->getChildByName("panel");
    this->addChild(_messageBox, 100);
    Button* btn = (Button*)_messageBox->getChildByName("block");
//    btn->addClickEventListener(CC_CALLBACK_0(Title::shareGameOk, this));
    btn = (Button*)panel->getChildByName("close");
    btn->setVisible(false);
    btn = (Button*)panel->getChildByName("ok");
    btn->addClickEventListener(CC_CALLBACK_0(SettingLayer::onWarnClosed, this));
    Text* lbl = (Text*)_messageBox->getChildByName("label");
    lbl->setFontName(LanguageManager::getInstance()->getStringForKey(STR_font));
    lbl->setString(LanguageManager::getInstance()->getStringForKey(STR_coin_not_saved));
}

void SettingLayer::onWarnClosed(){
    UserDefault::getInstance()->flush();
    GameManager::getInstance()->saveGameData();
    onLoadWarnCancel();
}

void SettingLayer::onLoadClick(){
    GameManager::getInstance()->currentScene = SCENE_LOAD_WARN;
    _messageBox = CSLoader::createNode("MessageBoxScene.csb");
    Node* panel = _messageBox->getChildByName("panel");
    this->addChild(_messageBox, 100);
    Button* btn = (Button*)_messageBox->getChildByName("block");
    //    btn->addClickEventListener(CC_CALLBACK_0(Title::shareGameOk, this));
    btn = (Button*)panel->getChildByName("close");
    btn->addClickEventListener(CC_CALLBACK_0(SettingLayer::onLoadWarnCancel, this));
    btn = (Button*)panel->getChildByName("ok");
    btn->addClickEventListener(CC_CALLBACK_0(SettingLayer::onLoadWarnOk, this));
    Text* lbl = (Text*)_messageBox->getChildByName("label");
    lbl->setFontName(LanguageManager::getInstance()->getStringForKey(STR_font));
    lbl->setString(LanguageManager::getInstance()->getStringForKey(STR_data_load_coin_warn));
}
void SettingLayer::onLoadWarnOk(){
    onLoadWarnCancel();
    GameManager::getInstance()->loadGameData();
}
void SettingLayer::onLoadWarnCancel(){
    GameManager::getInstance()->currentScene = SCENE_SETTING;
    _messageBox->removeFromParentAndCleanup(true);
}

void SettingLayer::musicSliderEvent(Ref *pSender, Slider::EventType type)
{
    if (type == Slider::EventType::ON_PERCENTAGE_CHANGED)
    {
        Slider* slider = dynamic_cast<Slider*>(pSender);
        int percent = slider->getPercent();
        GameManager::getInstance()->setMusicVolumn(percent*0.01f);
    }
}

void SettingLayer::soundSliderEvent(Ref *pSender, Slider::EventType type)
{
    CCLOG("soundSliderEvent");
    if (type == Slider::EventType::ON_PERCENTAGE_CHANGED)
    {
        Slider* slider = dynamic_cast<Slider*>(pSender);
        int percent = slider->getPercent();
        
        GameManager::getInstance()->setSoundVolumn(percent*0.02f);
    }
}
void SettingLayer::onMusicSliderTouch(Ref* pSender, ui::Widget::TouchEventType eEventType){
    GameManager::getInstance()->playSoundEffect(SOUND_DDALKACK);
    if (TouchEventType::TOUCH_EVENT_ENDED != (TouchEventType)eEventType) return;
    GameManager::getInstance()->setMusicVolumn(musicBar->getPercent()*0.01f);
}
void SettingLayer::onSoundSliderTouch(Ref* pSender, ui::Widget::TouchEventType eEventType){
    GameManager::getInstance()->playSoundEffect(SOUND_DDALKACK);
    if (TouchEventType::TOUCH_EVENT_ENDED != (TouchEventType)eEventType) return;
    GameManager::getInstance()->setSoundVolumn(soundBar->getPercent()*0.01f);
}
void SettingLayer::CloseSettingPopup(){
    GameManager::getInstance()->playSoundEffect(SOUND_DDALKACK);
    GameManager::getInstance()->settingLayer = NULL;
    this->removeFromParentAndCleanup(true);
}
void SettingLayer::onLike(){
    NativeInterface::NativeInterface::openUrl("http://www.facebook.com/FifteenSixKorea");
}
void SettingLayer::checkingLogin(float dt){
    if (!btnLogin->isVisible()) {
        return;
    }
    if(isPlayServiceLoggedIn){
        btnLogin->loadTextureNormal("signOut.png");
        btnLogin->loadTexturePressed("signOut.png");
    }else{
        btnLogin->loadTextureNormal("signIn.png");
        btnLogin->loadTexturePressed("signIn.png");
    }
}
// Gamecenter or play service
void SettingLayer::onPlayServiceLoginClick(){
    GameManager::getInstance()->playSoundEffect(SOUND_DDALKACK);
    
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    if(isPlayServiceLoggedIn){
        NativeInterface::NativeInterface::logoutPlayService();
    }else{
        NativeInterface::NativeInterface::loginPlayService();
    }
#endif
}
void SettingLayer::onPlayServiceLogin(bool isLogin){
    isPlayServiceLoggedIn = isLogin;
//    log("cpp OnPlayServiceLogin: %d", (int)isLogin);
}