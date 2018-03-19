#ifndef __Base__H__
#define __Base__H__

#include "cocos2d.h"

USING_NS_CC;

class Base : public Node
{
public:
	Base();
	//虚析构函数
	virtual ~Base();
	//继承自Node的虚函数 不同派生类定义自己的版本
	virtual bool init();
	//绑定刚体  不同派生类自己定义不同的刚体
	virtual void initBody();
	//设置精灵
	void setSprite(Sprite* sprite);
	//获取精灵
	Sprite * getSprite();
	//获取精灵大小
	Size getConSize();

private:
	Sprite * sprite;
	ParticleSystem *_emitter;
};
#endif

