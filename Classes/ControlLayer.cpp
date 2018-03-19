#include "ControlLayer.h"
#include "cocos-ext.h"
#include "Constants.h"

USING_NS_CC_EXT;
ControlLayer::ControlLayer(){
}
ControlLayer::~ControlLayer(){
}
bool ControlLayer::init(){
	//必须先执行父类的init()虚函数
	if (!Node::init()){
		return false;
	}
	return true;
}
void ControlLayer::createSprite(int num){
	auto visibleSize = Director::getInstance()->getVisibleSize();
	for (int i = 1; i < num; i++){
		String *str = String::createWithFormat("control%d.png", i);
		String *str_p = String::createWithFormat("control_press_%d.png", i);
		Scale9Sprite* btnNormal = Scale9Sprite::createWithSpriteFrameName(str->getCString());
		Scale9Sprite* btnPress = Scale9Sprite::createWithSpriteFrameName(str_p->getCString());
		auto crouchBtn = ControlButton::create(btnNormal);
		crouchBtn->setPosition(visibleSize.width / num * i - crouchBtn->getContentSize().width / 2, crouchBtn->getContentSize().height/2);
		crouchBtn->setBackgroundSpriteForState(btnPress, Control::State::HIGH_LIGHTED);
		crouchBtn->addTargetWithActionForControlEvents(
			this,
			cccontrol_selector(ControlLayer::crouchDown),
			Control::EventType::TOUCH_DOWN);
	}
}
void ControlLayer::crouchDown(Ref* pSender, Control::EventType event){

}
void ControlLayer::crouchUp(Ref* pSender, Control::EventType event){
}