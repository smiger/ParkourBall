#include "Ball.h"
#include "PlayScene.h"
#include "resource.h"
#include "Constants.h"

USING_NS_CC;
USING_NS_CC_EXT;
Ball::Ball(){
	cylinderColor = normal;
	times = 0;
}
Ball::~Ball(){

}
bool Ball::init(){
	if (!Base::init()){
		return false;
	}
	
	

	auto sprite = Sprite::createWithSpriteFrameName("normalSprite.png");
	//�󶨾���
	setSprite(sprite);
	//�󶨸���,initBody()�ض�����Ϊ���ü̳�Base�Ĳ�ͬ���в�ͬ�ĸ���
	initBody();
	
	getSprite()->setTag(ballTag);

	setTimes();
	return true;
}
void Ball::rotateAction(){
	RotateBy *rotate = RotateBy::create(0.3,180);
	getSprite()->runAction(rotate);
}

void Ball::initBody(){
	//����һ������body
	auto body = PhysicsBody::createCircle(getConSize().width / 2); 
	body->getShape(0)->setRestitution(1);
	//����Զ�����Щ��������໥���ò��������Ϸ��ʱ������Щ�໥���õ�֪ͨ
	body->setCategoryBitmask(0x01);
	//����������ײ�������һ������ķ�����������߼�������Ƚϡ���������һ������ֵ���������ᷢ����ײ��
	body->setCollisionBitmask(0x01);
	//ͨ�����ýӴ��������룬����Ծ���������ײ�󣬻ص������Ƿ�����Ӧ
	body->setContactTestBitmask(0x01);
	//��Ħ��
	body->getShape(0)->setFriction(0);
	//�������������;�������������
	getSprite()->setPhysicsBody(body);
}

void Ball::step(float dt){
	
}
void Ball::doAction(const char* actionName){
	
}
void Ball::jump(){
	auto mass = getSprite()->getPhysicsBody()->getMass() * times;
	//�����������ֱ����body���ٶȵ��� �����µ��ٶ�
	getSprite()->getPhysicsBody()->applyImpulse(Vect(0, mass));
}
void Ball::setBallColor(CylinderColor color){
	this->cylinderColor = color;
	CCSpriteFrame* frame = NULL;
	//���ò�ͬ��ɫ
	Sprite* sprite = NULL;
	switch (color)
	{
	case normal:
		frame = SpriteFrameCache::getInstance()->spriteFrameByName("normalSprite.png");
		break;
	case red:
		frame = SpriteFrameCache::getInstance()->spriteFrameByName("redSprite.png");
		break;
	case green:
		frame = SpriteFrameCache::getInstance()->spriteFrameByName("greenSprite.png");
		break;
	case blue:
		frame = SpriteFrameCache::getInstance()->spriteFrameByName("blueSprite.png");
		break;
	case yellow:
		frame = SpriteFrameCache::getInstance()->spriteFrameByName("yellowSprite.png");
		break;
	default:
		break;
	}
	getSprite()->setDisplayFrame(frame);
}
CylinderColor Ball::getBallColor(){
	return cylinderColor;
}
void Ball::setTimes(){
	//��ȡ�ؿ�
	int checkPoint = CCUserDefault::sharedUserDefault()->getIntegerForKey(curCheckPoint, 1);
	//��ñ���
	switch (checkPoint)
	{
	case 1:
		times = jumpImpulseTime_1;
		break;
	case 2:
		times = jumpImpulseTime_2;
		break;
	case 3:
		times = jumpImpulseTime_3;
		break;
	case 4:
		times = jumpImpulseTime_4;
		break;
	case 5:
		times = jumpImpulseTime_5;
		break;
	case 6:
		times = jumpImpulseTime_6;
		break;
	default:
		break;
	}
}