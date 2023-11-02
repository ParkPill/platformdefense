#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "PluginGoogleAnalytics/PluginGoogleAnalytics.h"
#include "HudLayer.h"
#include "GameManager.h"
#include "MyMessageBox.h"
#include "NativeInterface.h"
#include "LanguageManager.h"
using namespace cocos2d;
using namespace CocosDenshion;


Scene* HelloWorld::scene(int stage)
{
    GameManager::getInstance()->leftPressed = false;
    GameManager::getInstance()->rightPressed = false;
    GameManager::getInstance()->firePressed = false;
    GameManager::getInstance()->jumpPressed = false;
    GameManager::getInstance()->cPressed = false;
    GameManager::getInstance()->downPressed = false;
    GameManager::getInstance()->upPressed = false;
    Size size = Director::getInstance()->getWinSize();
    Scene *scene = Scene::create();
//    int theme = GameManager::getInstance()->theme;
//    int stg = stage;

    HudLayer* hudLayer = HudLayer::create();
    scene->addChild(hudLayer, 10);
    GameManager::getInstance()->setHudLayer(hudLayer);
//    hudLayer->addListener();
    HelloWorld *layer = HelloWorld::create();
    scene->addChild(layer);
    GameManager::getInstance()->setCurrentStageLayer(layer);
    
    int tag = cocos2d::random()%50;
    layer->setTag(tag);
    hudLayer->setTag(tag);
    log("** New Tag: %d, theme: %d, stage: %d", tag, GameManager::getInstance()->theme, stage);
    
    bool setStage = true;
    
    if (setStage) {
        layer->setStage(stage);
    }

    //GoogleBilling::Billing::googleAnalyticsScreenName(__String::createWithFormat("Stage %d-%d", theme + 1,stg + 1)->getCString());
    return scene;
}
// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    size = Director::getInstance()->getWinSize();
    //////////////////////////////
    // 1. super init first
    
    GameManager::getInstance()->isInMiddleOfGame = true;
    if (!Layer::init())
    {
        return false;
    }
    coinSpent = 0;
    _viewPoint = cocos2d::Point(23.1f, 26.0f);
    _scale = 4.8f;
    _shootTutorialStarted = false;
    _trapTurorial = false;
    _messageBox = NULL;
    _isGamePaused = false;
    _shootTutorialCount = 0;
	_lastStrongestEnemy = -1;
	_stageMode = 0;
    _checkPointStageCount = 10;
    isSetStageDone = false;
    everySecond = false;
    isTouchStarted = false;
    isPaused = false;
    isTeleporting = false;
    coinDelay = 0;
    otherDelay = 0;
    isTouchEnded = true;
	killedEnemyCount = 0;
    missileEffectCollapsedTime = 0;
    playerFireCoolTime = 0;
    //this->setKeypadEnabled(true);
//    this->setTouchEnabled(true);
    playerIgnoreGravity = false;
    bulletWasted = false;
    background = NULL;
    this->addListener();
    shootFilter = 0;

    GameManager::getInstance()->currentScene = SCENE_IN_GAME;
//    GameManager::getInstance()->saveCoin();
    return true;
}

HelloWorld::HelloWorld(){
    
}

void HelloWorld::dialogFinished(Ref* obj){
    
}

void HelloWorld::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* unused_event)
{
    if(keyCode == EventKeyboard::KeyCode::KEY_BACKSPACE){
        //GoogleBilling::Billing::destroyAds();
//        Director::getInstance()->replaceScene(StageSelectScene::scene(GameManager::getInstance()->theme));
    }
}

void HelloWorld::enemyUpdate(float dt)
{
    if (isGameOver) {
        return;
    }

    cocos2d::Rect projectileRect;
    
    for (auto drop: enemyArray){
        if (drop->getPosition().y-player->getPosition().y > 50 || drop->getPosition().y - player->getPosition().y < -50) {
            continue;
        }
        
        projectileRect = drop->demageBoundingBox();
        
        // Test codes
        /*Sprite* cocos2d::RectSprite = (Sprite*)drop->getUserData();
        if (!rectSprite) {
            cocos2d::RectSprite = Sprite::create("blackSquare.png");
            cocos2d::RectSprite->setAnchorPoint(Point::ZERO);
            tileMap->addChild(rectSprite, 100);
            drop->setUserData(rectSprite);
        }
        cocos2d::RectSprite->setScale(projectileRect.size.width, projectileRect.size.height);
        cocos2d::RectSprite->setPosition(projectileRect.origin);
        
        cocos2d::RectSprite = (Sprite*)player->getUserData();
        if (!rectSprite) {
            cocos2d::RectSprite = Sprite::create("blackSquare.png");
            cocos2d::RectSprite->setAnchorPoint(Point::ZERO);
            tileMap->addChild(rectSprite, 100);
            player->setUserData(rectSprite);
        }
        cocos2d::RectSprite->setScale(player->demageBoundingBox().size.width, player->demageBoundingBox().size.height);
        cocos2d::RectSprite->setPosition(player->demageBoundingBox().origin);
        */
        if (projectileRect.intersectsRect(player->demageBoundingBox())) {
//            log("Got damage %s");
            if (hitDary()) {
                return;
            }
        }
    }
    for (auto missile: enemyMissileArray) {
        projectileRect = cocos2d::Rect(missile->getPosition().x - (missile->getContentSize().width*missile->getScale()/2),
                              missile->getPosition().y - (missile->getContentSize().height*missile->getScale()/2),
                              missile->getContentSize().width*missile->getScale(),
                              missile->getContentSize().height*missile->getScale());
        projectileRect = RectInset(projectileRect, 3, 3);
        if (projectileRect.intersectsRect(player->demageBoundingBox())) {
			if (!player->immortal){
				demagingUnit = missile->secondTag;
				hitDary();
				enemyMissileArray.eraseObject(missile);
			}
            enemyMissileArray.eraseObject(missile);
            break;
        }
    }
}
void HelloWorld::immortalForAWhile(float dt){
    indestructableTime -= dt;
    if (indestructableTime < 0) {
        this->unschedule(schedule_selector(HelloWorld::immortalForAWhile));
    }
}
bool HelloWorld::hitDary(){
    if (indestructableTime <= 0 && !player->immortal) {
        indestructableTime =  1.6f;
        this->schedule(schedule_selector(HelloWorld::immortalForAWhile), 0.1f);
        if (player->shieldCount > 0) {
            player->demageShield();
        }else{
            if (!GameManager::getInstance()->developer) {
                player->energy--;
                GameManager::getInstance()->getHudLayer()->setLife(player->energy);
            }
        }
        
        player->freezed = true;
        this->scheduleOnce(schedule_selector(HelloWorld::stepBackForHit), 0.02f);
        this->scheduleOnce(schedule_selector(HelloWorld::unfreezePlayer), 0.4f);
        
        blinkingTime = 1.6f;
        this->schedule(schedule_selector(HelloWorld::blinkingPlayer), 0.1f);
		player->energy--;
        if (player->energy <= 0) {
            gameOver();
        }
        return true;
    }
    return false;
}

void HelloWorld::stepBackForHit(float dt)
{
    player->velocity = cocos2d::Point(player->isTowardLeft?1000:-1000, player->inWater?50:220);
}

void HelloWorld::unfreezePlayer(float dt)
{
    player->freezed = false;
}

cocos2d::Rect HelloWorld::RectInset(cocos2d::Rect rect, float x, float y)
{
    return cocos2d::Rect(rect.origin.x + x, rect.origin.y + y, rect.size.width - x*2, rect.size.height - y*2);
}
void HelloWorld::continueGame(){
    UserDefault::getInstance()->setBoolForKey(KEY_CONTINUED_GAME, true);
    if(_messageBox != NULL){
        _messageBox->removeFromParentAndCleanup(true);
        _messageBox = NULL;
    }
    
    _messageBox = CSLoader::createNode("MessageBoxScene.csb");
    Node* panel = _messageBox->getChildByName("panel");
    _hudLayer->addChild(_messageBox, 1000);
    
    Button* btn = (Button*)panel->getChildByName("close");
    btn->addClickEventListener(CC_CALLBACK_0(HelloWorld::onContinueGameClosed, this));
    btn = (Button*)panel->getChildByName("ok");
    btn->addClickEventListener(CC_CALLBACK_0(HelloWorld::onContinueGameClosed, this));
    Text* lbl = (Text*)_messageBox->getChildByName("label");
    lbl->setFontName(LanguageManager::getInstance()->getStringForKey(STR_font));
    lbl->setString(LanguageManager::getInstance()->getStringForKey(STR_next_gen).c_str());
    
    pauseLayer();
    
    GameManager::getInstance()->currentScene = SCENE_GAME_TUTORIAL;
//    _waveCount++;
}
void HelloWorld::onContinueGameClosed(){
    _messageBox->removeFromParentAndCleanup(true);
    _messageBox = NULL;
    GameManager::getInstance()->currentScene = SCENE_IN_GAME;
    resumeLayer();
    isGameOver = false;
    startWaveSchedule();
}
void HelloWorld::gameOver()
{
    sdkbox::PluginGoogleAnalytics::logEvent("Game Over", cocos2d::Value(_waveCount).asString(), "", 1);
    log("game over");
    pauseLayer();
    deadPoint = player->getPosition();
    isGameOver = true;
    GameManager::getInstance()->isInMiddleOfGame = false;
	this->unschedule(schedule_selector(HelloWorld::gravityUpdate));

    for (auto drop: enemyMissileArray) {
        drop->removeFromParentAndCleanup(true);
    }
    enemyMissileArray.clear();
    
    player->stopFootStep();
    GameManager::getInstance()->playSoundEffect(SOUND_GAMEOVER);
    
    log("game over 1");
    int totalCoinSpent = UserDefault::getInstance()->getIntegerForKey(KEY_TOTAL_COIN_SPENT, 0);
    totalCoinSpent += coinSpent;
    UserDefault::getInstance()->setIntegerForKey(KEY_TOTAL_COIN_SPENT, totalCoinSpent);
    
	UserDefault::getInstance()->setIntegerForKey(KEY_COIN_LEFT, CollectedCoinCount);
	
    saveTotalEnemyKill();

    
    log("game over 4");
	GameManager::getInstance()->getHudLayer()->ShowGameOverLayer(_stageNumber + 1, _waveCount - 1, _earnedCoin, (_initCoinCount + _earnedCoin) - CollectedCoinCount, enemyKilledCount, _totalTime);
    
    log("game over 5");
    
    bool gameoverTutorial = UserDefault::getInstance()->getBoolForKey(KEY_TUTORIAL_GAMEOVER, false);
    if(!gameoverTutorial){
        UserDefault::getInstance()->setBoolForKey(KEY_TUTORIAL_GAMEOVER, true);
        showTutorial(LanguageManager::getInstance()->getStringForKey(STR_tutorialGameOver).c_str());
        pauseLayer();
        GameManager::getInstance()->currentScene = SCENE_GAME_TUTORIAL;
    }else{
        if (GameManager::getInstance()->market == MARKET_PLAYSTORE_FREE &&
            !UserDefault::getInstance()->getBoolForKey(KEY_PAID_USER, false)) {
            NativeInterface::NativeInterface::showAds();
        }
    }
    
    log("game over 6");
    UserDefault::getInstance()->flush();
}
void HelloWorld::saveTotalEnemyKill(){
    int enemyKill = UserDefault::getInstance()->getIntegerForKey(KEY_TOTAL_ENEMY_KILL, 0);
    enemyKill += enemyKilledCount;
    UserDefault::getInstance()->setIntegerForKey(KEY_TOTAL_ENEMY_KILL, enemyKill);
    
    char buf[30];
    sprintf(buf, KEY_ACHIEVEMENT_PROGRESS_FORMAT, 3);
    UserDefault::getInstance()->setIntegerForKey(buf, enemyKill);
    
    log("game over 2");
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    NativeInterface::NativeInterface::postScore(enemyKill, "platform_defense_total_enemy_kill");
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    NativeInterface::NativeInterface::postScore(enemyKill, "CgkIut_jhL8VEAIQAQ");
#endif
    
    log("game over 3");
    
    if(GameManager::getInstance()->market == MARKET_APPSTORE_PAID){
        float percent = enemyKill*100.0f/getAchievementTotal(3);
        NativeInterface::NativeInterface::sendAchievement("pd_achievement_3", percent);
    }else  {
        NativeInterface::NativeInterface::sendAchievement("CgkIut_jhL8VEAIQDQ", enemyKill);
    }
}
int HelloWorld::getAchievementTotal(int i){
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

void HelloWorld::missileUpdate(float dt)
{
    if (isGameOver) {
        return;
    }

    cocos2d::Rect mRect;
    enemiesToRemove.clear();

    Vector<Droppable*> missileToRemove;
    EnemyBase* drop;
    for (auto missile: heroMissileArray) {
        mRect = cocos2d::Rect(missile->getPosition().x - (missile->getContentSize().width*missile->getScaleX()/2),
                            missile->getPosition().y - (missile->getContentSize().height*missile->getScaleY()/2),
                            missile->getContentSize().width*missile->getScaleX(),
                            missile->getContentSize().height*missile->getScaleY());
		if (mRect.size.width < 0){
			mRect.size.width *= -1;
			mRect.origin.x -= mRect.size.width;
		}
        for (int i = 0; i<enemyArray.size(); i++) {
            drop = enemyArray.at(i);
            
            if (drop->untouchable ||
                drop->getPosition().y-missile->getPosition().y > 50 ||
                drop->getPosition().y - missile->getPosition().y < -50) {
                continue;
            }
            
            if (missile->secondTag == TRAP_CACTUS && drop->ability != SPECIAL_FLY) {
                continue;
            }
            
            if (drop->demageBoundingBox().intersectsRect(mRect)) {
                cocos2d::Point pos = drop->getPosition();
                if(!drop->untouchable){
					if (!drop->immortal && drop->energy > 0) {
                        float demage = missile->energy;// + missile->energy*extraPower/100.0f;
                      
                        bool hitBack = true;
//                        bool getDemage = true;
                        if(missile->secondTag == TRAP_ICEBALL || missile->secondTag == MISSILE_AOKIZI || drop->getTag() > 24){
                            hitBack = false;
                        }
						if ((missile->secondTag == TRAP_ICEBALL ||
                             missile->secondTag == MISSILE_AOKIZI) && drop->ability == SPECIAL_ICE){
							demage = 0;
//                            getDemage = false;
						}
                        if (missile->secondTag == MISSILE_HERMIONE && drop->ability == SPECIAL_FIRE){
                            demage = 0;
//                            getDemage = false;
                        }
                        
						if (!HitEnemy(drop, demage, 10, hitBack)){
                            if (drop->ability != SPECIAL_ICE && drop->ability != SPECIAL_BOSS) {
                                if (missile->secondTag == TRAP_ICEBALL || missile->secondTag == MISSILE_AOKIZI){
                                    if (drop->ability == SPECIAL_FAST || drop->ability == SPECIAL_SLIM ||
                                        drop->ability == SPECIAL_FAT){
                                        drop->FreezeTimeLeft = 0.6f;
                                    }
                                    else{
                                        drop->FreezeTimeLeft = 1.0f;
                                    }
                                    
                                }
                            }
							
                            if (missile->secondTag == MISSILE_HERMIONE){
                                if(drop->ability != SPECIAL_FIRE){
                                    if(drop->FireDemage < demage){
                                        drop->FireDemage = demage;
                                    }
                                }
                            }
						}
					}
					
					missileToRemove.pushBack(missile);
					this->runEffect(EFFECT_EXPLODE_MIDDLE, pos);
					//GameManager::getInstance()->playSoundEffect(SOUND_MACHINE_GUN_2);
					break;
                }
            }
        }
    }


    for (auto spt: missileToRemove){
        spt->stopAllActions();
        heroMissileArray.eraseObject(spt);
        if(spt->getTag() == UNIT_MISSILE_CHASING){
            chasingMissileArray.eraseObject(spt);
        }else if(spt->getTag() == UNIT_MISSILE_DROPPABLE){
            droppableMissileArray.eraseObject(spt);
        }else if(spt->getTag() == UNIT_MISSILE_CUSTOM){
            customMovingArray.eraseObject(spt);
        }else if(spt->getTag() == UNIT_MISSILE_STRAIGHT){
            straightMovingArray.eraseObject(spt);
        }
        spt->removeFromParentAndCleanup(true);
    }
	for (auto spt : enemiesToRemove){
		spt->stopAllActions();
		spt->unscheduleAllCallbacks();
		enemyArray.eraseObject(spt);
		spt->removeFromParentAndCleanup(true);
	}
	enemiesToRemove.clear();


	// trap
	Trap* trap;
	for (int i = 0; i < trapArray.size(); i++) {
		trapArray.at(i)->Update(dt);
		trap = trapArray.at(i);
		if (trap->TrapType == TRAP_FREEZER){
			if (trap->CooltimeLeft <= 0){
				trap->OnFire();
			}
		}
		else if (trap->TrapType == TRAP_HAND){
			if (trap->CooltimeLeft <= 0){
				trap->OnFire();
			}
		}
		else if (trap->TrapType == TRAP_WALLSPIKE){
			if (trap->CooltimeLeft <= 0){
				trap->OnFire();
			}
		}
		else if (trap->TrapType == TRAP_BOMB){
			if (trap->EffectTimeLeft <= 0){
				RemoveTrapRecord(trap->TilePos);
				splashDemage(trap->getPosition(), 40, trap->GetPower());
				runEffect(EFFECT_EXPLODE_HUGE, trap->getPosition());
				trapArray.eraseObject(trap);
				trap->removeFromParentAndCleanup(true);
			}
		}
		if (trap->TrapType == TRAP_TOPSPIKE){
			if (trap->IsReady){
				trap->OnFire();
				Missile* spike = Missile::createMissile(UNIT_MISSILE_DROPPABLE, trap->GetPower(), 0, 0, "topSpike.png");
				spike->isFregile = true;
				heroMissileArray.pushBack(spike);
				spike->setPosition(trap->getPosition());
				spriteBatch->addChild(spike);
				droppableMissileArray.pushBack(spike);
			}
		}
		if (trap->TrapType == TRAP_ICEBALL){
			if (trap->IsReady && enemyArray.size() > 0){
				trap->OnFire();
				float angle = GameManager::getInstance()->getAngle(trap->getPosition(), enemyArray.at(0)->getPosition()) - 180;
				Missile* missile = Missile::createMissile(UNIT_MISSILE_STRAIGHT, trap->GetPower(), angle, 90, "iceBall.png");
				missile->secondTag = TRAP_ICEBALL;
				missile->setPosition(trap->getPosition());
				spriteBatch->addChild(missile);
				heroMissileArray.pushBack(missile);
				straightMovingArray.pushBack(missile);
			}
        }
        if (trap->TrapType == TRAP_CACTUS){
            if (trap->IsReady){// && enemyArray.size() > 0){
                trap->OnFire();
                float angle = 110 - cocos2d::random()%40;//GameManager::getInstance()->getAngle(trap->getPosition(), enemyArray.at(0)->getPosition()) - 180;
                Missile* missile = Missile::createMissile(UNIT_MISSILE_STRAIGHT, trap->GetPower(), angle, 90, "heroWeapon16_1.png");
                missile->setColor(Color3B(100,255,255));
                missile->secondTag = TRAP_CACTUS;
                missile->setPosition(trap->getPosition() + Point(0, 7));
                spriteBatch->addChild(missile);
                heroMissileArray.pushBack(missile);
                straightMovingArray.pushBack(missile);
            }
        }
	}

	Rect projectileRect;
	EnemyBase* enemy;
	int demage; 
	for (int j = 0; j < enemyArray.size(); j++){
		enemy = enemyArray.at(j);
		
		// trap check
		for (int i = 0; i<trapArray.size(); i++) {
			trap = trapArray.at(i);
            
            if (enemy->ability == SPECIAL_FLY){
                if(trap->TrapType != TRAP_WALL){
                    continue;
                }
            }
            
			if (!trap->IsReady) {
				continue;
			}
		
			if (trap->GetTargetRect().intersectsRect(enemy->demageBoundingBox())){
				if (trap->TrapType == TRAP_GASBURNER){
					demage = trap->GetPower();
					if (enemy->ability == SPECIAL_FAST || enemy->ability == SPECIAL_SLIM){
						demage /= 4;
						if (demage == 0) demage = 1;
					}
					else if (enemy->ability == SPECIAL_FIRE){
						demage = 0;
					}
                    if(enemy->FireDemage < demage){
                        enemy->FireDemage = demage;
                    }
				}
				else if (trap->TrapType == TRAP_FREEZER)
				{
					if (trap->EffectTimeLeft > 0 && !enemy->freezed){
						demage = trap->GetPower();
						if (enemy->ability == SPECIAL_ICE){
							demage = 0;
						}else if (enemy->ability == SPECIAL_FAST || enemy->ability == SPECIAL_SLIM){
							enemy->FreezeTimeLeft = 0.6f;
						}
						else if (enemy->ability == SPECIAL_FAT || enemy->ability == SPECIAL_BOSS){
                            demage /= 4;
                            if (demage == 0) demage = 1;
                            enemy->FreezeTimeLeft = 0.6f;
						}
						else{
							enemy->FreezeTimeLeft = 1.4f;
						}

                        if(demage > 0){
                            HitEnemy(enemy, demage, 0, false);
                        }
						enemy->UpdateSideEffect();
					}
				}
				else if (trap->TrapType == TRAP_HAND)
				{
					if (trap->EffectTimeLeft > 0 && enemy->HandTimeLeft <= 0){
						HitEnemy(enemy, trap->GetPower(), 10, true);
						Point tilePos = this->tileCoordForPosition(enemy->getPosition());
                        if (!(enemy->ability == SPECIAL_FAT || enemy->getTag() > 24) || _waveCount > 100) {
                            enemy->oneWayCancelY = tilePos.y + 1;
                            enemy->runAction(Sequence::create(ScaleTo::create(0.2f, enemy->getScaleX()*1.7f, enemy->getScaleY()*1.7f), ScaleTo::create(0.2f, enemy->getScaleX(), enemy->getScaleY()), NULL));
                        }
						
						enemy->HandTimeLeft = 0.7f;
					}
				}
				else if (trap->TrapType == TRAP_FIRE)
				{
					demage = trap->GetPower();
					if (enemy->ability == SPECIAL_FAST || enemy->ability == SPECIAL_SLIM){
						demage /= 4;
						if (demage == 0) demage = 1;
					}
					else if (enemy->ability == SPECIAL_FIRE){
						demage = 0;
                    }
                    if(enemy->FireDemage < demage){
                        enemy->FireDemage = demage;
                    }
				}
				else if (trap->TrapType == TRAP_SPIKE)
				{
					if (enemy->SpikeTimeLeft <= 0){
						enemy->SpikeTimeLeft = 1;
						demage = trap->GetPower();
						if (enemy->ability == SPECIAL_FAST || enemy->ability == SPECIAL_SLIM){
							demage /= 4;
							if (demage == 0) demage = 1;
						}
						else if (enemy->ability == SPECIAL_HARD){
							demage = 0;
						}
						HitEnemy(enemy, demage, 0, true);
					}
				}
				else if (trap->TrapType == TRAP_BOMB){
					if (trap->EffectTimeLeft > trap->EffectTime)
						trap->OnFire();
				}
				else if (trap->TrapType == TRAP_MACEBALL)
				{
					if (trap->IsRolling){
						if (enemy->PressedTimeLeft <= 0){
							HitEnemy(enemy, trap->GetPower(), 0, true);
							enemy->PressedTimeLeft = 3.0f;
							enemy->UpdateSideEffect();
						}
					}
					else{
						RemoveTrapRecord(trap->TilePos);
						trap->extraSpeed = 0;
						trap->setTag(UNIT_MISSILE_DROPPABLE);
						trap->secondTag = TRAP_MACEBALL;
						trap->IsRolling = true;
						droppableMissileArray.pushBack(trap);

						trap->wantsToGoLeft = trap->getPosition().x > enemy->getPosition().x;
						trap->wantsToGoRight = !trap->wantsToGoLeft;
					}
				}
				else if (trap->TrapType == TRAP_WALL)
				{
					if (enemy->WallLeftTime <= 0){
						HitEnemy(enemy, trap->GetPower(), 0, true);
						enemy->WallLeftTime = 0.7f;
                        if(enemy->ability == SPECIAL_BOSS){
                            trap->energy -= 10;
                        }else{
                            trap->energy--;
                        }
						trap->showEnergy();
						if (trap->energy <= 0){
							trapArray.eraseObject(trap);
							runEffect(EFFECT_EXPLODE_MIDDLE, trap->getPosition());
							trap->removeFromParentAndCleanup(true);
							RemoveTrapRecord(trap->TilePos);
						}
					}
					
					enemy->velocity.x *= -100;
				}
				else if (trap->TrapType == TRAP_WALLSPIKE)
				{
					if (enemy->SpikeTimeLeft <= 0 && trap->EffectTimeLeft > 0 ){
						enemy->SpikeTimeLeft = 0.5f;
						HitEnemy(enemy, trap->GetPower(), 0, true);
					}
				}
			}
		}

		// [Side effects]	
		// fire demage
		if (enemy->FireTimeLeft == 0 && enemy->FireDemage > 0){	// first demage
//			enemy->desiredPosition = enemy->desiredPosition + cocos2d::Point(enemy->isTowardLeft ? 2 : -2, 2);
//			enemy->setPosition(enemy->getPosition() + cocos2d::Point(enemy->isTowardLeft ? 2 : -2, 2));
			HitEnemy(enemy, enemy->FireDemage*2, 0, true);
			enemy->FireDemage--;
			enemy->FireTimeLeft = 1.0f;
			enemy->UpdateSideEffect();
		}
		if (enemy->FireDemage > 0 && enemy->FireTimeLeft > 0){
			enemy->FireTimeLeft -= dt;
			if (enemy->FireTimeLeft < 0){
				enemy->FireTimeLeft = 1.0f;
				HitEnemy(enemy, enemy->FireDemage, 0, false);
				enemy->FireDemage--;
				enemy->UpdateSideEffect();
			}
		}
		// freeze time
		if (enemy->FreezeTimeLeft > 0){
			if (enemy->freezed){
				enemy->FreezeTimeLeft -= dt;
				if (enemy->FreezeTimeLeft <= 0){
					enemy->freezed = false;
					enemy->UpdateSideEffect();
				}
			}
			else{
				enemy->freezed = true;
				enemy->UpdateSideEffect();
			}
		}
		// spike time
		if (enemy->SpikeTimeLeft > 0){
			enemy->SpikeTimeLeft -= dt;
		}
		// hand time
		if (enemy->HandTimeLeft > 0){
			enemy->HandTimeLeft -= dt;
		}
		// pressed time
		if (enemy->PressedTimeLeft > 0){
			enemy->PressedTimeLeft -= dt;
			enemy->UpdateSideEffect();
		}
		// wall time
		if (enemy->WallLeftTime > 0){
			enemy->WallLeftTime -= dt;
		}
	}
}
void HelloWorld::RemoveTrapRecord(Point tilePos){
	char buf[50];
	sprintf(buf, KEY_INSTALLED_TRAP_FORMAT, _stageMode, GetCoordinateKey(tilePos).c_str());
	UserDefault::getInstance()->setIntegerForKey(buf, -1);

	sprintf(buf, KEY_INSTALLED_TRAP_LEVEL_FORMAT, _stageMode, GetCoordinateKey(tilePos).c_str());
	UserDefault::getInstance()->setIntegerForKey(buf, 0);
}
Node* HelloWorld::CreateNumberSprite(int number)
{
//	int place = 1;
	int digit = 0;
	Vector<Sprite*> sprites;
	if (number < 0) return NULL;
	char buf[6];
	while (true){
		digit = number % 10;
		sprintf(buf, "%d.png", digit);
		Sprite* sptNumber = Sprite::createWithSpriteFrameName(buf);
		sprites.insert(0, sptNumber);
		number /= 10;
		if (0 >= number){
			break;
		}
	}
	Node* result = Node::create();
	int width = 0;
	for (auto sprite : sprites){
		result->addChild(sprite);
		width += (sprite->getContentSize().width -  1);
	}
	int startWidth = -width / 2;
//	int index = 0;
	for (auto sprite : sprites){
		sprite->setPosition(Point(startWidth, 0));
		startWidth += (sprite->getContentSize().width - 1);
	}
	sprites.clear();
	return result;
}
bool HelloWorld::HitEnemy(EnemyBase* enemy, int demage, int criticalPercent, bool bounceBack){
	EnemyBase* enemyToRemove = NULL;
	if (cocos2d::random() % 100 < criticalPercent) {//critical
		demage *= 2;
		Sprite* critical = Sprite::createWithSpriteFrameName("critical.png");
		spriteBatch->addChild(critical, 100);
		critical->getTexture()->setAliasTexParameters();
		//                            critical->setColor(aColor3B(0,0,0));
		critical->setScale(0.5f);
		critical->setPosition(enemy->getPosition() + cocos2d::Point(0, 15));
		critical->runAction(EaseIn::create(MoveBy::create(1, cocos2d::Point(0, 25)), 0.3));
		critical->runAction(Sequence::create(DelayTime::create(0.7), FadeOut::create(0.3), CallFuncN::create(CC_CALLBACK_1(HelloWorld::spriteMoveDone, this)), NULL));

		shakeScreen();
	}

	if (demage >= 10) {
		demage = demage*0.8f + (cocos2d::random() % ((int)(demage*0.4f)));
	}

	enemy->energy -= (int)demage;
	enemy->showEnergy();
	totalPower += demage;
	if (enemy->ability == SPECIAL_HEALTHY || enemy->ability == SPECIAL_FAT || enemy->getTag() > 25){
		bounceBack = false;

	}
	if (enemy->onGround && bounceBack && droppableArray.find(enemy) != droppableArray.end()){
		enemy->desiredPosition = enemy->desiredPosition + cocos2d::Point(enemy->isTowardLeft?2:-2, 2);
		enemy->setPosition(enemy->getPosition() + cocos2d::Point(enemy->isTowardLeft ? 2 : -2, 2));
	}

    if(cocos2d::random()%2 == 0){
        GameManager::getInstance()->playSoundEffect(SOUND_MACHINE_GUN_1);
    }else{
        GameManager::getInstance()->playSoundEffect(SOUND_MACHINE_GUN_2);
    }
    
	Node* sptNumber = CreateNumberSprite(demage);
	tileMap->addChild(sptNumber, 100);
	sptNumber->setScale(0.5);

	sptNumber->setPosition(enemy->getPosition());
	sptNumber->runAction(EaseIn::create(MoveBy::create(1, cocos2d::Point(0, 25)), 0.3));
	sptNumber->runAction(Sequence::create(DelayTime::create(0.7), FadeOut::create(0.3), CallFuncN::create(CC_CALLBACK_1(HelloWorld::enemyBaseMoveDone, this)), NULL));
	if (enemy->energy <= 0) {
		killedEnemyCount++;
		if (killedEnemyCount % 20 == 0){
            GameManager::getInstance()->getHudLayer()->addStar();
		}

		enemyToRemove = enemy;
		this->runEffect(EFFECT_EXPLODE_BIG, enemy->getPosition());

		this->createRewards(enemy);
		enemyKilledCount++;

		// add coin
		int coin = 2;
        if (GameManager::getInstance()->market == MARKET_PLAYSTORE_FREE &&
            !UserDefault::getInstance()->getBoolForKey(KEY_PAID_USER, false)) {
            coin = 1;
        }
        
		_earnedCoin += coin;
		CollectedCoinCount += coin;
		GameManager::getInstance()->getHudLayer()->LblCoinCount->setString(cocos2d::Value(CollectedCoinCount).asString());
        
        // check tutorial
        bool trapTutorial = UserDefault::getInstance()->getBoolForKey(KEY_TUTORIAL_TRAP, false);
        if(CollectedCoinCount >=  GameManager::getInstance()->GetTrapPrice(0)&& !trapTutorial){
            UserDefault::getInstance()->setBoolForKey(KEY_TUTORIAL_TRAP, true);
            showTutorial(LanguageManager::getInstance()->getStringForKey(STR_tutorialTrap).c_str());
            _trapTurorial = true;
            _hudLayer->ShowTutorialHelper(0, cocos2d::Point::ZERO);
            pauseLayer();
            GameManager::getInstance()->currentScene = SCENE_GAME_TUTORIAL;
        }
        bool upgradeTutorial = UserDefault::getInstance()->getBoolForKey(KEY_TUTORIAL_UPGRADE, false);
        if(CollectedCoinCount >= 70 && !upgradeTutorial && trapArray.size() > 0 && trapArray.at(0)->TrapType == TRAP_SPIKE){
            UserDefault::getInstance()->setBoolForKey(KEY_TUTORIAL_UPGRADE, true);
            showTutorial(LanguageManager::getInstance()->getStringForKey(STR_tutorialUpgrade).c_str());
            pauseLayer();
            
            cocos2d::Point pos = trapArray.at(0)->TilePos;
            float cX = (pos.x*TILE_SIZE + TILE_SIZE / 2)*this->getScale() + this->getPosition().x;
            float cY = ((tileMap->getMapSize().height - pos.y - 1)*TILE_SIZE + TILE_SIZE / 2)*this->getScale() + this->getPosition().y;
            
            _hudLayer->ShowTutorialHelper(1, cocos2d::Point(cX, cY - this->getScale()));
            GameManager::getInstance()->currentScene = SCENE_GAME_TUTORIAL;
        }

		Droppable* sptCoin = Droppable::createDroppable(0, 0, 0, "coin0.png");
		sptCoin->runAnimation("coin", true);
		sptCoin->setPosition(enemy->getPosition());
		spriteBatch->addChild(sptCoin);
		sptCoin->runAction(Sequence::create(JumpBy::create(0.6f, cocos2d::Point(0, 0), 24, 1), CallFuncN::create(CC_CALLBACK_1(HelloWorld::spriteMoveDone, this)), NULL));
		GameManager::getInstance()->playSoundEffect(SOUND_COIN);
	}
	else
	{
		enemy->blinkForAWhile();
	}

	if (enemyToRemove != NULL){
		droppableArray.eraseObject(enemyToRemove);
		enemyToRemove->stopAllActions();
		enemyToRemove->unscheduleAllCallbacks();
		enemyArray.eraseObject(enemyToRemove);
		enemyToRemove->removeFromParentAndCleanup(true);
		return true;
	}
	return false;
}
void HelloWorld::shakeScreen(){
	tileMap->stopAllActions();
    tileMap->runAction(Sequence::create(MoveTo::create(0.05, cocos2d::Point(2,2)), MoveTo::create(0.05, cocos2d::Point(-2,0)), MoveTo::create(0.05, cocos2d::Point(0,-2)), NULL));
    this->scheduleOnce(schedule_selector(HelloWorld::shakeScreenEnd), 0.17);
}
void HelloWorld::shakeScreenEnd(float dt){
    tileMap->runAction(MoveTo::create(0.05, cocos2d::Point(0,0)));
}
void HelloWorld::removeEnemy(EnemyBase* spt){
    Droppable* drop;
    for (int i = 0; i < chasingMissileArray.size(); i++){
        drop = (Droppable*)chasingMissileArray.at(i);
        if (drop->target == spt) {
            drop->target = NULL;
        }
    }
    
	spt->stopAllActions();
	spt->unscheduleAllCallbacks();
    enemyArray.eraseObject(spt);
    droppableArray.eraseObject(spt);
    flyingSwimingEnemyArray.eraseObject(spt);
    chasingMissileArray.eraseObject(spt);
    inWaterArray.eraseObject(spt);
    if(fireableArray.find((FireableBase*)spt) == fireableArray.end()){
        fireableArray.eraseObject((FireableBase*)spt);
    }
    
    spiderArray.eraseObject(spt);
    spt->removeFromParentAndCleanup(true);
}

void HelloWorld::createRewards(EnemyBase* enemy){
    if (enemy->rewards <= 0 || enemy->rewards > 100000) {
        return;
    }
    Droppable* coin;
    for (int i = 0; i < enemy->rewards; i++) {
        /*if(cocos2d::random()%101 < 3 + extraFruit){
            const char* name;
            int food = cocos2d::random()%12;
            switch (food) {
                case FOOD_APPLE:
                    name = "apple.png";
                    break;
                case FOOD_GREEN_APPLE :
                    name = "greemApple.png";
                    break;
                case FOOD_PINE_APPLE :
                    name = "pineApple.png";
                    break;
                case FOOD_PEAR :
                    name = "pear.png";
                    break;
                case FOOD_BANANA :
                    name = "banana.png";
                    break;
                case FOOD_STRAWBERRY :
                    name = "strawberry.png";
                    break;
                case FOOD_CHERRY :
                    name = "cherry.png";
                    break;
                case FOOD_CARROT :
                    name = "carrot.png";
                    break;
                case FOOD_MUSHROOM :
                    name = "mushroom.png";
                    break;
                case FOOD_CORN :
                    name = "corn.png";
                    break;
                case FOOD_WATERMELON :
                    name = "waterMelon.png";
                    break;
                case FOOD_FISH :
                    name = "fish.png";
                    break;
            }
            coin = Droppable::createDroppable(UNIT_FOOD, 1, 0, name);
            coin->secondTag = food;
        }else if(cocos2d::random()%101 < enemy->goldPossiblity){
            coin = Droppable::createDroppable(UNIT_GOLD_COIN, 1, 0, "goldCoin0.png");
           // coin->runAnimation("goldCoin", true);
        }else*/{
            coin = Droppable::createDroppable(UNIT_COIN, 1, 0, "coin0.png");
            coin->runAnimation("coin", true);
        }
        coin->velocity = cocos2d::Point(400 - cocos2d::random()%800, 50 + cocos2d::random()%200);
        coinArray.pushBack(coin);
        spriteBatch->addChild(coin);
        droppableCoinArray.pushBack(coin);
        coin->setPosition(enemy->getPosition());
        coin->runAction(Sequence::create(DelayTime::create(8), FadeOut::create(1), CallFuncN::create(CC_CALLBACK_1(HelloWorld::coinWaitDone, this)), NULL));
    }
}

void HelloWorld::coinUpdate(float dt){
    if (isGameOver) {
        return;
    }

    cocos2d::Rect projectileRect;

    coinToRemove.clear();
    cocos2d::Point thisPos, spritePos;
    
    for (auto drop: coinArray){
        
        if (drop->getPosition().y-player->getPosition().y > 50 || drop->getPosition().y - player->getPosition().y < -50) {
            continue;
        }
        
//        projectileRect = cocos2d::Rect(drop->getPosition().x - (drop->getContentSize().width*drop->getScaleX()/2),
//                                    drop->getPosition().y - (drop->getContentSize().height*drop->getScaleY()/2),
//                                    drop->getContentSize().width*drop->getScaleX(),
//                                    drop->getContentSize().height*drop->getScaleY());
        
        if (drop->getBoundingBox().intersectsRect(player->collectBoundingBox())) {
            switch (drop->getTag()) {
                case UNIT_COIN:
					CollectedCoinCount += 1;
                    if(droppableCoinArray.getIndex(drop) >= 0)
                        droppableCoinArray.eraseObject(drop);
                    showCoinCount(drop->getPosition(), 1);
                    break;
               
                default:
                    break;
            }
            
            GameManager::getInstance()->playSoundEffect(SOUND_COIN);
            this->runEffect(EFFECT_TWINKLE, drop->getPosition());
            coinToRemove.pushBack(drop);
        }
    }
    

    for (auto drop: coinToRemove){
        drop->stopAllActions();
        coinArray.eraseObject(drop);
        spriteBatch->removeChild(drop, true);
        starArray.eraseObject(drop);
    }
    
//    coinToRemove.clear();


    
}

void HelloWorld::teleportLaterForThemeThree(float dt){
    isTeleporting = false;
    playerIgnoreGravity = false;
    player->setScale(1);
    player->stopAllActions();
    player->setRotation(0);
    player->onGround = false;
    player->velocity = cocos2d::Point::ZERO;
    player->desiredPosition = player->getPosition();
    spriteBatch->setLocalZOrder(stageLayer->getLocalZOrder() + 1);
}
void HelloWorld::teleportLater(float dt){
    player->setAnchorPoint(Point(0.5, 0.5));
    if(GameManager::getInstance()->theme == 3){
        player->setPosition(positionToTeleport);
        
        cocos2d::Point plPos = this->tileCoordForPosition(player->getPosition() + cocos2d::Point(0, TILE_SIZE*2));
        int tgid = stageLayer->getTileGIDAt(plPos);
        bool isTopEmpty = false;
        if(tgid > 0){
			cocos2d::Value property = tileMap->getPropertiesForGID(tgid);
			if (property.getType() != cocos2d::Value::Type::NONE && property.asValueMap().at("Type").asString().compare("OneWay") != 0){

            }else{
                isTopEmpty = true;
            }
        }else{
            isTopEmpty = true;
        }
        
        float dur = 1.0f;
        int dist = 30;
        player->runAction(MoveBy::create(dur, cocos2d::Point(0, isTopEmpty?dist:-dist)));
        this->scheduleOnce(schedule_selector(HelloWorld::teleportLaterForThemeThree), dur);
    }else{
        playerIgnoreGravity = false;
        player->setScale(1);
        player->stopAllActions();
        player->setPosition(positionToTeleport);
        player->setRotation(0);
        player->onGround = false;
        player->velocity = cocos2d::Point::ZERO;
    }
}
void HelloWorld::showCoinCount(Point pos, int count){
    return;
	Label* lbl = Label::createWithTTF(cocos2d::Value(count).asString(), "bitdust2.ttf", 50);
    tileMap->addChild(lbl, 1000);
    lbl->setScale(0.25);
    lbl->setPosition(pos);
    lbl->setTextColor(Color4B(253,227,12, 255));
    lbl->enableOutline(Color4B(81, 89, 102, 255), 5);
    lbl->runAction(EaseOut::create(MoveBy::create(1, cocos2d::Point(0, 20)), 8));
    lbl->runAction(Sequence::create(DelayTime::create(1), CallFuncN::create(CC_CALLBACK_1(HelloWorld::nodeMoveDone, this)), NULL));
    lbl->getFontAtlas()->setAliasTexParameters();
}
void HelloWorld::jumpBySpring(float dt)
{
    player->velocity.y = 500;
}
void HelloWorld::runEffect(int effect, cocos2d::Point point)
{
    float duration = 0.2;
    float baseDuration = 0.08;
    duration = baseDuration*2 + baseDuration*(cocos2d::random()%3 + 1);
    if (effect == EFFECT_EXPLODE_BIG) {
        int explodeCount = 8;
        int particleCount = 6;
        
        int range = 40;
        float easeRate = 2;
        float scale = 1;
        for (int i = 0; i < explodeCount; i++) {
            Sprite* explode = Sprite::createWithSpriteFrameName("whiteExplode.png");
            spriteBatch->addChild(explode);
            explode->setScale(scale + (cocos2d::random()%4 + 1)*0.1f - 0.2f);
            explode->setColor(Color3B(237, 70, 37));
            explode->runAction(Sequence::create(DelayTime::create(duration*0.8f), TintTo::create(duration*0.4f, 255, 205, 52), TintTo::create(duration/2, 127, 110, 105), NULL));
            explode->runAction(Sequence::create(DelayTime::create(duration*1.3f), ScaleTo::create(duration, 0.1), NULL));
            explode->runAction(Sequence::create(DelayTime::create(duration*2), CallFuncN::create(CC_CALLBACK_1(HelloWorld::spriteMoveDone, this)), NULL));
            float x = cocos2d::random()%(int)range - range/2;
            float y = cocos2d::random()%(int)range - range/2;
            explode->setPosition(point );
            explode->runAction(EaseOut::create(MoveBy::create(duration, cocos2d::Point(x, y)), easeRate));
        }
        for (int i = 0; i < particleCount; i++) {
            Sprite* particle = Sprite::createWithSpriteFrameName("whiteExplode.png");
            spriteBatch->addChild(particle);
            int angle = cocos2d::random()%360;
            float x = cos(angle*3.14/180)*range*1.1;
            float y = sin(angle*3.14/180)*range*1.1;
            particle->setScale(0.2);
            particle->setColor(Color3B(237, 70, 37));
            particle->runAction(FadeOut::create(baseDuration));
            particle->setPosition(point);
            particle->runAction(Sequence::create(EaseOut::create(MoveBy::create(baseDuration*2, cocos2d::Point(x, y)), easeRate), CallFuncN::create(CC_CALLBACK_1(HelloWorld::spriteMoveDone, this)), NULL));
        }
    }else if(effect == EFFECT_EXPLODE_SMALL){
        float scale = 0.5f;
        duration = 0.15;
        Sprite* explode = Sprite::createWithSpriteFrameName("whiteExplode.png");
        spriteBatch->addChild(explode);
        explode->setScale(scale + (cocos2d::random()%4 + 1)*0.1f - 0.2f);
        explode->setColor(Color3B(237, 70, 37));
        explode->runAction(Sequence::create(DelayTime::create(duration*0.8f), TintTo::create(duration*0.4f, 255, 205, 52), TintTo::create(duration/2, 127, 110, 105), NULL));
        explode->runAction(Sequence::create(DelayTime::create(duration*1.3f), ScaleTo::create(duration, 0.1), NULL));
        explode->runAction(Sequence::create(DelayTime::create(duration*2), CallFuncN::create(CC_CALLBACK_1(HelloWorld::spriteMoveDone, this)), NULL));
        explode->setPosition(point);
    }else if(effect == EFFECT_EXPLODE_MIDDLE){
        int particleCount = 3;
        int range = 30;
        float easeRate = 2;
        float scale = 1.2f;

        Sprite* explode = Sprite::createWithSpriteFrameName("whiteExplode.png");
        spriteBatch->addChild(explode);
        explode->setScale(scale + (cocos2d::random()%4 + 1)*0.1f - 0.2f);
        explode->setColor(Color3B(237, 70, 37));
        explode->runAction(Sequence::create(DelayTime::create(duration*0.8f), TintTo::create(duration*0.4f, 255, 205, 52), TintTo::create(duration/2, 127, 110, 105), NULL));
        explode->runAction(Sequence::create(DelayTime::create(duration*1.3f), ScaleTo::create(duration, 0.1), NULL));
        explode->runAction(Sequence::create(DelayTime::create(duration*2), CallFuncN::create(CC_CALLBACK_1(HelloWorld::spriteMoveDone, this)), NULL));
        explode->setPosition(point);
        
        for (int i = 0; i < particleCount; i++) {
            Sprite* particle = Sprite::createWithSpriteFrameName("whiteExplode.png");
            spriteBatch->addChild(particle);
            int angle = cocos2d::random()%360;
            float x = cos(angle*3.14/180)*range*1.1f;
            float y = sin(angle*3.14/180)*range*1.1f;
            particle->setScale(0.3);
            particle->setColor(Color3B(237, 70, 37));
            particle->runAction(FadeOut::create(baseDuration));
            particle->setPosition(point);
            particle->runAction(Sequence::create(EaseOut::create(MoveBy::create(baseDuration, cocos2d::Point(x, y)), easeRate), CallFuncN::create(CC_CALLBACK_1(HelloWorld::spriteMoveDone, this)), NULL));
        }
    }else if(effect == EFFECT_EXPLODE_HUGE){
        Sprite* explode = Sprite::createWithSpriteFrameName("explosion0.png");
        spriteBatch->addChild(explode);
        explode->setPosition(point);
        Animation* animation = AnimationCache::getInstance()->getAnimation("explosion");
        Animate* animate = Animate::create(animation);
        explode->runAction(Sequence::create(animate, CallFuncN::create(CC_CALLBACK_1(HelloWorld::spriteMoveDone, this)), NULL));
    }else if(effect == EFFECT_SMOKE){
        float duration = 0.5;
        Sprite* spt = Sprite::createWithSpriteFrameName("whiteExplode.png");
        spriteBatch->addChild(spt, -1);
        spt->setPosition(point);
        spt->setScale(0.5);
        spt->setRotation(cocos2d::random()%360);
        spt->setColor(Color3B(200,200,200));
        spt->runAction(Sequence::create(EaseIn::create(MoveBy::create(duration, cocos2d::Point(0, 7)), 2), CallFuncN::create(CC_CALLBACK_1(HelloWorld::enemyBaseMoveDone, this)), NULL));
        spt->runAction(EaseOut::create(MoveBy::create(duration, cocos2d::Point(player->isTowardLeft?-8:8, 0)), 4));
        spt->runAction(EaseIn::create(FadeOut::create(duration), 2));
        
        spt->runAction(RotateBy::create(duration, 160));
    }else if(effect == EFFECT_GREEN_SMOKE){
        float duration = 0.5;
        Sprite* spt = Sprite::createWithSpriteFrameName("whiteExplode.png");
        spriteBatch->addChild(spt, -1);
        spt->setPosition(point);
        spt->setRotation(cocos2d::random()%360);
        spt->setScale(0.3);
        spt->setColor(Color3B(130,250,130));
        spt->runAction(Sequence::create(EaseIn::create(ScaleTo::create(duration, 0.05), 2), CallFuncN::create(CC_CALLBACK_1(HelloWorld::enemyBaseMoveDone, this)), NULL));
//        spt->runAction(EaseOut::create(MoveBy::create(duration, cocos2d::Point(player->isTowardLeft?-8:8, 0)), 4));
        spt->runAction(EaseIn::create(FadeOut::create(duration), 2));
//        spt->runAction(RotateBy::create(duration, 160));
    }else if(effect == EFFECT_RED_SMOKE){
        float duration = 0.5;
        Sprite* spt = Sprite::createWithSpriteFrameName("whiteExplode.png");
        spriteBatch->addChild(spt, -1);
        spt->setPosition(point);
        spt->setRotation(cocos2d::random()%360);
        spt->setScale(0.5);
        spt->setColor(Color3B(237, 70, 37));
        spt->runAction(Sequence::create(EaseIn::create(MoveBy::create(duration, cocos2d::Point(0, 7)), 2), CallFuncN::create(CC_CALLBACK_1(HelloWorld::enemyBaseMoveDone, this)), NULL));
        spt->runAction(EaseOut::create(MoveBy::create(duration, cocos2d::Point(player->isTowardLeft?-8:8, 0)), 4));
        spt->runAction(EaseIn::create(FadeOut::create(duration), 2));
        spt->runAction(RotateBy::create(duration, 160));
    }else if(effect == EFFECT_TWINKLE){
        int initRange = 10;
        int range = initRange;
        baseDuration = 0.3f;
        float duration = 0;
        for (int i = 0; i < 15; i++) {
            Sprite* particle = Sprite::createWithSpriteFrameName("particle.png");
            spriteBatch->addChild(particle, player->getLocalZOrder() + 1);
            int angle = cocos2d::random()%360;
            range = initRange + initRange*(cocos2d::random()%50)*0.02;
            float x = cos(angle*3.14/180)*range*1.1f;
            float y = sin(angle*3.14/180)*range*1.1f;
            particle->setScale(0.5 + (cocos2d::random()%10)*0.05);
            particle->setColor(Color3B::YELLOW);
            duration = baseDuration + baseDuration*(cocos2d::random()%10)*0.1;
            particle->runAction(FadeOut::create(duration));
            particle->setPosition(point);
            particle->runAction(Sequence::create(EaseOut::create(MoveBy::create(duration, cocos2d::Point(x, y)), 1.6f), CallFuncN::create(CC_CALLBACK_1(HelloWorld::spriteMoveDone, this)), NULL));
        }
    }
}
/*
void HelloWorld::addGlowEffect(Sprite* sprite,const Color3B& colour, const Size& size)
{
    cocos2d::Point pos = cocos2d::Point(sprite->getContentSize().width / 2,
                      sprite->getContentSize().height / 2);
    
    Sprite* glowSprite = Sprite::createWithSpriteFrameName("lizardStand.png");
    
    glowSprite->setColor(colour);
    glowSprite->setPosition(pos);
    glowSprite->setRotation(sprite->getRotation());
    
    ccBlendFunc f = {GL_ONE, GL_ONE};
    glowSprite->setBlendFunc(f);
    sprite->addChild(glowSprite, -1);
    
    // Run some animation which scales a bit the glow
    ScaleTo* scale1= ScaleTo::create(0.9f, 4);
    ScaleTo* scale2 = ScaleTo::create(0.9f, 3.75f);

    glowSprite->runAction(RepeatForever::create((ActionInterval*)Sequence::create(scale1, scale2, NULL)));
}*/
void HelloWorld::powerTestSchedule(float dt){
    powerTestTime += dt;
    if(powerTestTime > 10){
        powerTestTime = 0;
        totalPower = 0;
    }
    powerPerSec = totalPower/powerTestTime;
    log("power/sec: %f", powerPerSec);
}
void HelloWorld::startWaveSchedule(){
    this->schedule(schedule_selector(HelloWorld::WaveSchedule), 0.5f);
}
void HelloWorld::WaveSchedule(float dt){
    if(_isGamePaused) return;
	if (enemyArray.size() > 0 || !_isEnemyCreated ){
		return;
	}
    
    bool shootTutorial = UserDefault::getInstance()->getBoolForKey(KEY_TUTORIAL_SHOOT, false);
    if(!shootTutorial){
        UserDefault::getInstance()->setBoolForKey(KEY_TUTORIAL_SHOOT, true);
        showTutorial(LanguageManager::getInstance()->getStringForKey(STR_tutorialShoot).c_str());
        _hudLayer->ShowTutorialHelper(1, cocos2d::Point(300, 70));
        _shootTutorialStarted = true;
        pauseLayer();
        GameManager::getInstance()->currentScene = SCENE_GAME_TUTORIAL;
        this->unschedule(schedule_selector(HelloWorld::WaveSchedule));
        return;
    }
    
    if(_waveCount == 101 && !UserDefault::getInstance()->getBoolForKey(KEY_CONTINUED_GAME, false)){
        isGameOver = true;
        GameManager::getInstance()->isInMiddleOfGame = false;
        _hudLayer->ShowGameClearLayer(_stageNumber, 100, 10);
        UserDefault::getInstance()->setIntegerForKey(KEY_CHECK_POINT, 101);
        this->unschedule(schedule_selector(HelloWorld::WaveSchedule));
        return;
    }
    
    if (UserDefault::getInstance()->getBoolForKey(KEY_ANTIAIR_USED, false) == false) {
        char buf[30];
        sprintf(buf, KEY_ACHIEVEMENT_PROGRESS_FORMAT, 1);
        int count = UserDefault::getInstance()->getIntegerForKey(buf, 0);
        if (count < _waveCount) {
            UserDefault::getInstance()->setIntegerForKey(buf, _waveCount);
            
            if(GameManager::getInstance()->market == MARKET_APPSTORE_PAID){
                float percent = _waveCount*100.0f/getAchievementTotal(1);
                NativeInterface::NativeInterface::sendAchievement("pd_achievement_1", percent);
            }else {
                NativeInterface::NativeInterface::sendAchievement("CgkIut_jhL8VEAIQCw", _waveCount);
            }
        }
    }
    if (UserDefault::getInstance()->getBoolForKey(KEY_TRAP_USED, false) == false) {
        char buf[30];
        sprintf(buf, KEY_ACHIEVEMENT_PROGRESS_FORMAT, 2);
        int count = UserDefault::getInstance()->getIntegerForKey(buf, 0);
        if (count < _waveCount) {
            UserDefault::getInstance()->setIntegerForKey(buf, _waveCount);
            
            if(GameManager::getInstance()->market == MARKET_APPSTORE_PAID){
                float percent = _waveCount*100.0f/getAchievementTotal(2);
                NativeInterface::NativeInterface::sendAchievement("pd_achievement_2", percent);
            }else {
                NativeInterface::NativeInterface::sendAchievement("CgkIut_jhL8VEAIQDA", _waveCount);
            }
        }
    }
    const char* leaderboardID = "platform_defense_wave_ranking";;
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
//    leaderboardID = "platform_defense_wave_ranking";
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    leaderboardID = "CgkIut_jhL8VEAIQCQ";
#endif
    NativeInterface::NativeInterface::postScore(_waveCount, leaderboardID);
    
    if (_waveCount >= 1001) {
        if (GameManager::getInstance()->market == MARKET_PLAYSTORE_PAID ||
            GameManager::getInstance()->market == MARKET_PLAYSTORE_FREE) {
            NativeInterface::NativeInterface::unlockAchievement("CgkIut_jhL8VEAIQBA");
        }
        
//        isGameOver = true;
//        GameManager::getInstance()->isInMiddleOfGame = false;
//        _hudLayer->ShowArriveDestination();
//        this->unschedule(schedule_selector(HelloWorld::WaveSchedule));
//        return;
    }
    
    bool checkPoint = ((_waveCount) % _checkPointStageCount == 0);
	if (checkPoint && _waveCount != 100 && _waveCount%100 != 0 ){
		int previousCheckPoint = UserDefault::getInstance()->getIntegerForKey(KEY_CHECK_POINT, 1);
		if (previousCheckPoint < _waveCount){
			UserDefault::getInstance()->setIntegerForKey(KEY_CHECK_POINT, _waveCount);
		}
	}
    if(_waveCount == 1001){
        isGameOver = true;
        GameManager::getInstance()->isInMiddleOfGame = false;
        GameManager::getInstance()->currentScene = SCENE_GAME_ENDING;
        UserDefault::getInstance()->setBoolForKey(KEY_CONTINUE_AVAILABLE, false);
        _hudLayer->showEnding();
    }else if(checkPoint && _waveCount != 100  && _waveCount%100 != 0 ){
        if(_stageNumber == _waveCount - 1){
            GameManager::getInstance()->getHudLayer()->ShowWave(_waveCount, true);
        }else{
            GameManager::getInstance()->getHudLayer()->ShowCheckPoint(_waveCount);
            this->unschedule(schedule_selector(HelloWorld::WaveSchedule));
            return;
        }
    }else{
        GameManager::getInstance()->getHudLayer()->ShowWave(_waveCount, false);
    }
    loadWaveSchedule();
}
void HelloWorld::loadWaveSchedule(){
    _nextEnemyTime = 0;
    int waveTotalCount = 100;
    int totalEnemyRange = 31;
    char buf[20];
    sprintf(buf, "WAVE %d", _waveCount);
    GameManager::getInstance()->getHudLayer()->LblWaveCount->setString(buf);
    
    //	bool isNewMonster = false;
    
    float newMonsterDelayTime = 1.8f;
    if ((_waveCount%100) == 0) {
//        this->unschedule(schedule_selector(HelloWorld::WaveSchedule));
//        int clearCount = UserDefault::getInstance()->getIntegerForKey(KEY_CLEAR_COUNT, 1);
        for (int i = 0; i < 1; i++){
            _enemyToCreate.push_back(32);
        }
        //sprintf(buf, "NewMonster%d", 32);
        if (_lastStrongestEnemy != 32){//UserDefault::getInstance()->getBoolForKey(buf, true)){
            //UserDefault::getInstance()->setBoolForKey(buf, false);
            GameManager::getInstance()->getHudLayer()->ShowNewMonster(32, newMonsterDelayTime);
            _lastStrongestEnemy = 32;
            _nextEnemyTime += 3.0f;
        }
        _waveCount++;
        _isEnemyCreated = false;
        return;
    }
    int startMonster = 0;
    
    int strongestEnemy = (totalEnemyRange - startMonster)*(((_waveCount%100) * 1.0f) / waveTotalCount);
    if (strongestEnemy <= 0) strongestEnemy = 0;
    
    _waveCount++;
    _nextEnemyTime = 2.0f;
    int enemyCountToCreate = ((_waveCount%100) / 10) * 5 + 7;
    //sprintf(buf, "NewMonster%d", strongestEnemy);
    if (_lastStrongestEnemy != strongestEnemy){//UserDefault::getInstance()->getBoolForKey(buf, true)){
        //UserDefault::getInstance()->setBoolForKey(buf, false);
        GameManager::getInstance()->getHudLayer()->ShowNewMonster(strongestEnemy, newMonsterDelayTime);
        _lastStrongestEnemy = strongestEnemy;
        for (int i = 0; i < enemyCountToCreate/2; i++){
            _enemyToCreate.push_back(strongestEnemy);
        }
        _nextEnemyTime += 3.0f;
    }
    
    if (strongestEnemy - 3 > 0){
        startMonster = strongestEnemy - 3;
    }
    for (int i = 0; i < enemyCountToCreate; i++){
        _enemyToCreate.push_back(startMonster + cocos2d::random() % (strongestEnemy - startMonster + 1));
    }
    _isEnemyCreated = false;
}
void HelloWorld::CreatingEnemies(float dt){
    if(_isGamePaused) return;
	if (_enemyToCreate.size() <= 0){
		_isEnemyCreated = true;
		return;
	}
	
	_nextEnemyTime -= dt;
	if (_nextEnemyTime < 0){
        
        if(_waveCount == _checkPointStageCount + 1){
            bool checkpointTutorial = UserDefault::getInstance()->getBoolForKey(KEY_TUTORIAL_CHECKPOINT, false);
            if(!checkpointTutorial){
                UserDefault::getInstance()->setBoolForKey(KEY_TUTORIAL_CHECKPOINT, true);
                showTutorial(LanguageManager::getInstance()->getStringForKey(STR_tutorialCheckpoint).c_str());
                pauseLayer();
                GameManager::getInstance()->currentScene = SCENE_GAME_TUTORIAL;
            }
        }
		_nextEnemyTime = 0.1f + (cocos2d::random() % 5)*0.1f;
	}
	else{
		return;
	}
	int value = _enemyToCreate.at(0);
	int enemyIndex = value;
	_enemyToCreate.erase(_enemyToCreate.begin());
	

	if (enemyIndex > 32){
        if (UserDefault::getInstance()->getBoolForKey(KEY_CONTINUED_GAME, false)) {
            enemyIndex = enemyIndex%32 + 1;
        }else{
            enemyIndex = 32;
        }
		
	}
    
	char buf[16];
	sprintf(buf, "monster%d_0.png", enemyIndex);

	int energy = getEnergyForWave(_waveCount, enemyIndex);
    
    int defaultEnergy = (getEnergyForWave(100, enemyIndex)*0.7f)*((_waveCount - 1)/100);
    energy += defaultEnergy;
    /*
    int absWaveCount = (_waveCount - 1)%100;
    if(absWaveCount == 101){
        energy -= defaultEnergy;
        energy *= 15.5f;
    }else if(absWaveCount%100 > 98){
        energy *= 2.8f;
    }else if(absWaveCount%100 > 96){
        energy *= 2.7f;
    }else if(absWaveCount%100 > 93){
        energy *= 2.6f;
    }else if(absWaveCount%100 > 90){
        energy *= 2.5f;
    }else if(absWaveCount%100 > 80){
        energy *= 2.4f;
    }else if(absWaveCount%100 > 70){
        energy *= 2.3f;
    }else if(absWaveCount%100 > 60){
        energy *= 2.2f;
    }else if(absWaveCount%100 > 50){
        energy *= 1.9f;
    }else if(absWaveCount%100 > 40){
        energy *= 1.6f;
    }else if(absWaveCount%100 > 30){
        energy *= 1.4f;
    }else if(absWaveCount%100 > 20){
        energy *= 1.2f;
    }else if(absWaveCount%100 > 10){
        energy *= 1.1f;
    }*/
//    log("monster wave: %d", absWaveCount%100);
	int extraSpd = 0; // base speed 50

	int ability = GameManager::getInstance()->GetSpecialSkill(enemyIndex);
	switch (ability)
	{
	case SPECIAL_NORMAL:
		break;
	case SPECIAL_SLIM:
		energy *= 0.8f;
		extraSpd = 7;
		break;
	case SPECIAL_HARD:
		energy *= 1.2f;
		extraSpd = 0;
		break;
	case SPECIAL_HEALTHY:
		energy *= 1.5f;
		extraSpd = -5;
		break;
	case SPECIAL_FAT:
		energy *= 2.5f;
		extraSpd = -8;
		break;
	case SPECIAL_FIRE:
		energy *= 1.3f;
		extraSpd = 3;
		break;
	case SPECIAL_FAST:
		energy *= 0.4f;
		extraSpd = 23;
		break;
	case SPECIAL_ICE:
		energy *= 1.3f;
		extraSpd = -3;
		break;
	case SPECIAL_FLY:
		energy *= 0.4f;
		extraSpd = 12;
		break;
	case SPECIAL_BOSS:
		energy *= 50.5f;
		extraSpd = -10;
		break;
	}

	EnemyBase* enemy = BlueWorm::create(enemyIndex, energy, extraSpd, buf, 0, 1, ENEMY_MOVE_WANDER_ON_A_PLATFORM, ENEMY_MOVE_WANDER_ON_A_PLATFORM, 100);
	enemy->setTarget(player);
	enemy->onGround = false;
	enemy->ability = ability;
	spriteBatch->addChild(enemy);
	enemy->setPosition(Point(_spawnPoint.x + cocos2d::random()%10 - 10, _spawnPoint.y + enemy->getContentSize().height/2));
	enemy->desiredPosition = enemy->getPosition();
	enemyArray.pushBack(enemy);
	droppableArray.pushBack(enemy);
}

void HelloWorld::dungeonWaveSchedule(float dt){
    
}

int HelloWorld::getEnergyForWave(int wave, int enemyIndex){
    return ((wave + (wave>50?70:0)) / 10) * 5 + wave/2 + enemyIndex*2;
}
void HelloWorld::BlinkGuideLayer(float dt)
{
	int startX = StartPos.x;
	int startY = StartPos.y;
//	int category = 0;
	int tgid = 0;
	Point pos = cocos2d::Point(startX, startY + 1);
	int maxSpotCount = (EndPos.x - StartPos.x + 1)*(StartPos.y - EndPos.y + 1);
	for (int i = 0; i < maxSpotCount; i++){
		if (i % (int)(EndPos.x - StartPos.x + 1) == 0){
			pos.y -= 1;
			pos.x = startX;
		}
		else{
			pos.x += 1;
		}

		tgid = guideLayer->getTileGIDAt(pos);
		if (tgid == 161){
			guideLayer->setTileGID(165, pos);
		}
		else if (tgid == 165){
			guideLayer->setTileGID(161, pos);
		}
	}
	guideLayer->setVisible(true);
}

void HelloWorld::showGuide(int type){
	_guidingTrapType = type;
	int startX = StartPos.x;
	int startY = StartPos.y;
	bool isValid = false;
	int category = 0;
	Point pos = cocos2d::Point(startX, startY + 1);
	

	if (type == TRAP_GASBURNER) // ground trap
	{
		category = TRAPTYPE_FLOOR;
	}
	else if (type == TRAP_FREEZER){
		category = TRAPTYPE_BEHIND;
	}
	else if (type == TRAP_FIRE){
		category = TRAPTYPE_BEHIND;
	}
	else if (type == TRAP_SPIKE){
		category = TRAPTYPE_BEHIND;
	}
	else if (type == TRAP_TOPSPIKE){
		category = TRAPTYPE_CEILING;
	}
	else if (type == TRAP_ICEBALL){
		category = TRAPTYPE_CEILING;
	}
	else if (type == TRAP_BOMB){
		category = TRAPTYPE_BEHIND;
	}
	else if (type == TRAP_MACEBALL){
		category = TRAPTYPE_CORNER;
	}
	else if (type == TRAP_FLASK){
		category = TRAPTYPE_CEILING;
	}
	else if (type == TRAP_WALL){
		category = TRAPTYPE_BEHIND;
	}
	else if (type == TRAP_HAND){
		category = TRAPTYPE_BEHIND;
	}
	else if (type == TRAP_WALLSPIKE){
		category = TRAPTYPE_CORNER;
    }
    else if (type == TRAP_CACTUS){
        category = TRAPTYPE_BEHIND;
    }
	int maxSpotCount = (EndPos.x - StartPos.x + 1)*(StartPos.y - EndPos.y + 1);
	for (int i = 0; i < maxSpotCount; i++){
		isValid = false;
		if (i % (int)(EndPos.x - StartPos.x + 1) == 0){
			pos.y -= 1;
			pos.x = startX;
		}
		else{
			pos.x += 1;
		}
		if (category == TRAPTYPE_FLOOR){
			isValid = IsOneWay(stageLayer->getTileGIDAt(pos));
		}
		else if (category == TRAPTYPE_BEHIND){
			isValid = IsOneWay(stageLayer->getTileGIDAt(Point(pos.x, pos.y + 1)));
		}
		else if (category == TRAPTYPE_CEILING){
			isValid = IsOneWay(stageLayer->getTileGIDAt(Point(pos.x, pos.y - 1)));
		}
		else if (category == TRAPTYPE_CORNER){
			isValid = IsOneWay(stageLayer->getTileGIDAt(Point(pos.x, pos.y + 1)));
			isValid = isValid && (IsBlock(stageLayer->getTileGIDAt(Point(pos.x + 1, pos.y))) || IsBlock(stageLayer->getTileGIDAt(Point(pos.x - 1, pos.y))));
		}
		if (isValid && IsOccupied(category, pos)){
			isValid = false;
		}
        if(pos.x == 22 && pos.y == 13){
            isValid = false;
        }
		guideLayer->setTileGID(isValid?165:163, pos);
	}
	guideLayer->setVisible(true);
	this->schedule(schedule_selector(HelloWorld::BlinkGuideLayer), 0.5f);
}
void HelloWorld::setStage(int stageNumer)
{
	_hudLayer = GameManager::getInstance()->getHudLayer();
	_totalTime = 0;
	_stageNumber = stageNumer;
    GameManager::getInstance()->preLoadAllSoundEffect();
	
	_isFastMode = false;
    enemyKilledCount = 0;
    enemyMaxCount = 0;
	_earnedCoin = 0;
	_initCoinCount = UserDefault::getInstance()->getIntegerForKey(KEY_COIN_LEFT, 0);
	CollectedCoinCount = _initCoinCount;
	
	GameManager::getInstance()->ResetTrapCount();
    int themeIndex = GameManager::getInstance()->theme;
    if(themeIndex > 3){
        themeIndex = 3;
    }
    //GoogleBilling::Billing::destroyAds();
    
    playerFireCoolTime = 0;
    indestructableTime = 0;
    starCount = 0;
    spriteBatch = SpriteBatchNode::create("pdImages.png");
    
    _sptTarget = Sprite::createWithSpriteFrameName("targeting.png");
    spriteBatch->addChild(_sptTarget, 1000);
    _sptTarget->setVisible(false);
    _targetGap = 150;
    
    float spd = 80;
    int eng = 1;
    _playerType = UserDefault::getInstance()->getIntegerForKey(KEY_SELECTED_HERO, HERO_INDIAN);
    int weapon = 0;
    spd = 10;
    
    heroPositionArray = cocos2d::PointArray::create(50);
    heroPositionArray->retain();

    player = Hero::createHero(_playerType, eng, spd);
    //player->setOpacity(0);
    player->weapon = weapon;
    player->doubleJump = false;
    player->maxEnergy = eng;
    player->energy = eng;
    player->setRestOfBodyPart();
    player->runHatCheck();
    
    if(_playerType == HERO_JONNY){
        player->setSpriteFrame("hero6_0.png");
    }else if(_playerType == HERO_AOKIZI){
        player->setSpriteFrame("hero18_0.png");
    }else if(_playerType == HERO_HERMIONE){
        player->setSpriteFrame("hero20_0.png");
    }else if(_playerType == HERO_DARY){
        player->setSpriteFrame("hero21_0.png");
    }
	droppableArray.pushBack(player);
    
    for (int i = 0; i < 5; i++) {
        char buf[30];
        sprintf(buf, KEY_ACHIEVEMENT_UNLOCKED_FORMAT, i);
        bool isUnlocked = UserDefault::getInstance()->getBoolForKey(buf);
        sprintf(buf, KEY_ACHIEVEMENT_PROGRESS_FORMAT, i);
        int achievementCount = UserDefault::getInstance()->getIntegerForKey(buf, 0);
//        bool canUnlock = achievementCount >= getAchievementTotal(i);
        const char* key;
        if (i == 0) {
            key = KEY_BREE;
        }else if (i == 1) {
            key = KEY_TOM;
        }else if (i == 2) {
            key = KEY_KAREN;
        }else if (i == 3) {
            key = KEY_KARL;
        }else if (i == 4) {
            key = KEY_ANDREW;
        }
        if (isUnlocked || UserDefault::getInstance()->getBoolForKey(key, false)) {
            Hero* hero = NULL;
            
            int left = 19;
            int right = 15;
            if(i == 0){
                hero = Hero::createHero(HERO_BREE, eng, spd);
                hero->setSpriteFrame("hero2_0.png");
                hero->setPosition(Point(360 - left, 576));
            }else if(i == 1){
                hero = Hero::createHero(HERO_TOM, eng, spd);
                hero->setSpriteFrame("hero13_0.png");
                hero->setPosition(Point(360 - left*2, 576));
            }else if(i == 2){
                hero = Hero::createHero(HERO_KAREN, eng, spd);
                hero->setSpriteFrame("hero0_0.png");
                hero->setPosition(Point(360 + right, 576));
            }else if(i == 3){
                hero = Hero::createHero(HERO_KARL, eng, spd);
                hero->setSpriteFrame("hero3_0.png");
                hero->setPosition(Point(360 + right*2, 576));
            }else if(i == 4){
                hero = Hero::createHero(HERO_ANDREW, eng, spd);
                hero->setSpriteFrame("hero11_0.png");
                hero->setPosition(Point(360 + right*3, 576));
            }
            hero->setWeapon(hero->getTag());
            hero->weapon = weapon;
            hero->doubleJump = false;
            hero->maxEnergy = eng;
            hero->energy = eng;
            hero->setRestOfBodyPart();
            hero->runHatCheck();
            hero->setFlippedX(false);
            
            friends.pushBack(hero);
            spriteBatch->addChild(hero);
        }
    }

    setPlayerInfo();
    setWeaponInfo(_playerType);

	GameManager::getInstance()->getHudLayer()->initializeSreen(eng);
	GameManager::getInstance()->getHudLayer()->LblCoinCount->setString(cocos2d::Value(CollectedCoinCount).asString());

    if(GameManager::getInstance()->isVideoRewardEnergy){
        GameManager::getInstance()->getHudLayer()->addEnergyEffect();
    }
	_isClear = false;

    _waveCount = _stageNumber + 1;
	_nextCallWaitTime = 5.0f;
	_isEnemyCreated = true;
	this->schedule(schedule_selector(HelloWorld::WaveSchedule), 0.5f);
	this->schedule(schedule_selector(HelloWorld::CreatingEnemies), 0.15f);

    furtherBack = NULL;
    farBack = NULL;
	nearBack = NULL;
//	Sprite* spt;

	char buf[80];
	/*
    if (GameManager::getInstance()->theme == 2) {
        
    }else{
        const char* strBack ="backgroundGradient.png";
        if(GameManager::getInstance()->theme == 3){
            strBack = "gradientTheme3.png";
        }
        background = Sprite::create(strBack);
        this->getParent()->addChild(background, -10);
        background->setAnchorPoint(Point(0,0));
        background->setScaleY(size.height/background->getContentSize().height);
        background->setScaleX(size.width);
    }
    
    if(GameManager::getInstance()->theme == 3){
        furtherWidth = 399;//625;
        furtherHeight = 207; //293;
        farWidth = 437;//759;
        farHeight = 247;//257;
        nearWidth = 490;//838;
        nearHeight = 290;//284;
    }else{
        furtherWidth = 600;//625;
        furtherHeight = 200; //293;
        farWidth = 800;//759;
        farHeight = 300;//257;
        nearWidth = 1200;//838;
        nearHeight = 600;//284;
    }
    
    
    float nextX = 0;
    float wallY = -40;
    float wallHeight = 143;
    sprintf( buf, "theme_%d_Background_tower_%d_shadow.png", GameManager::getInstance()->theme, cocos2d::random()%2);
    
    if(GameManager::getInstance()->theme == 0){
        furtherBack = Sprite::create();
        farBack = Sprite::create();
        nearBack = Sprite::create();
        Layout* layoutFurther = dynamic_cast<Layout*>(GUIReader::getInstance()->widgetFromJsonFile("LegendDary_UI_Theme_0_Further.json"));
        layoutFurther->setTouchEnabled(false);
        furtherBack->addChild(layoutFurther);
        
        Layout* layoutFar = dynamic_cast<Layout*>(GUIReader::getInstance()->widgetFromJsonFile("LegendDary_UI_Theme_0_Far.json"));
        farBack->addChild(layoutFar);
        layoutFar->setTouchEnabled(false);
        
        while(true){
            spt = Sprite::create("theme_0_Background_wall.png");
            nearBack->addChild(spt);
            spt->setPosition(Point(nextX, wallY));
            spt->setAnchorPoint(Point(0,0));
            wallHeight = spt->getContentSize().height;
            
            nextX += spt->getContentSize().width - 1;
            if(nextX > nearWidth) break;
        }
        
        this->addChild(furtherBack, -9);
        this->addChild(farBack, -8);
        this->addChild(nearBack, -7);
    }else if(GameManager::getInstance()->theme == 1){
        nearBack = Sprite::create("blackSquare.png");
        Sprite* island = Sprite::create("backgroundGround.png");
        island->setPosition(Point(nearWidth/4, nearHeight/3));
        island->getTexture()->setAliasTexParameters();
        nearBack->addChild(island);
        
        Sprite* island2 = Sprite::create("backgroundGround.png");
        island2->setPosition(Point(nearWidth*3/4, nearHeight*2/3));
        island2->getTexture()->setAliasTexParameters();
        nearBack->addChild(island2);
        
        this->addChild(nearBack, -7);
        
        furtherBack = Sprite::create("blackSquare.png");
        
        Sprite* q0 = Sprite::create("backgroundSunQuarter.png");
        q0->setAnchorPoint(Point(1, 0));
        q0->setRotation(0);
        q0->setPosition(Point(0, 100) + cocos2d::Point(furtherWidth/2, furtherHeight/2));
        q0->getTexture()->setAliasTexParameters();
        furtherBack->addChild(q0);
        Sprite* q1 = Sprite::create("backgroundSunQuarter.png");
        q1->setAnchorPoint(Point(1, 0));
        q1->setRotation(90);
        q1->setPosition(Point(0, 100) + cocos2d::Point(furtherWidth/2, furtherHeight/2));
        q1->getTexture()->setAliasTexParameters();
        furtherBack->addChild(q1);
        Sprite* q2 = Sprite::create("backgroundSunQuarter.png");
        q2->setAnchorPoint(Point(1, 0));
        q2->setRotation(180);
        q2->setPosition(Point(0, 100) + cocos2d::Point(furtherWidth/2, furtherHeight/2));
        q2->getTexture()->setAliasTexParameters();
        furtherBack->addChild(q2);
        Sprite* q3 = Sprite::create("backgroundSunQuarter.png");
        q3->setAnchorPoint(Point(1, 0));
        q3->setRotation(270);
        q3->setPosition(Point(0, 100) + cocos2d::Point(furtherWidth/2, furtherHeight/2));
        q3->getTexture()->setAliasTexParameters();
        furtherBack->addChild(q3);
        
        this->addChild(furtherBack, -9);
        
        farBack = Sprite::create("blackSquare.png");
//        nearBack->setAnchorPoint(Point(0, 1));
        
        float extraY = -270;
        Sprite* sea0 = Sprite::create("backgroundSea.png");
        sea0->setAnchorPoint(Point(0, 1));
        farBack->addChild(sea0);
        sea0->setPosition(Point(sea0->getContentSize().width*0, extraY) + cocos2d::Point(farWidth/2, farWidth/2));
        
        Sprite* sea1 = Sprite::create("backgroundSea.png");
        sea1->setAnchorPoint(Point(0, 1));
        farBack->addChild(sea1);
        sea1->setPosition(Point(sea1->getContentSize().width*1, extraY) + cocos2d::Point(farWidth/2, farWidth/2));
        
        Sprite* sea2 = Sprite::create("backgroundSea.png");
        sea2->setAnchorPoint(Point(0, 1));
        farBack->addChild(sea2);
        sea2->setPosition(Point(sea2->getContentSize().width*2, extraY) + cocos2d::Point(farWidth/2, farWidth/2));
        
        Sprite* seaN1 = Sprite::create("backgroundSea.png");
        seaN1->setAnchorPoint(Point(0, 1));
        farBack->addChild(seaN1);
        seaN1->setPosition(Point(-seaN1->getContentSize().width*1, extraY) + cocos2d::Point(farWidth/2, farWidth/2));
        
        Sprite* seaN2 = Sprite::create("backgroundSea.png");
        seaN2->setAnchorPoint(Point(0, 1));
        farBack->addChild(seaN2);
        seaN2->setPosition(Point(-seaN2->getContentSize().width*2, extraY) + cocos2d::Point(farWidth/2, farWidth/2));
        
        this->addChild(farBack, -8);
    }else if(GameManager::getInstance()->theme == 3){
        nearBack = Sprite::create("backgroundTheme3Near.png");
        this->addChild(nearBack, -7);
        nearBack->setAnchorPoint(Point(0, -0.07));
        
        furtherBack = Sprite::create("backgroundTheme3Further.png");
        this->addChild(furtherBack, -9);
        furtherBack->setAnchorPoint(Point(0, -0.07));
        
        farBack = Sprite::create("backgroundTheme3Far.png");
        this->addChild(farBack, -8);
        farBack->setAnchorPoint(Point(0, -0.07));
        
        this->schedule(schedule_selector(HelloWorld::cloudSchedule), 1);
        Sprite* cloud = Sprite::createWithSpriteFrameName("cloudTheme3_0.png");
        float speed = (cocos2d::random()%10);
        cloud->setPosition(Point( farBack->getContentSize().width/4, farBack->getContentSize().height*1.7));
        cloudArray.pushBack(cloud);
        cloud->runAction(Sequence::create(MoveTo::create(cloud->getPosition().x/speed, cocos2d::Point(0, cloud->getPosition().y)), MoveTo::create(0, cocos2d::Point(farBack->getContentSize().width, cloud->getPosition().y)), MoveTo::create((farBack->getContentSize().width - cloud->getPosition().x)/speed, cocos2d::Point(cloud->getPosition().x, cloud->getPosition().y)), NULL));
        farBack->addChild(cloud);
        cloud = Sprite::createWithSpriteFrameName("cloudTheme3_1.png");
        cloud->setPosition(Point( farBack->getContentSize().width*3/4, farBack->getContentSize().height*1.5));
        cloudArray.pushBack(cloud);
        cloud->runAction(Sequence::create(MoveTo::create(cloud->getPosition().x/speed, cocos2d::Point(0, cloud->getPosition().y)), MoveTo::create(0, cocos2d::Point(farBack->getContentSize().width, cloud->getPosition().y)), MoveTo::create((farBack->getContentSize().width - cloud->getPosition().x)/speed, cocos2d::Point(cloud->getPosition().x, cloud->getPosition().y)), NULL));
        cloud = Sprite::createWithSpriteFrameName("cloudTheme3_1.png");
        cloud->setPosition(Point( farBack->getContentSize().width*3/7, farBack->getContentSize().height*1.6));
        cloudArray.pushBack(cloud);
        cloud->runAction(Sequence::create(MoveTo::create(cloud->getPosition().x/speed, cocos2d::Point(0, cloud->getPosition().y)), MoveTo::create(0, cocos2d::Point(farBack->getContentSize().width, cloud->getPosition().y)), MoveTo::create((farBack->getContentSize().width - cloud->getPosition().x)/speed, cocos2d::Point(cloud->getPosition().x, cloud->getPosition().y)), NULL));
        farBack->addChild(cloud);
        cloud = Sprite::createWithSpriteFrameName("cloudTheme3_2.png");
        cloud->setPosition(Point( farBack->getContentSize().width*4/5, farBack->getContentSize().height*1.4));
        cloudArray.pushBack(cloud);
        cloud->runAction(Sequence::create(MoveTo::create(cloud->getPosition().x/speed, cocos2d::Point(0, cloud->getPosition().y)), MoveTo::create(0, cocos2d::Point(farBack->getContentSize().width, cloud->getPosition().y)), MoveTo::create((farBack->getContentSize().width - cloud->getPosition().x)/speed, cocos2d::Point(cloud->getPosition().x, cloud->getPosition().y)), NULL));
        farBack->addChild(cloud);
        cloud = Sprite::createWithSpriteFrameName("cloudTheme3_3.png");
        cloud->setPosition(Point( farBack->getContentSize().width/3, farBack->getContentSize().height*1.2));
        cloudArray.pushBack(cloud);
        cloud->runAction(Sequence::create(MoveTo::create(cloud->getPosition().x/speed, cocos2d::Point(0, cloud->getPosition().y)), MoveTo::create(0, cocos2d::Point(farBack->getContentSize().width, cloud->getPosition().y)), MoveTo::create((farBack->getContentSize().width - cloud->getPosition().x)/speed, cocos2d::Point(cloud->getPosition().x, cloud->getPosition().y)), NULL));
        farBack->addChild(cloud);
    }else{
        
    }*/
    
    // play music
    GameManager::getInstance()->playSoundEffect(SOUND_BGM_WHOS_BOSS);
    
	cocos2d::Value stageMap;
    int tutorialStages = 3;
    
//    int theTheme = GameManager::getInstance()->theme+1;
    sprintf( buf, "pdStageEasy");
    stageMap = (buf);

    if (GameManager::getInstance()->theme+1 == 1 && stageNumer+1 <= tutorialStages){
        if (Application::getInstance()->getCurrentLanguage() == LanguageType::KOREAN) {
//            stageMap = GameManager::getInstance()->getStringWithFormat("LegendDary2_%d_%d_Kr", GameManager::getInstance()->theme+1, stageNumer+1).asString().c_str();
        }
    }
	
    sprintf( buf, "%s.tmx", stageMap.asString().c_str());
    
    log("Map name: %s", buf);
   
    tileMap = cocos2d::TMXTiledMap::create(buf);


/*
 tileMap = cocos2d::experimental::TMXTiledMap::create(buf);

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
    tileMap = cocos2d::experimental::TMXTiledMap::create(buf);
#else
    tileMap = cocos2d::TMXTiledMap::create(buf);
#endif
*/
    spriteBatch->addChild(player, 10);
//    tileMap->retain();
    this->setAnchorPoint(Point(0,0));
	this->setScale(_scale);
    this->addChild(tileMap, -1);

//    blackSquare = Sprite::create("blackSquare.png");
//    this->addChild(blackSquare, 1);
    
    GameManager::getInstance()->firstPlayed = false;
    
    isGameOver = false;
    keyCollected = false;
    
    stageLayer = tileMap->getLayer("Stage");
    unitLayer = tileMap->getLayer("Unit");
	guideLayer = tileMap->getLayer("Guide");
	guideLayer->setVisible(false);
    stageLayer->setVisible(true);
    tileMap->addChild(spriteBatch, unitLayer->getLocalZOrder());
    stageLayer->setLocalZOrder(unitLayer->getLocalZOrder() - 1);

    // change coin tile into coin animation
    int totalWidth = tileMap->getMapSize().width;
    int totalHeight = tileMap->getMapSize().height;
    cocos2d::Point point;
    int tileWidth = tileMap->getTileSize().width;
    int tileHeight = tileMap->getTileSize().height;
    int talkCount = 0;
    cocos2d::PointArray* pointArrayToRemoveInUnitLayer = cocos2d::PointArray::create(3000);
    for (int i = 0; i < totalWidth; i++) {
        for (int j = 0; j < totalHeight; j++) {
            point = cocos2d::Point(i,j);
            int tileGid = unitLayer->getTileGIDAt(point);
            if (tileGid) {
				cocos2d::Value properties = tileMap->getPropertiesForGID(tileGid);
                
				if (properties.getType() == cocos2d::Value::Type::MAP)
                {
                    ValueMap map = properties.asValueMap();
                    if(!keyExist(map, "MovingPlatformLine") &&
                       !keyExist(map, "MovingPlatformEnd")){
                        unitLayer->removeTileAt(point);
                    }
                    if(keyExist(map, "Coin")){
                        Droppable* coinSprite = Droppable::createDroppable(UNIT_COIN, 1, 0, "silverCoin0.png");
                        spriteBatch->addChild(coinSprite);
                        coinArray.pushBack(coinSprite);
                        coinSprite->runAnimation("silverCoin", true);
                        coinSprite->setPosition(Point(i*tileWidth + 9, (totalHeight-j)*tileHeight - 7));
                    }else if(keyExist(map, "Player")){
                        player->setPosition(Point(i*tileWidth, (totalHeight-j)*tileHeight) + cocos2d::Point(TILE_SIZE/2, TILE_SIZE/2));
                        log("Player pos: %f, %f", player->getPositionX(), player->getPositionY());
					}
					else if (keyExist(map, "Spawn")){
                        float delay = 9.0f;
                        float fadeDur = 8.0f;
						_spawnPoint = (Point(i*tileWidth, (totalHeight - j)*tileHeight) + cocos2d::Point(TILE_SIZE / 2, -TILE_SIZE / 2));
						Sprite* gateBack = Sprite::createWithSpriteFrameName("doorBack.png");
						gateBack->setLocalZOrder(-1);
						spriteBatch->addChild(gateBack);
						Point gatePos = cocos2d::Point(_spawnPoint.x + TILE_SIZE*2.0f - 3, _spawnPoint.y +TILE_SIZE + 1);
						gateBack->setPosition(gatePos);
                        gateBack->runAction(Sequence::create(DelayTime::create(delay), FadeOut::create(fadeDur), NULL));
						Sprite* gateFront = Sprite::createWithSpriteFrameName("doorFront.png");
						gateFront->setLocalZOrder(100);
						gateFront->setPosition(gatePos);
						spriteBatch->addChild(gateFront);
                        gateFront->runAction(Sequence::create(DelayTime::create(delay), FadeOut::create(fadeDur), NULL));
						theDoor = Droppable::createDroppable(0, 0, 0, "door0.png");
						spriteBatch->addChild(theDoor);
						theDoor->setPosition(gatePos);
						theDoor->setLocalZOrder(200);
						theDoor->runAnimation("door", false);
                        theDoor->runAction(Sequence::create(DelayTime::create(delay), FadeOut::create(fadeDur), NULL));
                        GameManager::getInstance()->playSoundEffect(SOUND_CASTLE_DOOR);
					}
					else if (keyExist(map, "Mark")){
						if (map.at("Mark").asString().compare("Start") == 0){
							StartPos = point;
						}
						else if (map.at("Mark").asString().compare("End") == 0){
							EndPos = point;
						}
					}
					else if (keyExist(map, "Candle")){
                        Sprite* sprite = Sprite::createWithSpriteFrameName("doubleCandle0.png");
                        Animation* animation = AnimationCache::getInstance()->getAnimation("candle");
                        Animate* animate = Animate::create(animation);
                        sprite->runAction(RepeatForever::create(animate));
                        spriteBatch->addChild(sprite);
						sprite->setPosition(Point(i*tileWidth + 8, (totalHeight - j)*tileHeight - 11));
                    }else if(keyExist(map, "Enemy")){
//                        EnemyBase* enemy = NULL;
//                        FireableBase* fEnemy = NULL;
                       
                        
                    }else if(keyExist(map, "Trap")){
                        
                    }else if(keyExist(map, "Helper")){
                        
                        
                    }
                    else if(keyExist(map, "MovingPlatformStart") || keyExist(map, "MovingPlatformTriggerStart")){
                        bool trigger = keyExist(map, "MovingPlatformTriggerStart");
                        pointArrayToRemoveInUnitLayer->addControlPoint(point);
                        int x = i;
                        int y = j;
                        bool solved = false;
                        bool notFoundAny = false;
                        int startX = x;
                        int startY = y;
                        int searchX, searchY;

                        while (!solved) {
                            notFoundAny = true;
                            for (int t = 0; t < 9; t++) { //3
                                int c = t % 3;
                                int r = (int)(t / 3);
                                searchX = x + c - 1;
                                searchY = y + r - 1;
                                int tileGid = unitLayer->getTileGIDAt(Point(searchX, searchY));
                                if (tileGid) {
									cocos2d::Value properties = tileMap->getPropertiesForGID(tileGid);
                                    
                                    if (!properties.isNull())
                                    {
                                        ValueMap newMap = properties.asValueMap();
                                        if (keyExist(newMap, "MovingPlatformLine")) {
                                            x = searchX;
                                            y = searchY;

                                            unitLayer->removeTileAt(Point(x, y));
                                            notFoundAny = false;
                                            break;
                                        }else if(keyExist(newMap, "MovingPlatformEnd")){
                                            x = searchX;
                                            y = searchY;

                                            cocos2d::Point startPosition =Point(startX*tileWidth + 10, (totalHeight - startY)*tileHeight);
                                            cocos2d::Point endPosition = cocos2d::Point(x*tileWidth + 10, (totalHeight - y)*tileHeight);
                                            pointArrayToRemoveInUnitLayer->addControlPoint(Point(x,y));
                                            const char* sptName;
                                            if (GameManager::getInstance()->theme == 0) {
                                                sptName = "movingPlatform0.png";
                                            }else if(GameManager::getInstance()->theme == 1){
                                                sptName = "movingPlatform3.png";
                                            }else{
                                                sptName = "theme2MovingPlatform.png";
                                            }
                                            MovingPlatform* platform = MovingPlatform::create(sptName, startPosition, endPosition);
                                            movingPlatformArray.pushBack(platform);
                                            spriteBatch->addChild(platform);

                                            platform->setPosition(startPosition);
                                            platform->desiredRect = platform->getBoundingBox();
                                            
                                            platform->isTowardEnd = true;
                                            
                                            notFoundAny = false;
                                            solved = true;
                                            
                                            if (trigger) {
                                                platform->isTriggerPlatform = true;
                                                platform->isTriggered = false;
                                            }
                                            
                                            break;
                                        }
                                    }
                                }
                            }
                            if (notFoundAny) {
                                log("Moving Platform Creation Failed, Platform line not complete");
                                break;
                            }
                        }
                    }
                    else if(keyExist(map, "Arrow"))
                    {
                        ValueMap newMap = properties.asValueMap();
                        if (newMap.at("Arrow").asString().compare("Up") == 0)
                        {
                            Sprite* sprite = Sprite::createWithSpriteFrameName("finger.png");
                            spriteBatch->addChild(sprite);
                            sprite->setPosition(Point(i*tileWidth + tileWidth/2, (totalHeight-j)*tileHeight - tileHeight));
                            sprite->runAction(RepeatForever::create((ActionInterval*)Sequence::create(MoveBy::create(1, cocos2d::Point(0, 50)), DelayTime::create(0.5f), MoveBy::create(0, cocos2d::Point(0, -50)), NULL)));
                        }
                        else if(newMap.at("Arrow").asString().compare("Down") == 0)
                        {
                            Sprite* sprite = Sprite::createWithSpriteFrameName("finger.png");
                            spriteBatch->addChild(sprite);
                            sprite->setPosition(Point(i*tileWidth + tileWidth/2, (totalHeight-j)*tileHeight - tileHeight));
                            sprite->runAction(RepeatForever::create((ActionInterval*)Sequence::create(MoveBy::create(1, cocos2d::Point(0, -50)), DelayTime::create(0.5f), MoveBy::create(0, cocos2d::Point(0, 50)), NULL)));
                        }
                    }
                }
            }
            
        }
    }
    log("setStage map done");
    TMXObjectGroup* objectGroup = tileMap->getObjectGroup("Object");
    
    if (objectGroup != NULL) {
        for(auto obj:objectGroup->getObjects()){
            if (keyExist(obj.asValueMap(), "InfoDesc")) {
               
            }
        }
    }
    log("setStage object map done");
    if (pointArrayToRemoveInUnitLayer) {
        for (int i = 0; i < pointArrayToRemoveInUnitLayer->count(); i++) {
            cocos2d::Point pos = pointArrayToRemoveInUnitLayer->getControlPointAtIndex(i);
            unitLayer->removeTileAt(pos);
        }
    }
    
    player->oneWayCancelY = 0;
    
    setPlayerInfo();
    
    this->schedule(schedule_selector(HelloWorld::gravityUpdate));
    this->schedule(schedule_selector(HelloWorld::gravityUpdateForCoins));
    this->schedule(schedule_selector(HelloWorld::gravityUpdateForCustomMoving));
    this->schedule(schedule_selector(HelloWorld::gravityUpdateForFlyingOrSwimingEnemies));
    
    if (talkCount > 0) {
        this->schedule(schedule_selector(HelloWorld::talkUpdate), 0.1f);
    }
    
	extraCritical = 0;
    extraPower = 0;
    extraFruit = 0;
    
    
    player->setFlippedX(player->isTowardLeft);
    
    if (GameManager::getInstance()->isVideoRewardCoins) {
        Droppable* coin;
        cocos2d::Point coinPos;
        cocos2d::Point pos;
        int spread = 70;
        for (int i = 0; i < 40; i++) {
            coinPos = player->getPosition() + cocos2d::Point(spread - cocos2d::random()%(spread*2), spread - cocos2d::random()%(spread*2));
            pos = this->tileCoordForPosition(coinPos);
            if(pos.x < stageLayer->getLayerSize().width && pos.y < stageLayer->getLayerSize().height && pos.x >=0 && pos.y >=0){
                
            }else{
                i--;
                continue;
            }
            
            int tgid = stageLayer->getTileGIDAt(pos);
            if(tgid > 0){
                i--;
                continue;
            }
            coin = Droppable::createDroppable(UNIT_COIN, 1, 0, "silverCoin0.png");
            coin->runAnimation("silverCoin", true);
            
            coinArray.pushBack(coin);
            spriteBatch->addChild(coin);
            coin->setPosition(coinPos);
            coin->runAction(Sequence::create(DelayTime::create(8), FadeOut::create(1), CallFuncN::create(CC_CALLBACK_1(HelloWorld::coinWaitDone, this)), NULL));
        }
    }
    
    isVideoRewardAttack = GameManager::getInstance()->isVideoRewardAttack;
    if (isVideoRewardAttack) {
        Sprite* particle;
        GameManager::getInstance()->playSoundEffect(SOUND_MEDICINE);
        for (int i = 0; i < 20; i++) {
            particle = Sprite::createWithSpriteFrameName("particle.png");
            spriteBatch->addChild(particle, player->getLocalZOrder()+1);
            particle->setScale(1 + (cocos2d::random()%10)*0.05);
//            particle->setColor(Color3B(28, 135, 171));
            particle->setPosition(player->getPosition() + cocos2d::Point(cocos2d::random()%30 - 15, cocos2d::random()%40 - 20));
            //            particle->runAction(FadeOut::create(0));
            float dur = 0.5;
            particle->runAction(RotateBy::create(dur, 360));
            particle->runAction(MoveBy::create(dur - (cocos2d::random()*10)*0.1f*dur, cocos2d::Point(0, 30)));
            particle->runAction(Sequence::create(FadeIn::create(dur*3/10), DelayTime::create(dur*4/10), FadeOut::create(dur*3/10), CallFuncN::create(CC_CALLBACK_1(HelloWorld::spriteMoveDone, this)), NULL));
        }
    }
    
    GameManager::getInstance()->isVideoRewardEnergy = false;
    GameManager::getInstance()->isVideoRewardAttack = false;
    GameManager::getInstance()->isVideoRewardCoins = false;
    

	Sprite* sptBackColor = Sprite::createWithSpriteFrameName("whiteSquare.png");
	sptBackColor->setColor(Color3B(254, 241, 216));//forest theme back color
	//sptBackColor->setColor(Color3B(189, 203, 186));//winter theme back color
	spriteBatch->addChild(sptBackColor);
	sptBackColor->setAnchorPoint(Point(0, 0));
	sptBackColor->setLocalZOrder(-250);
	sptBackColor->setScale(TILE_SIZE * 11 / sptBackColor->getContentSize().width, TILE_SIZE * 13 / sptBackColor->getContentSize().height);
	sptBackColor->setPosition((StartPos.x)* TILE_SIZE, tileMap->getMapSize().height*TILE_SIZE - (11)*TILE_SIZE);

	int extraY = 0;
	const char* backgroundName = "background.png";
	if (false){	// winter
		extraY = -6;
		backgroundName = "background2.png";
	}
	else if (false){	// village
		backgroundName = "background1.png";
	}
	Sprite* sptBackground = Sprite::createWithSpriteFrameName(backgroundName);
	sptBackground->setAnchorPoint(Point(0, 0));
	spriteBatch->addChild(sptBackground);
	sptBackground->setLocalZOrder(-250);
	sptBackground->setPosition((StartPos.x)* TILE_SIZE, tileMap->getMapSize().height*TILE_SIZE - (EndPos.y+1)*TILE_SIZE);

    isSetStageDone = true;

	if (GameManager::getInstance()->IsNewGame){
		ResetPreviousTraps();
		UserDefault::getInstance()->setIntegerForKey(KEY_CHECK_POINT, 1);
	}
	else{
		LoadPreviousTraps();
	}

    log("setStage done");
}
void HelloWorld::cloudSchedule(float dt){
    for (auto cloud : cloudArray) {
        if(cloud->getPosition().x < 0){
            cloud->setPosition(Point(cloud->getParent()->getContentSize().width + cloud->getContentSize().width/2, cloud->getPosition().y));
        }
    }
}


void HelloWorld::blinkForeLayer(float dt){

}

void HelloWorld::sacrificeDone(Ref* obj){
    Droppable* drop = (Droppable*)obj;
    runEffect(EFFECT_TWINKLE, drop->getPosition());
	drop->unscheduleAllCallbacks();
    drop->stopAllActions();
    drop->removeFromParentAndCleanup(true);
    player->energy++;    player->maxEnergy++;
}

void HelloWorld::splashDemage(Point pos, int radius, int demage){
    cocos2d::Rect mRect;
    EnemyBase* drop;

	enemiesToRemove.clear();
    for (int i = 0; i<enemyArray.size(); i++) {
        drop = enemyArray.at(i);
        
        cocos2d::Point tPos = drop->getPosition();
        float distance = sqrtf((pos.x - tPos.x)*(pos.x - tPos.x) + (pos.y - tPos.y)*(pos.y - tPos.y));
        
        if (distance < radius){
            log("splash!");
            if(!drop->untouchable){
                if (!drop->immortal) {
                    if (demage >= 10) {
                        demage = demage*0.8f + (cocos2d::random()%((int)(demage*0.2f)));
                    }
                    
                    if (drop->energy < demage) {
                        demage = drop->energy;
                    }
                    drop->energy -= (int)demage;
                    drop->showEnergy();
                    
					Label* lblDemage = Label::createWithBMFont("alphabet_dot_attack.fnt", cocos2d::Value((int)demage).asString());
                    
                    tileMap->addChild(lblDemage, 100);
                    lblDemage->setScale(0.5);
					lblDemage->getFontAtlas()->setAliasTexParameters();
                    lblDemage->setPosition(drop->getPosition());
                    lblDemage->runAction(EaseIn::create(MoveBy::create(1, cocos2d::Point(0, 25)), 0.3));
                    lblDemage->runAction(Sequence::create(DelayTime::create(0.7), FadeOut::create(0.3), CallFuncN::create(CC_CALLBACK_1(HelloWorld::enemyBaseMoveDone, this)), NULL));
                    
                    if (drop->moveTypeAfterFindHero != drop->currentMoveType) {
                        //                    drop->doActionAfterFindTarget();
                    }
                    
					drop->blinkForAWhile();

					if (drop->energy <= 0) {
						enemiesToRemove.pushBack(drop);
						this->runEffect(EFFECT_EXPLODE_BIG, drop->getPosition());

						this->createRewards(drop);
						if (drop->rewards > 0) {
							enemyKilledCount++;
						}
					}
                }
                
            }
            
        }
        
    }

	for (auto spt : enemiesToRemove){
		spt->stopAllActions();
		spt->unscheduleAllCallbacks();
		enemyArray.eraseObject(spt);
		spt->removeFromParentAndCleanup(true);
	}
	enemiesToRemove.clear();

}
void HelloWorld::updateIndicators(){
    Droppable* unit;
    float eX = 0;
    float eY = 0;
    float centerX = -this->getPosition().x + size.width/2;
    float centerY = -this->getPosition().y + size.height/2;
    float a;float y;float x;cocos2d::Rect rect;
    int outOfBoxCount = 0;
    Sprite* indicator;
    Sprite* spt;
    float scale = this->getScale();
    float offset = 15;
    for (int i = 0; i < indicatedArray.size(); i++) {
        spt = (Sprite*)indicatedArray.at(i);
      
        rect = cocos2d::Rect(-this->getPosition().x/scale, -this->getPosition().y/scale, size.width/scale, size.height/scale);
        if(rect.containsPoint(unit->getPosition())){
            continue;
        }else{
            
        }
        eX = unit->getPosition().x*scale;
        eY = unit->getPosition().y*scale;
        a = (eY - centerY)/(eX - centerX);
        y = centerY > eY? (-size.height/2 + offset):(size.height/2 - offset);
        x = y/a;
        if (x > size.width/2) {// right
            x = size.width/2 - offset;
            y = x*a;
        }else if(x < -size.width/2){//left
            x = -size.width/2 + offset;
            y = x*a;
        }else{
            
        }
        
        if (indicatorArray.size() > outOfBoxCount) {
//            indicator = indicatorArray.at(outOfBoxCount);
            for (auto indi: indicatorArray) {
                if(indi->getTag() == unit->getTag()){
                    indicator = indi;
                    break;
                }
            }
        }else{
            indicator = Sprite::create("blueArrow.png");\
            indicator->setTag(spt->getTag());
      
            
            spt->setTag(79);
            indicator->addChild(spt);

            spt->setPosition(Point(-50, indicator->getContentSize().height*indicator->getScale()/2));
            indicator->setScale(0.5);
            this->getParent()->addChild(indicator);
            GameManager::getInstance()->makeItSiluk(indicator);
            indicatorArray.pushBack(indicator);
        }

        indicator->setRotation(-atan2(y,x)*180.0f/3.14f);
        indicator->setPosition(Point(size.width/2 + x, size.height/2 + y));

        spt = (Sprite*)indicator->getChildByTag(79);
        spt->setRotation(-indicator->getRotation());
        
        outOfBoxCount++;
    }
    
    if (indicatorArray.size() > outOfBoxCount) {
//        for (int i = (int)indicatorArray.size() - 1; i >= outOfBoxCount; i--) {
//            indicator = (Sprite*)indicatorArray.at(i);
//            indicatorArray.eraseObject(indicator);
//            indicator->removeFromParentAndCleanup(true);
//        }
        for(auto indi: indicatorArray){
            indi->removeFromParentAndCleanup(true);
        }
        indicatorArray.clear();
        updateIndicators();
    }
}

void HelloWorld::removeUsedAssets(){
	log("HelloWorld removeUsedAssets");
	if (background)
		background->removeFromParentAndCleanup(true);
	unscheduleAll();
	heroPositionArray->release();
	for (auto lbl : talkLabelArray){
		lbl->release();
	}
	//    _eventDispatcher->removeEventbener(listener);
	player->removeFromParentAndCleanup(true);
	this->removeAllChildrenWithCleanup(true);
	this->unscheduleAllCallbacks();
	this->stopAllActions();
	GameManager::getInstance()->setHudLayer(NULL);
	GameManager::getInstance()->setCurrentStageLayer(NULL);
}

void HelloWorld::useBomb(){
	if (isGameOver)
		return;

	log("Use Bomb!");
	isPaused = true;
	for (auto spt : enemyArray)
	{
		spt->pause();
	}

	for (auto spt : heroMissileArray)
	{
		spt->pause();
	}

	for (auto spt : enemyMissileArray)
	{
		spt->pause();
	}

	bombTargetMissileArray.clear();
	bombTargetEnemyArray.clear();
	Point pos = player->getPosition();
	Point tPos;
	cocos2d::Rect rect = cocos2d::Rect(player->getPosition().x - size.width / 8, player->getPosition().y - size.height / 8, size.width / 4, size.height / 4);
	for (auto enemy : enemyArray) {
		if (enemy->untouchable || enemy->immortal) continue;
		tPos = enemy->getPosition();
		if (rect.containsPoint(tPos)){//distance < targetDistance) {
			bombTargetEnemyArray.pushBack(enemy);
		}
	}
	for (auto ms : enemyMissileArray) {
		tPos = ms->getPosition();
		if (rect.containsPoint(tPos)){//distance < targetDistance) {
			bombTargetMissileArray.pushBack(ms);
		}
	}

	Sprite* effect = Sprite::createWithSpriteFrameName("daryFace.png");
	effect->setPosition(player->getPosition());
	spriteBatch->addChild(effect);
	effect->runAction(Sequence::create(ScaleTo::create(1, 15), CallFuncN::create(CC_CALLBACK_1(HelloWorld::enemyBaseMoveDone, this)), NULL));
	effect->runAction(Sequence::create(DelayTime::create(0.5), FadeOut::create(0.4), NULL));

	this->schedule(schedule_selector(HelloWorld::doTheBombToMissiles), 0.1, bombTargetMissileArray.size(), 1);
}

void HelloWorld::doTheBombToMissiles(float dt){
	log("doTheBombToMissiles");
	if (bombTargetMissileArray.size() <= 0) {
		this->unschedule(schedule_selector(HelloWorld::doTheBombToMissiles));
		bombTargetEnemyIndex = 0;
		this->schedule(schedule_selector(HelloWorld::doTheBombToEnemies), 0.1, bombTargetEnemyArray.size(), 0.1);
		return;
	}

	Droppable* drop = bombTargetMissileArray.at(0);
	Sprite* sprite = Sprite::createWithSpriteFrameName("daryFace.png");
	sprite->runAction(Sequence::create(ScaleTo::create(0.5, 5), CallFuncN::create(CC_CALLBACK_1(HelloWorld::enemyBaseMoveDone, this)), NULL));
	sprite->runAction(FadeOut::create(0.5));
	spriteBatch->addChild(sprite);
	switch (drop->getTag()) {
	case UNIT_MISSILE_CHASING:
		chasingMissileArray.eraseObject(drop);
		break;
	case UNIT_MISSILE_DROPPABLE:
		droppableMissileArray.eraseObject(drop);
		break;
	case UNIT_MISSILE_CUSTOM:
		customMovingArray.eraseObject(drop);
		break;
	case UNIT_MISSILE_STRAIGHT:
		straightMovingArray.eraseObject(drop);
		break;
	default:
		break;
	}
	sprite->setPosition(drop->getPosition());
	bombTargetMissileArray.erase(0);
	enemyMissileArray.eraseObject(drop);
	spriteBatch->removeChild(drop, true);

	Point screenPos = this->getPosition();
	this->runAction(Sequence::create(MoveBy::create(0.05, cocos2d::Point(cocos2d::random() % 36 - 18, cocos2d::random() % 36 - 18)),
		MoveTo::create(0.05, screenPos), NULL));
}

void HelloWorld::doTheBombToEnemies(float dt){
	float demage = 1;

	if (bombTargetEnemyArray.size() <= bombTargetEnemyIndex) {
		this->unschedule(schedule_selector(HelloWorld::doTheBombToEnemies));
		isPaused = false;

		for (auto spt : enemyArray)
		{
			spt->resume();
		}

		for (auto spt : heroMissileArray)
		{
			spt->resume();
		}

		for (auto spt : enemyMissileArray)
		{
			spt->resume();
		}

		for (auto enemy : bombTargetEnemyArray){
			if (enemy->untouchable){

			}
			else{
				enemy->energy -= (int)demage;
				enemy->desiredPosition = enemy->desiredPosition + cocos2d::Point(enemy->isTowardLeft ? 2 : -2, 2);
				enemy->setPosition(enemy->getPosition() + cocos2d::Point(enemy->isTowardLeft ? 2 : -2, 2));
				Label* lblDemage = Label::createWithBMFont("alphabet_dot_attack.fnt", cocos2d::Value((int)demage).asString());

				tileMap->addChild(lblDemage, 100);
				lblDemage->setScale(0.5);
				lblDemage->getFontAtlas()->setAliasTexParameters();
				lblDemage->setPosition(enemy->getPosition());
				lblDemage->runAction(EaseIn::create(MoveBy::create(1, cocos2d::Point(0, 25)), 0.3));
				lblDemage->runAction(Sequence::create(DelayTime::create(0.7), FadeOut::create(0.3), CallFuncN::create(CC_CALLBACK_1(HelloWorld::enemyBaseMoveDone, this)), NULL));

				if (enemy->energy <= 0) {
					this->runEffect(EFFECT_EXPLODE_MIDDLE, enemy->getPosition());
					this->createRewards(enemy);

					enemyArray.eraseObject(enemy);
					enemy->stopAllActions();
					enemy->unscheduleAllCallbacks();
					droppableArray.eraseObject(enemy);
					spriteBatch->removeChild(enemy, true);
				}
				else{
					enemy->blinkForAWhile();
				}
			}
		}

		return;
	}

	EnemyBase* enemy = bombTargetEnemyArray.at(bombTargetEnemyIndex);
	Sprite* sprite = Sprite::createWithSpriteFrameName("daryFace.png");
	sprite->runAction(Sequence::create(ScaleTo::create(0.5, 5), CallFuncN::create(CC_CALLBACK_1(HelloWorld::enemyBaseMoveDone, this)), NULL));
	sprite->runAction(FadeOut::create(0.5));
	spriteBatch->addChild(sprite);
	sprite->setPosition(enemy->getPosition());

	Point screenPos = this->getPosition();
	this->runAction(Sequence::create(MoveBy::create(0.05, cocos2d::Point(cocos2d::random() % 18 - 9, cocos2d::random() % 18 - 9)),
		MoveTo::create(0.05, screenPos), NULL));

	bombTargetEnemyIndex++;
}
void HelloWorld::usePotion(){
	if (isGameOver)
		return;

	player->energy = player->maxEnergy;
	GameManager::getInstance()->getHudLayer()->setLife(player->maxEnergy);
	// heal effect
	log("Use Potion!");
}

bool HelloWorld::keyExist(ValueMap map, const char* key){
	return map.find(key) != map.end();
}

void HelloWorld::revive(){
	resumeLayer();
	player->setPosition(deadPoint);

	isGameOver = false;
	indestructableTime = -1;

	player->velocity = cocos2d::Point(0, 100);

	GameManager::getInstance()->isInMiddleOfGame = true;

	indestructableTime = 1.6f;
	this->schedule(schedule_selector(HelloWorld::blinkingPlayer), 0.1f);

	GameManager::getInstance()->getHudLayer()->enableButtons(true);

	player->energy = player->maxEnergy;
	GameManager::getInstance()->getHudLayer()->setLife(player->maxEnergy);
	GameManager::getInstance()->playSoundEffect(SOUND_MEDICINE);
}

void HelloWorld::enemyFireLoop(float dt)
{
	for (auto enemy : fireableArray)
	{
		if (enemy->missile){
			if (enemy->missile->getTag() == UNIT_MISSILE_DROPPABLE){
				droppableMissileArray.pushBack(enemy->missile);
				enemyMissileArray.pushBack(enemy->missile);
				
				enemy->missile = NULL;
			}
			else if (enemy->missile->getTag() == UNIT_MISSILE_STRAIGHT){
				straightMovingArray.pushBack(enemy->missile);
				enemyMissileArray.pushBack(enemy->missile);
				enemy->missile->secondTag = enemy->getTag();
				enemy->missile = NULL;
			}
			else if (enemy->missile->getTag() == UNIT_MISSILE_CUSTOM){
				customMovingArray.pushBack(enemy->missile);
				enemyMissileArray.pushBack(enemy->missile);
				enemy->missile->secondTag = enemy->getTag();
				enemy->missile = NULL;
			}
			else if (enemy->missile->getTag() == UNIT_MISSILE_CHASING){
				chasingMissileArray.pushBack(enemy->missile);
				log("** NOT bee hive");
				enemy->missile->secondTag = enemy->getTag();
				enemyMissileArray.pushBack(enemy->missile);
				enemy->missile = NULL;
			}
		}
	}
}

void HelloWorld::setPlayerInfo()
{
	player->extraSpeed = 60;

}

void HelloWorld::setWeaponInfo(int weapon){
	_weapon = weapon;
	player->setWeapon(weapon);
	playerWeaponType = weapon;

//	player->fireCoolTime = inteval;
}
void HelloWorld::talkUpdate(float dt)
{
	Sprite* sprite;
	Rect projectileRect;
	bool talkEncountered = false;
	bool shouldReset = true;
	for (int i = 0; i<talkArray.size(); i++) {
		sprite = (Sprite*)talkArray.at(i);

		if (sprite->getPosition().y - player->getPosition().y > 50 || sprite->getPosition().y - player->getPosition().y < -50) {
			continue;
		}

		projectileRect = cocos2d::Rect(sprite->getPosition().x - (sprite->getContentSize().width*sprite->getScale() / 2),
			sprite->getPosition().y - (sprite->getContentSize().height*sprite->getScale() / 2),
			sprite->getContentSize().width*sprite->getScale(),
			sprite->getContentSize().height*sprite->getScale());
		if (projectileRect.intersectsRect(player->collisionBoundingBox())) {
			shouldReset = false;
			if (currentTalkIndex != i) {

				currentTalkIndex = i;
				talkEncountered = true;
				GameManager::getInstance()->playSoundEffect(SOUND_COIN);
				break;
				//                GameManager::getInstance()->getHudLayer()->enableLayerAndMenus(false);
			}
		}
	}

	if (talkEncountered) {
		GameManager::getInstance()->getHudLayer()->showTalk(talkLabelArray.at(currentTalkIndex)->getString().c_str());
		//        log("encountered: %s", talkLabelArray.at(currentTalkIndex)->getString().c_str());
	}
	if (shouldReset) {
		currentTalkIndex = -1;
	}
}
void HelloWorld::changeSpeed(){
	_isFastMode = !_isFastMode;
}
void HelloWorld::gravityUpdate(float dt)
{
    if(_isGamePaused)
        return;
    
	if (_hudLayer->IsFastMode) _totalTime += dt/2;
	else _totalTime += dt;

	
	if (_isFastMode){
		//dt *= 16;
	}
	if (isPaused) {
		return;
	}

	if (dt > 0.03) {
		dt = 0.03;
	}

	//player->desiredPosition = player->getPosition();
	if (isGameOver) {
		return;
	}

	//    log("%d, %d, %d", (int)spriteBatch->getChildren().size(), (int)this->getChildren().size(), (int)tileMap->getChildren().size());
	//    log("left: %d, right: %d", GameManager::getInstance()->leftPressed, GameManager::getInstance()->rightPressed);
	bool left = GameManager::getInstance()->leftPressed;
	bool right = GameManager::getInstance()->rightPressed;
	bool jump = GameManager::getInstance()->jumpPressed || GameManager::getInstance()->upPressed;
	//GameManager::getInstance()->jumpPressed = false;
	if (player->jumped) GameManager::getInstance()->jumpPressed = false;

	GameManager::getInstance()->upPressed = false;


	if (player->freezed) {
		jump = false;
		left = false;
		right = false;
	}
	player->wantsToJump = jump;
	player->wantsToGoLeft = left;
	player->wantsToGoRight = right;
	if ((left || right) && player->onGround && !player->isFootStepStarted){
		player->startFootStep();
	}
	if ((!left && !right && player->isFootStepStarted) || jump || !player->onGround){
		player->stopFootStep();
	}

	if (!left && !right) {
		player->wantsToStop = true;
	}
	else{
		player->wantsToStop = false;
	}

	if (!playerIgnoreGravity) {
		//player->updatePosition(dt);
	}

	for (auto platform : movingPlatformArray)
	{
		platform->updatePosition(dt);
	}

	if (!isGameOver && !playerIgnoreGravity) {
		//checkForAndResolveCollisions(player);
	}

	if (!isGameOver) {
		for (auto drop : droppableArray){
			if (drop->ignoreGravity) {
				continue;
			}
			drop->updatePosition(dt);
			checkForAndResolveCollisions(drop);
		}
	}

	//    if (GameManager::getInstance()->downPressed && player->oneWayCancelY == 0) {
	//        player->oneWayCancelY = player->collisionBoundingBox().origin.y;
	//    }

	if (!isGameOver && player->getPosition().y < tileMap->getTileSize().height) {
		gameOver();
	}

	//this->setViewPointCenter(player->getPosition());
	this->setViewPoint(_viewPoint.x, _viewPoint.y);


	// fire
	playerFireCoolTime -= dt;
	if (playerFireCoolTime < -10) {
		playerFireCoolTime = -1;
	}

	if (GameManager::getInstance()->firePressed) {
		fire();
	}
	else{
//        if (player->weapon == WEAPON_GUN) {
//            playerFireCoolTime = 0;
//        }
	}

	for (auto drop : droppableMissileArray){
		drop->updatePosition(dt);
		if (checkDroppableMissileCollision(drop)) break;
	}


	this->chasingMissileUpdate(dt);
	this->enemyUpdate(dt);
	this->missileUpdate(dt);
	this->coinUpdate(dt);
	this->missileEffectUpdate(dt);
	this->enemyFireLoop(dt);
	this->gravityUpdateForStraight(dt);
}
void HelloWorld::coinMagnet(){
//	int range = 80;
	float x, y;
	Point pos = player->getPosition() + cocos2d::Point(8, 16);
	for (auto coin : coinArray){
		x = pos.x - coin->getPosition().x;
		y = pos.y - coin->getPosition().y;
		coin->setPosition(coin->getPosition() + cocos2d::Point(x > 0 ? 2 : -2, y > 0 ? 2 : -2));
	}
}
Sprite* HelloWorld::getTeleport(int teleport){
	for (auto tele : teleportArray){
		if (tele->getTag() == teleport)
			return tele;
	}
	return NULL;
}
void HelloWorld::gravityUpdateForFlyingOrSwimingEnemies(float dt)
{
	if (isPaused) {
		return;
	}
	if (dt > 0.05) {
		dt = 0.05;
	}

	for (auto drop : flyingSwimingEnemyArray){
		if (drop->currentMoveType == ENEMY_MOVE_FLY_TO_HERO_FOLLOW) {
			drop->updatePositionToFlyToFollowTarget(dt);
			checkForAndResolveCollisions(drop);
		}
		else if (drop->currentMoveType == ENEMY_MOVE_SWIM_TO_HERO_FOLLOW) {
			drop->updatePositionToFlyToFollowTarget(dt);

			checkForAndResolveCollisions(drop);

			//            checkForAndResolveCollisionsForWater(drop);
		}
	}
}

void HelloWorld::gravityUpdateForCoins(float dt)
{
	if (isPaused) {
		return;
	}
	if (dt > 0.05) {
		dt = 0.05;
	}

	for (auto drop : droppableCoinArray)
	{
		drop->updatePositionForBouncing(dt);
		//        checkForAndResolveCollisions(drop);
		checkForAndResolveCollisionsForBouncing(drop);
	}
}
void HelloWorld::gravityUpdateForStraight(float dt)
{
	if (isPaused) {
		return;
	}
	Vector<Droppable*> toRemove;
	Droppable* brokenMissile;
	for (auto drop : straightMovingArray)
	{
		drop->updatePositionForStraight(dt);
		brokenMissile = checkForAndResolveCollisionsForMissile(drop);
		if (brokenMissile != NULL){
			toRemove.pushBack(brokenMissile);
		}
	}
	for (auto missile : toRemove){
		straightMovingArray.eraseObject(missile);
		missile->stopAllActions();
		missile->removeFromParentAndCleanup(true);
	}
	toRemove.clear();
}
void HelloWorld::gravityUpdateForCustomMoving(float dt)
{
	if (isPaused) {
		return;
	}
	Vector<Droppable*> toRemove;
	Droppable* brokenMissile;
	for (auto drop : customMovingArray)
	{
		drop->desiredPosition = drop->getPosition();
		brokenMissile = checkForAndResolveCollisionsForMissile(drop);
		if (brokenMissile != NULL){
			toRemove.pushBack(brokenMissile);
		}
	}
	for (auto missile : toRemove){
		customMovingArray.eraseObject(missile);
		missile->stopAllActions();
		missile->removeFromParentAndCleanup(true);
	}
	toRemove.clear();
}
void HelloWorld::chasingMissileUpdate(float dt)
{
	Vector<Droppable*> toRemove;
	Droppable* brokenMissile;
	for (auto drop : chasingMissileArray)
	{
		if (drop->getTag() == UNIT_MISSILE_CHASING && drop->target != NULL) {
			float xGap = drop->getPosition().x - drop->target->getPosition().x;
			float yGap = drop->getPosition().y - drop->target->getPosition().y;

			double th = atan2(yGap, xGap);
			double angle = th * 180 / 3.14 + 180;
			float gap = angle - drop->movingAngle;
			drop->movingAngle += gap > 0 ? (gap > 180 ? -2 : 2) : (gap < -180 ? 2 : -2);
			//            drop->setRotation(-angle);
			drop->setRotation(-drop->movingAngle);
			if (drop->movingAngle < 0)
				drop->movingAngle += 360;
			if (drop->movingAngle > 360) {
				drop->movingAngle -= 360;
			}
			drop->setFlippedY(drop->movingAngle > 90 && drop->movingAngle < 270);
		}
		drop->updatePositionForStraight(dt);
		brokenMissile = checkForAndResolveCollisionsForMissile(drop);
		if (brokenMissile != NULL){
			toRemove.pushBack(brokenMissile);
		}
	}
	for (auto missile : toRemove){
		chasingMissileArray.eraseObject(missile);
		missile->stopAllActions();
		missile->removeFromParentAndCleanup(true);
	}
	toRemove.clear();
}
void HelloWorld::setViewPoint(float x, float y){
	int tileSize = 16;
	float scale = getScale();
//	float totalWidth = tileMap->getMapSize().width * tileMap->getTileSize().width;
	float totalHeight = tileMap->getMapSize().height * tileMap->getTileSize().height;
	
	Point actualPosition = cocos2d::Point(x*tileSize, totalHeight - y*tileSize);
	actualPosition *= scale;
	Point centerOfView = cocos2d::Point(size.width / 2, 0);
	Point viewPoint = centerOfView - actualPosition;
	this->setPosition(viewPoint);
}
void HelloWorld::setViewPointCenter(Point position)
{
	float scale = getScale();
	int x = MAX(position.x*scale, size.width / 2);
	float totalWidth = tileMap->getMapSize().width * tileMap->getTileSize().width;
	int maxX = (totalWidth)*scale - size.width;
	int y = MAX(position.y*scale, size.height / 2);
	float totalHeight = tileMap->getMapSize().height * tileMap->getTileSize().height;
	int maxY = (totalHeight)*scale - size.height;



	if (nearBack)
		nearBack->setPosition(Point((totalWidth - nearWidth)*(x - size.width / 2) / maxX, (totalHeight - nearHeight)*(y - size.height / 2) / maxY));
	if (farBack)
		farBack->setPosition(Point((totalWidth - farWidth)*(x - size.width / 2) / maxX, (totalHeight - farHeight)*(y - size.height / 2) / maxY));
	if (furtherBack)
		furtherBack->setPosition(Point((totalWidth - furtherWidth)*(x - size.width / 2) / maxX, (totalHeight - furtherHeight)*(y - size.height / 2) / maxY));

	//x = MIN(x, (tileMap->getMapSize().width * tileMap->getTileSize().width)*scale - size.width / (2*scale));
	//y = MIN(y, (tileMap->getMapSize().height * tileMap->getTileSize().height)*scale - size.height/(2*scale));
	Point actualPosition = cocos2d::Point(x, y);

	Point centerOfView = cocos2d::Point(size.width / 2, size.height / 2);
	Point viewPoint = centerOfView - actualPosition;
	this->setPosition(viewPoint);
}
Point HelloWorld::tileCoordForPosition(Point position)
{
	float x = floor(position.x / TILE_WIDTH);
	float levelHeightInPixels = tileMap->getMapSize().height * TILE_HEIGHT;
	float y = floor((levelHeightInPixels - position.y) / TILE_HEIGHT);
	return cocos2d::Point(x, y);
}

Rect HelloWorld::tileRectFromTileCoords(Point tileCoords)
{
	float levelHeightInPixels = tileMap->getMapSize().height * TILE_HEIGHT;
	Point origin = cocos2d::Point(tileCoords.x * TILE_WIDTH, levelHeightInPixels - ((tileCoords.y + 1) * TILE_HEIGHT));
	return cocos2d::Rect(origin.x, origin.y, TILE_WIDTH, TILE_HEIGHT);
}

void HelloWorld::menuCloseCallback(Ref* pSender)
{
	Director::getInstance()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
	exit(0);
#endif
}

void HelloWorld::registerScriptAccelerateHandler(int handler)
{
	//    Director::getInstance()->getEventDispatcher()->addTargetedDelegate(this, 0, true);
}

void HelloWorld::setPlayerPosition(Point position) {
	player->setPosition(position);
}

void HelloWorld::addListener(){
    // Register Touch Event
//    multitouchlistener = EventListenerTouchAllAtOnce::create();
    
//    multitouchlistener->onTouchesBegan = CC_CALLBACK_2(HelloWorld::onTouchesBegan, this);
//    multitouchlistener->onTouchesMoved = CC_CALLBACK_2(HelloWorld::onTouchesMoved, this);
//    multitouchlistener->onTouchesEnded = CC_CALLBACK_2(HelloWorld::onTouchesEnded, this);
//    multitouchlistener->onTouchesCancelled = CC_CALLBACK_2(HelloWorld::onTouchesCancelled, this);
    
//    _eventDispatcher->addEventListenerWithSceneGraphPriority(multitouchlistener, this);
    
	listener = cocos2d::EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);

	listener->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(HelloWorld::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(HelloWorld::onTouchEnded, this);
    
//    listener->onTouchCancelled = CC_CALLBACK_2(MenuLayerMainMenu::onTouchCancelled, this);

	_eventDispatcher->addEventListenerWithFixedPriority(listener, 1);
}
void HelloWorld::Dispose(){
	this->stopAllActions();
	this->unscheduleAll();
	_eventDispatcher->removeEventListener(listener);
}
void HelloWorld::Shoot(Point location){
//	Point playerPos = player->getPosition();
//	playerPos *= this->getScale();
//	Point playerPosOnScreen = playerPos + this->getPosition();
    if (_isGamePaused) {
        return;
    }
    int power = 1;
    int weaponType = _playerType;
    int secondTag = MISSILE_INDIAN;
    int extraSpeed = 80;
    bool isDary = false;
    if(weaponType == HERO_DARY){
        isDary = true;
        weaponType = cocos2d::random()%HERO_DARY;
    }
	const char* weaponName = "heroWeapon0_0.png";
    if(weaponType == HERO_INDIAN){
        power = 1;
    }else if(weaponType == HERO_JONNY){
        weaponName = "spike.png";
        extraSpeed = 120;
        power = 20;
    }else if(weaponType == HERO_AOKIZI){
        weaponName = "iceBall.png";
        secondTag = MISSILE_AOKIZI;
        power = 1 + UserDefault::getInstance()->getIntegerForKey(KEY_AOKIZI_LEVEL, 0);
    }else if(weaponType == HERO_HERMIONE){
        weaponName = "fire0_0.png";
        secondTag = MISSILE_HERMIONE;
        power = 3;
    }else if(weaponType == HERO_DARY){
        
    }
    if (isDary) {
        power = 1+  UserDefault::getInstance()->getIntegerForKey(KEY_DARY_LEVEL, 0);
    }

	Droppable* missile = Droppable::createDroppable(UNIT_MISSILE_STRAIGHT, power, extraSpeed, weaponName);
	missile->isFregile = true;
	missile->setPosition(player->getPosition());
    double angle = GameManager::getInstance()->getAngle(location, player->getPosition());//location, playerPosOnScreen);
    if(weaponType == HERO_INDIAN ||
       weaponType == HERO_AOKIZI){
        missile->setRotation(cocos2d::random()%720 - 360);
        missile->runAction(RepeatForever::create(RotateBy::create(0.3, cocos2d::random()%720 - 360)));
    }else{
        missile->setRotation(-angle + 90);
    }
    if(weaponType == HERO_HERMIONE){
        missile->setRotation(-angle - 90);
        missile->runAnimation("fire", true);
        missile->setScale(1.5f);
    }
    missile->secondTag = secondTag;
	missile->movingAngle = angle;
	spriteBatch->addChild(missile);
	straightMovingArray.pushBack(missile);
	heroMissileArray.pushBack(missile);
    player->setFlippedX(player->getPositionX() > _sptTarget->getPositionX());

    shootFilter++;
    
    if (shootFilter < 2) {
        return;
    }
    shootFilter = 0;
    for (int i = 0; i < 5; i++) {
        if (friends.size() <= i) {
            break;
        }
        Hero* hero = friends.at(i);
        const char* weaponName = "heroWeapon2_0.png";
        
        
        int index = 0;
        if(hero->getTag() == HERO_BREE){
            index = 0;
            weaponName = "heroWeapon2_0.png";
        }else if(hero->getTag() == HERO_TOM){
            index = 1;
            weaponName = "heroWeapon13_0.png";
            extraSpeed = 120;
        }else if(hero->getTag() == HERO_KAREN){
            extraSpeed = 70;
            index = 2;
            weaponName = "heroWeapon0_0.png";
        }else if(hero->getTag() == HERO_KARL){
            extraSpeed = 100;
            index = 3;
            weaponName = "heroWeapon3_0.png";
        }else if(hero->getTag() == HERO_ANDREW){
            extraSpeed = 110;
            index = 4;
            weaponName = "heroWeapon11_0.png";
        }
        char levelBuf[30];
        sprintf(levelBuf, KEY_FRIEND_LEVEL_FORMAT, index);
        int level = UserDefault::getInstance()->getIntegerForKey(levelBuf);
        power = 1 + level;
        
        Droppable* missile = Droppable::createDroppable(UNIT_MISSILE_STRAIGHT, power, extraSpeed, weaponName);
        missile->isFregile = true;
        missile->setPosition(hero->getPosition());
        double angle = GameManager::getInstance()->getAngle(location + Point((index - 2)*5, 0), hero->getPosition());//location, playerPosOnScreen);
        missile->setRotation(cocos2d::random()%720 - 360);
        missile->runAction(RepeatForever::create(RotateBy::create(0.3, cocos2d::random()%720 - 360)));

        missile->secondTag = hero->getTag();
        missile->movingAngle = angle;
        spriteBatch->addChild(missile);
        straightMovingArray.pushBack(missile);
        heroMissileArray.pushBack(missile);
        hero->setFlippedX(hero->getPositionX() > _sptTarget->getPositionX());
    }
    
    if(_shootTutorialStarted && _shootTutorialCount == 3){
        _shootTutorialCount++;
        _hudLayer->HideTutrorialHelper();
        resumeLayer();
    }
}
void HelloWorld::onTutorialClosed(){
    if(isGameOver || _waveCount == _checkPointStageCount + 1){
        resumeLayer();
    }

    if(_messageBox != NULL){
        _messageBox->removeFromParentAndCleanup(true);
        _messageBox = NULL;
    }
    bool shoot  = UserDefault::getInstance()->getBoolForKey(KEY_TUTORIAL_SHOOT, false);
    bool trap   = UserDefault::getInstance()->getBoolForKey(KEY_TUTORIAL_TRAP, false);
    bool upgrd  = UserDefault::getInstance()->getBoolForKey(KEY_TUTORIAL_UPGRADE, false);
    bool gOver  = UserDefault::getInstance()->getBoolForKey(KEY_TUTORIAL_GAMEOVER, false);
    bool check  = UserDefault::getInstance()->getBoolForKey(KEY_TUTORIAL_CHECKPOINT, false);
    bool done   = UserDefault::getInstance()->getBoolForKey(KEY_TUTORIAL_DONE, false);
    if(shoot && trap && upgrd && gOver && check && !done){
        UserDefault::getInstance()->setBoolForKey(KEY_TUTORIAL_DONE, true);
        showTutorial(LanguageManager::getInstance()->getStringForKey(STR_tutorialDone).c_str());
        GameManager::getInstance()->currentScene = SCENE_GAME_TUTORIAL;
    }
}

void HelloWorld::showTutorial(const char* msg){
    if(_messageBox != NULL){
        _messageBox->removeFromParentAndCleanup(true);
        _messageBox = NULL;
    }
    pauseLayer();
    _messageBox = CSLoader::createNode("MessageBoxScene.csb");
    Node* panel = _messageBox->getChildByName("panel");
    _hudLayer->addChild(_messageBox, 1000);

    Button* btn = (Button*)panel->getChildByName("close");
    btn->addClickEventListener(CC_CALLBACK_0(HelloWorld::onTutorialClosed, this));
    btn = (Button*)panel->getChildByName("ok");
    btn->addClickEventListener(CC_CALLBACK_0(HelloWorld::onTutorialClosed, this));
    Text* lbl = (Text*)_messageBox->getChildByName("label");
    lbl->setFontName(LanguageManager::getInstance()->getStringForKey(STR_font));
    lbl->setString(msg);
}
bool HelloWorld::onTouchBegan(Touch *touch, Event *unused_event){
	Point location = touch->getLocationInView();
	location = Director::getInstance()->convertToGL(location);
    log("touch");
    touchBegan(location);
    return true;
}
void HelloWorld::onTouchMoved(Touch *touch, Event *unused_event){
    Point location = touch->getLocationInView();
    location = Director::getInstance()->convertToGL(location);
    touchMoved(location);
}
void HelloWorld::onTouchEnded(Touch *touch, Event *unused_event){
    Point location = touch->getLocationInView();
    location = Director::getInstance()->convertToGL(location);
    touchEnded(location);
}
void HelloWorld::onTouchesBegan(const std::vector<Touch*>& touches, Event *unused_event)
{
    Touch *touch = touches.at(0);
    cocos2d::Point location = touch->getLocationInView();
    location = Director::getInstance()->convertToGL(location);

//    touchBegan(location);
//	return true;
}

void HelloWorld::onTouchesMoved(const std::vector<Touch*>& touches, Event *unused_event){
    
    Touch *touch = touches.at(0);
    cocos2d::Point location = touch->getLocationInView();
    location = Director::getInstance()->convertToGL(location);
//    touchMoved(location);
}
void HelloWorld::onTouchesEnded(const std::vector<Touch*>& touches, Event *unused_event){
    Touch *touch = touches.at(0);
    cocos2d::Point location = touch->getLocationInView();
    location = Director::getInstance()->convertToGL(location);
 
//    touchEnded(location);
}

void HelloWorld::touchBegan(Point location){
    /*
    float x = _viewPoint.x;
    float y = _viewPoint.y;
    if(location.y > size.height/2){
        if(location.x < size.width/2){
            y += 0.1f;
        }else{
            y -= 0.1f;
        }
    }else{
        if(location.x < size.width/2){
            //            _scale += 0.1f;
        }else{
            //            _scale -= 0.1f;
        }
    }
    _viewPoint = cocos2d::Point(x, y);
    
    setViewPoint(_viewPoint.x, _viewPoint.y);
    setScale(_scale);
    
    log("x: %f, y: %f", x, y);
    log("scale: %f", _scale);
    */
    if (!isTouchEnded) {
        return;
    }
    isTouchEnded = false;
    
    _touchStart = location;
    _touchPoint = location;
    if(!this->isPaused){
        this->unschedule(schedule_selector(HelloWorld::resetTouchStart));
        this->scheduleOnce(schedule_selector(HelloWorld::resetTouchStart), 0.5);
    }
    _isActoinDone = false;
    if (guideLayer->isVisible()){
        cocos2d::Point posInStage = location - this->getPosition();
        cocos2d::Point scaledPos = cocos2d::Point(posInStage.x / this->getScale(), posInStage.y / this->getScale());
        cocos2d::Point pos = tileCoordForPosition(scaledPos);
        
        
        resumeLayer();
        if (guideLayer->getTileGIDAt(pos) == 161 || guideLayer->getTileGIDAt(pos) == 165){
            AddTrap(pos, _guidingTrapType, 0, false);
            
            _trapTurorial = false;
            GameManager::getInstance()->showParticleExplosion(this, "coin0.png", scaledPos, 16, 1, Widget::TextureResType::PLIST);
            GameManager::getInstance()->playSoundEffect(SOUND_DDALKACK);
            GameManager::getInstance()->playSoundEffect(SOUND_COIN);
            GameManager::getInstance()->getHudLayer()->HideTrapInfo();
            UserDefault::getInstance()->flush();
            GameManager::getInstance()->playSoundEffect(SOUND_DDALKACK);
            
        }
        else{
            if(_trapTurorial){
                //                showTutorial(LanguageManager::getInstance()->getStringForKey("tutorialTrap").c_str());
                pauseLayer();
                _hudLayer->ShowTutorialHelper(0, cocos2d::Point::ZERO);
            }
            _hudLayer->ShowWrongRect();
            GameManager::getInstance()->playSoundEffect(SOUND_ROCKEY_LAUNCH);
        }
        GameManager::getInstance()->currentScene = SCENE_IN_GAME;
        _isActoinDone = true;
    }
    else{
        cocos2d::Point posInStage = location - this->getPosition();
        cocos2d::Point scaledPos = cocos2d::Point(posInStage.x / this->getScale(), posInStage.y / this->getScale());
        cocos2d::Point pos = tileCoordForPosition(scaledPos);
        
        bool exist = false;
        for (auto trap : trapArray){
            if (trap->TilePos == pos){
                exist = true;
                break;
            }
        }
        if (exist && _touchStart.x > 0){    // upgrade
            float cX = (pos.x*TILE_SIZE + TILE_SIZE / 2)*this->getScale() + this->getPosition().x;
            float cY = ((tileMap->getMapSize().height - pos.y - 1)*TILE_SIZE + TILE_SIZE / 2)*this->getScale() + this->getPosition().y;
            
            GameManager::getInstance()->getHudLayer()->ShowUpgradeLayer(location, pos, cocos2d::Point(cX, cY - this->getScale()));
        }else{  // shoot
            
            cocos2d::Point targetPos = cocos2d::Point((location.x - this->getPositionX())/this->getScale(), (location.y + _targetGap - this->getPositionY())/this->getScale());
            _sptTarget->setVisible(true);
            _sptTarget->setPosition(targetPos);
            Shoot(_sptTarget->getPosition());
            resumeLayer();
            _isDown = true;
            _touchStart = location;
            float interval = 0.2f;
            if(_playerType == HERO_INDIAN){
                
            }else if(_playerType == HERO_JONNY){
                interval = 0.3f;
            }else if(_playerType == HERO_AOKIZI){
                interval = 0.3f;
            }else if(_playerType == HERO_HERMIONE){
                interval = 0.3f;
            }else if(_playerType == HERO_DARY){
                interval = 0.2f;
            }
            this->schedule(schedule_selector(HelloWorld::ShootingForHold), interval);
            _hudLayer->HideTrapInfo();
        }
    }
    UserDefault::getInstance()->flush();
    guideLayer->setVisible(false);
    this->unschedule(schedule_selector(HelloWorld::BlinkGuideLayer));
}
void HelloWorld::touchMoved(Point location){
   cocos2d::Point targetPos = cocos2d::Point((location.x - this->getPositionX())/this->getScale(), (location.y + _targetGap - this->getPositionY())/this->getScale());
    _sptTarget->setPosition(targetPos);
    
    _touchPoint = location;
    //_isDown = true;
    //touchStart = location;
}
void HelloWorld::touchEnded(Point location){
    _sptTarget->setVisible(false);
    this->unschedule(schedule_selector(HelloWorld::ShootingForHold));
    /*
    if (abs((int)_touchStart.x - (int)location.x) < 16 && abs((int)_touchStart.y - (int)location.y) < 16 && _isDown && !_isActoinDone){
        cocos2d::Point posInStage = location - this->getPosition();
        cocos2d::Point scaledPos = cocos2d::Point(posInStage.x / this->getScale(), posInStage.y / this->getScale());
        cocos2d::Point pos = tileCoordForPosition(scaledPos);
        
        bool exist = false;
        for (auto trap : trapArray){
            if (trap->TilePos == pos){
                exist = true;
                break;
            }
        }
        if (exist && _touchStart.x > 0){
            float cX = (pos.x*TILE_SIZE + TILE_SIZE / 2)*this->getScale() + this->getPosition().x;
            float cY = ((tileMap->getMapSize().height - pos.y - 1)*TILE_SIZE + TILE_SIZE / 2)*this->getScale() + this->getPosition().y;
            
            GameManager::getInstance()->getHudLayer()->ShowUpgradeLayer(location, pos, cocos2d::Point(cX, cY - this->getScale()));
        }
    }*/
    isTouchEnded = true;
    _isDown = false;
}
void HelloWorld::ShootingForHold(float dt)
{
    if(_shootTutorialCount < 3){
        _shootTutorialCount++;
        if(_shootTutorialCount == 3){
            GameManager::getInstance()->currentScene = SCENE_IN_GAME;
            startWaveSchedule();
            resumeLayer();
        }
    }
	Shoot(_sptTarget->getPosition());
}
void HelloWorld::LoadPreviousTraps(){
	int startX = StartPos.x;
	int startY = StartPos.y;
	int trapType; int trapLevel;
	Point pos = cocos2d::Point(startX, startY + 1);
	char buf[100];
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

		sprintf(buf, KEY_INSTALLED_TRAP_LEVEL_FORMAT, _stageMode, GetCoordinateKey(pos).c_str());
		trapLevel = UserDefault::getInstance()->getIntegerForKey(buf, 0);
		
		if (trapType > -1){
			AddTrap(pos, trapType, trapLevel, true);
		}
	}
}

void HelloWorld::ResetPreviousTraps(){
	int startX = StartPos.x;
	int startY = StartPos.y;
	Point pos = cocos2d::Point(startX, startY + 1);
	char buf[100];
	char buff[100];
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
		UserDefault::getInstance()->setIntegerForKey(buf, -1);
		
		sprintf(buff, KEY_INSTALLED_TRAP_LEVEL_FORMAT, _stageMode, GetCoordinateKey(pos).c_str());
		UserDefault::getInstance()->setIntegerForKey(buff, 0);
	}
}
void HelloWorld::AddTrap(Point pos, int trapType, int trapLevel, bool isFree)
{
    if (UserDefault::getInstance()->getBoolForKey(KEY_TRAP_USED, false) == false) {
        UserDefault::getInstance()->setBoolForKey(KEY_TRAP_USED, true);
    }
    if (trapType == TRAP_CACTUS) {
        UserDefault::getInstance()->setBoolForKey(KEY_ANTIAIR_USED, true);
    }
	Point extraPos;
	Point posInStageForTilePos = cocos2d::Point(pos.x*TILE_SIZE + TILE_SIZE / 2, (tileMap->getMapSize().height - pos.y)*TILE_SIZE - TILE_SIZE / 2);
	Trap* trap;
	if (trapType == TRAP_GASBURNER){
		trap = GasBurner::create("gasBurner.png", 0.3f);
		trap->SetAttackRect(posInStageForTilePos.x - TILE_SIZE / 2, posInStageForTilePos.y + TILE_SIZE / 2, TILE_SIZE, TILE_SIZE / 2);
		extraPos = cocos2d::Point(0, 3);

		Sprite* sptBack = Sprite::createWithSpriteFrameName("whiteSquare.png");
		sptBack->setColor(Color3B(40, 34, 27));
		trap->addChild(sptBack);
		sptBack->setLocalZOrder(-1);
		sptBack->setPosition(Point(trap->getContentSize().width / 2, trap->getContentSize().height / 2));
		sptBack->setScaleX(trap->getContentSize().width / sptBack->getContentSize().width);
		sptBack->setScaleY(trap->getContentSize().height / sptBack->getContentSize().height);
	}
	else if (trapType == TRAP_FREEZER){
		trap = Freezer::create("fridge.png", 2.3f);
		trap->SetAttackRect(posInStageForTilePos.x - TILE_SIZE / 2, posInStageForTilePos.y - TILE_SIZE / 2, TILE_SIZE, TILE_SIZE);
		trap->setLocalZOrder(-10);
		extraPos = cocos2d::Point(0, -1);
	}
	else if (trapType == TRAP_FIRE){
		trap = Fire::create("fire0_0.png", 0.3f);
		trap->SetAttackRect(posInStageForTilePos.x - TILE_SIZE / 2, posInStageForTilePos.y - TILE_SIZE / 2, TILE_SIZE, TILE_SIZE);
	}
	else if (trapType == TRAP_SPIKE){
		trap = Spike::create("spike.png", 1.0f);
        trap->setAnchorPoint(Point(0.5, 0));
		trap->SetAttackRect(posInStageForTilePos.x - TILE_SIZE / 4, posInStageForTilePos.y - TILE_SIZE / 2, TILE_SIZE/2, TILE_SIZE);
        trap->runAction(RepeatForever::create(Sequence::create(DelayTime::create(1), ScaleTo::create(0.3f, 1.3f, 2.0f), ScaleTo::create(0.4f, 1.8f, 1.3f), ScaleTo::create(0.5f, 1.5f, 1.5f), NULL)));
		extraPos = cocos2d::Point(0, -9);
	}
	else if (trapType == TRAP_TOPSPIKE){
		trap = TopSpike::create("topSpike.png", 2.0f);
//		trap->CooltimeLeft = 2;
		trap->IsReady = false;
		extraPos = cocos2d::Point(0, 8 - trap->getContentSize().height / 2);
	}
	else if (trapType == TRAP_ICEBALL){
		trap = IceBall::create("iceBall.png", 1.5f);
	}
	else if (trapType == TRAP_BOMB){
		trap = Bomb::create("bomb2.png", 1.0f);
		extraPos = cocos2d::Point(0, -8 + trap->getContentSize().height / 2);
		trap->SetAttackRect(posInStageForTilePos.x - TILE_SIZE / 2, posInStageForTilePos.y - TILE_SIZE / 2, TILE_SIZE, TILE_SIZE);
	}
	else if (trapType == TRAP_MACEBALL){
		trap = MaceBall::create("maceBall.png", 1.0f);
		trap->SetAttackRect(posInStageForTilePos.x - TILE_SIZE / 2, posInStageForTilePos.y - TILE_SIZE / 2, TILE_SIZE, TILE_SIZE);
		extraPos = cocos2d::Point(0, 10);
	}
	else if (trapType == TRAP_FLASK){
		trap = Flask::create("flask.png", 1.0f);
		trap->setTag(UNIT_MISSILE_DROPPABLE);
		trap->secondTag = TRAP_FLASK;
		trap->weight = 0.3f;
		trap->velocity.y = 200;
		droppableMissileArray.pushBack(trap);
		trap->runAction(RepeatForever::create(RotateBy::create(0.7, 500)));
		if (pos.x == EndPos.x)
			extraPos = cocos2d::Point(-1, 0);
	}
	else if (trapType == TRAP_WALL){
		trap = Wall::create("wall.png", 1.0f);
		trap->SetAttackRect(posInStageForTilePos.x - TILE_SIZE / 2, posInStageForTilePos.y - TILE_SIZE / 2, TILE_SIZE, TILE_SIZE);
		extraPos = cocos2d::Point(0, -8 + trap->getContentSize().height / 2);
		trap->maxEnergy += trapLevel * 6;
		trap->energy = trap->maxEnergy;
	}
	else if (trapType == TRAP_HAND){
		trap = Hand::create("hand4.png", 3.3f);
		trap->SetAttackRect(posInStageForTilePos.x - TILE_SIZE / 4, posInStageForTilePos.y - TILE_SIZE / 2, TILE_SIZE/2, TILE_SIZE);
		trap->setLocalZOrder(-10);
		extraPos = cocos2d::Point(0, 0);
	}
	else if (trapType == TRAP_WALLSPIKE){
		trap = WallSpike::create("spike.png", 2.5f);
		trap->setOpacity(0);
		((WallSpike*)trap)->SetDirection(pos.x == StartPos.x);
		trap->SetAttackRect(posInStageForTilePos.x - TILE_SIZE*1.5f / 2, posInStageForTilePos.y - TILE_SIZE / 2, TILE_SIZE*2.0f, TILE_SIZE);
		extraPos = cocos2d::Point(0, 0);
    }
    else if (trapType == TRAP_CACTUS){
        trap = Cactus::create("heroWeapon18_0.png", 0.5f);
        trap->setRotation(-90);
        trap->setAnchorPoint(Point(0, 0.5));
        trap->runAction(RepeatForever::create(Sequence::create(ScaleTo::create(0.2f, 1.5f, 1.5f),ScaleTo::create(0.1f, 1.3f, 2.0f), ScaleTo::create(0.2f, 1.8f, 1.3f),  NULL)));
        extraPos = cocos2d::Point(0, -8);
    }

	trapArray.pushBack(trap);
	trap->setPosition(posInStageForTilePos + extraPos);
	trap->OnPlaced();
	trap->Level = trapLevel;
	trap->TilePos = pos;
	spriteBatch->addChild(trap);

	int price = GameManager::getInstance()->GetTrapPrice(trapType);
	trap->PriceValue = price + trapLevel * 20;
	if (!isFree){
		CollectedCoinCount -= price;
        coinSpent += price;
        int totalCoinSpent = UserDefault::getInstance()->getIntegerForKey(KEY_TOTAL_COIN_SPENT, 0);
        totalCoinSpent += coinSpent;

        UserDefault::getInstance()->setIntegerForKey(KEY_TOTAL_COIN_SPENT, totalCoinSpent);
        coinSpent = 0;
	}
	GameManager::getInstance()->AddTrapCount(trapType);
	
	GameManager::getInstance()->getHudLayer()->UpdateTrapButton();
    GameManager::getInstance()->getHudLayer()->LblCoinCount->setString(cocos2d::Value(CollectedCoinCount).asString());
    UserDefault::getInstance()->setIntegerForKey(KEY_COIN_LEFT, CollectedCoinCount);
    UserDefault::getInstance()->flush();

	SaveTrap(pos, trap->TrapType, trap->Level);
}
void HelloWorld::SaveTrap(Point pos, int trapType, int level)
{
	char buf[100];
	sprintf(buf, KEY_INSTALLED_TRAP_FORMAT, _stageMode, GetCoordinateKey(pos).c_str());
	UserDefault::getInstance()->setIntegerForKey(buf, trapType);
	log("save_%s,%d", buf, trapType);
	
	sprintf(buf, KEY_INSTALLED_TRAP_LEVEL_FORMAT, _stageMode, GetCoordinateKey(pos).c_str());
	UserDefault::getInstance()->setIntegerForKey(buf, level);
	log("%s,%d", buf, level);
}
std::string HelloWorld::GetCoordinateKey(Point pos){
	char buf[20];
	sprintf(buf, "%d_%d", (int)pos.x, (int)pos.y);
	return buf;
}
bool HelloWorld::IsOccupied(int category, cocos2d::Point pos){
	for (auto trap : trapArray){
		if (pos == trap->TilePos){
			return true;
		}
	}
	return false;
}

void HelloWorld::SellTrap(Point tilePos){
	Trap* trapToRemove = NULL;
	for (auto trap : trapArray){
		if (trap->TilePos == tilePos){
			trapToRemove = trap;
			break;
		}
	}

	if (trapToRemove != NULL){
		GameManager::getInstance()->RemoveTrapCount(trapToRemove->TrapType);
		trapArray.eraseObject(trapToRemove);
		trapToRemove->removeFromParentAndCleanup(true);
		_hudLayer->UpdateTrapButton();
	}
}

Trap* HelloWorld::GetTrap(Point tilePos){
	for (auto trap : trapArray){
		if (trap->TilePos == tilePos){
			return trap;
		}
	}
	return NULL;
}

int HelloWorld::GetTrapLevel(Point tilePos){
	for (auto trap : trapArray){
		if (trap->TilePos == tilePos){
			return trap->Level;
		}
	}
	return -1;
}
int HelloWorld::GetTrapType(Point tilePos){
	for (auto trap : trapArray){
		if (trap->TilePos == tilePos){
			return trap->TrapType;
		}
	}
	return -1;
}
void HelloWorld::UpgradeTrap(Point tilePos){
	for (auto trap : trapArray){
		if (trap->TilePos == tilePos){
			trap->Level++;
			trap->PriceValue += 20;
			char buf[20];
			if (trap->TrapType == TRAP_WALL){
				trap->maxEnergy += 6;
				trap->energy = trap->maxEnergy;
				trap->showEnergy();
			}
			sprintf(buf, KEY_INSTALLED_TRAP_LEVEL_FORMAT, _stageMode, GetCoordinateKey(tilePos).c_str());
			UserDefault::getInstance()->setIntegerForKey(buf, trap->Level);
            
            int totalCoinSpent = UserDefault::getInstance()->getIntegerForKey(KEY_TOTAL_COIN_SPENT, 0);
            totalCoinSpent += coinSpent;

            UserDefault::getInstance()->setIntegerForKey(KEY_TOTAL_COIN_SPENT, totalCoinSpent);
            UserDefault::getInstance()->setIntegerForKey(KEY_COIN_LEFT, CollectedCoinCount);
            UserDefault::getInstance()->flush();
            coinSpent = 0;
			break;
		}
	}
}
void HelloWorld::resetTouchStart(float dt){
	_touchStart = cocos2d::Point(-1, size.height);
}
void HelloWorld::solveCollision(Droppable* p, cocos2d::Point pos, cocos2d::Rect rect){

}
bool HelloWorld::IsOneWay(int id){
	id--;
	if ((id > 63 && id < 90) ||
		(id > 108 && id < 122)  ||
		(id > 172 && id < 186))
	{
		return true;
	}
	return false;
}
bool HelloWorld::IsBlock(int id){
	id--;
    
    if(id == 128) return true;
    
	if ((id > 95 && id < 106)||
		(id > 204 && id < 218)){
		return true;
	}
	return false;
}
float HelloWorld::checkTop(Droppable* p){
	return 0;
}
float HelloWorld::checkBottom(Droppable* p){
	cocos2d::Rect rect;
	Point pos;
	Point plPos;
	Point newPlPos;
	bool ground = false;
	bool escape = false;
	float temp = -1;
	// check bottom
	rect = p->collisionBoundingBox();
	pos = rect.origin + cocos2d::Point(rect.size.width, 0);
	do{
		if (pos.x <= rect.origin.x){
			pos.x = rect.origin.x; escape = true;
		}
		if (p->velocity.y > 0)break;
		plPos = this->tileCoordForPosition(pos);
		int tgid = stageLayer->getTileGIDAt(plPos);
		if (tgid > 0){
			cocos2d::Rect tileRect = this->tileRectFromTileCoords(plPos);
			cocos2d::Rect inter = intersection(tileRect, rect);
            if (inter.size.width > 0) {
                if (IsOneWay(tgid)){
                    if (p->oneWayCancelY == plPos.y){
                        p->onGround = false;
                        break;
                    }
                }
                bool validGround = false;
                if (IsBlock(tgid))
                {
                    validGround = true;
                }
                if (IsOneWay(tgid)){
                    Point currentPos = p->getPosition();
                    Point origPos = pos + cocos2d::Point(0, currentPos.y - p->desiredPosition.y + 3);
                    Point newTilePos = this->tileCoordForPosition(origPos);
                    if (newTilePos.y < plPos.y){
                        validGround = true;
                    }
                    else{
                        validGround = false;
                    }
                }
                if (validGround){
                    temp = inter.size.height;
                    ground = true;
                    p->velocity.y = 0;
                }
                break;
            }
		}
		else{
			
		}
		pos = pos - cocos2d::Point(TILE_SIZE / 2, 0);
	} while (!escape);

	//    return ground;
	return temp;
}
void HelloWorld::checkForAndResolveCollisions(Droppable* p){
	cocos2d::Rect rect;
	Point pos;
	Point plPos;
	Point newPlPos;
	bool ground = false;
	bool escape = false;
	float tempTotal = 0;
	float temp = -1;

	//    ground = checkBottom(p);
	temp = checkBottom(p);
	if (temp > 0){
		ground = true;
		tempTotal += temp;
	}

	// check top
	escape = false;
	int xOffset = 2;
	rect = p->collisionBoundingBox();
	pos = rect.origin + cocos2d::Point(xOffset, rect.size.height);
	do{
		if (pos.x >= rect.origin.x + rect.size.width - xOffset){
			pos.x = rect.origin.x + rect.size.width - xOffset; escape = true;
		}
		if (p->velocity.y < 0)break;

		plPos = this->tileCoordForPosition(pos);
        if (plPos.y < 0) {
            removeEnemy((EnemyBase*)p);
            return;
        }
		int tgid = stageLayer->getTileGIDAt(plPos);
		if (tgid > 0){
			if (!IsOneWay(tgid)){
				Rect tileRect = this->tileRectFromTileCoords(plPos);
				Rect inter = intersection(tileRect, rect);

				if (inter.size.width > 3 && inter.size.height > 0 && inter.size.width > inter.size.height){
					if (IsBlock(tgid)) {
						p->desiredPosition.y -= inter.size.height;
						p->velocity.y = 0;
						break;
					}
				}
			}
		}
		pos = pos + cocos2d::Point(TILE_SIZE / 2, 0);
	} while (!escape);

	// check left
	escape = false;
	rect = p->collisionBoundingBox();
	if (tempTotal > 0){
		rect.origin.y += tempTotal;
	}
	pos = rect.origin + cocos2d::Point(0, xOffset);
	do{
		if (pos.y >= rect.origin.y + rect.size.height - xOffset){
			pos.y = rect.origin.y + rect.size.height - xOffset; escape = true;
		}
		if (p->velocity.x > 0) break;
		plPos = this->tileCoordForPosition(pos);
		int tgid = stageLayer->getTileGIDAt(plPos);
		if (tgid > 0){
			if (IsBlock(tgid)){
				Rect tileRect = this->tileRectFromTileCoords(plPos);
				Rect inter = intersection(tileRect, rect);
				if (inter.size.width > 0 && inter.size.height > 0) {
					p->desiredPosition.x += inter.size.width;

					//                        ground = checkBottom(p);
					temp = checkBottom(p);
					if (tempTotal > 0 && temp < 0){
						tempTotal = 0;
						ground = false;
					}


					if (p->isTowardLeft){
						p->encounteredWall();
						
					}

					break;
				}
			}
		}
		pos = pos + cocos2d::Point(0, TILE_SIZE / 2);
	} while (!escape);

	// check right
	escape = false;
	rect = p->collisionBoundingBox();
	if (tempTotal > 0){
		rect.origin.y += tempTotal;
	}
	pos = rect.origin + cocos2d::Point(rect.size.width, xOffset);
	do{
		if (pos.y >= rect.origin.y + rect.size.height - xOffset){
			pos.y = rect.origin.y + rect.size.height - xOffset; escape = true;
		}
		if (p->velocity.x < 0)break;
		plPos = this->tileCoordForPosition(pos);
		int tgid = stageLayer->getTileGIDAt(plPos);
		if (tgid > 0){
			if (IsBlock(tgid)) {
				Rect tileRect = this->tileRectFromTileCoords(plPos);
				Rect inter = intersection(tileRect, rect);
				if (inter.size.width > 0 && inter.size.height > 0){
					p->desiredPosition.x -= inter.size.width;

					temp = checkBottom(p);
					if (tempTotal > 0 && temp < 0){
						tempTotal = 0;
						ground = false;
					}

					if (!p->isTowardLeft){
						p->encounteredWall();
					}
					break;
				}
			}
		}

		pos = pos + cocos2d::Point(0, TILE_SIZE / 2);
	} while (!escape);



	if (p == player){
		for (auto platform : movingPlatformArray)
		{
			Rect pRect = p->collisionBoundingBox();
			//        cocos2d::Rect pCurrentRect = player->currentBoundingBox();
			//            cocos2d::Rect platformRect = platform->getBoundingBox();
			Rect platformRect = platform->desiredRect;
			Rect intersectionR = intersection(pRect, platformRect);
			//        cocos2d::Rect intersectionWithCurrent  = intersection(pCurrentRect, platformRect);
			bool intersects = intersectionR.size.width > 0 && intersectionR.size.height > 0 && p->velocity.y < 0;
			if (intersects &&
				(platform == p->currentPlatform || (platform->desiredRect.origin.y + platform->desiredRect.size.height / 2 < p->desiredPosition.y && p->velocity.y < 0))){
				p->desiredPosition = cocos2d::Point(p->desiredPosition.x, p->desiredPosition.y + intersectionR.size.height);
				platform->player = player;
				if (platform->isTriggerPlatform && p == player) {
					platform->isTriggered = true;
				}
				p->currentPlatform = platform;
				p->velocity = cocos2d::Point(p->velocity.x, -0.0001);
				ground = true;
			}

			Point diff = platform->desiredRect.origin - platform->getBoundingBox().origin;
			platform->setPosition(platform->getPosition() + diff);
		}
	}

	if (p->onGround && !ground){
		//        if (p->getPositionY() == p->desiredPosition.y) {
		//            ground = true;
		//        }
	}

	if (ground){
		if (!p->onGround){
			p->onLanded();
		}
		p->velocity.y = -0.001;
		p->desiredPosition.y += tempTotal;
	}

	p->onGround = ground;
	p->setPosition(p->desiredPosition);
}

bool HelloWorld::checkDroppableMissileCollision(Droppable* p){
	//    bool ground = false;
	Point current = p->getPosition();
	Rect pRect = p->collisionBoundingBox(); //2

	Point plPos = tileCoordForPosition(p->getPosition());
	int tgid = stageLayer->getTileGIDAt(plPos);
	if (p->isFregile){
		if (tgid >= 0) {
			if (IsBlock(tgid) /*|| (p->velocity.y < 0 && IsOneWay(tgid))*/) {
				runEffect(EFFECT_EXPLODE_SMALL, p->getPosition());
				droppableMissileArray.eraseObject(p);
				heroMissileArray.eraseObject(p);
				enemyMissileArray.eraseObject(p);
				enemyArray.eraseObject((EnemyBase*)p);
				p->stopAllActions();
				p->unscheduleAllCallbacks();
				p->removeFromParent();
				return true;
			}
		}
	}
	else{
		if (p->velocity.y <= 0){
			float colHeight = checkBottom(p);
			if (colHeight > 0){
				if (p->secondTag == TRAP_FLASK){
					RemoveTrapRecord(((Trap*)p)->TilePos);
					splashDemage(p->getPosition(), 20, ((Trap*)p)->GetPower());
					runEffect(EFFECT_EXPLODE_HUGE, p->getPosition());
					trapArray.eraseObject((Trap*)p);
					heroMissileArray.eraseObject(p);
					droppableMissileArray.eraseObject(p);
					p->stopAllActions();
					p->removeFromParentAndCleanup(true);
					return true;
				}

				if (p->IsRolling){
					p->setRotation(p->getRotation() - (current.x - p->desiredPosition.x) * 5);
				}
				p->desiredPosition.y += colHeight;
				p->velocity.y = 0;
			}
		}
		if (p->velocity.y > 0){
			p->velocity.y = -0.0021;
			Rect tileRect = this->tileRectFromTileCoords(plPos);
			Rect intersectionR = intersection(pRect, tileRect);
			if (intersectionR.size.width > 0 && intersectionR.size.height > 0){
				p->setPosition(p->getPosition() + cocos2d::Point(0, -intersectionR.size.height));
			}
		}
		
		Point pos = p->getPosition() + cocos2d::Point((p->isTowardLeft ? -1 : 1)*p->getContentSize().width / 2, 0);
		tgid = stageLayer->getTileGIDAt(this->tileCoordForPosition(pos));
		if (IsBlock(tgid)) {
			runEffect(EFFECT_EXPLODE_SMALL, pos);
			droppableMissileArray.eraseObject(p);
			heroMissileArray.eraseObject(p);
			enemyMissileArray.eraseObject(p);
			enemyArray.eraseObject((EnemyBase*)p);
			trapArray.eraseObject((Trap*)p);
			p->stopAllActions();
			p->unscheduleAllCallbacks();
			p->removeFromParent();
			return true;
		}
	}
	p->setPosition(p->desiredPosition);
	return false;
}


Rect HelloWorld::intersection(Rect source, cocos2d::Rect rect)
{
	cocos2d::Rect intersection;
	intersection = cocos2d::Rect(source.getMinX()>rect.getMinX() ? source.getMinX() : rect.getMinX(),
		source.getMinY()>rect.getMinY() ? source.getMinY() : rect.getMinY(),
		0,
		0);
	intersection.size.width = (source.getMaxX()<rect.getMaxX() ? source.getMaxX() : rect.getMaxX()) - intersection.getMinX();
	intersection.size.height = (source.getMaxY()<rect.getMaxY() ? source.getMaxY() : rect.getMaxY()) - intersection.getMinY();
	return intersection;
}
void HelloWorld::checkForAndResolveCollisionsForWater(Droppable* p){
	
}

void HelloWorld::checkForAndResolveCollisionsForBouncing(Droppable* p){
	cocos2d::Rect rect;
	Point pos;
	Point plPos;
	bool ground = false;

	bool escape = false;


	//    p->setOnGround(false);
	// check top
	escape = false;
	rect = p->collisionBoundingBox();
	pos = rect.origin + cocos2d::Point(0, rect.size.height);
	do{
		if (pos.x >= rect.origin.x + rect.size.width){
			pos.x = rect.origin.x + rect.size.width; escape = true;
		}

		plPos = this->tileCoordForPosition(pos);
		int tgid = stageLayer->getTileGIDAt(plPos);
		if (tgid > 0){
			if (IsBlock(tgid)){
				Rect tileRect = this->tileRectFromTileCoords(plPos);
				Rect inter = intersection(tileRect, rect);
				if (inter.size.width > 3 && inter.size.height > 0 && inter.size.width > inter.size.height){
					p->desiredPosition.y -= inter.size.height;
					p->velocity.y = 0;
					break;
				}
			}
		}
		pos = pos + cocos2d::Point(TILE_SIZE / 2, 0);
	} while (!escape);

	// check left
	escape = false;
	rect = p->collisionBoundingBox();
	pos = rect.origin + cocos2d::Point(0, rect.size.height);
	do{
		if (pos.y <= rect.origin.y){
			break;//pos.y = rect.origin.y + rect.size.height; escape = true;
		}
		plPos = this->tileCoordForPosition(pos);
		int tgid = stageLayer->getTileGIDAt(plPos);
		if (tgid > 0 && p->velocity.x <= 0){
			if (IsBlock(tgid)){
				Rect tileRect = this->tileRectFromTileCoords(plPos);
				Rect inter = intersection(tileRect, rect);
				if (inter.size.width > 0 && inter.size.height > 0) {
					p->desiredPosition.x += inter.size.width;

					if (p->isTowardLeft){
						p->encounteredWall();
					}

					break;
				}
			}
		}
		pos = pos - cocos2d::Point(0, TILE_SIZE / 2);
	} while (!escape);

	// check right
	escape = false;
	rect = p->collisionBoundingBox();
	pos = rect.origin + rect.size;
	do{
		if (pos.y <= rect.origin.y){
			break;//pos.y = rect.origin.y; escape = true;
		}
		plPos = this->tileCoordForPosition(pos);
		int tgid = stageLayer->getTileGIDAt(plPos);
		if (tgid > 0 && p->velocity.x >= 0){
			if (IsBlock(tgid)){
				Rect tileRect = this->tileRectFromTileCoords(plPos);
				Rect inter = intersection(tileRect, rect);
				if (inter.size.width > 0 && inter.size.height > 0){
					p->desiredPosition.x -= inter.size.width;

					if (!p->isTowardLeft){
						p->encounteredWall();
					}
					break;
				}
			}
		}

		pos = pos - cocos2d::Point(0, TILE_SIZE / 2);
	} while (!escape);

	// check bottom
	escape = false;
	rect = p->collisionBoundingBox();
	pos = rect.origin + cocos2d::Point(rect.size.width, 0);
	do{
		if (pos.x <= rect.origin.x){
			pos.x = rect.origin.x; escape = true;
		}

		plPos = this->tileCoordForPosition(pos);
		int tgid = stageLayer->getTileGIDAt(plPos);
		if (tgid > 0){
			Rect tileRect = this->tileRectFromTileCoords(plPos);
			Rect inter = intersection(tileRect, rect);
			if (IsOneWay(tgid)){
				if ((int)p->oneWayCancelY == inter.origin.y + inter.size.height){
					break;
				}
			}
			if (p->velocity.y < 0){
				if (inter.size.width > 3 && inter.size.height > 0){
					p->desiredPosition.y += inter.size.height;
					ground = true;
					//                        p->velocity.y = 0;
					p->velocity = cocos2d::Point(p->velocity.x*0.8, -p->velocity.y*0.6);
					break;
				}
			}
		}
		else{
			if (p->onGround) {
				if (pos.x == rect.origin.x && p->isTowardLeft){
					p->encounteredCliff();
				}
				else if (pos == rect.origin + cocos2d::Point(rect.size.width, 0) && !p->isTowardLeft){
					p->encounteredCliff();
				}
			}
		}

		pos = pos - cocos2d::Point(TILE_SIZE / 2, 0);
	} while (!escape);
	p->setPosition(p->desiredPosition);
}

Droppable* HelloWorld::checkForAndResolveCollisionsForMissile(Droppable* p){
	Point plPos = this->tileCoordForPosition(p->getPosition());
	int tgid = stageLayer->getTileGIDAt(plPos);

	p->setPosition(p->desiredPosition); //7
	Rect pRect = p->collisionBoundingBox(); //2
	Rect tileRect = this->tileRectFromTileCoords(plPos);
	if (tgid) {
		Rect intersectionR = intersection(pRect, tileRect); //5
		if (intersectionR.size.width > 0 && intersectionR.size.height > 0){//pRect.intersectsRect(tileRect)) {
			if (IsBlock(tgid)) {
				if (p->velocity.x > 0){
					runEffect(EFFECT_EXPLODE_SMALL,//EFFECT_BULLET_AGAINST_WALL,
						Point(intersectionR.origin.x,
						intersectionR.origin.y + intersectionR.size.height / 2));//p->getPosition());
				}
				else{
					runEffect(EFFECT_EXPLODE_SMALL,//EFFECT_BULLET_AGAINST_WALL_LEFT,
						Point(intersectionR.origin.x + intersectionR.size.width,
						intersectionR.origin.y + intersectionR.size.height / 2));//p->getPosition());
				}
				bulletWasted = true;

				if (heroMissileArray.find(p) != heroMissileArray.end()){
					heroMissileArray.eraseObject(p);

				}
				if (enemyMissileArray.find(p) != enemyMissileArray.end()) {
					enemyMissileArray.eraseObject(p);
					//                            log("enemy missile erased! batch child: %d, custom: %d, straight: %d, hero: %d, droppable: %d", (int)spriteBatch->getChildren().size(), (int)customMovingArray.size(), (int)straightMovingArray.size(), (int)heroMissileArray.size(), (int)droppableMissileArray.size());
				}
				else{
					//                            log("it's not enemy missile!");
				}
				
				if (p->getTag() == UNIT_MISSILE_CUSTOM){
					//                            customMovingArray.eraseObject(p);
					missileMoveDone(p);
				}

				return p;
			}
		}
	}
	return NULL;
}
void HelloWorld::unscheduleAll(){
	this->unscheduleAllCallbacks();
}

void HelloWorld::rateResult(Node* node){
	if (node->getTag() == MESSAGE_BOX_RESULT_OK) {
		if (GameManager::getInstance()->market == MARKET_PLAYSTORE_PAID) {
			//GoogleBilling::Billing::openUrl("itms-apps://itunes.apple.com/app/id838183037");
		}
		else if (GameManager::getInstance()->market == MARKET_PLAYSTORE_PAID) {
			//GoogleBilling::Billing::openUrl("http://play.google.com/store/apps/details?id=com.magmon.LegendDaryTheTower");
		}
		else if (GameManager::getInstance()->market == MARKET_TSTORE_PAID){
			//GoogleBilling::Billing::openUrl("http://m.tstore.co.kr/mobilepoc/apps/appsDetail.omp?prodId=0000661603");
		}
		else if (GameManager::getInstance()->market == MARKET_NAVER_PAID){
			//GoogleBilling::Billing::openUrl("http://m.tstore.co.kr/mobilepoc/apps/appsDetail.omp?prodId=0000661603");
		}
		else if (GameManager::getInstance()->market == MARKET_MAC){
			//GoogleBilling::Billing::openUrl("macappstore://itunes.apple.com/app/id886115032?mt=12");
		}
		UserDefault::getInstance()->setBoolForKey("Rated", true);
	}
}
void HelloWorld::onUpdateResultErrorComplete(char const *status, char const *error) {
	log("Update result failed. Status: %s, Error: %s", status, error);
	activityIndicator->setVisible(false);
	activityIndicator->stopAllActions();
	activityIndicator->removeFromParentAndCleanup(true);

}

void HelloWorld::gameClearAnimationDone(Ref* obj)
{
	//    log("Show stage clear popup");
}

Droppable* HelloWorld::findTarget(int distance){
	Droppable* targetEnemy = NULL;

	float minDistance = distance;
	float x = 0;
	float y = 0;
	for (auto drop : enemyArray){
		if (drop->untouchable) {
			continue;
		}

		x = player->getPositionX() - drop->getPositionX();
		y = player->getPositionY() - drop->getPositionY();
		if (x < 0) x = -x;
		if (y < 0) y = -y;

		if (x + y < minDistance) {
			minDistance = x + y;
			targetEnemy = drop;
		}
	}
	return targetEnemy;
}

void HelloWorld::fire()
{
	if (playerFireCoolTime > 0) {
		return;
	}

	if (playerFireCoolTime <= 0) {
		playerFireCoolTime = player->fireCoolTime;
		if (playerFireCoolTime <= 0) {
			playerFireCoolTime = 0.2f;
		}
	}

	int angle;
	angle = player->isTowardLeft ? 180 : 0;
	player->fireGuage -= (9.5f - /*playerMissileRate*/9 * 0.9f)*0.1f;
	//    log("Fire: %f, rate: %d", player->fireGuage, playerMissileRate);

	cocos2d::Point point = player->getPosition();
	Droppable* ms;
	int extraY = -2;
	cocos2d::Point msPos = cocos2d::Point(player->getPosition().x + (player->isTowardLeft ? 2 : 14), player->getPosition().y + extraY + 8);

	ms->setPosition(msPos);

	if (player->isTowardLeft) ms->setFlippedX(true);
	ms->movingAngle = player->isTowardLeft ? 180 : 0;

	if (isVideoRewardAttack) {
		ms->energy = ms->energy*1.2;
	}
	spriteBatch->addChild(ms);
	heroMissileArray.pushBack(ms);

	/*Sprite* fireFlash = Sprite::createWithSpriteFrameName(CCString::createWithFormat("fireFlash%d.png", fireIndex)->getCString());
	fireFlash->setPosition(Point(point.x + (player->isTowardLeft?-5:5), cocos2d::Point.y));
	fireFlash->setFlippedX(player->isTowardLeft);
	spriteBatch->addChild(fireFlash, 10);
	DelayTime* delay = DelayTime::create(0.07f);
	CallFuncN* call = CallFuncN::create(CC_CALLBACK_1(HelloWorld::enemyBaseMoveDone));
	fireFlash->runAction(Sequence::create(delay, call, NULL));

	Sprite* fireFlashLight = Sprite::createWithSpriteFrameName("fireLight.png");
	fireFlashLight->setPosition(Point(point.x + (player->isTowardLeft?-5:5), cocos2d::Point.y));
	fireFlashLight->setFlippedX(player->isTowardLeft);
	spriteBatch->addChild(fireFlashLight, 10);
	DelayTime* delayForLight = DelayTime::create(0.07f);
	CallFuncN* callForLight = CallFuncN::create(CC_CALLBACK_1(HelloWorld::enemyBaseMoveDone));
	fireFlashLight->runAction(Sequence::create(delayForLight, callForLight, NULL));
	*/
	player->runFireAnimation();
}
void HelloWorld::missileEffectUpdate(float dt){
	missileEffectCollapsedTime += dt;
	if (missileEffectCollapsedTime < 0.053){
		return;
	}
	missileEffectCollapsedTime = 0;

	for (auto drop : heroMissileArray){
		if (drop->effectType == MISSILE_EFFECT_SMOKE)
			runEffect(EFFECT_SMOKE, drop->getPosition());
		if (drop->effectType == MISSILE_EFFECT_RED_BALL)
			runEffect(EFFECT_EXPLODE_SMALL, drop->getPosition());
		else if (drop->effectType == MISSILE_EFFECT_GREEN_SMOKE)
			runEffect(EFFECT_GREEN_SMOKE, drop->getPosition());
		else if (drop->effectType == MISSILE_EFFECT_RED_SMOKE)
			runEffect(EFFECT_RED_SMOKE, drop->getPosition());
	}
}
void HelloWorld::missileMoveDone(Ref* obj)
{
	EnemyBase* missile = (EnemyBase*)obj;
	heroMissileArray.eraseObject(missile);
	customMovingArray.eraseObject(missile);
	missile->stopAllActions();
	missile->removeFromParentAndCleanup(true);
}
void HelloWorld::fireBulletMoveDone(Ref* obj)
{
	Droppable* missile = (Droppable*)obj;
	heroMissileArray.eraseObject(missile);
	customMovingArray.eraseObject(missile);
	missile->stopAllActions();
	missile->removeFromParentAndCleanup(true);
}
void HelloWorld::enemyBaseMoveDone(Ref* obj)
{
	if (obj == NULL) {
		return;
	}
	EnemyBase* sprite = (EnemyBase*)obj;
	sprite->stopAllActions();
	sprite->unscheduleAllCallbacks();
	enemyArray.eraseObject(sprite);
	sprite->removeFromParentAndCleanup(true);
}
void HelloWorld::coinWaitDone(Ref* obj){
	Droppable* drop = (Droppable*)obj;
	coinArray.eraseObject(drop);
	droppableCoinArray.eraseObject(drop);
	spriteMoveDone(drop);
}
void HelloWorld::spriteMoveDone(Ref* obj)
{
	if (obj == NULL) {
		return;
	}
	Node* sprite = (Node*)obj;
	sprite->stopAllActions();
	sprite->removeFromParentAndCleanup(true);
}
void HelloWorld::nodeMoveDone(Ref* obj)
{
	if (obj == NULL) {
		return;
	}
	Label* lbl = (Label*)obj;

	lbl->stopAllActions();
	lbl->removeFromParentAndCleanup(true);
}

/*void HelloWorld::fireLoop(float dt)
{
if (player->fireGuage < 0) {
player->fireGuage += dt;
}

}*/

void HelloWorld::blinkingPlayer(float dt)
{
	blinkingTime -= dt;

	if (player->getColor().g == 115) {
		player->setColorTo(Color3B(255, 255, 255));
	}
	else{
		player->setColorTo(Color3B(255, 115, 115));
	}

	if (blinkingTime <= 0) {
		player->setColorTo(Color3B(255, 255, 255));

		this->unschedule(schedule_selector(HelloWorld::blinkingPlayer));
	}
}

void HelloWorld::pauseLayer()
{
	for (auto spt : enemyArray)
	{
		spt->pause();
	}

	for (auto spt : heroMissileArray)
	{
		spt->pause();
	}

	for (auto spt : enemyMissileArray)
	{
		spt->pause();
	}

	for (auto trap : trapArray){
		trap->pause();
	}
    _isGamePaused = true;
	player->pause();
//	this->pause();
}

void HelloWorld::resumeLayer()
{
	for (auto spt : enemyArray)
	{
		spt->resume();
	}

	for (auto spt : heroMissileArray)
	{
		spt->resume();
	}

	for (auto spt : enemyMissileArray)
	{
		spt->resume();
	}
	for (auto trap : trapArray){
		trap->resume();
	}
    
    _isGamePaused = false;
	player->resume();
//	this->resume();
}

void HelloWorld::refreshPlayerInfo(int costum, int weapon)
{
	setPlayerInfo();
}
