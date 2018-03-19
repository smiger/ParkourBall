#ifndef __BALL_SCENE_H__
#define __BALL_SCENE_H__

#include "cocos2d.h"
#include "Base.h"
#include "cocos-ext.h"
#include "Constants.h"
USING_NS_CC;
USING_NS_CC_EXT;


class Ball :public Base
{
public:
	Ball();
	~Ball();
	CREATE_FUNC(Ball);
	virtual bool init();
	void rotateAction();
	virtual void initBody();
	
	void setSpritePosition(float x, float y);

	void doAction(const char* actionName);

	void jump();

	void step(float dt);

	void setBallColor(CylinderColor color);

	CylinderColor getBallColor();

	void setTimes();
private:
	CylinderColor cylinderColor;
	int times;
};
#endif

