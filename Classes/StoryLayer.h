#ifndef __STORYLAYER_SCENE_H__
#define __STORYLAYER_SCENE_H__
#include "cocos2d.h"
#include "cocos-ext.h"
USING_NS_CC;
USING_NS_CC_EXT;

class StoryLayer :public cocos2d::Layer
{
public:
	StoryLayer();
	~StoryLayer();

	static cocos2d::Scene* createScene();

	virtual bool init();

	CREATE_FUNC(StoryLayer);

	void start(cocos2d::Ref* pSender);

	//‘§º”‘ÿ“Ù–ß
	void preLoadMusic();
};
#endif

