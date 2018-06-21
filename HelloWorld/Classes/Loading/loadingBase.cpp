//**************************************************
/*
 该文档用于把冗长的精灵创建代码拉到一个助教不能轻易看见的地方
 //你的代码怎么那么乱
 */
//**************************************************
#include "../Scene/GameScene.h"


void GameScene::baseBuildBallBack()
{
    MyData.TagNumber += 2;
    MyData.MyBase = BuildingsClass::createWithSpriteFileName("Common/camera.png");
    int width = MyData.MyBase->getContentSize().width;
    int height = MyData.MyBase->getContentSize().height;
    int x = 150-width/2;
    int y = 150-height/2;
    for (int i = 0; i <= width; i++)
    {
        for (int j = 0; j <= height; j++)
        {
            MyData.IsPositionHaveBuildings[x+i][y+j] = 1;
            MyData.PositionTag[x+i][y+j] = MyData.TagNumber-1;
        }
    }
    MyData.MyBase->setPosition(Vec2(150, 150));
    this->addChild(MyData.MyBase,1);
    MyData.MyBase->runAction(loadingBaseAction());
    auto BuildingHPBar = LoadingBar::create("GamePicture/HPBar.png");
    BuildingHPBar->setDirection(LoadingBar::Direction::LEFT);
    BuildingHPBar->setScale(0.07f);
    BuildingHPBar->setPercent(100);
    BuildingHPBar->setPosition(Vec2(150,200));
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








