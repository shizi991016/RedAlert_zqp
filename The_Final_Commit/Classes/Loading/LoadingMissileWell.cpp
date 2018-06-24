
//**************************************************
/*
 该文档用于把冗长的精灵创建代码拉到一个助教不能轻易看见的地方
 //你的代码怎么那么乱
 */
//**************************************************
#include "../Scene/GameScene.h"


void GameScene::missileWellMoveOnce(Sprite* ArmyName)
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
        Point LocationInWorld = this->convertToNodeSpace(touch->getLocation());//获得触摸点相对于世界地图的坐标
        Size ArmySize = ArmyTarget->getContentSize();
        int x = LocationInWorld.x - ArmySize.width/2;
        int y = LocationInWorld.y - ArmySize.height/2;
        for (int i = 0; i <= ArmySize.width; i++)
        {
            for (int j = 0; j <= ArmySize.height; j++)
            {
                if (IsPositionHaveBuildings[x+i][y+j] == 1 && x+i >= 0 && y+j >= 0 && x+i <= 1600 && y+j <= 1600)
                {
                    this->removeChild(ArmyName);
                    _eventDispatcher->removeEventListener(ArmyListener);
                    return false;
                }
            }
        }
       
        MyData.TagNumber += 2;
        
        std::stringstream stream1;
        stream1 << MyData.MyClientChoice;
        std::string MyOrder;
        stream1 >> MyOrder;
        MyOrder += "bm ";
        std::stringstream stream;
        stream << MyData.TagNumber << " " << "*" << x << "*" << "/" << y << "/";
        std::string order = stream.str();
        //std::string MyOrder = "1bd ";
        MyOrder += order;
        MyOrderList.push_back(MyOrder);
        
        for (int i = 0; i <= ArmySize.width; i++)
        {
            for (int j = 0; j <= ArmySize.height; j++)
            {
                if (x+i >= 0 && y+j >= 0 && x+i <= 1600 && y+j <= 1600)
                {
                    IsPositionHaveBuildings[x+i][y+j] = 1;
                    PositionTag[x+i][y+j] = MyData.TagNumber-1;
                }
            }
        }
        _eventDispatcher->removeEventListener(ArmyListener);
        MyData.LastTouchPosition = this->convertToNodeSpace(touch->getLocation());
        MyData.IsTouchPositionAvailable = 1;
        this->removeChild(ArmyName);
        missileWellBuildCallBack();
        MyData.MyMoney -= 400;
    };
    //将触摸监听添加到eventDispacher中去
    _eventDispatcher->addEventListenerWithSceneGraphPriority(ArmyListener, ArmyName);
}



void GameScene::missileWellBuildCallBack()
{
    auto BuildingSprite = BuildingsClass::createWithSpriteFileName("ArmyAction/ThreeMissileWell_action/ThreeMissileWell_action_15.png");
    BuildingSprite->setPosition(MyData.LastTouchPosition);
    this->addChild(BuildingSprite,1);
    BuildingSprite->runAction(loadingMissileWellAction());
    MyData.IsTouchPositionAvailable = 0;
    MyData.MyCannon.push_back(BuildingSprite);
    auto BuildingHPBar = LoadingBar::create("GamePicture/HPBar.png");
    BuildingHPBar->setDirection(LoadingBar::Direction::LEFT);
    BuildingHPBar->setScale(0.07f);
    BuildingHPBar->setPercent(100);
    BuildingHPBar->setPosition(Vec2(MyData.LastTouchPosition.x,MyData.LastTouchPosition.y+50));
    this->addChild(BuildingHPBar,2);
    auto HPBarDelay = DelayTime::create(24.0f);
    auto HPBarFadeIn = FadeIn::create(0.0f);
    auto HPBarFadeOut = FadeOut::create(0.0f);
    auto HPBarSequence = Sequence::create(HPBarFadeOut,HPBarDelay,HPBarFadeIn,NULL);
    BuildingHPBar->runAction(HPBarSequence);
    BuildingSprite->setHP(BuildingHPBar);
    BuildingSprite->setHPInterval(100.0f/BuildingSprite->getLifeValue());
    BuildingSprite->setTag(MyData.TagNumber-1);
    BuildingHPBar->setTag(MyData.TagNumber);
    
    auto ArmyListener = EventListenerTouchOneByOne::create();//创建一个触摸监听
    ArmyListener->setSwallowTouches(true);
    ArmyListener->onTouchBegan = [=](Touch* touch, Event* event)
    {
        MyData.ArmyFirstTouchPosition = this->convertToNodeSpace(touch->getLocation());
        auto ArmyTarget = static_cast<Sprite*>(event->getCurrentTarget());//获取的当前触摸的目标
        Point locationInNode = ArmyTarget->convertToNodeSpace(touch->getLocation());//获得触摸点相对于触摸目标的坐标
        Size ArmySize = ArmyTarget->getContentSize();//获得触摸目标的大小
        Rect ArmyRect = Rect(0, 0, ArmySize.width, ArmySize.height);//创建一个坐标在左下角的相对于触摸目标的坐标系
        if (ArmyRect.containsPoint(locationInNode))//判断触摸点是否在目标的范围内
            return true;
        else
            return false;
    };
    
    ArmyListener->onTouchEnded = [=](Touch* touch, Event* event)
    {
        MyData.ArmyLastTouchPosition = this->convertToNodeSpace(touch->getLocation());
        if (IsPositionHaveArmyAndTag
            [(int)MyData.ArmyLastTouchPosition.x][(int)MyData.ArmyLastTouchPosition.y])
        {
            BuildingsClass* BuildingSprite = (BuildingsClass*)this->getChildByTag
            (IsPositionHaveArmyAndTag
             [(int)MyData.ArmyLastTouchPosition.x][(int)MyData.ArmyLastTouchPosition.y]);
            BuildingSprite->setLifeValue(BuildingSprite->getLifeValue()-100);
            LoadingBar* HPBar = (LoadingBar*)this->getChildByTag(BuildingSprite->getTag()+1);
            HPBar->setPercent(HPBar->getPercent()-100);
            
            int Tag = HPBar->getTag();
            std::stringstream stream1;
            stream1 << MyData.MyClientChoice;
            std::string MyOrder;
            std::stringstream stream;
            stream << Tag << " " << "*" << 100 << "*";
            std::string order = stream.str();
            stream1 >> MyOrder;
            MyOrder += "a ";
            MyOrder += order;
            MyOrderList.push_back(MyOrder);
        }
        else
        {
            if (IsPositionHaveBuildings
                [(int)MyData.ArmyLastTouchPosition.x][(int)MyData.ArmyLastTouchPosition.y] == 1)
            {
                BuildingsClass* BuildingSprite = (BuildingsClass*)this->getChildByTag
                (PositionTag[(int)MyData.ArmyLastTouchPosition.x][(int)MyData.ArmyLastTouchPosition.y]);
                BuildingSprite->setLifeValue(BuildingSprite->getLifeValue()-100);
                LoadingBar* HPBar = (LoadingBar*)this->getChildByTag(BuildingSprite->getTag()+1);
                HPBar->setPercent(HPBar->getPercent()-100);
                
                int Tag = HPBar->getTag();
                std::stringstream stream1;
                stream1 << MyData.MyClientChoice;
                std::string MyOrder;
                std::stringstream stream;
                stream << Tag << " " << "*" << 100 << "*";
                std::string order = stream.str();
                stream1 >> MyOrder;
                MyOrder += "a ";
                MyOrder += order;
                MyOrderList.push_back(MyOrder);
            }
        }
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(ArmyListener, BuildingSprite);
}



Action* GameScene::loadingMissileWellAction()                                                    ////完工
{
    auto BuildingAnimation = Animation::create();
    for (int i = 1; i < 16; i++)
    {
        BuildingAnimation->addSpriteFrameWithFile
        (StringUtils::format("ArmyAction/ThreeMissileWell_action/ThreeMissileWell_action_%d.png",i));
    }
    BuildingAnimation->setDelayPerUnit(3.0f/2.0f);
    auto BuildingAction = Animate::create(BuildingAnimation);
    
    return BuildingAction;
}



void GameScene::emenyMissileWellBuildCallBack(int Tag)
{
    auto BuildingSprite = BuildingsClass::createWithSpriteFileName("ArmyAction/ThreeMissileWell_action/ThreeMissileWell_action_15.png");
    BuildingSprite->setPosition(MyData.LastTouchPosition);
    this->addChild(BuildingSprite,1);
    BuildingSprite->runAction(loadingMissileWellAction());
    
    int width = BuildingSprite->getContentSize().width;
    int height = BuildingSprite->getContentSize().height;
    int x = MyData.LastTouchPosition.x - width/2;
    int y = MyData.LastTouchPosition.y - height/2;
    for (int i = 0; i <= width; i++)
    {
        for (int j = 0; j <= height; j++)
        {
            IsPositionHaveBuildings[x+i][y+j] = 1;
            PositionTag[x+i][y+j] = Tag-1;
        }
    }
    
    
    MyData.IsTouchPositionAvailable = 0;
    MyData.MyMissileWell.push_back(BuildingSprite);
    auto BuildingHPBar = LoadingBar::create("GamePicture/HPBar.png");
    BuildingHPBar->setDirection(LoadingBar::Direction::LEFT);
    BuildingHPBar->setScale(0.07f);
    BuildingHPBar->setPercent(100);
    BuildingHPBar->setPosition(Vec2(MyData.LastTouchPosition.x,MyData.LastTouchPosition.y+40));
    this->addChild(BuildingHPBar,2);
    auto HPBarDelay = DelayTime::create(24.0f);
    auto HPBarFadeIn = FadeIn::create(0.0f);
    auto HPBarFadeOut = FadeOut::create(0.0f);
    auto HPBarSequence = Sequence::create(HPBarFadeOut,HPBarDelay,HPBarFadeIn,NULL);
    BuildingHPBar->runAction(HPBarSequence);
    BuildingSprite->setHP(BuildingHPBar);
    BuildingSprite->setHPInterval(100.0f/BuildingSprite->getLifeValue());
    BuildingSprite->setTag(Tag-1);
    BuildingHPBar->setTag(Tag);
}





