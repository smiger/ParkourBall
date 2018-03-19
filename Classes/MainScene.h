#ifndef __MAINSCENE_SCENE_H__
#define __MAINSCENE_SCENE_H__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "Ball.h"

USING_NS_CC;
USING_NS_CC_EXT;

class MainScene :public cocos2d::Layer
{
public:
	MainScene(); 
	~MainScene();
	static cocos2d::Scene* createScene();

	virtual bool init();

	CREATE_FUNC(MainScene);
private:
	int checkPoint;

	static Scene *_scene;

	Ball *ball;

	void update(float dt);

	bool saveScreenShot();

	void initPhysicWorld();

	void preLoadMusic();

	void onPlay(cocos2d::Ref* pSender);

	void onEnter();

	void onEnterTransitionDidFinish();

	void onExit();

	//��ײ��ʼ����
	bool onContactBegin(cocos2d::PhysicsContact& contact);
};
#endif

