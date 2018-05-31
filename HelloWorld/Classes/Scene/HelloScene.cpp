#include "HelloScene.h"
#include "SimpleAudioEngine.h"
#include "CountryChoiceScene.h"
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
    
    SimpleAudioEngine::getInstance()->preloadBackgroundMusic("Common/BackgroundMusic.mp3");
    SimpleAudioEngine::getInstance()->playBackgroundMusic("Common/BackgroundMusic.mp3",true);
    
    auto VisibleSize = Director::getInstance()->getVisibleSize();
    Vec2 Origin = Director::getInstance()->getVisibleOrigin();
    
    float x = Origin.x + VisibleSize.width/2;
    float y = Origin.y + VisibleSize.height/2;
    
    auto BackGroundPciture = Sprite::create("HelloScenePicture/HelloSceneBackGround.png");
    BackGroundPciture->setAnchorPoint(Vec2::ZERO);
    BackGroundPciture->setPosition(Vec2::ZERO);
    auto BackGround_Width = BackGroundPciture->getContentSize().width;
    auto BackGround_Height = BackGroundPciture->getContentSize().height;
    auto ScaleX = VisibleSize.width / BackGround_Width;
    auto ScaleY = VisibleSize.height / BackGround_Height;
    BackGroundPciture->setScale(ScaleX, ScaleY);
    
    
    this->addChild(BackGroundPciture, -1);
    
    auto StartItem = MenuItemImage::create("HelloScenePicture/StartNormal.png", "HelloScenePicture/StartSelected.png",
                                           CC_CALLBACK_1(HelloScene::menuStartCallback, this));
    auto SettingItem = MenuItemImage::create("HelloScenePicture/SettingNormal.png", "HelloScenePicture/SettingSelected.png",
                                             CC_CALLBACK_1(HelloScene::menuSettingCallback, this));
    auto ExitItem = MenuItemImage::create("HelloScenePicture/ExitNormal.png", "HelloScenePicture/ExitSelected.png",
                                          CC_CALLBACK_1(HelloScene::menuExitCallback, this));
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
    Director::getInstance()->replaceScene(TransitionFade::create(2.0f, CountryChoiceScene::createScene()));
}

void HelloScene::menuSettingCallback(cocos2d::Ref* pSender)
{
    Director::getInstance()->pushScene(TransitionFade::create(0.4f, SettingScene::createScene()));
}

