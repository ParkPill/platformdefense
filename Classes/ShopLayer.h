//
//  ShopLayer.h
//  AlmostLegendary
//
//  Created by SUNG PILL PACK on 13. 6. 29..
//
//

#ifndef AlmostLegendary_ShopLayer_h
#define AlmostLegendary_ShopLayer_h

#include "cocos2d.h"

#include "Droppable.h"
#include "cocostudio/CocoStudio.h"
#include "base/CCEventListenerController.h"
#include "base/CCEventController.h"
#include "base/CCController.h"
using namespace cocos2d;
using namespace cocos2d::ui;
using namespace cocostudio;
class ShopLayer : public Layer
{
private:
	cocos2d::Size _size;
    Node* _shopScene;
    Node* _indicator;
public:
    int purchasingIndex;
    bool isWaiting;
    
    
    virtual bool init();
    CREATE_FUNC(ShopLayer);

    void CountDown(float dt);
    void ShowIndicator();
    void HideIndicator();
    void UpdateCoinCount();
    void UpdatePrice();
	void CloseThis();
	void OnSmallClick();
	void OnMiddleClick();
	void OnBigClick();
    void OnHugeClick();
    void OnHeroClick();
    void OnRestoreClick();
    void OnRemoveAdsClick();
    EventListenerKeyboard* _listener;
    void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event) override;
};
#endif
