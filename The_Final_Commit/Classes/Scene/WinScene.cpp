#include "WinScene.h"
#include "ui/CocosGUI.h"

using namespace ui;
USING_NS_CC;

Scene* WinScene::createScene()
{
    return WinScene::create();
}


bool WinScene::init()
{
    if ( !Scene::init() )
    {
        return false;
    }
    
    auto VisibleSize = Director::getInstance()->getVisibleSize();
    Vec2 Origin = Director::getInstance()->getVisibleOrigin();
    
    float x = Origin.x + VisibleSize.width/2;
    float y = Origin.y + VisibleSize.height/2;
    
    auto BackGroundPciture = Sprite::create("GameOverPicture/Win.png");
    BackGroundPciture->setAnchorPoint(Vec2::ZERO);
    BackGroundPciture->setPosition(Vec2::ZERO);
    auto BackGround_Width = BackGroundPciture->getContentSize().width;
    auto BackGround_Height = BackGroundPciture->getContentSize().height;
    auto ScaleX = VisibleSize.width / BackGround_Width;
    auto ScaleY = VisibleSize.height / BackGround_Height;
    BackGroundPciture->setScale(ScaleX, ScaleY);
    
    this->addChild(BackGroundPciture, -1);
    
    auto ExitItem = MenuItemImage::create
    ("HelloScenePicture/ExitNormal.png", "HelloScenePicture/ExitSelected.png",
     CC_CALLBACK_1(WinScene::menuExitCallback, this));
    ExitItem->setScale(1.7f);
    ExitItem->setPosition(Vec2(x,y-150));
    auto menu = Menu::create(ExitItem,NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);
    
    auto text = Text::create("You Win","fonts/Marker Felt.ttf",60);
    text->setPosition(Vec2(x, y+100));
    this->addChild(text,1);
    
    return true;
}


void WinScene::menuExitCallback(cocos2d::Ref* pSender)
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



