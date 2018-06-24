//**************************************************
/*
 该文档用于把冗长的精灵创建代码拉到一个助教不能轻易看见的地方
 //你的代码怎么那么乱
 */
//**************************************************
#include "../Scene/GameScene.h"
int ClientChoice;

void GameScene::baseBuildBallBack()
{
    MyData.TagNumber += 2;
    MyData.MyBase = BuildingsClass::createWithSpriteFileName("Common/camera.png");
    int width = MyData.MyBase->getContentSize().width;
    int height = MyData.MyBase->getContentSize().height;
    /*
    int x = 150-width/2;
    int y = 150-height/2;
     */
    int x,y;
    switch (MyData.MyClientChoice) {
        case 1:
            x = 150-width/2;
            y = 150-height/2;
            break;
        case 2:
            x = 1450-width/2;
            y = 1450-height/2;
            break;
        default:
            break;
    }
    
    for (int i = 0; i <= width; i++)
    {
        for (int j = 0; j <= height; j++)
        {
            IsPositionHaveBuildings[x+i][y+j] = 1;
            PositionTag[x+i][y+j] = MyData.TagNumber-1;
        }
    }
    
    std::stringstream stream;
    stream << MyData.TagNumber << " " << "*" << x+width/2 << "*" << "/" << y+height/2 << "/";
    std::stringstream stream1;
    stream1 << MyData.MyClientChoice;
    std::string MyOrder;
    stream1 >> MyOrder;
    MyOrder += "bn ";
    std::string order = stream.str();
    MyOrder += order;
    MyOrderList.push_back(MyOrder);
    
    MyData.MyBase->setPosition(Vec2(x+width/2, y+height/2));
    this->addChild(MyData.MyBase,1);
    MyData.MyBase->runAction(loadingBaseAction());
    auto BuildingHPBar = LoadingBar::create("GamePicture/HPBar.png");
    BuildingHPBar->setDirection(LoadingBar::Direction::LEFT);
    BuildingHPBar->setScale(0.07f);
    BuildingHPBar->setPercent(100);
    BuildingHPBar->setPosition(Vec2(x+width/2, y+height/2+50));
    this->addChild(BuildingHPBar,2);
    auto HPBarDelay = DelayTime::create(7.0f);
    auto HPBarFadeIn = FadeIn::create(0.0f);
    auto HPBarFadeOut = FadeOut::create(0.0f);
    auto HPBarSequence = Sequence::create(HPBarFadeOut,HPBarDelay,HPBarFadeIn,NULL);
    BuildingHPBar->runAction(HPBarSequence);
    MyData.MyBase->setHP(BuildingHPBar);
    MyData.MyBase->setHPInterval(100.0f/MyData.MyBase->getLifeValue());
    MyData.MyBase->setTag(MyData.TagNumber-1);
    BuildingHPBar->setTag(MyData.TagNumber);
}



Action* GameScene::loadingBaseAction()                                                    ////完工
{
    auto BuildingDelayTime = DelayTime::create(4.0f);
    auto BuildingAnimation = Animation::create();
    for (int i = 1; i < 11; i++)
    {
        BuildingAnimation->addSpriteFrameWithFile
        (StringUtils::format("ArmyAction/Base_action/Base_action_%d.png",i));
    }
    BuildingAnimation->setDelayPerUnit(1.0f/3.0f);
    auto BuildingAction = Animate::create(BuildingAnimation);
    
    auto AllAction = Sequence::createWithTwoActions(BuildingDelayTime, BuildingAction);
    return AllAction;
}

void GameScene::emenyBaseBuildCallBack(int Tag)
{
    auto BuildingSprite = BuildingsClass::createWithSpriteFileName("Common/camera.png");
    BuildingSprite->setPosition(MyData.LastTouchPosition);
    this->addChild(BuildingSprite,1);
    BuildingSprite->runAction(loadingBaseAction());
    
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
    auto BuildingHPBar = LoadingBar::create("GamePicture/HPBar.png");
    BuildingHPBar->setDirection(LoadingBar::Direction::LEFT);
    BuildingHPBar->setScale(0.07f);
    BuildingHPBar->setPercent(100);
    BuildingHPBar->setPosition(Vec2(MyData.LastTouchPosition.x,MyData.LastTouchPosition.y+40));
    this->addChild(BuildingHPBar,2);
    auto HPBarDelay = DelayTime::create(7.0f);
    auto HPBarFadeIn = FadeIn::create(0.0f);
    auto HPBarFadeOut = FadeOut::create(0.0f);
    auto HPBarSequence = Sequence::create(HPBarFadeOut,HPBarDelay,HPBarFadeIn,NULL);
    BuildingHPBar->runAction(HPBarSequence);
    BuildingSprite->setHP(BuildingHPBar);
    BuildingSprite->setHPInterval(100.0f/BuildingSprite->getLifeValue());
    BuildingSprite->setTag(Tag-1);
    BuildingHPBar->setTag(Tag);
}






