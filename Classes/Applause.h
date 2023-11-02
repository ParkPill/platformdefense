//
//  Applause.h
//  AlmostLegendary
//
//  Created by SUNG PILL PACK on 13. 6. 29..
//
//

#ifndef AlmostLegendary_Applause_h
#define AlmostLegendary_Applause_h


#include "ui/UIWidget.h"
#include "cocos2d.h"

#include "Droppable.h"
#include "cocostudio/CocoStudio.h"
#include "base/CCEventListenerController.h"
#include "base/CCEventController.h"
#include "base/CCController.h"



using namespace cocos2d;

using namespace cocos2d::ui;
using namespace cocostudio;
class Applause : public Layer
{
private:
	SpriteBatchNode* spriteBatch;
	
public:
	
    virtual bool init();
    CREATE_FUNC(Applause);

	void addPeople(float height, Color3B color);
};
#endif
