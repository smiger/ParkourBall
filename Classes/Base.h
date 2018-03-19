#ifndef __Base__H__
#define __Base__H__

#include "cocos2d.h"

USING_NS_CC;

class Base : public Node
{
public:
	Base();
	//����������
	virtual ~Base();
	//�̳���Node���麯�� ��ͬ�����ඨ���Լ��İ汾
	virtual bool init();
	//�󶨸���  ��ͬ�������Լ����岻ͬ�ĸ���
	virtual void initBody();
	//���þ���
	void setSprite(Sprite* sprite);
	//��ȡ����
	Sprite * getSprite();
	//��ȡ�����С
	Size getConSize();

private:
	Sprite * sprite;
	ParticleSystem *_emitter;
};
#endif

