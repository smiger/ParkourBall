#include "LeaveLayer.h"
#include "Constants.h"
#include "resource.h"
#include "PlayScene.h"
#include "MainScene.h"
#include "EndLeave.h"
#include "EndNotLeave.h"
#include "SimpleAudioEngine.h"
USING_NS_CC;
using namespace CocosDenshion;
LeaveLayer::LeaveLayer(){
}



LeaveLayer::~LeaveLayer(){

}

Scene * LeaveLayer::createScene(){
	auto scene = Scene::create();
	LeaveLayer *layer = LeaveLayer::create();
	scene->addChild(layer);
	return scene;

}
bool LeaveLayer::init(){
	if (!Layer::init()){
		return false;
	}
	//��ȡ�ɼ��ߴ�
	auto size = Director::getInstance()->getVisibleSize();
	auto zeroPos = Director::getInstance()->getVisibleOrigin();
	auto centerPos = Point(size.width / 2, size.height / 2);

	//����һ����������
	auto *spriteBG = Sprite::create(LEAVE_BG);
	spriteBG->setPosition(centerPos);
	this->addChild(spriteBG);

	//�����˵�
	auto *menu = Menu::create();
	menu->setPosition(zeroPos);
	//�뿪��ť
	auto * menuLeave = MenuItemSprite::create(Sprite::create(WANT_BTN), Sprite::create(WANT_BTN)
		, CC_CALLBACK_1(LeaveLayer::leave, this));
	menuLeave->setPosition(Point(centerPos.x - menuLeave->getContentSize().width/2,centerPos.y - menuLeave->getContentSize().height/2));
	menu->addChild(menuLeave);

	//���뿪��ť
	auto * menuNotLeave = MenuItemSprite::create(Sprite::create(NOTWANT_BTN), Sprite::create(NOTWANT_BTN)
		, CC_CALLBACK_1(LeaveLayer::notLeave, this));
	menuNotLeave->setPosition(Point(centerPos.x + menuLeave->getContentSize().width / 2, centerPos.y -menuNotLeave->getContentSize().height/2));
	menu->addChild(menuNotLeave);

	this->addChild(menu);

	//����Ч��
	ParticleSystem *_emitter = ParticleRain::create();
	_emitter->setAutoRemoveOnFinish(true);
	_emitter->setPosition(size.width / 2, size.height);
	this->addChild(_emitter, 5);

	//�ж��Ƿ񲥷�����
	CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic(MUSIC_END);
	if (CCUserDefault::sharedUserDefault()->getBoolForKey("sound", true)){
		CocosDenshion::SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic(MUSIC_END,true);
	}
	else{
		CocosDenshion::SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
	}
	return true;
}
void LeaveLayer::leave(cocos2d::Ref* pSender){
	//�ж��Ƿ񲥷���Ч
	if (CCUserDefault::sharedUserDefault()->getBoolForKey("sound", true)){
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(MUSIC_EFFECT, false);
	}
	auto scene = TransitionFade::create(1, EndLeave::createScene());
	Director::getInstance()->replaceScene(scene);
}

void LeaveLayer::notLeave(cocos2d::Ref* pSender){
	//�ж��Ƿ񲥷���Ч
	if (CCUserDefault::sharedUserDefault()->getBoolForKey("sound", true)){
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(MUSIC_EFFECT, false);
	}
	auto scene = TransitionFade::create(1, EndNotLeave::createScene());
	Director::getInstance()->replaceScene(scene);
}


