#include "PlayScene.h"
#include "resource.h"
#include "PlayScene.h"
#include "Constants.h"
#include "SimpleAudioEngine.h"
#include "BaseManager.h"
#include "EndLayer.h"
#include "LeaveLayer.h"
USING_NS_CC;
using namespace CocosDenshion;

Scene * PlayScene::_scene = NULL;

PlayScene::PlayScene(){
	gravity = 0;
	isGameBegin = false;
	total = 0;
	time = 0;
	percent = 0;
	ball = NULL;
	m_manager = NULL;
	checkPoint = 1;
	labelTTFCardNumber = NULL;
}
PlayScene::~PlayScene(){

}

Scene * PlayScene::createScene(){
	//ʹ��Chipmunk2D�������档Cocos2d-x��������Chipmunk JSB API��
	//һ������������,��һ����������̵ġ�����ʹ�ø����Ѻõ��������ӿ�

	_scene = Scene::createWithPhysics();
	//����Debugģʽ����ῴ������ı��汻������Χ����ҪΪ���ڵ����и����׵ع۲�
	//_scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
	_scene->getPhysicsWorld()->setGravity(Vect(0, -1300));
	PlayScene *layer = PlayScene::create();
	_scene->addChild(layer);

	return _scene;

}

bool PlayScene::init(){
	if (!Layer::init()){
		return false;
	}
	//��ȡ�ؿ�
	checkPoint = CCUserDefault::sharedUserDefault()->getIntegerForKey(curCheckPoint, 1);

	//��ȡ�ɼ��ߴ�
	auto size = Director::getInstance()->getVisibleSize();
	auto zeroPos = Director::getInstance()->getVisibleOrigin();
	auto centerPos = Point(size.width / 2, size.height / 2);

	//Ԥ��������
	preLoadMusic();

	//SpriteFrameCacheһ����������plist�ļ�
	SpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(s_plist_parkour, s_img_parkour);

	//����һ����������
	auto *spriteBG = Sprite::create(PLAY_BG);
	spriteBG->setPosition(centerPos);
	this->addChild(spriteBG,0);
	
	//���С��
	ball = Ball::create();
	
	//����λ��
	ball->setPosition(size.width/4, size.height/2 + ball->getConSize().height / 2);
	this->addChild(ball,2); 
	setBallDefaultColor();

	initPhysicWorld();
	//������߾���
	_scene->getPhysicsWorld()->setAutoStep(false);
	//���º���
	this->schedule(schedule_selector(PlayScene::update), 0.02f, kRepeatForever, 0);
	
	//������  �Զ�����cylinder������
	m_manager = BaseManager::create();
	this->addChild(m_manager,1);
	
	//����Ч��
	ParticleSystem *_emitter = ParticleRain::create();
	_emitter->setAutoRemoveOnFinish(true);
	_emitter->setPosition(size.width/2,size.height);
	this->addChild(_emitter, 5);

	//��ɰٷֱ�
	labelTTFCardNumber = LabelTTF::create("0", "HiraKakuProN-w6", 50);
	labelTTFCardNumber->setColor(Color3B::WHITE);
	labelTTFCardNumber->setPosition(
		Point(size.width / 2, size.height / 2 + labelTTFCardNumber->getContentSize().height + 20));
	this->addChild(labelTTFCardNumber, 2);
	//Ĭ��0%
	labelTTFCardNumber->setString(
		StringUtils::format("%d%%", 0));
	//�����̳̲˵�
	createPlayWay();

	return true;
}
void PlayScene::onStart(cocos2d::Ref* pSender){
	isGameBegin = true;
	//�Ƴ��̳̲˵�
	this->removeChildByTag(playwayTag);
	//����
	playMusicByCheckPoint();
	//��������
	setWorldGravity();
	//������
	createControlBtn();
	//С��ʼ��Ծ
	ball->jump();
	//����ʱ��
	this->schedule(schedule_selector(PlayScene::addTime), 1.0f, kRepeatForever, 1);

}
//�������ں���
void PlayScene::onEnter(){
	Layer::onEnter();
	auto contactListenner = EventListenerPhysicsContact::create();

	//CC_CALLBACK��ʾ�ص������Ĳ�������
	contactListenner->onContactBegin = CC_CALLBACK_1(PlayScene::onContactBegin, this);
	contactListenner->onContactPostSolve = CC_CALLBACK_1(PlayScene::onContactPostSolve, this);
	//_eventDispatcherӦ����һ��ʱ���ɷ���ȫ�ֱ�������ȫ������������ǵ�
	_eventDispatcher->addEventListenerWithSceneGraphPriority(contactListenner, this);

	
}
void PlayScene::onEnterTransitionDidFinish(){
	Layer::onEnterTransitionDidFinish();
}
void PlayScene::onExit(){
	Layer::onExit();
	//ȡ���¼��ɷ�����
	_eventDispatcher->removeEventListenersForTarget(this);
}


void PlayScene::preLoadMusic(){
	SimpleAudioEngine * audioEngin = SimpleAudioEngine::getInstance();
	audioEngin->preloadBackgroundMusic(MUSIC_ONE);
	audioEngin->preloadBackgroundMusic(MUSIC_TWO);
	audioEngin->preloadBackgroundMusic(MUSIC_THREE);
	audioEngin->preloadBackgroundMusic(MUSIC_FOUR);
	audioEngin->preloadBackgroundMusic(MUSIC_FIVE);
	audioEngin->preloadBackgroundMusic(MUSIC_SIX);
}
bool PlayScene::onContactBegin(PhysicsContact& contact){
	auto size = Director::getInstance()->getVisibleSize();
	if (!isGameBegin){
		return true;
	}
	//�ж���ɫ�Ƿ���ȷ
	auto body1 = (Sprite *)contact.getShapeA()->getBody()->getNode();
	auto body2 = (Sprite *)contact.getShapeB()->getBody()->getNode();
	CylinderColor ballColor = normal;
	CylinderColor cylinderColor = normal;
	
	//��ȡ��ɫ
	if (body1->getTag() == ballTag){
		ballColor = ball->getBallColor();
	}
	if (body2->getTag() == ballTag){
		ballColor = ball->getBallColor();
	}

	//�ذ�û�и��� ԭ������Ħ����ײ��Ȼ��������� ���Եذ�ȡ���˸��� ���Ǹ���λ�û�ȡ��ɫ
	list <Cylinder*> cylinderList = m_manager->getCylinderList();
	Cylinder* cylinder;
	for (list<Cylinder*>::iterator iter = cylinderList.begin(); iter != cylinderList.end(); iter++){
		cylinder = (Cylinder*)*iter;


		if (cylinder->getPosition().x == size.width/4){
			//�ҵ�С������ĵذ�
			cylinderColor = cylinder->getCylinderColor();
			//Ĭ��0%


			break;
		}
	}
	//���ݹؿ��жϹ���
	switch (checkPoint)
	{
	case 1:
		//��ɫ����ȷ ��ɫ�ذ�ͽ����ذ������
		if (cylinderColor != normal && cylinderColor != endColor  && cylinderColor != ballColor){
			CocosDenshion::SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
			GameOver();
			auto endLayer = TransitionFade::create(1, EndLayer::createScene(false, percent));
			Director::getInstance()->replaceScene(endLayer);


			return false;
		}
		else if (cylinderColor == endColor){
			//�ж��Ƿ񲥷���Ч
			if (CCUserDefault::sharedUserDefault()->getBoolForKey("sound", true)){
				CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(MUSIC_WIN, false);
			}
			//��Ϸͨ��
			CocosDenshion::SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();

			//������һ��
			String *c_str = String::createWithFormat("checkPoint%i", checkPoint + 1);
			CCUserDefault::sharedUserDefault()->setBoolForKey(c_str->getCString(), true);
			//�л�����
			auto endLayer = TransitionFade::create(1, EndLayer::createScene(true, percent));
			Director::getInstance()->replaceScene(endLayer);
			return true;
		}
		break;
	case 2:
		//��ɫ����ȷ ��ɫ�ذ�ͽ����ذ������
		if (cylinderColor != normal && cylinderColor != endColor && cylinderColor == ballColor){
			CocosDenshion::SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
			GameOver();
			auto endLayer = TransitionFade::create(1, EndLayer::createScene(false, percent));
			Director::getInstance()->replaceScene(endLayer);
			return false;
		}
		//�������Ϸǰ�ɫ�ĵذ�
		else if (cylinderColor != normal && ballColor == normal){
			CocosDenshion::SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
			GameOver();
			auto endLayer = TransitionFade::create(1, EndLayer::createScene(false, percent));
			Director::getInstance()->replaceScene(endLayer);
			return false;
		}
		else if (cylinderColor == endColor){
			//�ж��Ƿ񲥷���Ч
			if (CCUserDefault::sharedUserDefault()->getBoolForKey("sound", true)){
				CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(MUSIC_WIN, false);
			}
			//��Ϸͨ��
			CocosDenshion::SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();

			//������һ��
			String *c_str = String::createWithFormat("checkPoint%i", checkPoint + 1);
			CCUserDefault::sharedUserDefault()->setBoolForKey(c_str->getCString(), true);
			//�л�����
			auto endLayer = TransitionFade::create(1, EndLayer::createScene(true, percent));
			Director::getInstance()->replaceScene(endLayer);
			return true;
		}
		break;
	case 3:
	case 4:
	case 5:
		//��ɫ����ȷ ��ɫ�ذ�ͽ����ذ������
		if (cylinderColor != normal && cylinderColor != endColor  && cylinderColor != ballColor){
			CocosDenshion::SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
			GameOver();
			auto endLayer = TransitionFade::create(1, EndLayer::createScene(false, percent));
			Director::getInstance()->replaceScene(endLayer);
			return false;
		}
		else if (cylinderColor == endColor){
			//�ж��Ƿ񲥷���Ч
			if (CCUserDefault::sharedUserDefault()->getBoolForKey("sound", true)){
				CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(MUSIC_WIN, false);
			}
			//��Ϸͨ��
			CocosDenshion::SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();

			//������һ��
			String *c_str = String::createWithFormat("checkPoint%i", checkPoint + 1);
			CCUserDefault::sharedUserDefault()->setBoolForKey(c_str->getCString(), true);
			//�л�����
			auto endLayer = TransitionFade::create(1, EndLayer::createScene(true, percent));
			Director::getInstance()->replaceScene(endLayer);
			return true;
		}
		break;
	case 6:
		//��ɫ����ȷ ��ɫ�ذ�ͽ����ذ������
		if (cylinderColor != normal && cylinderColor != endColor  && cylinderColor != ballColor){
			CocosDenshion::SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
			GameOver();
			auto endLayer = TransitionFade::create(1, EndLayer::createScene(false, percent));
			Director::getInstance()->replaceScene(endLayer);
			return false;
		}
		else if (cylinderColor == endColor){
			//�ж��Ƿ񲥷���Ч
			if (CCUserDefault::sharedUserDefault()->getBoolForKey("sound", true)){
				CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(MUSIC_WIN, false);
			}
			//��Ϸͨ��
			CocosDenshion::SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();

			//���һ��ͨ�أ������ֽ���
			auto scene = TransitionFade::create(1, LeaveLayer::createScene());
			Director::getInstance()->replaceScene(scene);
			return true;
		}
		break;
	default:
		break;
	}
	ball->rotateAction();
	m_manager->update(0);
	return true;
}
void PlayScene::onContactPostSolve(PhysicsContact& contact){
	
}
void PlayScene::update(float t){
	//�����Ҫ��߾��ȣ����Խ�dt��Ϊ����step��������ȡ��autoStep
	int steps = 2;
	float dt = CCDirector::getInstance()->getAnimationInterval() / (float)steps;
	PhysicsWorld* s = getScene()->getPhysicsWorld();
	for (int i = 0; i<steps; i++){
		s->step(dt);
	}

}
void PlayScene::initPhysicWorld(){
	//�������޳��ĵ���
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto origin = Director::getInstance()->getVisibleOrigin();
	auto body = PhysicsBody::createEdgeSegment(origin, Point(visibleSize.width, 0), PHYSICSBODY_MATERIAL_DEFAULT, 1);
	//��ȫ����
	body->getShape(0)->setRestitution(1);
	//��Ħ��
	body->getShape(0)->setFriction(0);
	//��������
	body->setGravityEnable(false);
	//3������
	body->setCategoryBitmask(0x01);
	body->setCollisionBitmask(0x01);
	body->setContactTestBitmask(0x01);
	auto edgeNode = Node::create();
	edgeNode->setPosition(0, visibleSize.height/2);
	edgeNode->setPhysicsBody(body);
	edgeNode->setTag(edgeNodeTag);
	this->addChild(edgeNode, 2);
}
void PlayScene::createControlBtn(){
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto zeroPos = Director::getInstance()->getVisibleOrigin();
	auto temp = Cylinder::create();
	//�����˵�
	auto *menu = Menu::create();
	menu->setPosition(zeroPos);
	//���ݹؿ�������ť
	int num = 2;
	switch (checkPoint)
	{
	case 1:
		//����ʱ��
		total = songTime1;
		//��ť����
		num = 2;
		for (int i = 1; i <= num; i++){
			String *str = String::createWithFormat("control%d.png", i);
			String *str_p = String::createWithFormat("control_press_%d.png", i);
			//����һ������˵�
			auto * menuItemPLay = MenuItemSprite::create(Sprite::createWithSpriteFrameName(str->getCString()), Sprite::createWithSpriteFrameName(str_p->getCString()), CC_CALLBACK_1(PlayScene::pressDown, this));
			float x = (visibleSize.width / num) / menuItemPLay->getContentSize().width;

			float y = (visibleSize.height/4) / menuItemPLay->getContentSize().height;
			menuItemPLay->setScale(x, y * 2);
			float orignX = menuItemPLay->getContentSize().width * x;
			float origny = menuItemPLay->getContentSize().height * y;
			menuItemPLay->setPosition(zeroPos.x + visibleSize.width / num * i - orignX / 2,zeroPos.y );
			menuItemPLay->setTag(i);
			menu->addChild(menuItemPLay);
		}
		this->addChild(menu);
		break;
	case 2:
		//����ʱ��
		total = songTime2;
		//��ť����
		num = 2; 
		for (int i = 1; i <= num; i++){
			String *str = String::createWithFormat("control%d.png", i);
			String *str_p = String::createWithFormat("control_press_%d.png", i);
			//����һ������˵�
			auto * menuItemPLay = MenuItemSprite::create(Sprite::createWithSpriteFrameName(str->getCString()), Sprite::createWithSpriteFrameName(str_p->getCString()), CC_CALLBACK_1(PlayScene::pressDown, this));
			float x = (visibleSize.width / num) / menuItemPLay->getContentSize().width;
			float y = (visibleSize.height/4) / menuItemPLay->getContentSize().height;
			menuItemPLay->setScale(x, y* 2);
			float orignX = menuItemPLay->getContentSize().width * x;
			float origny = menuItemPLay->getContentSize().height * y;
			menuItemPLay->setPosition(zeroPos.x + visibleSize.width / num * i - orignX / 2,zeroPos.y );
			menuItemPLay->setTag(i);
			menu->addChild(menuItemPLay);
		}
		this->addChild(menu);
		break;
	
	case 3:
		//����ʱ��
		total = songTime3;
		//��ť����
		num = 3;
		for (int i = 1; i <= num; i++){
			String *str = String::createWithFormat("control%d.png", i);
			String *str_p = String::createWithFormat("control_press_%d.png", i);
			//����һ������˵�
			auto * menuItemPLay = MenuItemSprite::create(Sprite::createWithSpriteFrameName(str->getCString()), Sprite::createWithSpriteFrameName(str_p->getCString()), CC_CALLBACK_1(PlayScene::pressDown, this));
			float x = (visibleSize.width / num) / menuItemPLay->getContentSize().width;
			float y = (visibleSize.height/4) / menuItemPLay->getContentSize().height;
			menuItemPLay->setScale(x, y* 2);
			float orignX = menuItemPLay->getContentSize().width * x;
			float origny = menuItemPLay->getContentSize().height * y;
			menuItemPLay->setPosition(zeroPos.x + visibleSize.width / num * i - orignX / 2,zeroPos.y );
			menuItemPLay->setTag(i);
			menu->addChild(menuItemPLay);
		}
		this->addChild(menu);
		break;
	case 4:
		//����ʱ��
		total = songTime4;
		//��ť����
		num = 3;
		for (int i = 1; i <= num; i++){
			String *str = String::createWithFormat("control%d.png", i);
			String *str_p = String::createWithFormat("control_press_%d.png", i);
			//����һ������˵�
			auto * menuItemPLay = MenuItemSprite::create(Sprite::createWithSpriteFrameName(str->getCString()), Sprite::createWithSpriteFrameName(str_p->getCString()), CC_CALLBACK_1(PlayScene::pressDown, this));
			float x = (visibleSize.width / num) / menuItemPLay->getContentSize().width;
			float y = (visibleSize.height/4) / menuItemPLay->getContentSize().height;
			menuItemPLay->setScale(x, y* 2);
			float orignX = menuItemPLay->getContentSize().width * x;
			float origny = menuItemPLay->getContentSize().height * y;
			menuItemPLay->setPosition(zeroPos.x + visibleSize.width / num * i - orignX / 2,zeroPos.y);
			menuItemPLay->setTag(i);
			menu->addChild(menuItemPLay);
		}
		this->addChild(menu);
		break;
	case 5:
		//����ʱ��
		total = songTime5;
		//��ť����
		num = 4;
		for (int i = 1; i <= num; i++){
			String *str = String::createWithFormat("control%d.png", i);
			String *str_p = String::createWithFormat("control_press_%d.png", i);
			//����һ������˵�
			auto * menuItemPLay = MenuItemSprite::create(Sprite::createWithSpriteFrameName(str->getCString()), Sprite::createWithSpriteFrameName(str_p->getCString()), CC_CALLBACK_1(PlayScene::pressDown, this));
			float x = (visibleSize.width / num) / menuItemPLay->getContentSize().width;
			float y = (visibleSize.height/4) / menuItemPLay->getContentSize().height;
			menuItemPLay->setScale(x, y* 2);
			float orignX = menuItemPLay->getContentSize().width * x;
			float origny = menuItemPLay->getContentSize().height * y;
			menuItemPLay->setPosition(zeroPos.x + visibleSize.width / num * i - orignX / 2,zeroPos.y);
			menuItemPLay->setTag(i);
			menu->addChild(menuItemPLay);
		}
		this->addChild(menu);
		break;
	case 6:
		//����ʱ��
		total = songTime6;
		//��ť����
		num = 4;
		for (int i = 1; i <= num; i++){
			String *str = String::createWithFormat("control%d.png", i);
			String *str_p = String::createWithFormat("control_press_%d.png", i);
			//����һ������˵�
			auto * menuItemPLay = MenuItemSprite::create(Sprite::createWithSpriteFrameName(str->getCString()), Sprite::createWithSpriteFrameName(str_p->getCString()), CC_CALLBACK_1(PlayScene::pressDown, this));
			float x = (visibleSize.width / num) / menuItemPLay->getContentSize().width;
			float y = (visibleSize.height/4) / menuItemPLay->getContentSize().height;
			menuItemPLay->setScale(x, y* 2);
			float orignX = menuItemPLay->getContentSize().width * x;
			float origny = menuItemPLay->getContentSize().height * y;
			menuItemPLay->setPosition(zeroPos.x + visibleSize.width / num * i - orignX / 2,zeroPos.y );
			menuItemPLay->setTag(i);
			menu->addChild(menuItemPLay);
		}
		this->addChild(menu);
		break;
	default:
		break;
	}
	
}
void PlayScene::pressDown(cocos2d::Ref* pSender){

	//ǿ������ת��
	CCMenuItem* item = dynamic_cast<CCMenuItem*>(pSender);
	//�õ��˵����ID
	int tag = item->getTag();
	switch (tag)
	{
	case 1:
		//red
		ball->setBallColor(red);
		break;
	case 2:
		//green
		ball->setBallColor(green);
		break;
	case 3:
		//blue
		ball->setBallColor(blue);
		break;
	case 4:
		//yellow
		ball->setBallColor(yellow);
		break;
	
	default:
		break;
	}
}
void PlayScene::playMusicByCheckPoint(){
	
		//��������
		switch (checkPoint)
		{
		case 1:
			CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic(MUSIC_ONE, false);	
			break;
		case 2:
			CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic(MUSIC_TWO, false);//���ű�������
			break;
		case 3:
			CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic(MUSIC_THREE, false);//���ű�������
			break;
		case 4:
			CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic(MUSIC_FOUR, false);//���ű�������
			break;
		case 5:
			CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic(MUSIC_FIVE, false);//���ű�������
			break;
		case 6:
			CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic(MUSIC_SIX, false);//���ű�������
			break;
		default:
			break;
		}
	//�ж��Ƿ񲥷�����
	if (!CCUserDefault::sharedUserDefault()->getBoolForKey("sound", true)){
		CocosDenshion::SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
	}
	
}
void PlayScene::createPlayWay(){
	//��ȡ�ɼ��ߴ�
	auto size = Director::getInstance()->getVisibleSize();
	auto zeroPos = Director::getInstance()->getVisibleOrigin();
	auto centerPos = Point(size.width / 2, size.height / 2);
	//�����˵�
	auto *menu = Menu::create();
	menu->setPosition(zeroPos);
	if (checkPoint == 1){
		//����һ������˵�
		auto * menuItemPLay = MenuItemSprite::create(Sprite::create(PLAY_WAY_1), Sprite::create(PLAY_WAY_1), CC_CALLBACK_1(PlayScene::onStart, this));
		menuItemPLay->setPosition(centerPos);
		menu->addChild(menuItemPLay);
	}
	else if (checkPoint == 2){
		//����һ������˵�
		auto * menuItemPLay = MenuItemSprite::create(Sprite::create(PLAY_WAY_2), Sprite::create(PLAY_WAY_2), CC_CALLBACK_1(PlayScene::onStart, this));
		menuItemPLay->setPosition(centerPos);
		menu->addChild(menuItemPLay);
	}
	else if (checkPoint == 3){
		//����һ������˵�
		auto * menuItemPLay = MenuItemSprite::create(Sprite::create(PLAY_WAY_3), Sprite::create(PLAY_WAY_3), CC_CALLBACK_1(PlayScene::onStart, this));
		menuItemPLay->setPosition(centerPos);
		menu->addChild(menuItemPLay);
	}
	else if (checkPoint == 4){
		//����һ������˵�
		auto * menuItemPLay = MenuItemSprite::create(Sprite::create(PLAY_WAY_4), Sprite::create(PLAY_WAY_4), CC_CALLBACK_1(PlayScene::onStart, this));
		menuItemPLay->setPosition(centerPos);
		menu->addChild(menuItemPLay);
	}
	else if (checkPoint == 5 ){
		//����һ������˵�
		auto * menuItemPLay = MenuItemSprite::create(Sprite::create(PLAY_WAY_5), Sprite::create(PLAY_WAY_5), CC_CALLBACK_1(PlayScene::onStart, this));
		menuItemPLay->setPosition(centerPos);
		menu->addChild(menuItemPLay);
	}
	else if ( checkPoint == 6){
		//����һ������˵�
		auto * menuItemPLay = MenuItemSprite::create(Sprite::create(PLAY_WAY_6), Sprite::create(PLAY_WAY_6), CC_CALLBACK_1(PlayScene::onStart, this));
		menuItemPLay->setPosition(centerPos);
		menu->addChild(menuItemPLay);
	}
	menu->setTag(playwayTag);
	this->addChild(menu,10);
}
void PlayScene::setWorldGravity(){
	//设置物理世界的重力
	switch (checkPoint)
	{
	case 1:
		gravity = gravity_1;
		break;
	case 2:
		gravity = gravity_2;
		break;
	case 3:
		gravity = gravity_3;
		break;
	case 4:
		gravity = gravity_4;
		break;
	case 5:
		gravity = gravity_5;
		break;
	case 6:
		gravity = gravity_6;
		break;
	default:
		break;
	}
	_scene->getPhysicsWorld()->setGravity(Vect(0,gravity));
}

void PlayScene::setBallDefaultColor(){

	switch (checkPoint)
	{
	case 1:
		ball->setBallColor(red);
		break;
	case 2:
		//��ɫ��ת  �ذ�Ĭ���Ǻ�ɫ С��Ĭ�Ͼ�Ҫ����ɫ
		ball->setBallColor(green);
		break;
	case 3:
		ball->setBallColor(red);
		break;
	case 4:
		ball->setBallColor(red);
		break;
	case 5:
		ball->setBallColor(red);
		break;
	case 6:
		ball->setBallColor(red);
		break;
	default:
		break;
	}
}
void PlayScene::addTime(float dt){
	//����ʱ�� �ó��ٷֱ�
	time += 1;
	percent = time / total * 100;
	if (percent < 0){
		percent = 0;
	}
	if (percent > 100){
		percent = 100;
	}
	labelTTFCardNumber->setString(
		StringUtils::format("%d%%", percent));
	

	//�жϽ��ȴﵽ100% ����ͨ��
	if (percent >= 90){
		m_manager->setMove(false);
	}
}
void PlayScene::GameOver(){
	//�ж��Ƿ񲥷���Ч
	if (CCUserDefault::sharedUserDefault()->getBoolForKey("sound", true)){
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(MUSIC_LOSE, false);
	}
	this->removeChildByTag(edgeNodeTag);
}
