//
//  TimeManager.cpp
//  LegendDaryKakao
//
//  Created by SUNG PILL PACK on 14. 2. 14..
//
//

#include "TimeManager.h"

#include "json/document.h"

//#include "Kakao/Common/GameUserInfo.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//#include "UrlJni.h"
#endif

TimeManager* TimeManager::m_mySingleton = NULL;

TimeManager::TimeManager()
{
    timeEstablished = false;
    isTrying = false;
}

TimeManager* TimeManager::getInstance()
{
    if(NULL == m_mySingleton)
    {
        m_mySingleton = TimeManager::create();
        m_mySingleton->webTime = -1;
        m_mySingleton->retain();
//        m_mySingleton->titleLayer = (TitleScene*)GameManager::sharedGameManager()->getTitleLayer();
    }
    
    return m_mySingleton;
}
bool TimeManager::init()
{
    Layer::init();
    return true;
}

void TimeManager::onLoadGameUserInfoErrorComplete(char const *status, char const *error) {
//    CCMessageBox("네트워크 상태를 확인해주세요", "게임 사용자 정보를 불러올 수 없습니다.");
    //    log("onLoadGameUserInfoErrorComplete : %s, %s", status, error);
}
void TimeManager::onLoadGameUserInfoComplete() {
    
    timeEstablished = true;
//    webTime = GameUserInfo::getInstance()->serverTime;
    log("web time: %ld", webTime);
    //    time_t startTime;
    struct tm * timeinfo;
    time (&startLocalTime);
    timeinfo = localtime (&startLocalTime);
}

void TimeManager::getHttpTime()
{
    isTrying = true;
    timeEstablished = false;
    HttpRequest* request = new HttpRequest();
    request->setUrl("http://api.facebook.com/method/fql.query?query=SELECT+now%28%29+FROM+link_stat+WHERE+url+%3D+%271.2%27&format=json");
    request->setRequestType(HttpRequest::Type::GET);
    request->setResponseCallback(this, httpresponse_selector(TimeManager::onHttpRequestCompleted));
    request->setTag("Http time from facebook");
    HttpClient::getInstance()->send(request);
    request->release();
}

void TimeManager::onHttpRequestCompleted(Node *sender, void *data)
{
    log("complete request http");
    HttpResponse *response = (HttpResponse*)data;
    
    isTrying = false;
    if (!response)
    {
        return;
    }
    
    // You can get original request type from: response->request->reqType
    if (0 != strlen(response->getHttpRequest()->getTag()))
    {
        log("%s completed", response->getHttpRequest()->getTag());
    }
    
    int statusCode = response->getResponseCode();
    char statusString[64] = {};
    sprintf(statusString, "HTTP Status Code: %d, tag = %s", statusCode, response->getHttpRequest()->getTag());
    //m_labelStatusCode->setString(statusString);
    log("response code: %d", statusCode);
    
    if (!response->isSucceed())
    {
        log("response failed");
        log("error buffer: %s", response->getErrorBuffer());
//        CCMessageBox("인터넷 연결 상태를 확인해주세요.", "인터넷 엑세스 오류");
//        GameManager::sharedGameManager()->exitGame();
        return;
    }
    
    // dump data
    std::vector<char> *buffer = response->getResponseData();
    const char* text = buffer->data();
    rapidjson::Document document;
    if(document.Parse(text).HasParseError()){
        log("http time json parsing error");
        return;
    }
    
    rapidjson::Value& s = document[0]["anon"];

    uint64_t timeInInt = s.GetUint64();
//    timeInInt += 60*60*24*8;
    time_t now;
    now = timeInInt;
    receivedTime = *localtime(&now); // 지역표준시로 변환한다 (대한민국은 KST)
    //ts = gmtime(&now);  // 국제표준시 GMT로 변환한다

    struct tm etet = *localtime(&now);
    
    uint64_t yesterdayTimeInInt = timeInInt - 60*60*24;
    time_t yesterdayNow;
    yesterdayNow = yesterdayTimeInInt;
    receivedYesterdayTime = *localtime(&yesterdayNow);

    
//    log("http time-> %d/%d/%d %d:%d:%d", receivedTime->tm_year,receivedTime->tm_mon, receivedTime->tm_mday, receivedTime->tm_hour, receivedTime->tm_min, receivedTime->tm_sec);
    timeEstablished = true;
    /*
//    printf("Http Test, dump data: ");
    //char numberInChar[12];
    int startIndex = -1;
    int endIndex = -1;
    for (unsigned int i = 0; i < buffer->size(); i++)
    {
        printf("%c", (*buffer)[i]);
        if ((*buffer)[i] == ':') {
            startIndex = i+1;
        }else if((*buffer)[i] == '}'){
            endIndex = i;
        }
    }
    long tick = 0;
    int num = 0;
    long p = 0;
    if (endIndex - startIndex > 10) {
        startIndex = endIndex - 10;
    }
    for (int i = startIndex; i < endIndex; i++) {
        num = ((int)((*buffer)[i]) - 48);
        //log("num: %d", num);
        p = pow(10, endIndex - i -1);
        //log("p: %d", p);
        tick += num*(p);
    }
    timeEstablished = true;
    webTime = tick;
    log("web time: %ld", webTime);
//    time_t startTime;
    struct tm * timeinfo;
    time (&startLocalTime);
    timeinfo = localtime (&startLocalTime);*/
}

long TimeManager::getCurrentTime(){
    time_t currentLocalTime;
    struct tm * timeinfo;
    time (&currentLocalTime);
    timeinfo = localtime (&currentLocalTime);
    
//    log("get current time web: %ld, startLocalTime: %ld, currentLocalTime: %ld", webTime, startLocalTime, currentLocalTime);
    return webTime + (currentLocalTime - startLocalTime);
//    return GameUserInfo::getInstance()->serverTime + (currentLocalTime - startLocalTime);
}

int TimeManager::getDay(){
    return receivedTime.tm_mday;
}
int TimeManager::getMonth(){
    return receivedTime.tm_mon;
}
int TimeManager::getYear(){
    return receivedTime.tm_year;
}

int TimeManager::getYesterdayDay(){
    return receivedYesterdayTime.tm_mday;
}
int TimeManager::getYesterdayMonth(){
    return receivedYesterdayTime.tm_mon;
}
int TimeManager::getYesterdayYear(){
    return receivedYesterdayTime.tm_year;
}


