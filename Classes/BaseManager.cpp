#include "BaseManager.h"
#include "Constants.h"

BaseManager::BaseManager()
{
	isMove = true;
	checkPoint = 1;
}

BaseManager::~BaseManager()
{
}

bool BaseManager::init(){
	if (!Node::init()){
		return false;
	}
	//获取当前关卡
	checkPoint = CCUserDefault::sharedUserDefault()->getIntegerForKey(curCheckPoint, 1);

	createCylinder();

	return true;
}
void BaseManager::createCylinder(){
	auto size = Director::getInstance()->getVisibleSize();
	Cylinder *cylinder = NULL;
	for (int i = 1; i <= cylinderNum; ++i){
		cylinder = Cylinder::create();
		cylinder->setVisible(true);
		cylinder->setPosition(size.width/4 + cylinder->getConSize().width * (i - 1), size.height/2 );
		if (i > 5){
			int num = 2;
			switch (checkPoint)
			{
			case 1:
			case 2:
				num = CCRANDOM_0_1() * 10;
				if (num <= 5){
					cylinder->setCylinderColor(red);
				}
				else{
					cylinder->setCylinderColor(green);
				}
			break;
			case 3:
			case 4:
				num = CCRANDOM_0_1() * 30;
				if (num <= 10){
					cylinder->setCylinderColor(red);
				}
				else if (num <= 20){
					cylinder->setCylinderColor(green);
				}
				else if (num <= 30){
					cylinder->setCylinderColor(blue);
				}
			
				break;
			
			case 5:
			case 6:
				num = CCRANDOM_0_1() * 40;
				if (num <= 10){
					cylinder->setCylinderColor(red);
				}
				else if (num <= 20){
					cylinder->setCylinderColor(green);
				}
				else if (num <= 30){
					cylinder->setCylinderColor(blue);
				}
				else if (num <= 40){
					cylinder->setCylinderColor(yellow);
				}
				break;
			default:
				break;
			}
		}
		else{
			//设置为红色
			cylinder->setCylinderColor(red);
		}
		this->addChild(cylinder);
		cylinderList.push_back(cylinder);
	}
}

void BaseManager::update(float dt){
	auto size = Director::getInstance()->getVisibleSize();
	Ref * obj = NULL;
	Cylinder * cylinder = NULL;
	//是否移动圆盘
	if (isMove){
		for (list<Cylinder*>::iterator iter = cylinderList.begin(); iter != cylinderList.end(); iter++){
			cylinder = (Cylinder*)*iter;
			if (cylinder->getPositionX() <= -cylinder->getConSize().width / 2){
				//如果圆盘已经移出左屏幕外，则移到右边最后面
				Cylinder *front = cylinder;
				Cylinder *back = cylinderList.back();
				front->setPosition(back->getPosition().x + back->getConSize().width, back->getPosition().y);
				//重新设置颜色
				int num = 2;
				switch (checkPoint)
				{
				case 1:
				case 2:
					num = CCRANDOM_0_1() * 10;
					if (num <= 5){
						front->setCylinderColor(red);
					}
					else{
						front->setCylinderColor(green);
					}
					break;
				case 3:
				case 4:
					num = CCRANDOM_0_1() * 30;
					if (num <= 10){
						front->setCylinderColor(red);
					}
					else if (num <= 20){
						front->setCylinderColor(green);
					}
					else if (num <= 30){
						front->setCylinderColor(blue);
					}

					break;

				case 5:
				case 6:
					num = CCRANDOM_0_1() * 40;
					if (num <= 10){
						front->setCylinderColor(red);
					}
					else if (num <= 20){
						front->setCylinderColor(green);
					}
					else if (num <= 30){
						front->setCylinderColor(blue);
					}
					else if (num <= 40){
						front->setCylinderColor(yellow);
					}
					break;
				default:
					break;
				}
				iter = cylinderList.erase(iter);
				cylinderList.push_back(front);
			}
		}
	}
	else{
		//最后一个设置结束颜色
		Cylinder *back = cylinderList.back();
		back->setCylinderColor(endColor);
	}
	
	for (list<Cylinder*>::iterator iter = cylinderList.begin(); iter != cylinderList.end(); iter++){
		cylinder = (Cylinder*)*iter;
		ActionInterval * moveDown = MoveBy::create(0.05f, Point(0, -10));
		ActionInterval * moveUp = MoveBy::create(0.05f, Point(0, 10));
		ActionInterval * moveBy = MoveBy::create(0.2f, Point(-cylinder->getConSize().width, 0));
		DelayTime * delay = DelayTime::create(0.1);
		
		if (cylinder->getPosition().x == size.width/4){
			//球碰撞的圆盘上下运动并向左移
			cylinder->runAction(Sequence::create(moveDown, moveUp, moveBy, NULL));
		}
		else {
			cylinder->runAction(Sequence::create(delay, moveBy, NULL));
		}
		
	}

}
list <Cylinder*>  BaseManager::getCylinderList(){
	return cylinderList;
}
void BaseManager::setMove(bool isMove){
	this->isMove = isMove;
}
