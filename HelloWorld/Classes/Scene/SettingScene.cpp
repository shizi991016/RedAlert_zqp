#include "HelloScene.h"
#include "SettingScene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;
using namespace CocosDenshion;

Scene* SettingScene::createScene()
{
    return SettingScene::create();
}

bool SettingScene::init()
{
    if ( !Scene::init() )
    {
        return false;
    }
    
    
    auto VisibleSize = Director::getInstance()->getVisibleSize();
    Vec2 Origin = Director::getInstance()->getVisibleOrigin();
    
    float x = Origin.x + VisibleSize.width/2;
    float y = Origin.y + VisibleSize.height/2;
    
    auto ReturnItem = MenuItemImage::create("HelloScenePicture/ReturnNormal.png", "HelloScenePicture/ReturnSelected.png",
                                            CC_CALLBACK_1(SettingScene::menuReturnCallback, this));
    ReturnItem->setPosition(Vec2(x,y-250));
    ReturnItem->setScale(1.5f);
    auto menu = Menu::create(ReturnItem,NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);
    

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
    
    
    
    
    auto DisplayBackGroundValue = Text::create("移动滑块调节背景音量","fonts/Marker Felt.ttf",35);
    DisplayBackGroundValue->setPosition(Vec2(x, y+150));
    this->addChild(DisplayBackGroundValue);
    
    auto BackGroundSlider = Slider::create();
    BackGroundSlider->loadBarTexture("SliderPicture/sliderTrack.png");
    BackGroundSlider->loadSlidBallTextures("SliderPicture/sliderThumb.png");
    BackGroundSlider->loadProgressBarTexture("SliderPicture/sliderProgress.png");
    BackGroundSlider->setPosition(Vec2(VisibleSize.width/2, VisibleSize.height/2+80));
    BackGroundSlider->setPercent(SimpleAudioEngine::getInstance()->getBackgroundMusicVolume() * 100);
    BackGroundSlider->addEventListener([=](Ref * pSender,Slider::EventType type)
    {
        if (type == Slider::EventType::ON_PERCENTAGE_CHANGED)
        {
            auto BackGroundMusicValue = BackGroundSlider->getPercent();
            SimpleAudioEngine::getInstance()->setBackgroundMusicVolume((float)BackGroundMusicValue/100);
            std::string value = StringUtils::format("背景音量 = %d %%",BackGroundMusicValue);
            DisplayBackGroundValue->setString(value);
        }
    });
    this->addChild(BackGroundSlider);
    
    auto DisplayEffectValue = Text::create("移动滑块调节特效音量","fonts/Marker Felt.ttf",35);
    DisplayEffectValue->setPosition(Vec2(x, y-20));
    this->addChild(DisplayEffectValue);
    
    auto EffectSlider = Slider::create();
    EffectSlider->loadBarTexture("SliderPicture/sliderTrack.png");
    EffectSlider->loadSlidBallTextures("SliderPicture/sliderThumb.png");
    EffectSlider->loadProgressBarTexture("SliderPicture/sliderProgress.png");
    EffectSlider->setPosition(Vec2(VisibleSize.width/2, VisibleSize.height/2-90));
    EffectSlider->setPercent(SimpleAudioEngine::getInstance()->getEffectsVolume() * 100);
    EffectSlider->addEventListener([=](Ref * pSender,Slider::EventType type)
                                        {
                                           if (type == Slider::EventType::ON_PERCENTAGE_CHANGED)
                                           {
                                               auto EffectValue = EffectSlider->getPercent();
                                               SimpleAudioEngine::getInstance()->setEffectsVolume((float)EffectValue/100);
                                               std::string value = StringUtils::format("特效音量 = %d %%",EffectValue);
                                               DisplayEffectValue->setString(value);
                                           }
                                       });
    this->addChild(EffectSlider);

    return true;
}

void SettingScene::menuReturnCallback(cocos2d::Ref * pSender)
{
    Director::getInstance()->popSceneWithTransition<TransitionFade>(0.4f);
}

