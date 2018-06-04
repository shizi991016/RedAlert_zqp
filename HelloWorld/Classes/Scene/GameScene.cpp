#include "GameScene.h"
#include "SimpleAudioEngine.h"
#include "CountryChoiceScene.h"
//#include "../Army/ArmyClass.cpp"

int CountryChoice;

USING_NS_CC;
using namespace CocosDenshion;
Scene* GameScene::createScene()
{
    return GameScene::create();
}

static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in GameSceneScene.cpp\n");
}


bool GameScene::init()
{
    if ( !Scene::init() )
    {
        return false;
    }
    
    VisibleSize = Director::getInstance()->getVisibleSize();
    Vec2 Origin = Director::getInstance()->getVisibleOrigin();
    
    
    ScreenWidth = VisibleSize.width;
    ScreenHeight = VisibleSize.height;
    
    TileMap = TMXTiledMap::create("RedAlertMap.tmx");
    TileMap->setAnchorPoint(Vec2::ZERO);
    TileMap->setPosition(Vec2::ZERO);
    this->addChild(TileMap,-1);
    
    /*
     获得tilemap中MiningYard 并且将地图所对应位置设置为有矿井与有建筑物
     */
    auto MiningYardPoints = TileMap->getObjectGroup("MiningYard");
    auto MiningYardPositionGroup = MiningYardPoints->getObjects();
    for (auto& obj : MiningYardPositionGroup)
    {
        auto dic = obj.asValueMap();
        int x = dic.at("x").asInt();
        int y = dic.at("y").asInt();
        for (int i = 0; i < 125; i++)
        {
            for (int j = 0; j < 125; j++)
            {
                MyData.IsPositionHaveBuildings[x+i][y+j] = 1;
                MyData.IsPositionHaveMiningYard[x+i][y+j] = 1;
            }
        }
    }
    
    
    Camera = Sprite::create("Common/camera.png");
    Camera->setPosition(Vec2(ScreenWidth/2,ScreenHeight/2));
    this->addChild(Camera);
    
    RightMenuPicture = Sprite::create("MenuPicture/RightMenu.png");
    RightMenuPicture->setPosition(Vec2(ScreenWidth-100, ScreenHeight/2));
    RightMenuPicture->setScale(1, 1.279);
    this->addChild(RightMenuPicture,3);
    
    auto listener = EventListenerKeyboard::create();
    
    
    listener->onKeyPressed = [=](EventKeyboard::KeyCode keyCode, Event* event)
    {
        keys[keyCode] = true;
    };
    
    
    listener->onKeyReleased = [=](EventKeyboard::KeyCode keyCode, Event* event)
    {
        keys[keyCode] = false;
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    
    commonGamePictureLoading();
    countryChoiceSwitch(CountryChoice);
    
    //************************************************************************************
    
    //SpriteFrameCache::getInstance()->addSpriteFramesWithFile("CommonElectricPowerPlant_action.plist");
    /*
    auto TexTSP = Sprite::create("CountryPicture/CountryOne.png");
    TexTSP->setPosition(Vec2(VisibleSize.width/2, VisibleSize.height/2));
    this->addChild(TexTSP);
    //AnimationCache::getInstance()->addAnimationsWithFile("CommonElectricPowerPlant_action.plist");
    
    auto animation = Animation::create();
    animation->addSpriteFrameWithFile("CommonElectricPowerPlant_action/CommonElectricPowerPlant_action_1.png");
    animation->addSpriteFrameWithFile("CommonElectricPowerPlant_action/CommonElectricPowerPlant_action_2.png");
    animation->addSpriteFrameWithFile("CommonElectricPowerPlant_action/CommonElectricPowerPlant_action_3.png");
    animation->addSpriteFrameWithFile("CommonElectricPowerPlant_action/CommonElectricPowerPlant_action_4.png");
    animation->addSpriteFrameWithFile("CommonElectricPowerPlant_action/CommonElectricPowerPlant_action_5.png");
    animation->addSpriteFrameWithFile("CommonElectricPowerPlant_action/CommonElectricPowerPlant_action_6.png");
    animation->addSpriteFrameWithFile("CommonElectricPowerPlant_action/CommonElectricPowerPlant_action_7.png");
    animation->addSpriteFrameWithFile("CommonElectricPowerPlant_action/CommonElectricPowerPlant_action_8.png");
    animation->addSpriteFrameWithFile("CommonElectricPowerPlant_action/CommonElectricPowerPlant_action_9.png");
    animation->addSpriteFrameWithFile("CommonElectricPowerPlant_action/CommonElectricPowerPlant_action_10.png");
    animation->addSpriteFrameWithFile("CommonElectricPowerPlant_action/CommonElectricPowerPlant_action_11.png");
    animation->addSpriteFrameWithFile("CommonElectricPowerPlant_action/CommonElectricPowerPlant_action_12.png");
    animation->addSpriteFrameWithFile("CommonElectricPowerPlant_action/CommonElectricPowerPlant_action_13.png");
    animation->addSpriteFrameWithFile("CommonElectricPowerPlant_action/CommonElectricPowerPlant_action_14.png");
    animation->addSpriteFrameWithFile("CommonElectricPowerPlant_action/CommonElectricPowerPlant_action_15.png");
    animation->setDelayPerUnit(3.0f/15.0f);
    auto animate = Animate::create(animation);
    auto rep = RepeatForever::create(animate);
    
    TexTSP->runAction(rep);
    */
    
    /*
    auto animatio = Animation::create();
    for (int i = 1; i <= 20; i++)
    {
        std::string szName = StringUtils::format("CommonElectricPowerPlant_action%d",i);
        animatio->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(szName));
    }
    animatio->setDelayPerUnit(3.0f/15.0f);
    animatio->setRestoreOriginalFrame(true);
    AnimationCache::getInstance()->addAnimation(animatio, "work");
    auto work = AnimationCache::getInstance()->getAnimation("work");
    auto animate = Animate::create(work);
    TexTSP->runAction(animate);
    */
    //************************************************************************************
    
    
    this->scheduleUpdate();
    
    return true;
}



void GameScene::setViewpointCenter(Vec2 position)
{
    Point actualPosition = Vec2(position.x,position.y);
    Point centerofView = Vec2(ScreenWidth/2,ScreenHeight/2);
    Point viewPoint = centerofView - actualPosition;
    this->setPosition(viewPoint);
}


void GameScene::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}


void GameScene::update(float delta)
{
    Node::update(delta);
    auto leftArrow = EventKeyboard::KeyCode::KEY_A, rightArrow = EventKeyboard::KeyCode::KEY_D,upArrow = EventKeyboard::KeyCode::KEY_W,downArrow = EventKeyboard::KeyCode::KEY_S;
    if(isKeyPressed(leftArrow))
    {
        keyPressedDuration(leftArrow);
    }
    else if(isKeyPressed(rightArrow))
    {
        keyPressedDuration(rightArrow);
    }
    else if(isKeyPressed(upArrow))
    {
        keyPressedDuration(upArrow);
    }
    else if(isKeyPressed(downArrow))
    {
        keyPressedDuration(downArrow);
    }
    //RightMenuLayer->runAction(Follow::create(camera));
    this->setViewpointCenter(Camera->getPosition());
}

bool GameScene::isKeyPressed(EventKeyboard::KeyCode keyCode)
{
    if(keys[keyCode]) {
        return true;
    } else {
        return false;
    }
}

void GameScene::keyPressedDuration(EventKeyboard::KeyCode code)
{
    int offsetX = 0, offsetY = 0;
    switch (code) {
        case EventKeyboard::KeyCode::KEY_A:
            if ((int)Camera->getPositionX() > ScreenWidth/2)
            {
                offsetX = -5;
            }
            else
            {
                offsetX = 0;
            }
            break;
        case EventKeyboard::KeyCode::KEY_D:
            if ((int)Camera->getPositionX() < TileMap->getMapSize().width * TileMap->getTileSize().width - ScreenWidth/2 - 10)
            {
                offsetX = 5;
            }
            else
            {
                offsetX = 0;
            }
            break;
        case EventKeyboard::KeyCode::KEY_W:
            if ((int)Camera->getPositionY() < TileMap->getMapSize().height * TileMap->getTileSize().height - ScreenHeight/2 - 10)
            {
                offsetY = 5;
            }
            else
            {
                offsetY = 0;
            }
            break;
        case EventKeyboard::KeyCode::KEY_S:
            if ((int)Camera->getPositionY() > ScreenHeight/2)
            {
                offsetY = -5;
            }
            else
            {
                offsetY = 0;
            }
            break;
        default:
            offsetY = offsetX = 0;
            break;
    }
    auto CameraMoveBy = MoveBy::create(0, Vec2(offsetX, offsetY));
    Camera->runAction(CameraMoveBy);
    auto RightMenuMoveBy = MoveBy::create(0, Vec2(offsetX, offsetY));
    RightMenuPicture->runAction(RightMenuMoveBy);
    auto CommonElectricPowerPlantMoveBy = MoveBy::create(0, Vec2(offsetX, offsetY));
    CommonElectricPowerPlantPicture->runAction(CommonElectricPowerPlantMoveBy);
}

void GameScene::countryChoiceSwitch(int CountryChoice)
{
    /*
     BattlePlane            战斗机
     Jet                    喷气机
     RTank                  坦克
     TTank                  坦克
     SoldierX               大兵
     MissileWell            核弹井
     */
    
    
    switch (CountryChoice)
    {
        case 1:
        {
            OneBattlePlaneButton  = Button::create("MenuPicture/CommonMenuButton.png");
            OneBattlePlanePicture = Sprite::create("GamePicture/OneBattlePlane.png");
            OneBattlePlaneButton->setPosition(Vec2(VisibleSize.width - 98, VisibleSize.height - 140));
            OneBattlePlanePicture->setPosition(Vec2(VisibleSize.width - 98, VisibleSize.height - 140));
            this->addChild(OneBattlePlaneButton,5);
            this->addChild(OneBattlePlanePicture,4);
            OneJetButton  = Button::create("MenuPicture/CommonMenuButton.png");
            OneJetPicture = Sprite::create("GamePicture/OneJet.png");
            OneJetButton->setPosition(Vec2(VisibleSize.width - 44, VisibleSize.height - 140));
            OneJetPicture->setPosition(Vec2(VisibleSize.width - 44, VisibleSize.height - 140));
            this->addChild(OneJetButton,5);
            this->addChild(OneJetPicture,4);
            break;
        }
            
        case 2:
        {
            TwoRTankButton  = Button::create("MenuPicture/CommonMenuButton.png");
            TwoRTankPicture = Sprite::create("GamePicture/TwoRtank.png");
            TwoRTankButton->setPosition(Vec2(VisibleSize.width - 98, VisibleSize.height - 140));
            TwoRTankPicture->setPosition(Vec2(VisibleSize.width - 98, VisibleSize.height - 140));
            this->addChild(TwoRTankButton,5);
            this->addChild(TwoRTankPicture,4);
            TwoTTankButton  = Button::create("MenuPicture/CommonMenuButton.png");
            TwoTTankPicture = Sprite::create("GamePicture/TwoTtank.png");
            TwoTTankButton->setPosition(Vec2(VisibleSize.width - 44, VisibleSize.height - 140));
            TwoTTankPicture->setPosition(Vec2(VisibleSize.width - 44, VisibleSize.height - 140));
            this->addChild(TwoTTankButton,5);
            this->addChild(TwoTTankPicture,4);
            break;
        }
            
        case 3:
        {
            ThreeSoldierXButton = Button::create("MenuPicture/CommonMenuButton.png");
            ThreeSoldierXPicture = Sprite::create("GamePicture/ThreeSoldierX.png");
            ThreeSoldierXButton->setPosition(Vec2(VisibleSize.width - 98, VisibleSize.height - 140));
            ThreeSoldierXPicture->setPosition(Vec2(VisibleSize.width - 98, VisibleSize.height - 140));
            this->addChild(ThreeSoldierXPicture,4);
            this->addChild(ThreeSoldierXButton,5);
            ThreeMissileWellButton = Button::create("MenuPicture/CommonMenuButton.png");
            ThreeMissileWellPicture = Sprite::create("GamePicture/ThreeMissileWell.png");
            ThreeMissileWellButton->setPosition(Vec2(VisibleSize.width - 44, VisibleSize.height - 140));
            ThreeMissileWellPicture->setPosition(Vec2(VisibleSize.width - 44, VisibleSize.height - 140));
            this->addChild(ThreeMissileWellPicture,4);
            this->addChild(ThreeMissileWellButton,5);
            break;
        }
            
        default:
        {
            
            break;
        }
    }
}

void GameScene::commonGamePictureLoading()
{
    
    /*
     Barracks               兵营 对应Soldier与SoldierX
     Cannon                 加农炮 对空伤害高对地伤害低
     ElectricPowerPlant     电厂 没有电厂不让建除矿场以外其他建筑 电厂越多建筑速度越快
     Refinery               矿场 没有矿场不给加钱 矿场越多加钱越快
     Soldier                大兵 对地伤害高对空伤害低
     Tank                   坦克
     WarFactory             战车工厂 对应Tank TTank RTank
     
     */
    
    CommonBarracksButton           = Button::create("MenuPicture/CommonMenuButton.png");
    CommonCannonButton             = Button::create("MenuPicture/CommonMenuButton.png");
    CommonElectricPowerPlantButton = Button::create("MenuPicture/CommonMenuButton.png");
    CommonRefineryButton           = Button::create("MenuPicture/CommonMenuButton.png");
    CommonSoldierButton            = Button::create("MenuPicture/CommonMenuButton.png");
    CommonTankButton               = Button::create("MenuPicture/CommonMenuButton.png");
    CommonWarFactoryButton         = Button::create("MenuPicture/CommonMenuButton.png");
    
    
    CommonBarracksPicture           = Sprite::create("GamePicture/CommonBarracks.png");
    CommonCannonPicture             = Sprite::create("GamePicture/CommonCannon.png");
    CommonElectricPowerPlantPicture = Sprite::create("GamePicture/CommonElectricPowerPlant.png");
    CommonRefineryPicture           = Sprite::create("GamePicture/CommonRefinery.png");
    CommonSoldierPicture            = Sprite::create("GamePicture/CommonSoldier.png");
    CommonTankPicture               = Sprite::create("GamePicture/CommonTank.png");
    CommonWarFactoryPicture         = Sprite::create("GamePicture/CommonWarFactory.png");
    
    
    CommonElectricPowerPlantButton->setPosition(Vec2(VisibleSize.width - 150, VisibleSize.height - 28));
    CommonElectricPowerPlantPicture->setPosition(Vec2(VisibleSize.width - 150, VisibleSize.height - 28));
    CommonElectricPowerPlantButton->addTouchEventListener([=](Ref* pSender,Widget::TouchEventType type)
    {
        switch (type)
        {
            case Widget::TouchEventType::ENDED:
            {
                auto ElectricPowerPlantPicture = Sprite::create
                ("CommonElectricPowerPlant_action/CommonElectricPowerPlant_action_15.png");
                ElectricPowerPlantPicture->setPosition(Vec2(Camera->getPosition().x,Camera->getPosition().y));
                ElectricPowerPlantPicture->setScale(0.25f);
                this->addChild(ElectricPowerPlantPicture,1);
                ElectricPowerPlantPicture->setOpacity(140);
                ArmyMoveOnce(ElectricPowerPlantPicture);
                /*
                auto animation = Animation::create();
                animation->addSpriteFrameWithFile("CommonElectricPowerPlant_action/CommonElectricPowerPlant_action_1.png");
                animation->addSpriteFrameWithFile("CommonElectricPowerPlant_action/CommonElectricPowerPlant_action_2.png");
                animation->addSpriteFrameWithFile("CommonElectricPowerPlant_action/CommonElectricPowerPlant_action_3.png");
                animation->addSpriteFrameWithFile("CommonElectricPowerPlant_action/CommonElectricPowerPlant_action_4.png");
                animation->addSpriteFrameWithFile("CommonElectricPowerPlant_action/CommonElectricPowerPlant_action_5.png");
                animation->addSpriteFrameWithFile("CommonElectricPowerPlant_action/CommonElectricPowerPlant_action_6.png");
                animation->addSpriteFrameWithFile("CommonElectricPowerPlant_action/CommonElectricPowerPlant_action_7.png");
                animation->addSpriteFrameWithFile("CommonElectricPowerPlant_action/CommonElectricPowerPlant_action_8.png");
                animation->addSpriteFrameWithFile("CommonElectricPowerPlant_action/CommonElectricPowerPlant_action_9.png");
                animation->addSpriteFrameWithFile("CommonElectricPowerPlant_action/CommonElectricPowerPlant_action_10.png");
                animation->addSpriteFrameWithFile("CommonElectricPowerPlant_action/CommonElectricPowerPlant_action_11.png");
                animation->addSpriteFrameWithFile("CommonElectricPowerPlant_action/CommonElectricPowerPlant_action_12.png");
                animation->addSpriteFrameWithFile("CommonElectricPowerPlant_action/CommonElectricPowerPlant_action_13.png");
                animation->addSpriteFrameWithFile("CommonElectricPowerPlant_action/CommonElectricPowerPlant_action_14.png");
                animation->addSpriteFrameWithFile("CommonElectricPowerPlant_action/CommonElectricPowerPlant_action_15.png");
                animation->setDelayPerUnit(3.0f/2.0f);
                auto animate = Animate::create(animation);
                auto rep = RepeatForever::create(animate);
                //std::string str = "CommonElectricPowerPlant_action/CommonElectricPowerPlant_action_15.png";
                
                
                if (IsTouchPositionAvailable)
                {
                    ArmyBuildCallBack(rep,"CommonElectricPowerPlant_action/CommonElectricPowerPlant_action_15.png");
                }
                 */
                break;
            }
                
            default:
                break;
        }
    });
    CommonRefineryButton->setPosition(Vec2(VisibleSize.width - 98, VisibleSize.height-28));
    CommonRefineryPicture->setPosition(Vec2(VisibleSize.width - 98, VisibleSize.height-28));
    CommonRefineryButton->addTouchEventListener([=](Ref* pSender,Widget::TouchEventType type)
     {
         switch (type)
         {
             case Widget::TouchEventType::ENDED:
             {
                 auto TextPic = Sprite::create("CommonElectricPowerPlant_action/CommonElectricPowerPlant_action_15.png");
                 TextPic->setPosition(Vec2(Camera->getPosition().x,Camera->getPosition().y));
                 this->addChild(TextPic,1);
                 break;
             }
                 
             default:
                 break;
         }
     });
    CommonWarFactoryButton->setPosition(Vec2(VisibleSize.width - 44, VisibleSize.height-28));
    CommonWarFactoryPicture->setPosition(Vec2(VisibleSize.width - 44, VisibleSize.height-28));
    CommonSoldierButton->setPosition(Vec2(VisibleSize.width - 150, VisibleSize.height - 85));
    CommonSoldierPicture->setPosition(Vec2(VisibleSize.width - 150, VisibleSize.height - 85));
    CommonTankButton->setPosition(Vec2(VisibleSize.width - 98, VisibleSize.height - 85));
    CommonTankPicture->setPosition(Vec2(VisibleSize.width - 98, VisibleSize.height - 85));
    CommonCannonButton->setPosition(Vec2(VisibleSize.width - 44, VisibleSize.height - 85));
    CommonCannonPicture->setPosition(Vec2(VisibleSize.width - 44, VisibleSize.height - 85));
    CommonBarracksButton->setPosition(Vec2(VisibleSize.width - 150, VisibleSize.height - 140));
    CommonBarracksPicture->setPosition(Vec2(VisibleSize.width - 150, VisibleSize.height - 140));
    
    this->addChild(CommonBarracksButton,5);
    this->addChild(CommonCannonButton,5);
    this->addChild(CommonElectricPowerPlantButton,5);
    this->addChild(CommonRefineryButton,5);
    this->addChild(CommonSoldierButton,5);
    this->addChild(CommonTankButton,5);
    this->addChild(CommonWarFactoryButton,5);
    
    
    this->addChild(CommonBarracksPicture,4);
    this->addChild(CommonCannonPicture,4);
    this->addChild(CommonElectricPowerPlantPicture,4);
    this->addChild(CommonRefineryPicture,4);
    this->addChild(CommonSoldierPicture,4);
    this->addChild(CommonTankPicture,4);
    this->addChild(CommonWarFactoryPicture,4);
}

void GameScene::ArmyMoveOnce(Sprite* ArmyName)
{
    auto ArmyListener = EventListenerTouchOneByOne::create();//创建一个触摸监听
    ArmyListener->setSwallowTouches(true);
    ArmyListener->onTouchBegan = [](Touch* touch, Event* event)
    {
        auto ArmyTarget = static_cast<Sprite*>(event->getCurrentTarget());//获取的当前触摸的目标
        Point locationInNode = ArmyTarget->convertToNodeSpace(touch->getLocation());//获得触摸点相对于触摸目标的坐标
        Size ArmySize = ArmyTarget->getContentSize();//获得触摸目标的大小
        Rect ArmyRect = Rect(0, 0, ArmySize.width, ArmySize.height);//创建一个坐标在左下角的相对于触摸目标的坐标系
        if (ArmyRect.containsPoint(locationInNode))//判断触摸点是否在目标的范围内
            return true;
        else
            return false;
    };
    ArmyListener->onTouchMoved = [](Touch* touch, Event* event)
    {
        auto ArmyTarget = static_cast<Sprite*>(event->getCurrentTarget());
        ArmyTarget->setPosition(ArmyTarget->getPosition() + touch->getDelta());
    };
    
    ArmyListener->onTouchEnded = [=](Touch* touch, Event* event)
    {
        auto ArmyTarget = static_cast<Sprite*>(event->getCurrentTarget());//获取的当前触摸的目标
        Point LocationInNode = ArmyTarget->convertToNodeSpace(touch->getLocation());//获得触摸点相对于触摸对象的坐标
        Point LocationInWorld = this->convertToNodeSpace(touch->getLocation());//获得触摸点相对于世界地图的坐标
        Size ArmySize = ArmyTarget->getContentSize();
        int x = LocationInWorld.x - LocationInNode.x;
        int y = LocationInWorld.y - LocationInNode.y;
        for (int i = 0; i <= ArmySize.width; i++)
        {
            for (int j = 0; j <= ArmySize.height; j++)
            {
                if (MyData.IsPositionHaveBuildings[x+i][y+j] == 1 && x+i >= 0 && y+j >= 0 && x+i <= 3200 && y+j <= 3200)
                {
                    this->removeChild(ArmyName);
                    _eventDispatcher->removeEventListener(ArmyListener);
                    return false;
                }
            }
        }
        for (int i = 0; i <= ArmySize.width; i++)
        {
            for (int j = 0; j <= ArmySize.height; j++)
            {
                if (x+i >= 0 && y+j >= 0 && x+i <= 3200 && y+j <= 3200)
                {
                    MyData.IsPositionHaveBuildings[x+i][y+j] = 1;
                }
            }
        }
        _eventDispatcher->removeEventListener(ArmyListener);
        TouchPosition = this->convertToNodeSpace(touch->getLocation());
        IsTouchPositionAvailable = 1;
        this->removeChild(ArmyName);
        auto animation = Animation::create();
        animation->addSpriteFrameWithFile("CommonElectricPowerPlant_action/CommonElectricPowerPlant_action_1.png");
        animation->addSpriteFrameWithFile("CommonElectricPowerPlant_action/CommonElectricPowerPlant_action_2.png");
        animation->addSpriteFrameWithFile("CommonElectricPowerPlant_action/CommonElectricPowerPlant_action_3.png");
        animation->addSpriteFrameWithFile("CommonElectricPowerPlant_action/CommonElectricPowerPlant_action_4.png");
        animation->addSpriteFrameWithFile("CommonElectricPowerPlant_action/CommonElectricPowerPlant_action_5.png");
        animation->addSpriteFrameWithFile("CommonElectricPowerPlant_action/CommonElectricPowerPlant_action_6.png");
        animation->addSpriteFrameWithFile("CommonElectricPowerPlant_action/CommonElectricPowerPlant_action_7.png");
        animation->addSpriteFrameWithFile("CommonElectricPowerPlant_action/CommonElectricPowerPlant_action_8.png");
        animation->addSpriteFrameWithFile("CommonElectricPowerPlant_action/CommonElectricPowerPlant_action_9.png");
        animation->addSpriteFrameWithFile("CommonElectricPowerPlant_action/CommonElectricPowerPlant_action_10.png");
        animation->addSpriteFrameWithFile("CommonElectricPowerPlant_action/CommonElectricPowerPlant_action_11.png");
        animation->addSpriteFrameWithFile("CommonElectricPowerPlant_action/CommonElectricPowerPlant_action_12.png");
        animation->addSpriteFrameWithFile("CommonElectricPowerPlant_action/CommonElectricPowerPlant_action_13.png");
        animation->addSpriteFrameWithFile("CommonElectricPowerPlant_action/CommonElectricPowerPlant_action_14.png");
        animation->addSpriteFrameWithFile("CommonElectricPowerPlant_action/CommonElectricPowerPlant_action_15.png");
        animation->setDelayPerUnit(3.0f/2.0f);
        auto animate = Animate::create(animation);
        auto rep = RepeatForever::create(animate);
        //std::string str = "CommonElectricPowerPlant_action/CommonElectricPowerPlant_action_15.png";
        
        
        if (IsTouchPositionAvailable)
        {
            ArmyBuildCallBack(rep,"CommonElectricPowerPlant_action/CommonElectricPowerPlant_action_15.png");
        }
    };
    //将触摸监听添加到eventDispacher中去
    _eventDispatcher->addEventListenerWithSceneGraphPriority(ArmyListener, ArmyName);
    
    
    
}

void GameScene::ArmyBuildCallBack(Action* BuildingAction,const std::string& FileName)
{
    auto BuildingSprite = Sprite::create(FileName);
    BuildingSprite->setPosition(TouchPosition);
    this->addChild(BuildingSprite);
    BuildingSprite->runAction(BuildingAction);
    IsTouchPositionAvailable = 0;
}




