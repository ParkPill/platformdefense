#include "LanguageManager.h"
LanguageManager* LanguageManager::_instance = 0;

LanguageManager::LanguageManager()
{
	string fileName;
	// detect current language
	LanguageType lang = Application::getInstance()->getCurrentLanguage();
	//lang = LanguageType::str;
	switch (lang)
	{
	case LanguageType::ENGLISH:
//		fileName = "en.json";
        setEnglish();
		break;
	case LanguageType::KOREAN:
//		fileName = "es.json";
        setKorean();
		break;
    case LanguageType::SPANISH:
//        fileName = "es.json";
        setSpanish();
        break;
    case LanguageType::JAPANESE:
//        fileName = "jp.json";
        setJapanese();
        break;
	default:
		log("Unknown language. Use english");
//		fileName = "en.json";
        setEnglish();
		break;
	};
    
    

    /*
	// below we open, read and parse language data file with rapidjson library
	ssize_t size;
    std::string rawString = FileUtils::getInstance()->getStringFromFile(fileName.c_str());
    const char* buf = rawString.c_str();//, "r", &size);
    

//FileUtils::getInstance()->getFileData(fileName.c_str(), "r", &size);
	string content(buf);
	string clearContent = content.substr(0, content.find('}') + 1);// originally content.rfind but someone says it causes crash. becaused some reason file reader return with random data

	document.Parse<0>(clearContent.c_str());
	if (document.HasParseError())
	{
		log("Language file parsing error!");
		return;
	}
    log("Language file parsing success!");*/
}

LanguageManager* LanguageManager::getInstance()
{
	if (!_instance)
		_instance = new LanguageManager();
	return _instance;
}
string LanguageManager::getStringForKey(int key){
    return strArray.at(key);
}
/*
string LanguageManager::getStringForKey(string key)
{
    string str = "text error! Sorry!";
    
    try {
        str = document[key.c_str()].GetString();
    } catch (...) {
        log("exception!");
    }
    log("str: %s", str.c_str());
    return str;
}*/