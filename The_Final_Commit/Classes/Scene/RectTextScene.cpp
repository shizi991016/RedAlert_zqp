#include "cocos2d.h"
#include "RectTextScene.h"

USING_NS_CC;

Scene* RectTextScene::createScene()
{
    return RectTextScene::create();
}

bool RectTextScene::init()
{
    if ( !Scene::init() )
    {
        return false;
    }
    
    auto destListener = EventListenerTouchOneByOne::create();
    destListener->onTouchBegan = [this](Touch* touch, Event* event)
    {
        MouseRect_->TouchStartPoint = touch->getLocation();
        return true;
    };
    
    destListener->onTouchMoved = [this](Touch* touch, Event* event)
    {
        MouseRect_->TouchEndPoint = touch->getLocation();
        MouseRect_->clear();
        MouseRect_->drawLine(MouseRect_->TouchStartPoint, MouseRect_->TouchEndPoint, Color4F::RED);
    };
    
    destListener->onTouchEnded = [this](Touch* touch, Event* event)
    {
        MouseRect_->TouchEndPoint = touch->getLocation();
        double x = MouseRect_->TouchEndPoint.x - MouseRect_->TouchStartPoint.x;
        double y = MouseRect_->TouchEndPoint.y - MouseRect_->TouchStartPoint.y;
        double x2 = x * x;
        double y2 = y * y;
        if (x2 >= 100 && y2 >= 100)//框选而非点选
        {
            getChildrenInRect(MouseRect_->TouchStartPoint,MouseRect_->TouchEndPoint);
        }
        else//点选
        {
            //执行点选操作 位于GameScene中
        }
    };
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(destListener, this);
    return true;
}


void RectTextScene::getChildrenInRect(cocos2d::Point P1, cocos2d::Point P2)
{
    int x1, y1, x2, y2;
    x1 = P1.x;
    y1 = P1.y;
    x2 = P2.x;
    y2 = P2.y;
    /*
    MyData.BuildingsList;
    for(int i = 0, i < MyData.BuildingList.size(), i++)
     {
        if(MyData.BuildingList[i]->getLocation.x >= x1 &&
           MyData.BuildingList[i]->getLocation.x <= x2 &&
           MyData.BuildingList[i]->getLocation.y >= y1 &&
           MyData.BuildingList[i]->getLocation.y <= y2 &&)
        {
            加入move的管理vector
            调用move函数遍历vector
        }
     }
    */
}





