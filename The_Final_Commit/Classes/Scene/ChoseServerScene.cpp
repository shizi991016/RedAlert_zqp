#include "ChoseServerScene.h"
#include "GameScene.h"

extern int ClientChoice;

USING_NS_CC;

Scene* ChoseServerScene::createScene()
{
    return ChoseServerScene::create();
}

bool ChoseServerScene::init()
{
    if ( !Scene::init() )
    {
        return false;
    }
    
    VisibleSize = Director::getInstance()->getVisibleSize();
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

    
    auto ClientOneLabel = Label::createWithTTF("ClientOne", "fonts/Marker Felt.ttf", 40);
    auto ClientOne = MenuItemLabel::create(ClientOneLabel,
                                        CC_CALLBACK_1(ChoseServerScene::clientOneCallBack, this));
    auto ClientTwoLabel = Label::createWithTTF("ClientTwo", "fonts/Marker Felt.ttf", 40);
    auto ClientTwo = MenuItemLabel::create(ClientTwoLabel,
                                           CC_CALLBACK_1(ChoseServerScene::clientTwoCallBack, this));
    
    auto menu = Menu::create(ClientOne,ClientTwo,NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu,1);
    
    ClientOne->setPosition(Vec2(VisibleSize.width/2 - 200, VisibleSize.height/2 + 50));
    ClientTwo->setPosition(Vec2(VisibleSize.width/2 + 200, VisibleSize.height/2 + 50));
    auto TopLabel = Text::create("Chose Your Client", "fonts/Marker Felt.ttf", 55);
    TopLabel->setPosition(Vec2(VisibleSize.width/2, VisibleSize.height/2 + 240));
    this->addChild(TopLabel,1);
    
    auto s = Size(250,40);
    auto _9Sprite = Scale9Sprite::create("Common/EditBox.png");
    auto IpEditBox = EditBox::create(s, _9Sprite);
    IpEditBox->setFont("fonts/Marker Felt.ttf", 30);
    IpEditBox->setPosition(Vec2(VisibleSize.width/2 , VisibleSize.height/2 - 150));
    this->addChild(IpEditBox,3);
    IpEditBox->setInputMode(EditBox::InputMode::SINGLE_LINE);
    IpEditBox->setReturnType(EditBox::KeyboardReturnType::DEFAULT);
    IpEditBox->setDelegate(this);
    
    auto Text = Text::create("请输入IP地址", "fonts/Marker Felt.ttf", 30);
    Text->setPosition(Vec2(VisibleSize.width/2 , VisibleSize.height/2 - 70));
    this->addChild(Text);
    
    return true;
}

void ChoseServerScene::clientOneCallBack(cocos2d::Ref *pSender)
{
    ClientChoice = 1;
    Director::getInstance()->replaceScene(TransitionFade::create(3.0f, GameScene::createScene()));
}

void ChoseServerScene::clientTwoCallBack(cocos2d::Ref *pSender)
{
    ClientChoice = 2;
    Director::getInstance()->replaceScene(TransitionFade::create(3.0f, GameScene::createScene()));
}




