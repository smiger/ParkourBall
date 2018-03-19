#include "EndLeave.h"
#include "Constants.h"
#include "resource.h"
#include "PlayScene.h"
#include "MainScene.h"
#include "SimpleAudioEngine.h"
USING_NS_CC;
using namespace CocosDenshion;
EndLeave::EndLeave(){
}



EndLeave::~EndLeave(){

}

Scene * EndLeave::createScene(){
	auto scene = Scene::create();
	EndLeave *layer = EndLeave::create();
	scene->addChild(layer);
	return scene;

}
bool EndLeave::init(){
	if (!Layer::init()){
		return false;
	}
	//��ȡ�ɼ��ߴ�
	auto size = Director::getInstance()->getVisibleSize();
	auto zeroPos = Director::getInstance()->getVisibleOrigin();
	auto centerPos = Point(size.width / 2, size.height / 2);

	//����һ����������
	auto *spriteBG = Sprite::create(END_LEAVE);
	spriteBG->setPosition(centerPos);
	this->addChild(spriteBG);

	//�����˵�
	auto *menu = Menu::create();
	menu->setPosition(zeroPos);
	//��ť
	auto * menuCheckPoint = MenuItemSprite::create(Sprite::create(MENU_NORMAL_SMALL), Sprite::create(MENU_PRESS_SMALL)
		, CC_CALLBACK_1(EndLeave::menu, this));
	menuCheckPoint->setPosition(Point(menuCheckPoint->getContentSize().width/2,size.height - menuCheckPoint->getContentSize().height/2));
	
	menu->addChild(menuCheckPoint);

	this->addChild(menu);
	return true;
}
void EndLeave::menu(cocos2d::Ref* pSender){
	//�ж��Ƿ񲥷���Ч
	if (CCUserDefault::sharedUserDefault()->getBoolForKey("sound", true)){
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(MUSIC_EFFECT, false);
	}
	auto mainScene = TransitionFade::create(1, MainScene::createScene());
	Director::getInstance()->replaceScene(mainScene);
}

