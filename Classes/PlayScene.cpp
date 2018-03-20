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
	//创建一个物理场景
	_scene = Scene::createWithPhysics();
	//_scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
	//设置物理世界的重力
	_scene->getPhysicsWorld()->setGravity(Vect(0, -1300));
	//添加层到物理世界中
	PlayScene *layer = PlayScene::create();
	_scene->addChild(layer);

	return _scene;

}

bool PlayScene::init(){
	if (!Layer::init()){
		return false;
	}
	//获取当前关卡
	checkPoint = CCUserDefault::sharedUserDefault()->getIntegerForKey(curCheckPoint, 1);

	auto size = Director::getInstance()->getVisibleSize();
	auto zeroPos = Director::getInstance()->getVisibleOrigin();
	auto centerPos = Point(size.width / 2, size.height / 2);

	//预加载音乐
	preLoadMusic();

	//加载plist
	SpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(s_plist_parkour, s_img_parkour);

	//背景
	auto *spriteBG = Sprite::create(PLAY_BG);
	spriteBG->setPosition(centerPos);
	this->addChild(spriteBG,0);
	
	//球
	ball = Ball::create();
	
	//球的初始位置
	ball->setPosition(size.width/4, size.height/2 + ball->getConSize().height / 2);
	this->addChild(ball,2); 
	setBallDefaultColor();

	initPhysicWorld();
	//禁用一个物理世界的auto step,然后通过设定PhysicsWorld::step(time)来手动刷新PhysicsWorld
	_scene->getPhysicsWorld()->setAutoStep(false);
	this->schedule(schedule_selector(PlayScene::update), 0.02f, kRepeatForever, 0);
	
	//cylinder管理类
	m_manager = BaseManager::create();
	this->addChild(m_manager,1);
	
	//粒子
	ParticleSystem *_emitter = ParticleRain::create();
	_emitter->setAutoRemoveOnFinish(true);
	_emitter->setPosition(size.width/2,size.height);
	this->addChild(_emitter, 5);

	//进度
	labelTTFCardNumber = LabelTTF::create("0", "HiraKakuProN-w6", 50);
	labelTTFCardNumber->setColor(Color3B::WHITE);
	labelTTFCardNumber->setPosition(
		Point(size.width / 2, size.height / 2 + labelTTFCardNumber->getContentSize().height + 20));
	this->addChild(labelTTFCardNumber, 2);
	labelTTFCardNumber->setString(
		StringUtils::format("%d%%", 0));
	//游戏玩的方法
	createPlayWay();

	return true;
}
void PlayScene::onStart(cocos2d::Ref* pSender){
	isGameBegin = true;
	//移除介绍游戏方式菜单
	this->removeChildByTag(playwayTag);
	//播放音乐
	playMusicByCheckPoint();
	//设置重力
	setWorldGravity();
	//创建控制按钮
	createControlBtn();
	//给小球速度
	ball->jump();
	//进度更新
	this->schedule(schedule_selector(PlayScene::addTime), 1.0f, kRepeatForever, 1);

}

void PlayScene::onEnter(){
	Layer::onEnter();
	auto contactListenner = EventListenerPhysicsContact::create();

	contactListenner->onContactBegin = CC_CALLBACK_1(PlayScene::onContactBegin, this);
	contactListenner->onContactPostSolve = CC_CALLBACK_1(PlayScene::onContactPostSolve, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(contactListenner, this);

	
}
void PlayScene::onEnterTransitionDidFinish(){
	Layer::onEnterTransitionDidFinish();
}
void PlayScene::onExit(){
	Layer::onExit();
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
	//获取碰撞物体
	auto body1 = (Sprite *)contact.getShapeA()->getBody()->getNode();
	auto body2 = (Sprite *)contact.getShapeB()->getBody()->getNode();
	CylinderColor ballColor = normal;
	CylinderColor cylinderColor = normal;
	
	//判断是否为球
	if (body1->getTag() == ballTag){
		ballColor = ball->getBallColor();
	}
	if (body2->getTag() == ballTag){
		ballColor = ball->getBallColor();
	}

	//从圆盘列表里获取当前碰撞圆盘的颜色
	list <Cylinder*> cylinderList = m_manager->getCylinderList();
	Cylinder* cylinder;
	for (list<Cylinder*>::iterator iter = cylinderList.begin(); iter != cylinderList.end(); iter++){
		cylinder = (Cylinder*)*iter;
		if (cylinder->getPosition().x == size.width/4){
			cylinderColor = cylinder->getCylinderColor();
			break;
		}
	}
	//关卡
	switch (checkPoint)
	{
	case 1:
		//如果颜色不同且圆盘不是最后一个则游戏结束
		if (cylinderColor != normal && cylinderColor != endColor  && cylinderColor != ballColor){
			CocosDenshion::SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
			GameOver();
			auto endLayer = TransitionFade::create(1, EndLayer::createScene(false, percent));
			Director::getInstance()->replaceScene(endLayer);


			return false;
		}
		else if (cylinderColor == endColor){
			if (CCUserDefault::sharedUserDefault()->getBoolForKey("sound", true)){
				CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(MUSIC_WIN, false);
			}

			CocosDenshion::SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();

			//设置下一关卡开启
			String *c_str = String::createWithFormat("checkPoint%i", checkPoint + 1);
			CCUserDefault::sharedUserDefault()->setBoolForKey(c_str->getCString(), true);
			//跳转到游戏成功结束界面
			auto endLayer = TransitionFade::create(1, EndLayer::createScene(true, percent));
			Director::getInstance()->replaceScene(endLayer);
			return true;
		}
		break;
	case 2:
		//关卡2是要求球与圆盘颜色相反，所以如果相同则失败
		if (cylinderColor != normal && cylinderColor != endColor && cylinderColor == ballColor){
			CocosDenshion::SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
			GameOver();
			auto endLayer = TransitionFade::create(1, EndLayer::createScene(false, percent));
			Director::getInstance()->replaceScene(endLayer);
			return false;
		}
		else if (cylinderColor != normal && ballColor == normal){
			CocosDenshion::SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
			GameOver();
			auto endLayer = TransitionFade::create(1, EndLayer::createScene(false, percent));
			Director::getInstance()->replaceScene(endLayer);
			return false;
		}
		else if (cylinderColor == endColor){
			if (CCUserDefault::sharedUserDefault()->getBoolForKey("sound", true)){
				CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(MUSIC_WIN, false);
			}
			CocosDenshion::SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();

			String *c_str = String::createWithFormat("checkPoint%i", checkPoint + 1);
			CCUserDefault::sharedUserDefault()->setBoolForKey(c_str->getCString(), true);

			auto endLayer = TransitionFade::create(1, EndLayer::createScene(true, percent));
			Director::getInstance()->replaceScene(endLayer);
			return true;
		}
		break;
	case 3:
	case 4:
	case 5:
		//
		if (cylinderColor != normal && cylinderColor != endColor  && cylinderColor != ballColor){
			CocosDenshion::SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
			GameOver();
			auto endLayer = TransitionFade::create(1, EndLayer::createScene(false, percent));
			Director::getInstance()->replaceScene(endLayer);
			return false;
		}
		else if (cylinderColor == endColor){
			if (CCUserDefault::sharedUserDefault()->getBoolForKey("sound", true)){
				CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(MUSIC_WIN, false);
			}
			CocosDenshion::SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();

			String *c_str = String::createWithFormat("checkPoint%i", checkPoint + 1);
			CCUserDefault::sharedUserDefault()->setBoolForKey(c_str->getCString(), true);

			auto endLayer = TransitionFade::create(1, EndLayer::createScene(true, percent));
			Director::getInstance()->replaceScene(endLayer);
			return true;
		}
		break;
	case 6:
		if (cylinderColor != normal && cylinderColor != endColor  && cylinderColor != ballColor){
			CocosDenshion::SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
			GameOver();
			auto endLayer = TransitionFade::create(1, EndLayer::createScene(false, percent));
			Director::getInstance()->replaceScene(endLayer);
			return false;
		}
		else if (cylinderColor == endColor){
			if (CCUserDefault::sharedUserDefault()->getBoolForKey("sound", true)){
				CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(MUSIC_WIN, false);
			}
			CocosDenshion::SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();

			auto scene = TransitionFade::create(1, LeaveLayer::createScene());
			Director::getInstance()->replaceScene(scene);
			return true;
		}
		break;
	default:
		break;
	}
	//球自旋转
	ball->rotateAction();
	//圆盘管理
	m_manager->update(0);
	return true;
}
void PlayScene::onContactPostSolve(PhysicsContact& contact){
	
}
void PlayScene::update(float t){
	int steps = 2;
	float dt = CCDirector::getInstance()->getAnimationInterval() / (float)steps;
	PhysicsWorld* s = getScene()->getPhysicsWorld();
	for (int i = 0; i<steps; i++){
		s->step(dt);
	}

}
void PlayScene::initPhysicWorld(){
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto origin = Director::getInstance()->getVisibleOrigin();
	//创建一个静态的线状刚体
	auto body = PhysicsBody::createEdgeSegment(origin, Point(visibleSize.width, 0), PHYSICSBODY_MATERIAL_DEFAULT, 1);
	//设置弹性  1-完全
	body->getShape(0)->setRestitution(1);
	//设置摩擦力  
	body->getShape(0)->setFriction(0);
	//设置body是否受引力影响
	body->setGravityEnable(false);
	//类别标示掩码
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
	auto *menu = Menu::create();
	menu->setPosition(zeroPos);
	int num = 2;
	switch (checkPoint)
	{
	case 1:
		total = songTime1;
		num = 2;
		for (int i = 1; i <= num; i++){
			String *str = String::createWithFormat("control%d.png", i);
			String *str_p = String::createWithFormat("control_press_%d.png", i);
			//创建控制菜单
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
		total = songTime2;
		num = 2; 
		for (int i = 1; i <= num; i++){
			String *str = String::createWithFormat("control%d.png", i);
			String *str_p = String::createWithFormat("control_press_%d.png", i);
			//创建控制菜单
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
		total = songTime3;
		num = 3;
		for (int i = 1; i <= num; i++){
			String *str = String::createWithFormat("control%d.png", i);
			String *str_p = String::createWithFormat("control_press_%d.png", i);
			//创建控制菜单
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
		total = songTime4;
		num = 3;
		for (int i = 1; i <= num; i++){
			String *str = String::createWithFormat("control%d.png", i);
			String *str_p = String::createWithFormat("control_press_%d.png", i);
			//创建控制菜单
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
		total = songTime5;
		num = 4;
		for (int i = 1; i <= num; i++){
			String *str = String::createWithFormat("control%d.png", i);
			String *str_p = String::createWithFormat("control_press_%d.png", i);
			//创建控制菜单
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
		total = songTime6;
		num = 4;
		for (int i = 1; i <= num; i++){
			String *str = String::createWithFormat("control%d.png", i);
			String *str_p = String::createWithFormat("control_press_%d.png", i);
			//创建控制菜单
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
	CCMenuItem* item = dynamic_cast<CCMenuItem*>(pSender);
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
		switch (checkPoint)
		{
		case 1:
			CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic(MUSIC_ONE, false);	
			break;
		case 2:
			CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic(MUSIC_TWO, false);
			break;
		case 3:
			CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic(MUSIC_THREE, false);
			break;
		case 4:
			CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic(MUSIC_FOUR, false);
			break;
		case 5:
			CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic(MUSIC_FIVE, false);
			break;
		case 6:
			CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic(MUSIC_SIX, false);
			break;
		default:
			break;
		}
	if (!CCUserDefault::sharedUserDefault()->getBoolForKey("sound", true)){
		CocosDenshion::SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
	}
	
}
void PlayScene::createPlayWay(){
	auto size = Director::getInstance()->getVisibleSize();
	auto zeroPos = Director::getInstance()->getVisibleOrigin();
	auto centerPos = Point(size.width / 2, size.height / 2);
	auto *menu = Menu::create();
	menu->setPosition(zeroPos);
	if (checkPoint == 1){
		auto * menuItemPLay = MenuItemSprite::create(Sprite::create(PLAY_WAY_1), Sprite::create(PLAY_WAY_1), CC_CALLBACK_1(PlayScene::onStart, this));
		menuItemPLay->setPosition(centerPos);
		menu->addChild(menuItemPLay);
	}
	else if (checkPoint == 2){
		auto * menuItemPLay = MenuItemSprite::create(Sprite::create(PLAY_WAY_2), Sprite::create(PLAY_WAY_2), CC_CALLBACK_1(PlayScene::onStart, this));
		menuItemPLay->setPosition(centerPos);
		menu->addChild(menuItemPLay);
	}
	else if (checkPoint == 3){
		auto * menuItemPLay = MenuItemSprite::create(Sprite::create(PLAY_WAY_3), Sprite::create(PLAY_WAY_3), CC_CALLBACK_1(PlayScene::onStart, this));
		menuItemPLay->setPosition(centerPos);
		menu->addChild(menuItemPLay);
	}
	else if (checkPoint == 4){
		auto * menuItemPLay = MenuItemSprite::create(Sprite::create(PLAY_WAY_4), Sprite::create(PLAY_WAY_4), CC_CALLBACK_1(PlayScene::onStart, this));
		menuItemPLay->setPosition(centerPos);
		menu->addChild(menuItemPLay);
	}
	else if (checkPoint == 5 ){
		auto * menuItemPLay = MenuItemSprite::create(Sprite::create(PLAY_WAY_5), Sprite::create(PLAY_WAY_5), CC_CALLBACK_1(PlayScene::onStart, this));
		menuItemPLay->setPosition(centerPos);
		menu->addChild(menuItemPLay);
	}
	else if ( checkPoint == 6){
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
	//游戏进度
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
	

	//当大于90则圆盘不再向左移动
	if (percent >= 90){
		m_manager->setMove(false);
	}
}
void PlayScene::GameOver(){
	if (CCUserDefault::sharedUserDefault()->getBoolForKey("sound", true)){
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(MUSIC_LOSE, false);
	}
	this->removeChildByTag(edgeNodeTag);
}
