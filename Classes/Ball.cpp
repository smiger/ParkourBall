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
	//绑定精灵
	setSprite(sprite);
	//绑定刚体,initBody()重定义是为了让继承Base的不同类有不同的刚体
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
	//创建一个方形body
	auto body = PhysicsBody::createCircle(getConSize().width / 2); 
	body->getShape(0)->setRestitution(1);
	//你可以定义哪些物理刚体相互作用并且你的游戏何时接受这些相互作用的通知
	body->setCategoryBitmask(0x01);
	//这个刚体的碰撞掩码和另一个刚体的分类掩码进行逻辑与运算比较。如果结果是一个非零值，这个刚体会发生碰撞。
	body->setCollisionBitmask(0x01);
	//通过设置接触测试掩码，你可以决定发生碰撞后，回调函数是否有响应
	body->setContactTestBitmask(0x01);
	//无摩擦
	body->getShape(0)->setFriction(0);
	//让物理引擎对象和精灵对象关联起来
	getSprite()->setPhysicsBody(body);
}

void Ball::step(float dt){
	
}
void Ball::doAction(const char* actionName){
	
}
void Ball::jump(){
	auto mass = getSprite()->getPhysicsBody()->getMass() * times;
	//不会产生力，直接与body的速度叠加 产生新的速度
	getSprite()->getPhysicsBody()->applyImpulse(Vect(0, mass));
}
void Ball::setBallColor(CylinderColor color){
	this->cylinderColor = color;
	CCSpriteFrame* frame = NULL;
	//设置不同颜色
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
	//获取关卡
	int checkPoint = CCUserDefault::sharedUserDefault()->getIntegerForKey(curCheckPoint, 1);
	//获得倍数
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