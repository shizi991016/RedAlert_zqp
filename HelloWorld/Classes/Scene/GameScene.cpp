#include "GameScene.h"
//#include "LoadingResource.cpp"
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
    
    MyData.MyCountryChoice = CountryChoice;
    
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
    
    RightMenuText = Text::create(" ","fonts/Marker Felt.ttf",23);
    RightMenuText->setPosition(Vec2(ScreenWidth-100, ScreenHeight/2+150));
    this->addChild(RightMenuText,4);
    
    PromptText = Text::create("将建筑拖到矿\n场或者已有建\n筑物上即可取\n消建筑","fonts/Marker Felt.ttf",23);
    PromptText->setPosition(Vec2(ScreenWidth-100, ScreenHeight/2-300));
    this->addChild(PromptText,4);
    
    MoneyText = Text::create(" ","fonts/Marker Felt.ttf",35);
    MoneyText->setPosition(Vec2(50, ScreenHeight-50));
    this->addChild(MoneyText,4);
    
    
    /*
    BuildingsClass* TextSp ;
    MyData.MyBuildings.push_back(TextSp);               //Text
    this->addChild(TextSp);                             //this->removechild可以删除vector中的指针
    this->removeChild(MyData.MyBuildings[0]);
    */
    
    
    
    
    
    
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
    
    TimeCount++;
    if (TimeCount % 60 == 0)
    {
        MyData.MyMoney += 1 * MyData.RefineryNumber;
        std::string value = StringUtils::format("%d $",MyData.MyMoney);
        MoneyText->setString(value);
    }
    
    MyData.WarFactoryNumber         = MyData.MyWarFactory.size();
    MyData.RefineryNumber           = MyData.MyRefinery.size();
    MyData.BarracksNumber           = MyData.MyBarracks.size();
    MyData.ElectricPowerPlantNumber = MyData.MyElectricPowerPlant.size();
    
    for (int i = 0; i < MyData.MyWarFactory.size(); i++)
    {
        if (MyData.MyWarFactory[i]->getLifeValue() <= 0)
        {
            MyData.MyWarFactory.erase(MyData.MyWarFactory.begin()+i);
            this->removeChild(MyData.MyWarFactory[i],true);
        }
    }
    
    for (int i = 0; i < MyData.MyBarracks.size(); i++)
    {
        if (MyData.MyBarracks[i]->getLifeValue() <= 0)
        {
            MyData.MyBarracks.erase(MyData.MyBarracks.begin()+i);
            this->removeChild(MyData.MyBarracks[i],true);
        }
    }
    
    for (int i = 0; i < MyData.MyElectricPowerPlant.size(); i++)
    {
        if (MyData.MyElectricPowerPlant[i]->getLifeValue() <= 0)
        {
            MyData.MyElectricPowerPlant.erase(MyData.MyElectricPowerPlant.begin()+i);
            this->removeChild(MyData.MyElectricPowerPlant[i],true);
        }
    }
    
    for (int i = 0; i < MyData.MyRefinery.size(); i++)
    {
        if (MyData.MyRefinery[i]->getLifeValue() <= 0)
        {
            MyData.MyRefinery.erase(MyData.MyRefinery.begin()+i);
            this->removeChild(MyData.MyRefinery[i],true);
        }
    }
    
    auto leftArrow =  EventKeyboard::KeyCode::KEY_A,  rightArrow =  EventKeyboard::KeyCode::KEY_D,
         upArrow   =  EventKeyboard::KeyCode::KEY_W,  downArrow  =  EventKeyboard::KeyCode::KEY_S;
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
    this->setViewpointCenter(Camera->getPosition());
    
    if (MyData.MyMoney < 60)
    {
        CommonRefineryButton->setTouchEnabled(false);
    }
    else
    {
        CommonRefineryButton->setTouchEnabled(true);
    }
    
    if (MyData.RefineryNumber <= 0 || MyData.ElectricPowerPlantNumber <= 0)
    {
        CommonCannonButton->setTouchEnabled(false);
        CommonBarracksButton->setTouchEnabled(false);
        CommonSoldierButton->setTouchEnabled(false);
        CommonTankButton->setTouchEnabled(false);
        CommonWarFactoryButton->setTouchEnabled(false);
        switch (MyData.MyCountryChoice)
        {
            case 1:
                OneJetButton->setTouchEnabled(false);
                OneBattlePlaneButton->setTouchEnabled(false);
                break;
                
            case 2:
                TwoRTankButton->setTouchEnabled(false);
                TwoTTankButton->setTouchEnabled(false);
                break;
                
            case 3:
                ThreeSoldierXButton->setTouchEnabled(false);
                ThreeMissileWellButton->setTouchEnabled(false);
                break;
                
            default:
                break;
        }
    }
    
    if (MyData.RefineryNumber <= 0)
        CommonElectricPowerPlantButton->setTouchEnabled(false);
    
    if (MyData.RefineryNumber > 0)
    {
        if (MyData.MyMoney > 50)
        {
            CommonElectricPowerPlantButton->setTouchEnabled(true);
        }
        else
            CommonElectricPowerPlantButton->setTouchEnabled(false);
    }
    
    
    if (MyData.RefineryNumber > 0 && MyData.ElectricPowerPlantNumber > 0)
    {
        if (MyData.MyMoney > 40)
        {
            CommonCannonButton->setTouchEnabled(true);
        }
        else
        {
            CommonCannonButton->setTouchEnabled(false);
        }
        
        if (MyData.MyMoney > 100)
        {
            CommonBarracksButton->setTouchEnabled(true);
        }
        else
        {
            CommonBarracksButton->setTouchEnabled(false);
        }
        
        if (MyData.MyMoney > 150)
        {
            CommonWarFactoryButton->setTouchEnabled(true);
        }
        else
        {
            CommonWarFactoryButton->setTouchEnabled(false);
        }
        
        if (MyData.MyMoney > 400)
        {
            switch (MyData.MyCountryChoice)
            {
                case 3:
                    ThreeMissileWellButton->setTouchEnabled(true);
                    break;
                    
                default:
                    break;
            }
        }
        else
        {
            switch (MyData.MyCountryChoice)
            {
                case 3:
                    ThreeMissileWellButton->setTouchEnabled(false);
                    break;
                    
                default:
                break;
            }
        }
    
        if (MyData.RefineryNumber > 0 && MyData.ElectricPowerPlantNumber > 0 && MyData.WarFactoryNumber > 0)
        {
            if (MyData.MyMoney > 50)
            {
                CommonTankButton->setTouchEnabled(true);
            }
            else
            {
                CommonTankButton->setTouchEnabled(false);
            }
        
            if (MyData.MyMoney > 40)
            {
                switch (MyData.MyCountryChoice)
                {
                    case 1:
                        OneJetButton->setTouchEnabled(true);
                        break;
                    
                    default:
                        break;
                }
            }
            else
            {
                switch (MyData.MyCountryChoice)
                {
                    case 1:
                        OneJetButton->setTouchEnabled(false);
                        break;
                        
                    default:
                        break;
                }
            }
        
            if (MyData.MyMoney > 60)
            {
                switch (MyData.MyCountryChoice)
                {
                    case 1:
                        OneBattlePlaneButton->setTouchEnabled(true);
                        break;
                    
                    case 2:
                        TwoTTankButton->setTouchEnabled(true);
                        TwoRTankButton->setTouchEnabled(true);
                    default:
                        break;
                }
            }
            else
            {
                switch (MyData.MyCountryChoice)
                {
                    case 1:
                        OneBattlePlaneButton->setTouchEnabled(false);
                        break;
                    
                    case 2:
                        TwoTTankButton->setTouchEnabled(false);
                        TwoRTankButton->setTouchEnabled(false);
                    default:
                        break;
                }
            }
        }
    
        if (MyData.RefineryNumber > 0 && MyData.ElectricPowerPlantNumber > 0 && MyData.BarracksNumber > 0)
        {
            if (MyData.MyMoney > 20)
            {
                CommonSoldierButton->setTouchEnabled(true);
            }
            else
            {
                CommonSoldierButton->setTouchEnabled(false);
            }
        
            if (MyData.MyMoney > 30)
            {
                switch (MyData.MyCountryChoice)
                {
                    case 3:
                        ThreeSoldierXButton->setTouchEnabled(true);
                        break;
                    
                    default:
                        break;
                }
            }
            else
            {
                switch (MyData.MyCountryChoice)
                {
                    case 3:
                        ThreeSoldierXButton->setTouchEnabled(false);
                        break;
                    
                    default:
                        break;
                }
            }
        }
    }
}

bool GameScene::isKeyPressed(EventKeyboard::KeyCode keyCode)
{
    if(keys[keyCode])
    {
        return true;
    }
    else
    {
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
    auto RightMenuMoveByAction = MoveBy::create(0, Vec2(offsetX, offsetY));
    rightMenuMoveBy(RightMenuMoveByAction);
    //RightMenuPicture->runAction(RightMenuMoveBy);
    //auto CommonElectricPowerPlantMoveBy = MoveBy::create(0, Vec2(offsetX, offsetY));
    //CommonElectricPowerPlantPicture->runAction(RightMenuMoveBy->clone());
}


void GameScene::armyMoveOnce(Sprite* ArmyName)
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
        MyData.LastTouchPosition = this->convertToNodeSpace(touch->getLocation());
        MyData.IsTouchPositionAvailable = 1;
        this->removeChild(ArmyName);
        
        if (MyData.IsTouchPositionAvailable)
        {
            armyBuildCallBack(loadingElectricPowerPlantAction(),"CommonElectricPowerPlant_action/CommonElectricPowerPlant_action_15.png");
        }
    };
    //将触摸监听添加到eventDispacher中去
    _eventDispatcher->addEventListenerWithSceneGraphPriority(ArmyListener, ArmyName);
}

void GameScene::armyBuildCallBack(Action* BuildingAction,const std::string& FileName)
{
    auto BuildingSprite = BuildingsClass::createWithSpriteFileName(FileName);
    BuildingSprite->setPosition(MyData.LastTouchPosition);
    this->addChild(BuildingSprite);
    BuildingSprite->runAction(BuildingAction);
    MyData.IsTouchPositionAvailable = 0;
    MyData.MyBuildings.push_back(BuildingSprite);
    //*****************以下为修改***********
    auto BuildingHPBar = LoadingBar::create("GamePicture/HPBar.png");
    BuildingHPBar->setDirection(LoadingBar::Direction::LEFT);
    BuildingHPBar->setScale(0.08f);
    BuildingHPBar->setPercent(100);
    BuildingHPBar->setPosition(Vec2(MyData.LastTouchPosition.x,MyData.LastTouchPosition.y+100));
    this->addChild(BuildingHPBar,2);
    auto HPBarDelay = DelayTime::create(21.0f);
    auto HPBarFadeIn = FadeIn::create(0.0f);
    auto HPBarFadeOut = FadeOut::create(0.0f);
    auto HPBarSequence = Sequence::create(HPBarFadeOut,HPBarDelay,HPBarFadeIn,NULL);
    BuildingHPBar->runAction(HPBarSequence);
    BuildingSprite->setHP(BuildingHPBar);
    BuildingSprite->setHPInterval(100.0f/BuildingSprite->getLifeValue());
}



//&%&%&%&%&%&%&%&%&%&%&%努力修改%&%&%&%&%&%&%&%&%&
void GameScene::armyMoveOnce(Sprite* ArmyName,Action* ArmyAction)
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
        MyData.LastTouchPosition = this->convertToNodeSpace(touch->getLocation());
        MyData.IsTouchPositionAvailable = 1;
        this->removeChild(ArmyName);
        
        if (MyData.IsTouchPositionAvailable)
        {
            armyBuildCallBack(ArmyAction,"CommonElectricPowerPlant_action/CommonElectricPowerPlant_action_15.png");
        }
    };
    //将触摸监听添加到eventDispacher中去
    _eventDispatcher->addEventListenerWithSceneGraphPriority(ArmyListener, ArmyName);
}














