#ifndef __ENDNOTLEAVE_SCENE_H__
#define __ENDNOTLEAVE_SCENE_H__
#include "cocos2d.h"
#include "cocos-ext.h"
USING_NS_CC;
USING_NS_CC_EXT;

class EndNotLeave :public cocos2d::Layer
{
public:
	EndNotLeave();
	~EndNotLeave();

	static cocos2d::Scene* createScene();

	virtual bool init();

	CREATE_FUNC(EndNotLeave);

	void menu(cocos2d::Ref* pSender);
};
#endif

