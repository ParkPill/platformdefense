
#include "GameManager.h"
#include "LanguageManager.h"
#include "SettingLayer.h"
#include "SimpleAudioEngine.h"
#include "NativeInterface.h"
#include "Title.h"
using namespace cocos2d;
using namespace CocosDenshion;

GameManager* GameManager::m_mySingleton = NULL;

GameManager* GameManager::getInstance()
{
    if(NULL == m_mySingleton)
    {
        m_mySingleton = new GameManager();
    }
    
    return m_mySingleton;
}
GameManager::GameManager()
{
    currentScene = SCENE_TITLE;
    settingLayer = NULL;
  	LoadingScene = NULL;
    isInMiddleOfGame = false;
    isGoogleSigned = false;
    isPaidGame = false;
    gameStarted = false;
    isUsingController = false;
	StageMode = 0;
    appAlreadyLaunched = false;
//    pushedLayer = NULL;
    initComplete = false;
    isPlayServiceLogIn = false;
	_starCount = UserDefault::getInstance()->getIntegerForKey(KEY_STAR_COUNT, 0);
    totalThemeCount = 7;
    pauseLayer = NULL;
   
    isGuestPlay = false;

    optionLayer = NULL;
    gameOverLayer = NULL;
    gameOverScene = NULL;
    currentStageLayer = NULL;
    achievementLayer = NULL;
    titleLayer = NULL;
    
    jumpPressed = false;
    cPressed = false;
    firePressed = false;
    leftPressed = false;
    rightPressed = false;
    downPressed = false;
    upPressed = false;
    settingLayer = NULL;
    shopLayer = NULL;
    
    isVideoRewardAttack = false;
    isVideoRewardEnergy = false;
    isVideoRewardCoins = false;
//    originallyDisabledMenusWhenLayerPushed = NULL;
    myPhoto = NULL;
    
    hudLayer = NULL;
    
    market = MARKET_PLAYSTORE_PAID;
//    priceArray = __Dictionary::create();
//    priceArray->retain();
    log("GameManager almost created");

    log("GameManager created");
}
void GameManager::onPlayServiceLogin(bool isLogin){
    if(settingLayer != NULL){
        ((SettingLayer*)settingLayer)->onPlayServiceLogin(isLogin);
    }
}
int GameManager::GetSpecialSkill(int index){
	switch (index){
	case 0:
	case 7:
		return SPECIAL_NORMAL;
	case 2:
	case 4:
	case 6:
		return SPECIAL_SLIM;
	case 14:
	case 15:
	case 16:
	case 26:
	case 30:
	case 31:
		return SPECIAL_HARD;
	case 1:
	case 3:
	case 5:
	case 12:
		return SPECIAL_HEALTHY;
	case 10:
	case 19:
	case 24:
	case 28:
		return SPECIAL_FAT;
	case 21:
	case 29:
	case 25:
		return SPECIAL_FIRE;
	case 8:
	case 9:
	case 11:
	case 17:
		return SPECIAL_FAST;
	case 13:
	case 20:
	case 22:
	case 27:
		return SPECIAL_ICE;
	case 18:
	case 23:
		return SPECIAL_FLY;
	case 32:
		return SPECIAL_BOSS;
	}
	return SPECIAL_NORMAL;
}
void GameManager::initGameManager(){
	float musicVolumn = UserDefault::getInstance()->getFloatForKey(KEY_MUSIC_VOLUMN, 1);
    setMusicVolumn(musicVolumn);
    float soundVolumn = UserDefault::getInstance()->getFloatForKey(KEY_SOUND_VOLUMN, 1);
    setSoundVolumn(soundVolumn);
	 initAchievement();
	_trapCount.clear();
	for (int i = 0; i < TOTAL_TRAP_COUNT; i++){
		_trapCount.push_back(0);
	}
    
    SmallPriceText = "$2.99";
    MiddlePriceText = "$9.99";
    BigPriceText = "$49.99";
    HugePriceText = "$99.99";
    HeroPriceText = "$4.99";
    
    breePriceText = "$0.99";
    tomPriceText = "$0.99";
    karenPriceText = "$1.99";
    karlPriceText = "$1.99";
    andrewPriceText = "$1.99";
    allFriendsPriceText = "$5.99";
    
    if (GameManager::getInstance()->market == MARKET_TSTORE_PAID ||
        GameManager::getInstance()->market == MARKET_NAVER_PAID){
        SmallPriceText = "₩3,300";
        MiddlePriceText = "₩11,000";
        BigPriceText = "₩55,000";
        HugePriceText = "₩100,000";
        HeroPriceText = "₩5,000";
    }
    if (GameManager::getInstance()->market == MARKET_PLAYSTORE_PAID ||
        GameManager::getInstance()->market == MARKET_PLAYSTORE_FREE ||
        GameManager::getInstance()->market == MARKET_APPSTORE_PAID ||
        GameManager::getInstance()->market == MARKET_APPSTORE_FREE) {
        sdkbox::IAP::setListener(this);
        sdkbox::IAP::init();
        sdkbox::IAP::refresh();
    }
}


// in app purchase
void GameManager::iapFailed(){
    if (GameManager::getInstance()->shopLayer != NULL) {
        GameManager::getInstance()->shopLayer->HideIndicator();
    }
    ((Title*)titleLayer)->HideIndicator();
}
void GameManager::iapSuccess(){
    iapSuccess(GameManager::getInstance()->shopLayer->purchasingIndex);
}
void GameManager::iapSuccess(int index){
    int coinLeft = UserDefault::getInstance()->getIntegerForKey(KEY_COIN_LEFT, 0);
    
    if (GameManager::getInstance()->currentStageLayer != NULL) {
        coinLeft = GameManager::getInstance()->currentStageLayer->CollectedCoinCount;
    }
    
    if(index == 0){
        coinLeft += 3000;
    }else if(index == 1){
        coinLeft += 11000;
    }else if(index == 2){
        coinLeft += 55000;
    }else if(index == 3){
        coinLeft += 125000;
    }else if(index == 4){
        UserDefault::getInstance()->setBoolForKey(KEY_ALL_HEROES, true);
    }else if(index == 5){
        // nothing but KEY_PAID_USER true which is below
    }
    UserDefault::getInstance()->setBoolForKey(KEY_PAID_USER, true);
    UserDefault::getInstance()->setIntegerForKey(KEY_COIN_LEFT, coinLeft);
    
    if (GameManager::getInstance()->currentStageLayer != NULL) {
        GameManager::getInstance()->currentStageLayer->CollectedCoinCount = coinLeft;
        GameManager::getInstance()->getHudLayer()->updateCoinCount();
    }
    
    if (GameManager::getInstance()->shopLayer != NULL) {
        GameManager::getInstance()->shopLayer->UpdateCoinCount();
        GameManager::getInstance()->shopLayer->HideIndicator();
        GameManager::getInstance()->shopLayer->UpdatePrice();
    }

    UserDefault::getInstance()->flush();
}
void GameManager::onSuccess(sdkbox::Product const& p) {
    log("onSuccess");
    if(p.name.compare("small_coin") == 0){
        iapSuccess(0);
    }else if(p.name.compare("middle_coin") == 0){
        iapSuccess(1);
    }else if(p.name.compare("big_coin") == 0){
        iapSuccess(2);
    }else if(p.name.compare("huge_coin") == 0){
        iapSuccess(3);
    }else if(p.name.compare("pd_all_characters") == 0){
        iapSuccess(4);
    }else if(p.name.compare("pdf_remove_ads") == 0){
        iapSuccess(5);
    }else if(p.name.compare("bree") == 0){
        UserDefault::getInstance()->setBoolForKey(KEY_BREE, true);
        ((Title*)GameManager::getInstance()->titleLayer)->UpdateFriendsPopup();
    }else if(p.name.compare("tom") == 0){
        UserDefault::getInstance()->setBoolForKey(KEY_TOM, true);
        ((Title*)GameManager::getInstance()->titleLayer)->UpdateFriendsPopup();
    }else if(p.name.compare("karen") == 0){
        UserDefault::getInstance()->setBoolForKey(KEY_KAREN, true);
        ((Title*)GameManager::getInstance()->titleLayer)->UpdateFriendsPopup();
    }else if(p.name.compare("karl") == 0){
        UserDefault::getInstance()->setBoolForKey(KEY_KARL, true);
        ((Title*)GameManager::getInstance()->titleLayer)->UpdateFriendsPopup();
    }else if(p.name.compare("andrew") == 0){
        UserDefault::getInstance()->setBoolForKey(KEY_ANDREW, true);
        ((Title*)GameManager::getInstance()->titleLayer)->UpdateFriendsPopup();
    }else if(p.name.compare("unlockfriends") == 0){
        UserDefault::getInstance()->setBoolForKey(KEY_BREE, true);
        UserDefault::getInstance()->setBoolForKey(KEY_TOM, true);
        UserDefault::getInstance()->setBoolForKey(KEY_KAREN, true);
        UserDefault::getInstance()->setBoolForKey(KEY_KARL, true);
        UserDefault::getInstance()->setBoolForKey(KEY_ANDREW, true);
        UserDefault::getInstance()->setBoolForKey(KEY_UNLOCK_FRIENDS_ALL, true);
        ((Title*)GameManager::getInstance()->titleLayer)->UpdateFriendsPopup();
    }
    UserDefault::getInstance()->flush();
}
void GameManager::onFailure(sdkbox::Product const& p, const std::string &msg){
    log("onFailure");
    iapFailed();
}
void GameManager::onCanceled(sdkbox::Product const& p) {
    log("onCanceled");
    iapFailed();
}
void GameManager::onRestored(sdkbox::Product const& p) {
    log("onRestored");

    if(p.name.compare("pd_all_characters") == 0){
        log("onRestored pd_all_characters");
        iapSuccess(4);
    }else if(p.name.compare("pdf_remove_ads") == 0){
        log("onRestored pdf_remove_ads");
        iapSuccess(5);
    }else if(p.name.compare("bree") == 0){
        UserDefault::getInstance()->setBoolForKey(KEY_BREE, true);
        ((Title*)GameManager::getInstance()->titleLayer)->UpdateFriendsPopup();
    }else if(p.name.compare("tom") == 0){
        UserDefault::getInstance()->setBoolForKey(KEY_TOM, true);
        ((Title*)GameManager::getInstance()->titleLayer)->UpdateFriendsPopup();
    }else if(p.name.compare("karen") == 0){
        UserDefault::getInstance()->setBoolForKey(KEY_KAREN, true);
        ((Title*)GameManager::getInstance()->titleLayer)->UpdateFriendsPopup();
    }else if(p.name.compare("karl") == 0){
        UserDefault::getInstance()->setBoolForKey(KEY_KARL, true);
        ((Title*)GameManager::getInstance()->titleLayer)->UpdateFriendsPopup();
    }else if(p.name.compare("andrew") == 0){
        UserDefault::getInstance()->setBoolForKey(KEY_ANDREW, true);
        ((Title*)GameManager::getInstance()->titleLayer)->UpdateFriendsPopup();
    }else if(p.name.compare("unlockfriends") == 0){
        UserDefault::getInstance()->setBoolForKey(KEY_BREE, true);
        UserDefault::getInstance()->setBoolForKey(KEY_TOM, true);
        UserDefault::getInstance()->setBoolForKey(KEY_KAREN, true);
        UserDefault::getInstance()->setBoolForKey(KEY_KARL, true);
        UserDefault::getInstance()->setBoolForKey(KEY_ANDREW, true);
        UserDefault::getInstance()->setBoolForKey(KEY_UNLOCK_FRIENDS_ALL, true);
        ((Title*)GameManager::getInstance()->titleLayer)->UpdateFriendsPopup();
    }
    iapFailed();
}
void GameManager::onProductRequestSuccess(std::vector<sdkbox::Product> const &products){
    log("onProductRequestSuccess");
    for(int i = 0;i < products.size();i++){
        sdkbox::Product product = products.at(i);
        log("product id: %s", product.id.c_str());
        log("product name: %s", product.name.c_str());
        log("product price: %s, value: %f", product.price.c_str(), product.priceValue);
        
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
        if(product.id.compare("com.magmon.PlatformDefense.small_coin") == 0){
            GameManager::getInstance()->SmallPriceText = product.price;
        }else if(product.id.compare("com.magmon.PlatformDefense.middle_coin") == 0){
            GameManager::getInstance()->MiddlePriceText = product.price;
        }else if(product.id.compare("com.magmon.PlatformDefense.big_coin") == 0){
            GameManager::getInstance()->BigPriceText = product.price;
        }else if(product.id.compare("com.magmon.PlatformDefense.huge_coin") == 0){
            GameManager::getInstance()->HugePriceText = product.price;
        }else if(product.id.compare("pd_all_characters") == 0){
            GameManager::getInstance()->HeroPriceText = product.price;
        }
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
        if(product.price.size() == 0) return;
        
        if(product.id.compare("com.magmon.platformdefense.small_coin") == 0){
            GameManager::getInstance()->SmallPriceText = product.price;
        }else if(product.id.compare("com.magmon.platformdefense.middle_coin") == 0){
            GameManager::getInstance()->MiddlePriceText = product.price;
        }else if(product.id.compare("com.magmon.platformdefense.big_coin") == 0){
            GameManager::getInstance()->BigPriceText = product.price;
        }else if(product.id.compare("com.magmon.platformdefense.huge_coin") == 0){
            GameManager::getInstance()->HugePriceText = product.price;
        }else if(product.id.compare("pd_all_characters") == 0){
            GameManager::getInstance()->HeroPriceText = product.price;
        }
#else
        
#endif
        if(product.price.size() == 0) return;
        if(product.id.compare("com.magmon.platformdefense.bree") == 0){
            GameManager::getInstance()->breePriceText = product.price;
        }else if(product.id.compare("com.magmon.platformdefense.tom") == 0){
            GameManager::getInstance()->tomPriceText = product.price;
        }else if(product.id.compare("com.magmon.platformdefense.karen") == 0){
            GameManager::getInstance()->karenPriceText = product.price;
        }else if(product.id.compare("com.magmon.platformdefense.karl") == 0){
            GameManager::getInstance()->karlPriceText = product.price;
        }else if(product.id.compare("com.magmon.platformdefense.andrew") == 0){
            GameManager::getInstance()->andrewPriceText = product.price;
        }else if(product.id.compare("com.magmon.platformdefense.unlockfriends") == 0){
            GameManager::getInstance()->allFriendsPriceText = product.price;
        }
    }
}

void GameManager::onProductRequestFailure(const std::string &msg) {
    log("onProductRequestFailure: %s", msg.c_str());
}
void GameManager::onRestoreComplete(bool ok, const std::string &msg){
    log("onRestoreComplete: %d, %s", (int)ok, msg.c_str());
    iapFailed();
}
void GameManager::onInitialized(bool ok){
    log("onInitialized: %d", (int)ok);
}
int GameManager::getGemForCoin(int coinCount){
    if(coinCount/100 == coinCount/100.0f){
        return coinCount/100;
    }else{
        return coinCount/100 + 1;
    }
}
void GameManager::addYellowTurningBackground(Node* node, int backWidth){
    Sprite* spt;
    for(int i = 0;i<2;i++){
        spt = Sprite::create("whiteSquare.png");
        spt->setScale(backWidth/spt->getContentSize().width);
        spt->setRotation(i*45);
        node->addChild(spt, -1);
        spt->setOpacity(100);
        spt->setColor(Color3B(255, 236, 79));
        BlendFunc bf = {GL_ONE, GL_ONE_MINUS_SRC_ALPHA};
        spt->setBlendFunc(bf);
        spt->getTexture()->setAntiAliasTexParameters();
        spt->setPosition(node->getContentSize().width/2, node->getContentSize().height/2);
        
        spt->runAction(RepeatForever::create(RotateBy::create(3, 360)));
    }
}
void GameManager::SetStringWithAutoLineBreak(TextBMFont* lbl, std::string str, float width){

	int next = 0;
	int spaceIndex = 0;
	for (int i = 1; i < str.length(); i++){
		lbl->setString(str.substr(next, i - next));
		if (lbl->getContentSize().width*lbl->getScaleX() > width){
			spaceIndex = i - 1;
			for (int j = spaceIndex; j > next; j--){
				if (str.substr(j, 1).compare(" ") == 0){
					spaceIndex = j;
					break;
				}
			}
			str.replace(spaceIndex,1, "\n");
			next = i;
		}
	}
	lbl->setString(str);
}
void GameManager::AddCoin(int howMuch){
	currentStageLayer->CollectedCoinCount += howMuch;
}
int GameManager::GetCoin(){
	return currentStageLayer->CollectedCoinCount;
}
int GameManager::GetUpgradePrice(Point pos){
	Trap* trap = currentStageLayer->GetTrap(pos);
	int price = (trap->Level + 1) * 50;
	switch (trap->TrapType){
	case TRAP_FREEZER:
		price = 25 + trap->Level * 10;
		break;
	case TRAP_WALLSPIKE:
		price = 35 + trap->Level * 10;
		break;
	case TRAP_SPIKE:
		price = 35 + trap->Level * 25;
		break;
	case TRAP_FLASK:
		price = 20 + trap->Level * 18;
		break;
	case TRAP_WALL:
		price = 35 + trap->Level * 30;
		break;
	case TRAP_GASBURNER:
		price = 35 + trap->Level * 15;
		break;
	case TRAP_TOPSPIKE:
		price = 25 + trap->Level * 15;
		break;
	case TRAP_BOMB:
		price = 25 + trap->Level * 10;
		break;
	case TRAP_ICEBALL:
		price = 30 + trap->Level * 18;
		break;
	case TRAP_MACEBALL:
		price = 28 + trap->Level * 18;
		break;
	case TRAP_FIRE:
		price = 50 + trap->Level * 20;
		break;
	case TRAP_HAND:
		price = 90 + trap->Level * 10;
		break;
	}
	return price*2.0f;
}
int GameManager::GetStarCount(){
	return _starCount;
}
void GameManager::SetStarCount(int star){
	_starCount = star;
}
void GameManager::AddStarCount(int star){
	_starCount += star;
	UserDefault::getInstance()->setIntegerForKey(KEY_STAR_COUNT, _starCount);
	getHudLayer()->LblStarCount->setString(cocos2d::Value(_starCount).asString());
    
    int starForAds = UserDefault::getInstance()->getIntegerForKey(KEY_STAR_TO_SEE_VIDEO_ADS, VIDEO_ADS_ALLOW_STAR_COUNT);
    starForAds += star;
    UserDefault::getInstance()->setIntegerForKey(KEY_STAR_TO_SEE_VIDEO_ADS, starForAds);
}
int GameManager::GetTrapPrice(int trapType){
	int count = _trapCount[trapType];
	switch (trapType){
	case TRAP_FREEZER:
		return 30 + count * 10;
	case TRAP_WALLSPIKE:
		return 50 + count * 30;
	case TRAP_SPIKE:
		return 30 + count * 25;
	case TRAP_FLASK:
		return 10;
	case TRAP_WALL:
		return 25;
	case TRAP_GASBURNER:
		return 70 + count* 15;
	case TRAP_TOPSPIKE:
		return 60 + count * 15;
	case TRAP_BOMB:
		return 25;
	case TRAP_ICEBALL:
		return 80 + count * 28;
	case TRAP_MACEBALL:
		return 35;
	case TRAP_FIRE :
		return 110 + count * 30;
	case TRAP_HAND :
		return 300 + count * 150;
	}
	return (count + 1) * 50;
}
void GameManager::ResetTrapCount(){
	for (int i = 0; i < _trapCount.size(); i++){
		_trapCount[i] = 0;
	}
}
void GameManager::AddTrapCount(int trapType){
	_trapCount[trapType]++;
}
void GameManager::RemoveTrapCount(int trapType){
	_trapCount[trapType]--;
}
void GameManager::setFontName(Label* lbl, const char* name, float fontSize){
    if(FileUtils::getInstance()->isFileExist(name))
    {
        TTFConfig config = lbl->getTTFConfig();
        config.fontFilePath = name;
        config.fontSize = fontSize;
        lbl->setTTFConfig(config);
    }
    else{
        lbl->setSystemFontName(name);
    }
}

std::string GameManager::GetSlotKey(int index){
	char buf[7];
	sprintf(buf, "Slot%d", index);
	return buf;
}

int GameManager::getPotionCount(){
    return UserDefault::getInstance()->getIntegerForKey("Potion_Count", 3);
}
void GameManager::setPotionCount(int potionCount){
    UserDefault::getInstance()->setIntegerForKey("Potion_Count", potionCount);
}
void GameManager::exitGame(){
    Director::getInstance()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

const char* GameManager::getShortenedKoreanString(std::string str, int length){
    const char* nick = "";
    if (str.length() > length) {
        log("original: %s", str.c_str());
        int normalIndex = -1;
        for (int k = 0; k < length; k++) {
            if (str[k] >= 0 && (int)str[k] < 128) {
                normalIndex = k;
            }
        }
        
        log("normal: %d", normalIndex);
        if (normalIndex < 0 || normalIndex == length) {
            nick = __String::createWithFormat("%s...",str.substr(0, length).c_str())->getCString();
            log("all korean: normalIndex: %d", normalIndex);
        }else{
            int last = normalIndex;
            for (int i = last; i < length - 3; i += 3) {
                last += 3;
            }
            log("ugly/ last: %d, normalIndex: %d", last, normalIndex);
            nick = __String::createWithFormat("%s...",str.substr(0, (last + 1)).c_str())->getCString();
        }
    }
    return nick;
}

double GameManager::getAngle(Point pos1, Point pos2){
    float xGap = pos1.x - pos2.x;
    float yGap = pos1.y - pos2.y;
    return atan2(yGap, xGap)*180/3.14;
}
void GameManager::makeItSiluk(Node* node){
    float originalScaleX = node->getScaleX();
    float originalScaleY = node->getScaleY();
    
    float duration = 0.3f;
    float scaleBit;
    if(node->getContentSize().width > node->getContentSize().height){
        scaleBit = node->getContentSize().height*0.05;
    }else{
        scaleBit = node->getContentSize().width*0.05;
    }
    node->runAction(RepeatForever::create(Sequence::create(ScaleTo::create(duration, (1+scaleBit/node->getContentSize().width)*originalScaleX, (1-scaleBit/node->getContentSize().width)*originalScaleY), ScaleTo::create(duration, (1-scaleBit/node->getContentSize().width)*originalScaleX, (1+scaleBit/node->getContentSize().width)*originalScaleY), NULL)));
}




void GameManager::setHudLayer(HudLayer* layer)
{
    hudLayer = layer;
//    hudLayer->retain();
}
HudLayer* GameManager::getHudLayer()
{
    if(!hudLayer){
        return NULL;
    }
    return hudLayer;
}

void GameManager::setStageScene(Scene* scene)
{
    isStageSetOnce = true;
//    stageScene->retain();
}

void GameManager::unscheduleAllNotifications(){
    //GoogleBilling::Billing::cancelAllLocalNotification();
}
void GameManager::scheduleLocalNotification(const char* title, const char* msg, int time){
    //GoogleBilling::Billing::registerLocalNotification(time, msg, title);
}
void GameManager::scheduleLocalNotification(){
    /*GoogleBilling::Billing::cancelAllLocalNotification();
    if (!this->getNotificationOn()) {
        return;
    }
    
    log("schedule");
    
    if(Application::getInstance()->getCurrentLanguage() == LanguageType::KOREAN){
        GoogleBilling::Billing::registerLocalNotification(60*60*24*1, "공주가 기다려요! 어서 공주를 구해주세요!", "공주가 기다려요!");
        GoogleBilling::Billing::registerLocalNotification(60*60*24*2, "공주가 기다려요! 대리님 구해주세요!", "공주가 기다려요!");
        GoogleBilling::Billing::registerLocalNotification(60*60*24*3, "깃발이 가득 찼어요! 맘껏 대리의 전설을 즐겨볼까요?", "깃발이 가득 찼어요!");
        GoogleBilling::Billing::registerLocalNotification(60*60*24*4, "공주가 기다려요!어서 공주를 구해주세요!", "공주가 기다려요!");
        GoogleBilling::Billing::registerLocalNotification(60*60*24*5, "공주가 기다려요! 어서 공주를 구해주세요!", "공주가 기다려요!");
        GoogleBilling::Billing::registerLocalNotification(60*60*24*6, "공주가 기다려요! 대리님 구해주세요!", "공주가 기다려요!");
        GoogleBilling::Billing::registerLocalNotification(60*60*24*7, "깃발이 가득 찼어요! 맘껏 대리의 전설을 즐겨볼까요?", "깃발이 가득 찼어요!");
        GoogleBilling::Billing::registerLocalNotification(60*60*24*8, "공주가 기다려요!어서 공주를 구해주세요!", "공주가 기다려요!");
        GoogleBilling::Billing::registerLocalNotification(60*60*24*9, "공주가 기다려요! 어서 공주를 구해주세요!", "공주가 기다려요!");
        GoogleBilling::Billing::registerLocalNotification(60*60*24*10, "공주가 기다려요! 대리님 구해주세요!", "공주가 기다려요!");
        GoogleBilling::Billing::registerLocalNotification(60*60*24*11, "깃발이 가득 찼어요! 맘껏 대리의 전설을 즐겨볼까요?", "깃발이 가득 찼어요!");
        GoogleBilling::Billing::registerLocalNotification(60*60*24*12, "공주가 기다려요!어서 공주를 구해주세요!", "공주가 기다려요!");
        GoogleBilling::Billing::registerLocalNotification(60*60*24*13, "공주가 기다려요! 어서 공주를 구해주세요!", "공주가 기다려요!");
        GoogleBilling::Billing::registerLocalNotification(60*60*24*14, "공주가 기다려요! 대리님 구해주세요!", "공주가 기다려요!");
        GoogleBilling::Billing::registerLocalNotification(60*60*24*15, "깃발이 가득 찼어요! 맘껏 대리의 전설을 즐겨볼까요?", "깃발이 가득 찼어요!");
        GoogleBilling::Billing::registerLocalNotification(60*60*24*16, "공주가 기다려요!어서 공주를 구해주세요!", "공주가 기다려요!");
        GoogleBilling::Billing::registerLocalNotification(60*60*24*17, "공주가 기다려요! 어서 공주를 구해주세요!", "공주가 기다려요!");
        GoogleBilling::Billing::registerLocalNotification(60*60*24*18, "공주가 기다려요! 대리님 구해주세요!", "공주가 기다려요!");
        GoogleBilling::Billing::registerLocalNotification(60*60*24*19, "깃발이 가득 찼어요! 맘껏 대리의 전설을 즐겨볼까요?", "깃발이 가득 찼어요!");
        GoogleBilling::Billing::registerLocalNotification(60*60*24*20, "공주가 기다려요!어서 공주를 구해주세요!", "공주가 기다려요!");
    }else{
        GoogleBilling::Billing::registerLocalNotification(60*60*24*1, "Come and save the princess!", "The princess is waiting for you");
        GoogleBilling::Billing::registerLocalNotification(60*60*24*3, "Come and save the princess!", "The princess is waiting for you");
        GoogleBilling::Billing::registerLocalNotification(60*60*24*5, "Come and save the princess!", "The princess is waiting for you");
        GoogleBilling::Billing::registerLocalNotification(60*60*24*7, "Come and save the princess!", "The princess is waiting for you");
        GoogleBilling::Billing::registerLocalNotification(60*60*24*9, "Come and save the princess!", "The princess is waiting for you");
        GoogleBilling::Billing::registerLocalNotification(60*60*24*11, "Come and save the princess!", "The princess is waiting for you");
        GoogleBilling::Billing::registerLocalNotification(60*60*24*13, "Come and save the princess!", "The princess is waiting for you");
        GoogleBilling::Billing::registerLocalNotification(60*60*24*15, "Come and save the princess!", "The princess is waiting for you");
        GoogleBilling::Billing::registerLocalNotification(60*60*24*17, "Come and save the princess!", "The princess is waiting for you");
        GoogleBilling::Billing::registerLocalNotification(60*60*24*19, "Come and save the princess!", "The princess is waiting for you");
        GoogleBilling::Billing::registerLocalNotification(60*60*24*21, "Come and save the princess!", "The princess is waiting for you");
        GoogleBilling::Billing::registerLocalNotification(60*60*24*30, "Come and save the princess!", "The princess is waiting for you");
    }*/
}
void GameManager::setMusicVolumn(float vol)
{
    float value = 0;
    float percent = vol * 100;
    if(percent > 30){
        value += 0.1;
        value += (percent - 30)*0.9f/70;
    }else{
        value = percent/300.0f;
    }
    
    CocosDenshion::SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(vol);
    UserDefault::getInstance()->setFloatForKey(KEY_MUSIC_VOLUMN, vol);
//    GameManager::getInstance()->saveCoin();
}
void GameManager::setSoundVolumn(float vol)
{
    CocosDenshion::SimpleAudioEngine::getInstance()->setEffectsVolume(vol);
    UserDefault::getInstance()->setFloatForKey(KEY_SOUND_VOLUMN, vol);
//    GameManager::getInstance()->saveCoin();
}
void GameManager::setNotificationOn(bool onOff)
{
    UserDefault::getInstance()->setBoolForKey(KEY_NOTIFICATION_ON, onOff);
//    GameManager::getInstance()->saveCoin();
}
float GameManager::getMusicVolumn()
{
    float value = 0;
    float percent = SimpleAudioEngine::getInstance()->getBackgroundMusicVolume() * 100;
    if(percent > 30){
        value += 0.1;
        value += (percent - 30)*0.9f/70;
    }else{
        value = percent/300.0f;
    }
    
    return value;
}
float GameManager::getSoundVolumn(){
    return SimpleAudioEngine::getInstance()->getEffectsVolume();
}
bool GameManager::getNotificationOn(){
    return UserDefault::getInstance()->getBoolForKey(KEY_NOTIFICATION_ON, true);
}

void GameManager::preLoadAllSoundEffect(){
    CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("Coin.wav");
    CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("Medicine.wav");
    CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("Spring.wav");
    CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("jump_sound.wav");
    CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("FireSmall.wav");
    CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("laser.wav");
    CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("MachineGun1.wav");
    CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("MachineGun2.wav");
    CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("DdalKack.wav");
    CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("powerUp45.wav");
    CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("rocketLaunch.wav");
    CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("noise37.wav");
    CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("fa.mp3");
    CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("misol.mp3");

    for (int i = 0; i < 9; i++) {
        int index = cocos2d::random()%9;
        char buf[20];
        sprintf(buf, "footStep%d.mp3", index);
        CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect(buf);
    }

    CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("waterSplash.mp3");
}
void GameManager::playSoundEffect(int sound){
    switch(sound)
	{
        case SOUND_COIN:
            CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Coin.wav");
            break;
        case SOUND_MEDICINE:
            CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Medicine.wav");
            break;
        case SOUND_SPRING:
            CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Spring.wav");
            break;
        case SOUND_JUMP:
            CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("jump_sound.wav");
            break;
        case SOUND_FIRE_SMALL:
            CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("FireSmall.wav");
            break;
        case SOUND_LASER:
            CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("laser.wav");
            break;
        case SOUND_MACHINE_GUN_1:
            CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("MachineGun1.wav");
            break;
        case SOUND_MACHINE_GUN_2:
            CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("MachineGun2.wav");
            break;
        case SOUND_DDALKACK:
            CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("DdalKack.wav");
            break;
        case SOUND_POWER_UP_45:
            CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("powerUp45.wav");
            break;
        case SOUND_ROCKEY_LAUNCH:
            CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("rocketLaunch.wav");
            break;
        case SOUND_NOISE_37:
            CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("noise37.wav");
            break;
        case SOUND_FA:
            CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("fa.mp3");
            break;
        case SOUND_MISOL:
            CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("misol.mp3");
            break;
        case SOUND_CASTLE_DOOR:
            CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("castleDoorSound.mp3");
            break;
        case SOUND_GAMEOVER:
            CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
            CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("GameOver_Music.mp3", false);
            break;
        case SOUND_REVIVE:
            CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
            CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("Revive_Music.mp3", false);
            break;
        case SOUND_WIN:
            CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
            CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("Win_Music.mp3", false);
            break;
        case SOUND_BGM_BRIGHT:
            CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
            CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("LegendDary2Bright.mp3", true);
            break;
        case SOUND_BGM_WHOS_BOSS:
            CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
            CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("LegendDary_whosBoss.mp3", true);
            break;
        case SOUND_BGM_MAYDAY:
            CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
            CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("mayday.mp3", true);
            break;
        case SOUND_FOOT_STEP:
        {
            int index = cocos2d::random()%9;
            char buf[20];
            sprintf(buf, "footStep%d.mp3", index);
            CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(buf);
        }
            break;
        case SOUND_WATER_SPLASH:
            CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("waterSplash.mp3");
            break;
        case SOUND_SMALL_EXPLOSION:
            CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("smallExplosion.wav");
            break;
        case SOUND_BGM_ALMOST_BRIGHT:
            CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
            CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("AlmostBright.mp3", true);
            break;
            
	}
}

Point GameManager::getGemCountPosition(){
    Size size = Director::getInstance()->getWinSize();
    return Point(size.width/2 + 190, size.height - 30);
}


Layer* GameManager::getShopLayer()
{
//    if (!shopLayer) {
//        shopLayer = ShopLayer::create();
//        shopLayer->retain();
//    }
    return NULL;//shopLayer;
}

Scene* GameManager::getStageScene(int stage)
{
    currentStageIndex = stage;
    
    return HelloWorld::scene(stage);
}

HelloWorld* GameManager::getCurrentStageLayer()
{
    return currentStageLayer;
}
void GameManager::setCurrentStageLayer(HelloWorld* layer)
{
    currentStageLayer = layer;
}const char* GameManager::getWeaponImageFileName(int weapon){
	return "heroWeapon0_0";
}
void GameManager::SpriteMoveDone(Node* node){
	node->stopAllActions();
	node->removeFromParentAndCleanup(true);
}
void GameManager::showParticleExplosion(Node* prt, const char* sptName, Point pos, int distance, float scale, Widget::TextureResType type){
	Sprite* particle;
	float delay = 0;
	float time;
	float angle = 0;
	for (int i = 0; i < 20; i++){
		if (type == Widget::TextureResType::PLIST){
			particle = Sprite::createWithSpriteFrameName(sptName);
		}
		else{
			particle = Sprite::create(sptName);
		}
		prt->addChild(particle);
		particle->setPosition(pos);
		angle = cocos2d::random() % 360;
		time = 0.3f + (cocos2d::random() % 10)*0.02f;
		delay = (cocos2d::random() % 10)*0.01f;
		particle->runAction(FadeOut::create(delay + time));
		particle->runAction(Sequence::create(
			DelayTime::create(delay),
			EaseOut::create(MoveBy::create(time, Point(distance*cos(angle*3.14f / 180), distance*sin(angle*3.14f/180))), 3),
			CallFuncN::create(CC_CALLBACK_1(GameManager::SpriteMoveDone, this)), NULL));
	}
}
void GameManager::pushLayer(Layer* parent, Layer* layer){
//    disableLayer(parent);
//    parent->addChild(layer, 1000);
    animateFadeIn(layer, parent);
}

void GameManager::pushLayerWithoutDisable(Layer* parent, Layer* layer){
    //    parent->addChild(layer, 1000);
    animateFadeIn(layer, parent);
}
void GameManager::googleSignInOrOut(bool sign){
    if(sign){
        //        MessageBox("Signed in", "Google Play Service");
        log("** google Signed in");
    }else{
        //        MessageBox("Signed out", "Google Play Service");
        log("** google not signed in");
    }
    
//    if(titleLayer != NULL && ((TitleLayer*)titleLayer)->titleMain != NULL){
//        ((TitleLayer*)titleLayer)->removeSignInButton();
//    }
    isGoogleSigned = sign;
    
}
void GameManager::animateFadeIn(Node* layer, Node* parent){
    parent->addChild(layer, 1000);
    layer->setScale(0.1f);
    layer->runAction(Sequence::create(ScaleTo::create(0.2, 1.1), ScaleTo::create(0.05, 0.95),ScaleTo::create(0.05, 1), NULL));
    
    Size size = Director::getInstance()->getWinSize();
    Sprite* blackBack = Sprite::create("blackSquare.png");
    parent->addChild(blackBack, 999);
    blackBack->setTag(7899);
    blackBack->setScaleX(size.width);
    blackBack->setScaleY(size.height);
    blackBack->setPosition(Point(size.width/2, size.height/2));
}
void GameManager::animateFadeOut(Node* layer){
//    layer->setScale(0.1f);
    layer->runAction(Sequence::create(ScaleTo::create(0.06, 0.85),ScaleTo::create(0.1, 1.2), ScaleTo::create(0.1, 0.1), CallFuncN::create(CC_CALLBACK_1(GameManager::animationFadeOutDone, this)), NULL));
}

void GameManager::animationFadeInDone(Node* layer){
//    log("fade in done");
}
void GameManager::animationFadeOutDone(Node* layer){
    Layer* parent = (Layer*)layer->getParent();
    parent->removeChildByTag(7899);
    layer->removeFromParentAndCleanup(true);
    log("layer removed");
//    enableLayer(parent);
}
/*
void GameManager::disableLayer(Layer* layer){
    layer->setTouchEnabled(false);
//    log("layer retain count: %d", layer->retainCount());
    CCMenu* menu;
    Layer* childLayer;
//    log("disable layer children count: %d", layer->getChildren().size());
    for (int i = 0; i < layer->getChildren().size(); i++) {
        menu = dynamic_cast<CCMenu*>(layer->getChildren().at(i));
        if (menu) {
            if (!menu->isEnabled()) {
                Vector<Droppable*>disabledMenusArray = dynamic_cast<CCArray*>((CCArray*)layer->getUserData());
                if (disabledMenusArray == NULL) {
                    disabledMenusArray = CCArray::create();
                    disabledMenusArray->retain();
                    layer->setUserData((void*)disabledMenusArray);
                }
                disabledMenusArray.pushBack(menu);
            }else{
                menu->setEnabled(false);
            }
        }else{
            childLayer = dynamic_cast<Layer*>(layer->getChildren().at(i));
            if (childLayer != NULL) {
                disableLayer(childLayer);
            }
        }
    }
}

void GameManager::popLayer(Layer* layer){
    animateFadeOut(layer);
}

void GameManager::popLayerWithoutAnimation(Layer* layer){
    animationFadeOutDone(layer);
}

void GameManager::enableLayer(Layer* layer){
    layer->setTouchEnabled(true);
    CCMenu* menu;
    Layer* childLayer;
    Vector<Droppable*>disabledMenusArray = NULL;
    for (int i = 0; i < layer->getChildren().size(); i++) {
        menu = dynamic_cast<CCMenu*>(layer->getChildren().at(i));
        if (menu) {
            disabledMenusArray = dynamic_cast<CCArray*>((CCArray*)layer->getUserData());
            
            if (disabledMenusArray != NULL && disabledMenusArray->containsObject(menu)) {
                
            }else{
                menu->setEnabled(true);
            }
        }
        childLayer = dynamic_cast<Layer*>(layer->getChildren().at(i));
        if (childLayer != NULL) {
            enableLayer(childLayer);
        }
    }

    if (disabledMenusArray != NULL) {
        disabledMenusArray->release();
        layer->setUserData(NULL);
    }
}*/


long GameManager::strToLong(const char* str){
    std::stringstream strValue;
    strValue << str;
    
    long longValue;
    strValue >> longValue;
    
    return longValue;
}
Sprite* GameManager::getGrayScaleImage(const char* src){
    Image *img=  new Image();
    img->initWithImageFile(src);

//    II) From Sprite :
    
//    II.1) Sprite -> RenderTexture2D
    
//    II.2) RenderTexture2D -> Image (Image *testImage = RenderText2D->newImage();)
    
//    2.Then You can do what you need :
    
//        Image *img= ... // make Image from Sprite
        int x=3;
    if(img->hasAlpha())
        x=4;
    
    unsigned char *data = new unsigned char[img->getDataLen()*x];
    data = img->getData();
    // [0][0] => Left-Top Pixel !
    // But cocos2d Location Y-axis is Bottom(0) to Top(max)
    
//    unsigned char *newData = new unsigned char[img->getDataLen()*x];
    
    for(int i=0;i<img->getWidth();i++)
    {
        for(int j=0;j<img->getHeight();j++)
        {
            unsigned char *pixel = data + (i + j * img->getWidth()) * x;
            
            // You can see/change pixels' RGBA value(0-255) here !
            unsigned char r = *pixel;
            unsigned char g = *(pixel + 1);
            unsigned char b = *(pixel + 2);
//            unsigned char a = *(pixel + 3);
            
            *pixel = (r+g+b)/3;
            *(pixel + 1) = (r+g+b)/3;
            *(pixel + 2) = (r+g+b)/3;
        }
    }
//    3.Then, convert it to texture_2D
    //Image -> Texture2d
    Texture2D* texture_2D= new Texture2D();
    texture_2D->initWithImage(img);
//    4.And Finally back to Sprite
    return Sprite::createWithTexture(texture_2D);
}

Sprite* GameManager::getSpriteShapeSolidColorSprite(const char* src, Color3B color){
    Image *img=  new Image();
    img->initWithImageFile(src);

    int x=3;
    if(img->hasAlpha())
        x=4;
    
    unsigned char *data = new unsigned char[img->getDataLen()*x];
    data = img->getData();
    
    for(int i=0;i<img->getWidth();i++)
    {
        for(int j=0;j<img->getHeight();j++)
        {
            unsigned char *pixel = data + (i + j * img->getWidth()) * x;
            
            if(img->hasAlpha())
            {
                if(*(pixel + 3) == 0){
                    continue;
                }
            }
            *pixel = color.r;
            *(pixel + 1) = color.g;
            *(pixel + 2) = color.b;
        }
    }
    Texture2D* texture_2D= new Texture2D();
    texture_2D->initWithImage(img);

    return Sprite::createWithTexture(texture_2D);
}
void GameManager::SendAchievement(const char* text, int count, int goal){
    const char* theText;
    if (GameManager::getInstance()->market == MARKET_MAC) {
        theText = __String::createWithFormat("%s_Mac", text)->getCString();
    }else{
        theText = text;
    }
    //GoogleBilling::Billing::updateAchievement(text, count, goal);
}
void GameManager::showVideo(){
    CocosDenshion::SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
    //GoogleBilling::Billing::showVideo();
}
void GameManager::showVideoDone(){
    getHudLayer()->videoDone();
//    ((TitleLayer*)titleLayer)->showVideoDone();
    CocosDenshion::SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
bool GameManager::checkDataSecure(){
    
    return true;
}
void GameManager::showVideoFailed(){
    if (getHudLayer()) {
        getHudLayer()->videoFailed();
    }
//    ((TitleLayer*)titleLayer)->showVideoFailed();
    CocosDenshion::SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
void GameManager::clickAdsFailed(){
    
}
void GameManager::clickAdsDone(){
    
}
/*
RenderTexture* GameManager::createAdditiveBorder( Sprite* label, int size, Color3B color, GLubyte opacity )
{
    RenderTexture* rt = RenderTexture::create(label->getTexture()->getContentSize().width + size * 2,
                                                  label->getTexture()->getContentSize().height+size * 2
                                                  );
    
    Point originalPos = label->getPosition();
    
    Color3B originalColor = label->getColor();
    
    GLubyte originalOpacity = label->getOpacity();
    
    bool originalVisibility = label->isVisible();
    
    label->setColor(color);
    
    label->setOpacity(opacity);
    
    label->setVisible(true);
    
    ccBlendFunc originalBlend = label->getBlendFunc();
    
    ccBlendFunc bf = {GL_SRC_ALPHA, GL_ONE}; // GL_SRC_ALPHA for glow effect
    label->setBlendFunc(bf);
    
    Point bottomLeft = Point(
                             label->getTexture()->getContentSize().width * label->getAnchorPoint().x + size,
                             label->getTexture()->getContentSize().height * label->getAnchorPoint().y + size);
    
    Point positionOffset= Point(
                                label->getTexture()->getContentSize().width  * label->getAnchorPoint().x - label->getTexture()->getContentSize().width / 2,
                                label->getTexture()->getContentSize().height * label->getAnchorPoint().y - label->getTexture()->getContentSize().height / 2);
    
    
    Point position = ccpSub(originalPos, positionOffset);
    
    //rt->getSprite()->getTexture()->setAntiAliasTexParameters();
    rt->begin();
    
    for (int i=0; i<360; i+= 15) // you should optimize that for your needs
    {
        label->setPosition(Point(bottomLeft.x + sin(CC_DEGREES_TO_RADIANS(i))*size, bottomLeft.y + cos(CC_DEGREES_TO_RADIANS(i))*size)
                           );
        label->visit();
    }
    rt->end();
    
    label->setPosition(originalPos);
    label->setColor(originalColor);
    label->setBlendFunc(originalBlend);
    label->setVisible(originalVisibility);
    label->setOpacity(originalOpacity);
    
    rt->setPosition(position);
    
    return rt;
}


RenderTexture* GameManager::createAdditive( Sprite* label, Color3B color, GLubyte opacity, int additiveCount)
{
    RenderTexture* rt = RenderTexture::create(label->getTexture()->getContentSize().width ,
                                                  label->getTexture()->getContentSize().height);
    
    Point originalPos = label->getPosition();
    
    Color3B originalColor = label->getColor();
    
    GLubyte originalOpacity = label->getOpacity();
    
    bool originalVisibility = label->isVisible();
    
    label->setColor(color);
    
    label->setOpacity(opacity);
    
    label->setVisible(true);
    
    ccBlendFunc originalBlend = label->getBlendFunc();
    
    ccBlendFunc bf = {GL_SRC_ALPHA, GL_ONE}; // GL_SRC_ALPHA for glow effect
    label->setBlendFunc(bf);
    
    Point bottomLeft = Point(
                             label->getTexture()->getContentSize().width * label->getAnchorPoint().x,
                             label->getTexture()->getContentSize().height * label->getAnchorPoint().y);
    
    Point positionOffset= Point(
                                label->getTexture()->getContentSize().width  * label->getAnchorPoint().x - label->getTexture()->getContentSize().width / 2,
                                label->getTexture()->getContentSize().height * label->getAnchorPoint().y - label->getTexture()->getContentSize().height / 2);
    
    
    Point position = originalPos - positionOffset;
    
    //rt->getSprite()->getTexture()->setAntiAliasTexParameters();
    rt->begin();
    
    label->setPosition(Point(bottomLeft.x, bottomLeft.y));
    for(int i = 0;i < additiveCount;i++){
        label->visit();
    }
    
    rt->end();
    
    label->setPosition(originalPos);
    label->setColor(originalColor);
    label->setBlendFunc(originalBlend);
    label->setVisible(originalVisibility);
    label->setOpacity(originalOpacity);
    
    rt->setPosition(position);
    
    return rt;
}
*/

bool GameManager::isBoss(int theme, int stage){
    if(theme == 0 && stage == 23) return true;
    if(theme == 1 && stage == 23) return true;
    if(theme == 2 && stage == 23) return true;
    return false;
}

void GameManager::initAchievement(){
    setAchievementGoal(ACHIEVEMENT_STAR_COLLECTER, 20);
    setAchievementGoal(ACHIEVEMENT_KEEPER, 1);
    setAchievementGoal(ACHIEVEMENT_FIRST_FRUIT, 1);
    setAchievementGoal(ACHIEVEMENT_RICH_MAN, 10000);
    setAchievementGoal(ACHIEVEMENT_KILLER, 5000);
    setAchievementGoal(ACHIEVEMENT_HOT_KETTLE, 20);
    setAchievementGoal(ACHIEVEMENT_WHOS_BOSS, 10);
    setAchievementGoal(ACHIEVEMENT_SOUL_LEGEND, 1);
}

//__String* GameManager::getInAppItemPrice(const char* name){
//    CCObject* obj = priceArray->objectForKey(name);
//    return (__String*)obj;
//}

//void GameManager::setInAppItemPrice(const char* name, const char* price){
//    GameManager::getInstance()->priceArray->setObject(__String::create(price), name);
//    log("cpp price: %s", ((__String*)GameManager::getInstance()->priceArray->objectForKey(name))->getCString());
//}
void GameManager::setAchievementGoal(int achievementIndex, int goalCount){
    char buf[50];
    sprintf(buf,"Achievement_goal%d" , achievementIndex);
    UserDefault::getInstance()->setIntegerForKey(buf, goalCount);
    saveCoin();
}
bool GameManager::setAchievement(int achievementIndex, int countToAdd){
    char buf[50];
	sprintf(buf, "Achievement_goal%d", achievementIndex);
    int goalCount = UserDefault::getInstance()->getIntegerForKey(buf, 0);

    char buff[50];
	sprintf(buff, "Achievement_current%d", achievementIndex);
    int original = UserDefault::getInstance()->getIntegerForKey(buff, 0);
    int currentCount = countToAdd;//
    
    if (currentCount > goalCount) {
        currentCount = goalCount;
    }
    UserDefault::getInstance()->setIntegerForKey(buff, currentCount);
    return goalCount <= currentCount && original < currentCount;
}
int GameManager::getAchievementGoalCount(int achievementIndex){
    char buf[50];
	sprintf(buf, "Achievement_goal%d", achievementIndex);
    return UserDefault::getInstance()->getIntegerForKey(buf, 0);
}
int GameManager::getAchievementCurrentCount(int achievementIndex){
    char buf[50];
	sprintf(buf, "Achievement_current%d", achievementIndex);
    return UserDefault::getInstance()->getIntegerForKey(buf, 0);
}
int GameManager::getCurrentExp(){
    int expUnit = GAME_EXP_UNIT;
    
    int originalExp = UserDefault::getInstance()->getIntegerForKey("GExp", 0);
    int expLeft = originalExp;
    int level = 0;
    for (int i = 0; i < 100; i++) {
        expLeft -= expUnit*i;
        level++;
        if (expLeft < 0) {
            level--;
            expLeft += expUnit*i;
            break;
        }else if(expLeft == 0){
            break;
        }
    }
    if (getGameLevel() >= MAX_LEVEL) {
        UserDefault::getInstance()->setIntegerForKey("GExp", originalExp - (expLeft - getCurrentExpMax()));
        saveCoin();
        expLeft = getCurrentExpMax();
    }
    return expLeft;
}
int GameManager::getCurrentExpMax(){
    return GAME_EXP_UNIT*getGameLevel();
}

int GameManager::getGameLevel(){
    int expUnit = GAME_EXP_UNIT;
    int expLeft = UserDefault::getInstance()->getIntegerForKey("GExp", 0);
    int level = 0;

    for (int i = 0; i < MAX_LEVEL; i++) {
        expLeft -= expUnit*i;
        level++;
        if (expLeft < 0) {
            level--;
            break;
        }else if(expLeft == 0){
            break;
        }
    }
    return level;
}
void GameManager::addGameExp(int exp){
    int expLeft = UserDefault::getInstance()->getIntegerForKey("GExp", 0);
    if (getGameLevel() < 100) {
        expLeft += exp;
    }
    UserDefault::getInstance()->setIntegerForKey("GExp", expLeft);
    saveCoin();
}
int GameManager::getIndexOf(std::vector<int> vec, int index){
    for (int i = 0; i < vec.size(); i++) {
        if(vec.at(i) == index){
            return i;
        }
    }
    return -1;
}

void GameManager::setLoadedData(const char* key, const char* value){
    log("setLoadedData: %s, %s", key, value);
    if(std::strcmp(value, "true") == 0 || std::strcmp(value, "false") == 0){
		UserDefault::getInstance()->setBoolForKey(key, cocos2d::Value(value).asBool());
    }else{
		UserDefault::getInstance()->setIntegerForKey(key, cocos2d::Value(value).asInt());
    }
}
void GameManager::reset(){
   
}
std::string GameManager::GetCoordinateKey(Point pos){
    char buf[20];
    sprintf(buf, "%d_%d", (int)pos.x, (int)pos.y);
    return buf;
}
void GameManager::loadGameData(){
    NativeInterface::NativeInterface::loadGame();
}
void GameManager::saveGameData(){
    int _stageMode = 0;
    Point StartPos = Point(19, 23);
    Point EndPos = Point(25, 13);
    int startX = StartPos.x;
    int startY = StartPos.y;
    int trapType; int trapLevel;
    Point pos = cocos2d::Point(startX, startY + 1);
    char buf[100];
    char leveBuf[100];
    int maxSpotCount = (EndPos.x - StartPos.x + 1)*(StartPos.y - EndPos.y + 1);
    for (int i = 0; i < maxSpotCount; i++){
        if (i % (int)(EndPos.x - StartPos.x + 1) == 0){
            pos.y -= 1;
            pos.x = startX;
        }
        else{
            pos.x += 1;
        }
        
        sprintf(buf, KEY_INSTALLED_TRAP_FORMAT, _stageMode, GetCoordinateKey(pos).c_str());
        trapType = UserDefault::getInstance()->getIntegerForKey(buf, -1);
        NativeInterface::NativeInterface::setIntForSavedGame(buf, trapType);
        
        sprintf(leveBuf, KEY_INSTALLED_TRAP_LEVEL_FORMAT, _stageMode, GetCoordinateKey(pos).c_str());
        trapLevel = UserDefault::getInstance()->getIntegerForKey(leveBuf, 0);
        NativeInterface::NativeInterface::setIntForSavedGame(leveBuf, trapLevel);
    }
    
    NativeInterface::NativeInterface::setIntForSavedGame(KEY_COIN_LEFT, 0);
    NativeInterface::NativeInterface::setIntForSavedGame(KEY_STAR_COUNT, UserDefault::getInstance()->getIntegerForKey(KEY_STAR_COUNT, 0));
    NativeInterface::NativeInterface::setIntForSavedGame(KEY_TOTAL_ENEMY_KILL, UserDefault::getInstance()->getIntegerForKey(KEY_TOTAL_ENEMY_KILL, 0));
    NativeInterface::NativeInterface::setStringForSavedGame(KEY_CONTINUE_AVAILABLE, UserDefault::getInstance()->getBoolForKey(KEY_CONTINUE_AVAILABLE, false)?"true":"false");
    NativeInterface::NativeInterface::setIntForSavedGame(KEY_CHECK_POINT, UserDefault::getInstance()->getIntegerForKey(KEY_CHECK_POINT, 1));
    
    NativeInterface::NativeInterface::setStringForSavedGame(KEY_TUTORIAL_CHECKPOINT, "true");
    NativeInterface::NativeInterface::setStringForSavedGame(KEY_TUTORIAL_DONE, "true");
    NativeInterface::NativeInterface::setStringForSavedGame(KEY_TUTORIAL_GAMEOVER, "true");
    NativeInterface::NativeInterface::setStringForSavedGame(KEY_TUTORIAL_SHOOT, "true");
    NativeInterface::NativeInterface::setStringForSavedGame(KEY_TUTORIAL_TRAP, "true");
    NativeInterface::NativeInterface::setStringForSavedGame(KEY_TUTORIAL_UPGRADE, "true");
    
    for (int i = 0; i < 5; i++) {
        char buf[30];
        sprintf(buf, KEY_ACHIEVEMENT_UNLOCKED_FORMAT, i);
        NativeInterface::NativeInterface::setStringForSavedGame(buf, UserDefault::getInstance()->getBoolForKey(buf, false)?"true":"false");
    }
    NativeInterface::NativeInterface::setStringForSavedGame(KEY_HERMIONE_UNLOCKED, UserDefault::getInstance()->getBoolForKey(KEY_HERMIONE_UNLOCKED, false)?"true":"false");
    NativeInterface::NativeInterface::setIntForSavedGame(KEY_CLEAR_COUNT, UserDefault::getInstance()->getIntegerForKey(KEY_CLEAR_COUNT, 0));
    NativeInterface::NativeInterface::setStringForSavedGame(KEY_ALL_HEROES, UserDefault::getInstance()->getBoolForKey(KEY_ALL_HEROES, false)?"true":"false");
    NativeInterface::NativeInterface::setIntForSavedGame(KEY_TOTAL_COIN_SPENT, UserDefault::getInstance()->getIntegerForKey(KEY_TOTAL_COIN_SPENT, 0));
    NativeInterface::NativeInterface::setIntForSavedGame(KEY_AOKIZI_LEVEL, UserDefault::getInstance()->getIntegerForKey(KEY_AOKIZI_LEVEL, 0));
    NativeInterface::NativeInterface::setIntForSavedGame(KEY_DARY_LEVEL, UserDefault::getInstance()->getIntegerForKey(KEY_DARY_LEVEL, 0));
    
    for (int i = 0; i < 5; i++) {
        char levelBuf[30];
        sprintf(levelBuf, KEY_FRIEND_LEVEL_FORMAT, i);
        int level = UserDefault::getInstance()->getIntegerForKey(levelBuf);
        NativeInterface::NativeInterface::setIntForSavedGame(levelBuf, level);
    }
    
    for (int i = 0; i < 5; i++) {
        char achieveBuf[30];
        sprintf(achieveBuf, KEY_ACHIEVEMENT_PROGRESS_FORMAT, i);
        int achieveCount = UserDefault::getInstance()->getIntegerForKey(achieveBuf);
        NativeInterface::NativeInterface::setIntForSavedGame(achieveBuf, achieveCount);
    }
    
    NativeInterface::NativeInterface::saveGame();
}

const char* GameManager::getAchievementId(int achievementIndex){
	return "achievementID";
}
void GameManager::saveCoin(){

}

/*
 switch (this->getTag()) {
 case PET_NONE:
 
 case PET_BOOGI:
 
 case PET_BURI:
 
 case PET_CHUS:
 
 case PET_DORA_0:
 
 case PET_DORA_1:
 
 case PET_DORA_2:
 
 case PET_DORA_3:
 
 case PET_DORIS_0:
 
 case PET_DORIS_1:
 
 case PET_DORIS_2:
 
 case PET_DORIS_3:
 
 case PET_FRITH_0:
 
 case PET_FRITH_1:
 
 case PET_FRITH_2:
 
 case PET_FRITH_3:
 
 case PET_KIWI:
 
 case PET_KUMURI:
 
 case PET_LEXY:
 
 case PET_LINDIS_0:
 
 case PET_LINDIS_1:
 
 case PET_LINDIS_2:
 
 case PET_LINDIS_3:
 
 case PET_NEPTUN:
 
 case PET_OXTO:
 
 case PET_PLUTO:
 
 case PET_RIUS:
 
 case PET_ROLO_0:
 
 case PET_ROLO_1:
 
 case PET_ROLO_2:
 
 case PET_ROLO_3:
 
 case PET_RURIK_0:
 
 case PET_RURIK_1:
 
 case PET_RURIK_2:
 
 case PET_RURIK_3:
 
 case PET_TERRY:
 
 case PET_TORI:
 
 case PET_TORORI:
 
 case PET_UNO:
 
 case PET_VETH:
 
 case PET_WALOONG_0:
 
 
 case PET_WALOONG_1:
 
 case PET_WALOONG_2:
 
 case PET_WALOONG_3:
 
 default:
 
 break;
 }
 */
void GameManager::scrollTheLayer(ScrollView* scrollLayer, bool isLeft, bool isHorizontal, int howMuch){
    Point pos;
    if(isHorizontal){
        float x = scrollLayer->getInnerContainer()->getPosition().x + scrollLayer->getContentSize().width*(isLeft?1:-1)*howMuch;
        if(x > 0){
            x = 0;
        }else if(x < -(scrollLayer->getInnerContainerSize().width - scrollLayer->getContentSize().width)){
            x = -(scrollLayer->getInnerContainerSize().width - scrollLayer->getContentSize().width);
        }
        scrollLayer->getInnerContainer()->runAction(MoveTo::create(0.3, Point(x, scrollLayer->getInnerContainer()->getPosition().y)));
    }else{
        
    }
    /*
    return;
    int currentX = scrollLayer->getInnerContainer()->getPosition().x;
    int width = scrollLayer->getContentSize().width;
    int totalWidth = scrollLayer->getInnerContainerSize().width - width;
    if (isLeft){
        currentX += width;
        if(currentX > 0){
            currentX = 0;
        }
        if(isHorizontal)
            scrollLayer->scrollToPercentHorizontal(-currentX*100/totalWidth, 0.1, true);
        else
            scrollLayer->scrollToPercentVertical(-currentX*100/totalWidth, 0.1, true);
    }else{
        float rightEndX = -currentX + width;
        if(rightEndX > totalWidth){
            rightEndX = totalWidth;
        }
        int percent = rightEndX*100/totalWidth;
        if(isHorizontal)
            scrollLayer->scrollToPercentHorizontal(percent, 0.1, true);
        else
            scrollLayer->scrollToPercentVertical(percent, 0.1, true);
    }*/
}
Layer* GameManager::getTitleLayer(){
    return titleLayer;
}
void GameManager::setFontSize(Label* lbl, float size){
    TTFConfig config = lbl->getTTFConfig();
    config.fontSize = size;
    lbl->setTTFConfig(config);
}
Color3B GameManager::getRandomColor(){
    int ranNum = cocos2d::random()%6;
    switch (ranNum) {
        case 0:
            return Color3B(255, cocos2d::random()%255, cocos2d::random()%255);
        case 1:
            return Color3B(cocos2d::random()%255, 255, cocos2d::random()%255);
        case 2:
            return Color3B(cocos2d::random()%255, cocos2d::random()%255, 255);
        case 3:
            return Color3B(255, 255, cocos2d::random()%255);
        case 4:
            return Color3B(cocos2d::random()%255, 255, 255);
        case 5:
            return Color3B(255, cocos2d::random()%255, 255);
        default:
            return Color3B::WHITE;
    }
}
/*RepeatForever* GameManager::getScaleUpDownAction(float scale)
{
    return RepeatForever::create((CCActionInterval*)Sequence::create(ScaleTo::create(0.5, scale + 0.02), ScaleTo::create(0.5, scale - 0.02), NULL));
}*/