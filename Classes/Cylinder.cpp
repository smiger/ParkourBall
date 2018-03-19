#include "Cylinder.h"
#include "Base.h"

bool Cylinder::init(){
	//必须先执行父类的init()虚函数
	if (!Base::init()){
		return false;
	}
	auto sprite = Sprite::createWithSpriteFrameName("cylinder.png");
	//绑定精灵
	setSprite(sprite);

	getSprite()->setTag(cylinderTag);

	//绑定刚体
	//initBody();

	return true;
}

void Cylinder::initBody(){
	auto origin = Director::getInstance()->getVisibleOrigin();
	auto phyBody = PhysicsBody::createEdgeSegment(origin, Point(getConSize().width, 0), PHYSICSBODY_MATERIAL_DEFAULT, 1);
	//你可以定义哪些物理刚体相互作用并且你的游戏何时接受这些相互作用的通知
	phyBody->setCategoryBitmask(0x01);
	//这个刚体的碰撞掩码和另一个刚体的分类掩码进行逻辑与运算比较。如果结果是一个非零值，这个刚体会发生碰撞。
	phyBody->setCollisionBitmask(0x01);
	//通过设置接触测试掩码，你可以决定发生碰撞后，回调函数是否有响应
	phyBody->setContactTestBitmask(0x01);
	//不受重力
	phyBody->setGravityEnable(false);
	//无摩擦
	phyBody->getShape(0)->setFriction(0);
	//反弹系数  1完全反弹  0不反弹
	phyBody->getShape(0)->setRestitution(1);
	this->setPhysicsBody(phyBody);
}
void Cylinder::setCylinderColor(CylinderColor color){
	this->cylinderColor = color;
	//设置不同颜色
	Sprite* sprite = NULL;
	switch (color)
	{
	case normal:
		sprite = Sprite::createWithSpriteFrameName("cylinder_normal.png");
		break;
	case red:
		sprite = Sprite::createWithSpriteFrameName("cylinder_red.png");
		break;
	case green:
		sprite = Sprite::createWithSpriteFrameName("cylinder_green.png");
		break;
	case blue:
		sprite = Sprite::createWithSpriteFrameName("cylinder_blue.png");
		break;
	case yellow:
		sprite = Sprite::createWithSpriteFrameName("cylinder_yellow.png");
		break;
	case endColor:
		sprite = Sprite::createWithSpriteFrameName("cylinder_end.png");
		break;
	default:
		break;
	}
	
	//绑定精灵
	setSprite(sprite);
}
CylinderColor Cylinder::getCylinderColor(){
	return cylinderColor;
}
