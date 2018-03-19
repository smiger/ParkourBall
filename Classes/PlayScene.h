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
	//���ݹؿ���������
	int gravity;
	//�Ƿ�ʼ��Ϸ�ı�־λ
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

	//��ײ��ʼ����
	bool onContactBegin(cocos2d::PhysicsContact& contact);
	//��ײ�Ѿ�����
	void onContactPostSolve(cocos2d::PhysicsContact& contact);

	void initPhysicWorld();
	//���ݹؿ���������
	void playMusicByCheckPoint();
	//���ݹؿ�������ť
	void createControlBtn();
	//���ݹؿ������̳�
	void createPlayWay();
	//���ݹؿ���������
	void setWorldGravity();
	//���ݹؿ�����С����ɫ ��Ҫ�ǵڶ�������ɫ��ת���ذ��С����ɫĬ�Ͼ�Ҫ��ͬ��
	void setBallDefaultColor();

	void pressDown(cocos2d::Ref* pSender);

	//��Ϸ����
	void GameOver();

	//����ʱ��
	void addTime(float dt);
};
#endif

