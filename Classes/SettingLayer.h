
#ifndef _SETTING_H_
#define _SETTING_H_

#include "cocos2d.h"

#include "ui/UISlider.h"

using namespace cocos2d;
using namespace cocos2d::ui;
using namespace cocostudio;
class SettingLayer : public Layer
{
private:
    Size size;
    Node* settingPopup;
    Node* _messageBox;
    Slider* musicBar;
    Slider* soundBar;
    bool isPlayServiceLoggedIn;
    Button* btnLogin;
    EventListenerKeyboard* _listener;
public:
    virtual bool init() override;
    CREATE_FUNC(SettingLayer);
    
    void onWarnClosed();
    void onSaveClick();
    void onLoadClick();
    void onLoadWarnOk();
    void onLoadWarnCancel();
    void onLike();
    void CloseSettingPopup();
    void checkingLogin(float dt);
    void musicSliderEvent(Ref *pSender, Slider::EventType type);
    void soundSliderEvent(Ref *pSender, Slider::EventType type);
    void onMusicSliderTouch(Ref* pSender, ui::Widget::TouchEventType eEventType);
    void onSoundSliderTouch(Ref* pSender, ui::Widget::TouchEventType eEventType);
    void onPlayServiceLoginClick();
    void onPlayServiceLogin(bool isLogin);
    
};
#endif