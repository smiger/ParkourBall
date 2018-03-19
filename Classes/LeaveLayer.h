#ifndef __LEAVELAYER_SCENE_H__
#define __LEAVELAYER_SCENE_H__
#include "cocos2d.h"
#include "cocos-ext.h"
USING_NS_CC;
USING_NS_CC_EXT;

class LeaveLayer :public cocos2d::Layer
{
public:
	LeaveLayer();
	~LeaveLayer();

	static cocos2d::Scene* createScene();

	virtual bool init();

	CREATE_FUNC(LeaveLayer);

	void leave(cocos2d::Ref* pSender);
	void notLeave(cocos2d::Ref* pSender);
};
#endif

