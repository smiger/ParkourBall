#ifndef __ENDLEAVE_SCENE_H__
#define __ENDLEAVE_SCENE_H__
#include "cocos2d.h"
#include "cocos-ext.h"
USING_NS_CC;
USING_NS_CC_EXT;

class EndLeave :public cocos2d::Layer
{
public:
	EndLeave();
	~EndLeave();

	static cocos2d::Scene* createScene();

	virtual bool init();

	CREATE_FUNC(EndLeave);

	void menu(cocos2d::Ref* pSender);
};
#endif

