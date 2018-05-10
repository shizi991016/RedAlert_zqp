#include "HelloScene.h"
#include "SimpleAudioEngine.h"
#include "HelloWorldScene.h"
#include "SettingScene.h"
USING_NS_CC;
using namespace CocosDenshion;

Scene* HelloScene::createScene()
{
    return HelloScene::create();
}

bool HelloScene::init()
{
    if ( !Scene::init() )
    {
        return false;
    }
    //SimpleAudioEngine::getInstance()->preloadBackgroundMusic("HelloMusic.mp3");
    //SimpleAudioEngine::getInstance()->playBackgroundMusic("HelloMusic.mp3",true);
    SimpleAudioEngine::getInstance()->preloadBackgroundMusic("BackgroundMusic.mp3");
    SimpleAudioEngine::getInstance()->playBackgroundMusic("BackgroundMusic.mp3",true);
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    float x = origin.x + visibleSize.width/2;
    float y = origin.y + visibleSize.height/2;
    
    //auto menu = Menu::create(closeItem, NULL);
    
    auto BackGroundPciture = Sprite::create("HelloSceneBackGround.png");
    BackGroundPciture->setAnchorPoint(Vec2::ZERO);
    BackGroundPciture->setPosition(Vec2::ZERO);
    auto BackGround_Width = BackGroundPciture->getContentSize().width;
    auto BackGround_Height = BackGroundPciture->getContentSize().height;
    auto ScaleX = visibleSize.width / BackGround_Width;
    auto ScaleY = visibleSize.height / BackGround_Height;
    BackGroundPciture->setScale(ScaleX, ScaleY);
    
    
    this->addChild(BackGroundPciture, -1);
    
    auto StartItem = MenuItemImage::create("StartNormal.png", "StartSelected.png",CC_CALLBACK_1(HelloScene::menuStartCallback, this));
    auto SettingItem = MenuItemImage::create("SettingNormal.png", "SettingSelected.png",CC_CALLBACK_1(HelloScene::menuSettingCallback, this));
    auto ExitItem = MenuItemImage::create("ExitNormal.png", "ExitSelected.png",CC_CALLBACK_1(HelloScene::menuExitCallback, this));
    StartItem->setScale(1.7f);
    SettingItem->setScale(1.7f);
    ExitItem->setScale(1.7f);
    StartItem->setPosition(Vec2(x,y+50));
    SettingItem->setPosition(Vec2(x,y-50));
    ExitItem->setPosition(Vec2(x,y-150));
    auto menu = Menu::create(StartItem,SettingItem,ExitItem,NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);
    
    return true;
}


void HelloScene::menuExitCallback(cocos2d::Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
    MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
    return;
#endif
    Director::getInstance()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

void HelloScene::menuStartCallback(cocos2d::Ref* pSender)
{
    Director::getInstance()->replaceScene(TransitionFade::create(4.0f, HelloWorld::createScene()));
}

void HelloScene::menuSettingCallback(cocos2d::Ref* pSender)
{
    Director::getInstance()->pushScene(TransitionFade::create(0.4f, SettingScene::createScene()));
}

void HelloScene::Exit()
{
    Director::getInstance()->end();
}
