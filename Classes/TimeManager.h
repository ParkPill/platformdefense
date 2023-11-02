//
//  TimeManager.h
//  LegendDaryKakao
//
//  Created by SUNG PILL PACK on 14. 2. 14..
//
//

#ifndef __LegendDaryKakao__TimeManager__
#define __LegendDaryKakao__TimeManager__

#include <iostream>
#include <time.h>
#include "cocos2d.h"
#include "ExtensionMacros.h"

#include "network/HttpRequest.h"
#include "network/HttpResponse.h"
#include "network/HttpClient.h"


//#include "Title.h"
using namespace cocos2d;
using namespace cocos2d::network;

class TimeManager: public Layer
{
private:
    //Constructor
    TimeManager();
    
    //Instance of the singleton
    static TimeManager* m_mySingleton;
   
    struct tm receivedTime;
    struct tm receivedYesterdayTime;
public:
    time_t startLocalTime;
    long webTime;
    bool timeEstablished;
    bool isTrying;

    long getCurrentTime();
    const char* getRemainTime(long targetTime);
    static TimeManager* getInstance();
    
    virtual bool init();
    CREATE_FUNC(TimeManager);
    void getHttpTime();
    void onHttpRequestCompleted(Node *sender, void *data);
    void updateTick(float dt);
    
    void onLoadGameUserInfoErrorComplete(char const *status, char const *error);
    void onLoadGameUserInfoComplete();
    
    int getDay();
    int getMonth();
    int getYear();
    
    int getYesterdayDay();
    int getYesterdayMonth();
    int getYesterdayYear();
};

#endif /* defined(__LegendDaryKakao__TimeManager__) */


