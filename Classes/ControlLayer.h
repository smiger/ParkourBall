#ifndef __CONTROLLAYER_SCENE_H__
#define __CONTROLLAYER_SCENE_H__
#include "cocos2d.h"
#include "cocos-ext.h"
USING_NS_CC;
USING_NS_CC_EXT;

class ControlLayer: public Node{
public:
	ControlLayer();
	~ControlLayer();
	CREATE_FUNC(ControlLayer);
	bool init();
	void createSprite(int num);
	void crouchDown(Ref* pSender, Control::EventType event);
	void crouchUp(Ref* pSender, Control::EventType event);
private:
	Vector<Sprite*> spriteVec;

};
#endif