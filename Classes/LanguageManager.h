#ifndef LanguageManager_h
#define LanguageManager_h


#include "cocos2d.h"
#include <string>

#include "json/document.h"

#define STR_font 0
#define STR_new_game 1
#define STR_tutorialShoot 2
#define STR_tutorialTrap 3
#define STR_tutorialUpgrade 4
#define STR_tutorialGameOver 5
#define STR_tutorialCheckpoint 6
#define STR_tutorialDone 7
#define STR_ability_normal 8
#define STR_ability_slim 9
#define STR_ability_hard 10
#define STR_ability_healthy 11
#define STR_ability_fat 12
#define STR_ability_fire 13
#define STR_ability_fast 14
#define STR_ability_ice 15
#define STR_ability_fly 16
#define STR_ability_boss 17
#define STR_tip0 18
#define STR_tip1 19
#define STR_tip2 20
#define STR_tip3 21
#define STR_tip4 22
#define STR_tip5 23
#define STR_tip6 24
#define STR_tip7 25
#define STR_tip8 26
#define STR_tip9 27
#define STR_tip10 28
#define STR_tip11 29
#define STR_askShare0 30
#define STR_askShare1 31
#define STR_askShare2 32
#define STR_askShare3 33
#define STR_askShare4 34
#define STR_share_game 35
#define STR_next_gen 36
#define STR_daily_reward 37
#define STR_TRAP_GASBURNER 38
#define STR_TRAP_FREEZER 39
#define STR_TRAP_WALL 40
#define STR_TRAP_FLASK 41
#define STR_TRAP_MACEBALL 42
#define STR_TRAP_BOMB 43
#define STR_TRAP_FIRE 44
#define STR_TRAP_SPIKE 45
#define STR_TRAP_TOPSPIKE 46
#define STR_TRAP_ICEBALL 47
#define STR_TRAP_HAND 48
#define STR_TRAP_WALLSPIKE 49
#define STR_TRAP_CACTUS 50
#define STR_friend_condition_0 51
#define STR_friend_condition_1 52
#define STR_friend_condition_2 53
#define STR_friend_condition_3 54
#define STR_friend_condition_4 55
#define STR_coin_not_saved 56
#define STR_data_warn 57
#define STR_data_load_coin_warn 58
#define STR_rate_us 59

USING_NS_CC;
using std::string;
//#include "external\json\document.h"
//#include "..\external\json\rapidjson.h"
using namespace rapidjson; // library that we use for json parsing

class LanguageManager
{
	Document document; // current document with language data
    
    std::vector<std::string> strArray;
    
	LanguageManager(); // constructor is private
	static LanguageManager* _instance;
public:
	static LanguageManager* getInstance();
//	string getStringForKey(string key);
    string getStringForKey(int key);
    void setKorean();
    void setEnglish();
    void setJapanese();
    void setSpanish();
	
};
#endif