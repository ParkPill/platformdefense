#include "cocos2d.h"
#include "AppDelegate.h"
#include "HelloWorldScene.h"
#ifdef SDKBOX_ENABLED
#include "PluginGoogleAnalytics/PluginGoogleAnalytics.h"
#endif
#ifdef SDKBOX_ENABLED
#include "Sdkbox/Sdkbox.h"
#include "PluginChartboost/PluginChartboost.h"
#include "PluginGoogleAnalytics/PluginGoogleAnalytics.h"
#endif
#include "PluginIAP/PluginIAP.h"
#include "GameManager.h"
#include "LanguageManager.h"
#include "Title.h"
#include "FifteenSixLogo.h"
#include "NativeInterface.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

static cocos2d::Size designResolutionSize = cocos2d::Size(640, 1136);
static cocos2d::Size smallResolutionSize = cocos2d::Size(740, 1136);
static cocos2d::Size mediumResolutionSize = cocos2d::Size(840, 1136);//1024, 768);
static cocos2d::Size largeResolutionSize = cocos2d::Size(1024, 1136);//2048, 1536);

AppDelegate::AppDelegate() {
    
}

AppDelegate::~AppDelegate()
{
}

//if you want a different context,just modify the value of glContextAttrs
//it will takes effect on all platforms
void AppDelegate::initGLContextAttrs()
{
    //set OpenGL context attributions,now can only set six attributions:
    //red,green,blue,alpha,depth,stencil
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};
    
    GLView::setGLContextAttrs(glContextAttrs);
}

// If you want to use packages manager to install more packages,
// don't modify or remove this function
static int register_all_packages()
{
    return 0; //flag for packages manager
}

bool AppDelegate::applicationDidFinishLaunching() {
#ifdef SDKBOX_ENABLED
    sdkbox::PluginGoogleAnalytics::init();
#endif
#ifdef SDKBOX_ENABLED
    sdkbox::PluginChartboost::init();
    sdkbox::PluginGoogleAnalytics::init();
#endif
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
        glview = GLViewImpl::createWithRect("PlatformDefense", Rect(0, 0, designResolutionSize.width, designResolutionSize.height));
#else
        glview = GLViewImpl::create("PlatformDefense");
#endif
        director->setOpenGLView(glview);
    }
    
    SpriteFrameCache* cache = SpriteFrameCache::getInstance();
    
    cache->addSpriteFramesWithFile("pdImages.plist");
    FileUtils::getInstance()->addSearchPath("Language");
    FileUtils::getInstance()->addSearchPath("fonts");
    FileUtils::getInstance()->addSearchPath("res");
    FileUtils::getInstance()->addSearchPath("sounds");
    FileUtils::getInstance()->addSearchPath("Logo");
    AnimationCache::getInstance()->addAnimationsWithFile("animations.plist");
    
    GameManager::getInstance()->originalSize = director->getWinSize();
    GameManager::getInstance()->market = MARKET_APPSTORE_PAID;
    
    GameManager::getInstance()->version = "1.3";
    GameManager::getInstance()->totalThemeCount = 3;
    GameManager::getInstance()->totalStage = 24*3;
    GameManager::getInstance()->developer = false;
    GameManager::getInstance()->initGameManager();
    
    // turn on display FPS
    director->setDisplayStats(false);
    
    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);
    
    // Set the design resolution
    glview->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, ResolutionPolicy::FIXED_HEIGHT);
    /*Size frameSize = glview->getFrameSize();
     // if the frame's height is larger than the height of medium size.
     if (frameSize.height > mediumResolutionSize.height)
     {
     director->setContentScaleFactor(MIN(largeResolutionSize.height/designResolutionSize.height, largeResolutionSize.width/designResolutionSize.width));
     }
     // if the frame's height is larger than the height of small size.
     else if (frameSize.height > smallResolutionSize.height)
     {
     director->setContentScaleFactor(MIN(mediumResolutionSize.height/designResolutionSize.height, mediumResolutionSize.width/designResolutionSize.width));
     }
     // if the frame's height is smaller than the height of medium size.
     else
     {
     director->setContentScaleFactor(MIN(smallResolutionSize.height/designResolutionSize.height, smallResolutionSize.width/designResolutionSize.width));
     }*/
    
    register_all_packages();
    
    //    LanguageManager* lang = LanguageManager::getInstance();
    //    const char* name = lang->getStringForKey("font").c_str();
    //    log("font: %s", name);
    
    //    sdkbox::IAP::setDebug(true);
    //    sdkbox::IAP::refresh();
    
    // create a scene. it's an autorelease object
    auto scene = Scene::create();
    scene->addChild(FifteenSixLogo::create());
    //    scene->addChild(HelloWorld::scene(0));
    
    // run
    director->runWithScene(scene);
    
    sdkbox::IAP::init();
    
    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
#ifdef SDKBOX_ENABLED
    sdkbox::sessionEnd();
#endif
    Director::getInstance()->stopAnimation();
    sdkbox::PluginGoogleAnalytics::stopSession();
    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
#ifdef SDKBOX_ENABLED
    sdkbox::sessionStart();
#endif
    Director::getInstance()->startAnimation();
    sdkbox::PluginGoogleAnalytics::startSession();
    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}

/*
 #if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
 
 #elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
 
 #else
 
 #endif
 
 export ANDROID_SDK_ROOT=“/Users/rightguy22/Library/Android/sdk”
 export PATH=“$PATH:$ANDROID_SDK_ROOT/tools”
 
 export ANDROID_NDK_ROOT=“/Users/rightguy22/DevUtil/android-ndk-r10e”
 export NDK_ROOT=“/User/rightguy22/android-ndk-r10e”
 
 export PATH=$PATH:$ANDROID_NDK_ROOT
 export PATH=/opt/local/bin:/opt/local/sbin:$PATH
 
 export ANT_HOME=“/User/rightguy22/DevUtil/apache-ant-1.9.6”
 export ANT_ROOT=$ANT_HOME/bin
 export PATH=$PATH:$ANT_HOME:$ANT_ROOT
 
 
 */
