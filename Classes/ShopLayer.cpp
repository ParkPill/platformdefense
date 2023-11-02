//
//  ShopLayer.cpp
//  AlmostLegendary
//
//  Created by SUNG PILL PACK on 13. 6. 29..
//
//

#include "ShopLayer.h"
#include "GameManager.h"
#include "HelloWorldScene.h"
#include "MyMessageBox.h"
//#include "GoogleBilling.h"
#include "LanguageManager.h"
#include "ui/UIHelper.h"
#include "PluginIAP/PluginIAP.h"
#include "NativeInterface.h"

bool ShopLayer::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}
    _indicator = NULL;
	_size = Director::getInstance()->getVisibleSize();
    isWaiting = false;
    purchasingIndex = -1;
	_shopScene = CSLoader::createNode("ShopScene.csb");
	_shopScene->setContentSize(_size);
	ui::Helper::doLayout(_shopScene);
	this->addChild(_shopScene);
	Node* panel = _shopScene->getChildByName("panel");

	Button* btn = (Button*)panel->getChildByName("btnSmall");
	btn->addClickEventListener(CC_CALLBACK_0(ShopLayer::OnSmallClick, this));
	Text* lblPrice = (Text*)btn->getChildByName("lblPrice");
    lblPrice->setString(GameManager::getInstance()->SmallPriceText);
    lblPrice->enableOutline(Color4B::BLACK, 2);

	btn = (Button*)panel->getChildByName("btnMiddle");
	btn->addClickEventListener(CC_CALLBACK_0(ShopLayer::OnMiddleClick, this));
	lblPrice = (Text*)btn->getChildByName("lblPrice");
    lblPrice->setString(GameManager::getInstance()->MiddlePriceText);
    lblPrice->enableOutline(Color4B::BLACK, 2);

	btn = (Button*)panel->getChildByName("btnBig");
	btn->addClickEventListener(CC_CALLBACK_0(ShopLayer::OnBigClick, this));
	lblPrice = (Text*)btn->getChildByName("lblPrice");
	lblPrice->setString(GameManager::getInstance()->BigPriceText);
    lblPrice->enableOutline(Color4B::BLACK, 2);

	btn = (Button*)panel->getChildByName("btnHuge");
	btn->addClickEventListener(CC_CALLBACK_0(ShopLayer::OnHugeClick, this));
	lblPrice = (Text*)btn->getChildByName("lblPrice");
    lblPrice->setString(GameManager::getInstance()->HugePriceText);
    lblPrice->enableOutline(Color4B::BLACK, 2);
    
    btn = (Button*)panel->getChildByName("btnHeroes");
    btn->addClickEventListener(CC_CALLBACK_0(ShopLayer::OnHeroClick, this));
    lblPrice = (Text*)btn->getChildByName("lblPrice");
    lblPrice->setString(GameManager::getInstance()->HeroPriceText);
    lblPrice->enableOutline(Color4B::BLACK, 2);
    if (UserDefault::getInstance()->getBoolForKey(KEY_ALL_HEROES, false)) {
        lblPrice->setString("GOT IT");
        btn->setEnabled(false);
    }

	btn = (Button*)panel->getChildByName("close");
	btn->addClickEventListener(CC_CALLBACK_0(ShopLayer::CloseThis, this));
	btn = (Button*)panel->getChildByName("ok");
	btn->addClickEventListener(CC_CALLBACK_0(ShopLayer::CloseThis, this));
    btn = (Button*)panel->getChildByName("restore");
    
    btn->addClickEventListener(CC_CALLBACK_0(ShopLayer::OnRestoreClick, this));
    if (GameManager::getInstance()->market == MARKET_APPSTORE_PAID ||
        GameManager::getInstance()->market == MARKET_APPSTORE_FREE) {
        btn->setVisible(true);
    }else{
//        btn->setVisible(false);
    }
    
    btn = (Button*)panel->getChildByName("removeAds");
    btn->addClickEventListener(CC_CALLBACK_0(ShopLayer::OnRemoveAdsClick, this));
    if (GameManager::getInstance()->market == MARKET_PLAYSTORE_FREE &&
        !UserDefault::getInstance()->getBoolForKey(KEY_PAID_USER, false)) {
        btn->setVisible(true);
    }else{
        btn->setVisible(false);
    }
    
    UpdateCoinCount();
    GameManager::getInstance()->shopLayer = this;
    
    _listener = EventListenerKeyboard::create();
    _listener->onKeyReleased = CC_CALLBACK_2(ShopLayer::onKeyReleased, this);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(_listener, this);
    
	return true;
}
void ShopLayer::OnRemoveAdsClick(){
    GameManager::getInstance()->playSoundEffect(SOUND_DDALKACK);
    ShowIndicator();
    GameManager::getInstance()->shopLayer->purchasingIndex = 5;
    sdkbox::IAP::purchase("pdf_remove_ads");
}
void ShopLayer::OnRestoreClick(){
    GameManager::getInstance()->playSoundEffect(SOUND_DDALKACK);
    ShowIndicator();
    sdkbox::IAP::restore();
}
void ShopLayer::UpdateCoinCount(){
    TextBMFont* lblCoinCount = (TextBMFont*)_shopScene->getChildByName("lblCoinCount");
    int coin = UserDefault::getInstance()->getIntegerForKey(KEY_COIN_LEFT, 0);
    lblCoinCount->setString(cocos2d::Value(coin).asString());
}
void ShopLayer::UpdatePrice(){
    Node* panel = _shopScene->getChildByName("panel");
    Button* btn = (Button*)panel->getChildByName("btnSmall");
    Text* lblPrice = (Text*)btn->getChildByName("lblPrice");
    lblPrice->setString(GameManager::getInstance()->SmallPriceText);
    
    btn = (Button*)panel->getChildByName("btnMiddle");
    lblPrice = (Text*)btn->getChildByName("lblPrice");
    lblPrice->setString(GameManager::getInstance()->MiddlePriceText);
    
    btn = (Button*)panel->getChildByName("btnBig");
    lblPrice = (Text*)btn->getChildByName("lblPrice");
    lblPrice->setString(GameManager::getInstance()->BigPriceText);
    
    btn = (Button*)panel->getChildByName("btnHuge");
    lblPrice = (Text*)btn->getChildByName("lblPrice");
    lblPrice->setString(GameManager::getInstance()->HugePriceText);
    
    btn = (Button*)panel->getChildByName("btnHeroes");
    lblPrice = (Text*)btn->getChildByName("lblPrice");
    if (UserDefault::getInstance()->getBoolForKey(KEY_ALL_HEROES, false)) {
        lblPrice->setString("GOT IT");
        btn->setEnabled(false);
    }
    
    
    btn = (Button*)panel->getChildByName("removeAds");
    if (UserDefault::getInstance()->getBoolForKey(KEY_PAID_USER, false)) {
        btn->setVisible(false);
    }
}

void ShopLayer::ShowIndicator(){
    _indicator = CSLoader::createNode("Indicator.csb");
    _indicator->setContentSize(_size);
    ui::Helper::doLayout(_indicator);
    this->addChild(_indicator);
    cocostudio::timeline::ActionTimeline* action = CSLoader::createTimeline("Indicator.csb");
    _indicator->runAction(action);
    action->gotoFrameAndPlay(0, true);
    isWaiting = true;
}
void ShopLayer::HideIndicator(){
    if(_indicator != NULL){
        _indicator->removeFromParentAndCleanup(true);
        _indicator = NULL;
    }
    isWaiting = false;
}
void ShopLayer::CloseThis(){
    GameManager::getInstance()->shopLayer = this;

    GameManager::getInstance()->playSoundEffect(SOUND_DDALKACK);
    Director::getInstance()->getEventDispatcher()->removeEventListener(_listener);
	this->removeFromParentAndCleanup(true);
    if (GameManager::getInstance()->currentScene == SCENE_TITLE_SHOP) {

    }else if (GameManager::getInstance()->currentScene == SCENE_IN_GAME_SHOP) {
        GameManager::getInstance()->currentScene = SCENE_PAUSE;
    }else if (GameManager::getInstance()->currentScene == SCENE_GAME_OVER_SHOP) {
        GameManager::getInstance()->currentScene = SCENE_GAME_OVER;
    }
}
void ShopLayer::OnSmallClick(){
    GameManager::getInstance()->playSoundEffect(SOUND_DDALKACK);
    ShowIndicator();
    GameManager::getInstance()->shopLayer->purchasingIndex = 0;
    if (GameManager::getInstance()->market == MARKET_TSTORE_PAID) {
        NativeInterface::NativeInterface::requestPaymentTStore("0910038482");
        purchasingIndex = 0;
    }else if (GameManager::getInstance()->market == MARKET_NAVER_PAID) {
        NativeInterface::NativeInterface::naverPurchase("1000020397");
    }else{
        sdkbox::IAP::purchase("small_coin");
    }
}

void ShopLayer::OnMiddleClick(){
    GameManager::getInstance()->playSoundEffect(SOUND_DDALKACK);
    ShowIndicator();
    GameManager::getInstance()->shopLayer->purchasingIndex = 1;
    if (GameManager::getInstance()->market == MARKET_TSTORE_PAID) {
        NativeInterface::NativeInterface::requestPaymentTStore("0910038483");
        purchasingIndex = 1;
    }else if (GameManager::getInstance()->market == MARKET_NAVER_PAID) {
        NativeInterface::NativeInterface::naverPurchase("1000020398");
    }else{
        sdkbox::IAP::purchase("middle_coin");
    }
}
void ShopLayer::OnBigClick(){
    GameManager::getInstance()->playSoundEffect(SOUND_DDALKACK);
    ShowIndicator();
    GameManager::getInstance()->shopLayer->purchasingIndex = 2;
    if (GameManager::getInstance()->market == MARKET_TSTORE_PAID) {
        NativeInterface::NativeInterface::requestPaymentTStore("0910038484");
        purchasingIndex = 2;
    }else if (GameManager::getInstance()->market == MARKET_NAVER_PAID) {
        NativeInterface::NativeInterface::naverPurchase("1000020399");
    }else{
        sdkbox::IAP::purchase("big_coin");
    }
}
void ShopLayer::OnHugeClick(){
    GameManager::getInstance()->playSoundEffect(SOUND_DDALKACK);
    ShowIndicator();
    GameManager::getInstance()->shopLayer->purchasingIndex = 3;
    if (GameManager::getInstance()->market == MARKET_TSTORE_PAID) {
        NativeInterface::NativeInterface::requestPaymentTStore("0910038485");
        purchasingIndex = 3;
    }else if (GameManager::getInstance()->market == MARKET_NAVER_PAID) {
        NativeInterface::NativeInterface::naverPurchase("1000020400");
    }else{
        sdkbox::IAP::purchase("huge_coin");
    }
}

void ShopLayer::OnHeroClick(){
    GameManager::getInstance()->playSoundEffect(SOUND_DDALKACK);
    ShowIndicator();
    GameManager::getInstance()->shopLayer->purchasingIndex = 3;
    if (GameManager::getInstance()->market == MARKET_TSTORE_PAID) {
        NativeInterface::NativeInterface::requestPaymentTStore("0910038485");
        purchasingIndex = 3;
    }else if (GameManager::getInstance()->market == MARKET_NAVER_PAID) {
        NativeInterface::NativeInterface::naverPurchase("1000020400");
    }else{
        sdkbox::IAP::purchase("pd_all_characters");
    }
}
void ShopLayer::onKeyReleased(EventKeyboard::KeyCode keyCode, Event *event)
{
    if (keyCode == EventKeyboard::KeyCode::KEY_BACK)
    {
        if (!isWaiting) {
            CloseThis();
        }
    }
}