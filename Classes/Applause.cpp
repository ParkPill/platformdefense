//
//  Applause.cpp
//  AlmostLegendary
//
//  Created by SUNG PILL PACK on 13. 6. 29..
//
//

#include "Applause.h"
#include "GameManager.h"
#include "HelloWorldScene.h"
#include "MyMessageBox.h"
//#include "GoogleBilling.h"
#include "LanguageManager.h"
#include "ShopLayer.h"
#include "SimpleAudioEngine.h"
bool Applause::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
	
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("applause.plist");
	AnimationCache::getInstance()->addAnimationsWithFile("applauseAnimation.plist");
	
    CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
    CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("applause.wav");//"longApplause.mp3");
	
    spriteBatch = SpriteBatchNode::create("applause.png");
	this->addChild(spriteBatch, 1);
    
    addPeople(150, Color3B(200, 200, 200));
	addPeople(98, Color3B(150, 150, 150));
	addPeople(45, Color3B(80, 80, 80));
	addPeople(-8, Color3B(0, 0, 0));
    return true;
}

void Applause::addPeople(float height, Color3B color){

	Sprite* sptPerson;
	char buf[20];
	// black
//	int index = 0;
	Size size = Director::getInstance()->getVisibleSize();
	int peopleCount = 8;
	int gap = size.width / peopleCount;
	for (int i = 0; i < peopleCount; i++){
		sprintf(buf, "man%d", cocos2d::random()%8);
		sptPerson = Sprite::createWithSpriteFrameName("manClap0_0.png");

		Animation* animation = AnimationCache::getInstance()->getAnimation(buf);
		Animate* animate = Animate::create(animation);
		RepeatForever* forever = RepeatForever::create(animate);
		sptPerson->runAction(forever);
		sptPerson->setScale(8);
		sptPerson->setColor(color);
		sptPerson->setAnchorPoint(Point(0.5, 0));
		sptPerson->setPosition(Point(gap / 2 + i*gap + cocos2d::random() % (gap / 2) - gap / 4, height - cocos2d::random() % (gap / 4)));
		
		spriteBatch->addChild(sptPerson);
	}
}