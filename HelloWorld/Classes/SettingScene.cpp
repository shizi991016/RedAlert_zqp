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
    
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    float x = origin.x + visibleSize.width/2;
    float y = origin.y + visibleSize.height/2;
    
    auto ReturnItem = MenuItemImage::create("ReturnNormal.png", "ReturnSelected.png",
                                            CC_CALLBACK_1(SettingScene::menuReturnCallback, this));
    ReturnItem->setPosition(Vec2(x,y-250));
    ReturnItem->setScale(1.5f);
    auto menu = Menu::create(ReturnItem,NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);
    
    
    /*
    
    // 设置标签初始文字、位置，并添加为当前层的子节点
    
    _displayValueLabel = Label::createWithSystemFont("移动滑块调节音量." ,"Marker Felt", 32);
    _displayValueLabel->setPosition(visibleSize.width/2, visibleSize.height/2+100);
    addChild(_displayValueLabel);
    // 创建一个拖动滑块
    auto slider = ControlSlider::create("sliderTrack.png","sliderProgress.png" ,"sliderThumb.png");
    // 设置拖动滑块的范围最小值
    slider->setMinimumValue(0.0f);
    // 设置拖动滑块的范围最大值
    slider->setMaximumValue(10.0f);
    slider->setValue(5.0f);
    auto BackGroundMusicValue = slider->getValue();
    auto NowBackGroundMusicValue = SimpleAudioEngine::getInstance()->getBackgroundMusicVolume();
    SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(BackGroundMusicValue * NowBackGroundMusicValue);
    
    // 绑定事件处理函数，当滑块被拖动时被调用
    slider->addTargetWithActionForControlEvents(this, cccontrol_selector(SettingScene::valueChanged), Control::EventType::VALUE_CHANGED);
    slider->setPosition(visibleSize.width/2, visibleSize.height/2);
    this->addChild(slider);

    
    this->scheduleUpdate();
     
     
     */
    
    
    
    auto BackGroundPciture = Sprite::create("SettingBackGroundPicture.png");
    BackGroundPciture->setAnchorPoint(Vec2::ZERO);
    BackGroundPciture->setPosition(Vec2::ZERO);
    auto BackGround_Width = BackGroundPciture->getContentSize().width;
    auto BackGround_Height = BackGroundPciture->getContentSize().height;
    auto ScaleX = visibleSize.width / BackGround_Width;
    auto ScaleY = visibleSize.height / BackGround_Height;
    BackGroundPciture->setScale(ScaleX, ScaleY);
    
    
    this->addChild(BackGroundPciture, -1);
    
    
    
    
    auto DisplayBackGroundValue = Text::create("移动滑块调节音量","Marker Felt.ttf",35);
    DisplayBackGroundValue->setPosition(Vec2(x, y+150));
    this->addChild(DisplayBackGroundValue);
    
    auto BackGroundSlider = Slider::create();
    BackGroundSlider->loadBarTexture("sliderTrack.png");
    BackGroundSlider->loadSlidBallTextures("sliderThumb.png");
    BackGroundSlider->loadProgressBarTexture("sliderProgress.png");
    BackGroundSlider->setPosition(Vec2(visibleSize.width/2, visibleSize.height/2+80));
    BackGroundSlider->setPercent(SimpleAudioEngine::getInstance()->getBackgroundMusicVolume() * 100);
    BackGroundSlider->addEventListener([=](Ref * pSender,Slider::EventType type)
    {
        if (type == Slider::EventType::ON_PERCENTAGE_CHANGED)
        {
            auto BackGroundMusicValue = BackGroundSlider->getPercent();
            SimpleAudioEngine::getInstance()->setBackgroundMusicVolume((float)BackGroundMusicValue/100);
            std::string value = StringUtils::format("音量 = %d %%",BackGroundMusicValue);
            DisplayBackGroundValue->setString(value);
        }
    });
    this->addChild(BackGroundSlider);
    
    /////////////////////////////////////////////////////
    auto DisplayEffectValue = Text::create("移动滑块调节音量","Marker Felt.ttf",35);
    DisplayEffectValue->setPosition(Vec2(x, y-20));
    //DisplayEffectValue->setColor(Color3B::BLACK);
    this->addChild(DisplayEffectValue);
    
    auto EffectSlider = Slider::create();
    EffectSlider->loadBarTexture("sliderTrack.png");
    EffectSlider->loadSlidBallTextures("sliderThumb.png");
    EffectSlider->loadProgressBarTexture("sliderProgress.png");
    EffectSlider->setPosition(Vec2(visibleSize.width/2, visibleSize.height/2-90));
    EffectSlider->setPercent(SimpleAudioEngine::getInstance()->getEffectsVolume() * 100);
    EffectSlider->addEventListener([=](Ref * pSender,Slider::EventType type)
                                       {
                                           if (type == Slider::EventType::ON_PERCENTAGE_CHANGED)
                                           {
                                               auto EffectValue = EffectSlider->getPercent();
                                               SimpleAudioEngine::getInstance()->setEffectsVolume((float)EffectValue/100);
                                               std::string value = StringUtils::format("音量 = %d %%",EffectValue);
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

/*
void SettingScene::valueChanged(Ref *pSender, Control::EventType controlEvent){
    // 将pSender类型转成拖动滑块ControlSlider
    ControlSlider* pSlider = (ControlSlider*)pSender;
    // 获取滑块的当前值，并更新标签内容
    _displayValueLabel->setString(StringUtils::format("音量 = %.f %%", pSlider->getValue()*10));
}
*/
