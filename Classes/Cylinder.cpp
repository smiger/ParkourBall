#include "Cylinder.h"
#include "Base.h"

bool Cylinder::init(){
	//������ִ�и����init()�麯��
	if (!Base::init()){
		return false;
	}
	auto sprite = Sprite::createWithSpriteFrameName("cylinder.png");
	//�󶨾���
	setSprite(sprite);

	getSprite()->setTag(cylinderTag);

	//�󶨸���
	//initBody();

	return true;
}

void Cylinder::initBody(){
	auto origin = Director::getInstance()->getVisibleOrigin();
	auto phyBody = PhysicsBody::createEdgeSegment(origin, Point(getConSize().width, 0), PHYSICSBODY_MATERIAL_DEFAULT, 1);
	//����Զ�����Щ��������໥���ò��������Ϸ��ʱ������Щ�໥���õ�֪ͨ
	phyBody->setCategoryBitmask(0x01);
	//����������ײ�������һ������ķ�����������߼�������Ƚϡ���������һ������ֵ���������ᷢ����ײ��
	phyBody->setCollisionBitmask(0x01);
	//ͨ�����ýӴ��������룬����Ծ���������ײ�󣬻ص������Ƿ�����Ӧ
	phyBody->setContactTestBitmask(0x01);
	//��������
	phyBody->setGravityEnable(false);
	//��Ħ��
	phyBody->getShape(0)->setFriction(0);
	//����ϵ��  1��ȫ����  0������
	phyBody->getShape(0)->setRestitution(1);
	this->setPhysicsBody(phyBody);
}
void Cylinder::setCylinderColor(CylinderColor color){
	this->cylinderColor = color;
	//���ò�ͬ��ɫ
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
	
	//�󶨾���
	setSprite(sprite);
}
CylinderColor Cylinder::getCylinderColor(){
	return cylinderColor;
}
