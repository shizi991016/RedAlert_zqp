#include "CountryChoiceScene.h"
#include "GameScene.h"
//#include "../PublicData.h"
extern int CountryChoice;

USING_NS_CC;


Scene* CountryChoiceScene::createScene()
{
    return CountryChoiceScene::create();
}

bool CountryChoiceScene::init()
{
    if ( !Scene::init() )
    {
        return false;
    }
    
    auto VisibleSize = Director::getInstance()->getVisibleSize();
    Vec2 Origin = Director::getInstance()->getVisibleOrigin();
    
    auto BackGroundPciture = Sprite::create("HelloScenePicture/SettingBackGroundPicture.png");
    BackGroundPciture->setAnchorPoint(Vec2::ZERO);
    BackGroundPciture->setPosition(Vec2::ZERO);
    auto BackGround_Width = BackGroundPciture->getContentSize().width;
    auto BackGround_Height = BackGroundPciture->getContentSize().height;
    auto ScaleX = VisibleSize.width / BackGround_Width;
    auto ScaleY = VisibleSize.height / BackGround_Height;
    BackGroundPciture->setScale(ScaleX, ScaleY);
    //让地图铺满屏幕
    this->addChild(BackGroundPciture, -1);
    
    auto CountryOne = MenuItemImage::create("CountryPicture/CountryOne.png",
                                            "CountryPicture/CountryOne.png",
                                            CC_CALLBACK_1(CountryChoiceScene::countryOneCallBack, this));
    auto CountryTwo = MenuItemImage::create("CountryPicture/CountryTwo.png",
                                            "CountryPicture/CountryTwo.png",
                                            CC_CALLBACK_1(CountryChoiceScene::countryTwoCallBack, this));
    auto CountryThree = MenuItemImage::create("CountryPicture/CountryThree.png",
                                              "CountryPicture/CountryThree.png",
                                              CC_CALLBACK_1(CountryChoiceScene::countryThreeCallBack, this));
    CountryOne->setPosition(Vec2(VisibleSize.width/4, VisibleSize.height/2 - 30));
    CountryTwo->setPosition(Vec2(VisibleSize.width/2, VisibleSize.height/2 - 30));
    CountryThree->setPosition(Vec2(VisibleSize.width/4*3, VisibleSize.height/2 - 30));
    
    auto menu = Menu::create(CountryOne,CountryTwo,CountryThree,NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu,1);
    
    auto TopLabel = Text::create("Fight For Your Country", "fonts/Marker Felt.ttf", 55);
    TopLabel->setPosition(Vec2(VisibleSize.width/2, VisibleSize.height/2 + 140));
    this->addChild(TopLabel,1);
    
    auto CountryOneText = Text::create("拥有强大的空中部队\n地面防守薄弱","fonts/Marker Felt.ttf", 25);
    auto CountryTwoText = Text::create("拥有强大的地面部队\n空战能力弱","fonts/Marker Felt.ttf", 25);
    auto CountryThreeText = Text::create("拥有强大的人力部队\n有核打击能力","fonts/Marker Felt.ttf", 25);
    CountryOneText->setPosition(Vec2(VisibleSize.width/4, VisibleSize.height/2 - 150));
    CountryTwoText->setPosition(Vec2(VisibleSize.width/2, VisibleSize.height/2 - 150));
    CountryThreeText->setPosition(Vec2(VisibleSize.width/4*3, VisibleSize.height/2 - 150));
    this->addChild(CountryThreeText,1);
    this->addChild(CountryOneText,1);
    this->addChild(CountryTwoText,1);
    
    
    return true;
}

void CountryChoiceScene::countryOneCallBack(cocos2d::Ref* pSender)
{
    CountryChoice = 1;
    Director::getInstance()->replaceScene(TransitionFade::create(3.0f, GameScene::createScene()));
}

void CountryChoiceScene::countryTwoCallBack(cocos2d::Ref* pSender)
{
    CountryChoice = 2;
    Director::getInstance()->replaceScene(TransitionFade::create(3.0f, GameScene::createScene()));
}

void CountryChoiceScene::countryThreeCallBack(cocos2d::Ref* pSender)
{
    CountryChoice = 3;
    Director::getInstance()->replaceScene(TransitionFade::create(3.0f, GameScene::createScene()));
}










