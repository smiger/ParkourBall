#ifndef __PLAYSCENE_SCENE_H__
#define __PLAYSCENE_SCENE_H__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "Ball.h"
#include "BaseManager.h"

USING_NS_CC;
USING_NS_CC_EXT;

class PlayScene :public cocos2d::Layer
{
public:
	PlayScene();
	~PlayScene();
	static cocos2d::Scene* createScene();

	virtual bool init();

	CREATE_FUNC(PlayScene);

private:
	//重力
	int gravity;
	//游戏开始标识
	bool isGameBegin;
	double total;
	double time;
	int percent;

	LabelTTF* labelTTFCardNumber;
	int checkPoint;

	BaseManager *  m_manager;

	static Scene * _scene;

	Ball *ball;

	void update(float dt);

	void preLoadMusic();

	void onStart(cocos2d::Ref* pSender);

	void onEnter();

	void onEnterTransitionDidFinish();

	void onExit();

	//碰撞
	bool onContactBegin(cocos2d::PhysicsContact& contact);
	void onContactPostSolve(cocos2d::PhysicsContact& contact);

	void initPhysicWorld();
	//播放不同关卡的音乐
	void playMusicByCheckPoint();
	//创建控制按钮
	void createControlBtn();
	//玩游戏的方法
	void createPlayWay();
	//设置重力
	void setWorldGravity();
	//设置球默认颜色
	void setBallDefaultColor();

	void pressDown(cocos2d::Ref* pSender);

	void GameOver();

	void addTime(float dt);
};
#endif

