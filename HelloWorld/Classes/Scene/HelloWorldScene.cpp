#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;
using namespace CocosDenshion;
Scene* HelloWorld::createScene()
{
    return HelloWorld::create();
}

static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}


bool HelloWorld::init()
{
    if ( !Scene::init() )
    {
        return false;
    }
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    
    _screenWidth = visibleSize.width;
    _screenHeight = visibleSize.height;
    
    _tileMap = TMXTiledMap::create("RedAlertMap.tmx");
    _tileMap->setAnchorPoint(Vec2::ZERO);
    _tileMap->setPosition(Vec2::ZERO);
    this->addChild(_tileMap,-1);

    
    camera = Sprite::create("Common/camera.png");
    camera->setPosition(Vec2(_screenWidth/2,_screenHeight/2));
    this->addChild(camera);
    
    RightMenuPicture = Sprite::create("MenuPicture/RightMenu.png");
    RightMenuPicture->setPosition(Vec2(_screenWidth-100, _screenHeight/2));
    RightMenuPicture->setScale(1, 1.279);
    this->addChild(RightMenuPicture);
    
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
    
    
    this->scheduleUpdate();
    
    return true;
}



void HelloWorld::setViewpointCenter(Vec2 position)
{
    Point actualPosition = Vec2(position.x,position.y);
    Point centerofView = Vec2(_screenWidth/2,_screenHeight/2);
    Point viewPoint = centerofView - actualPosition;
    this->setPosition(viewPoint);
}


void HelloWorld::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();

    #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}


void HelloWorld::update(float delta)
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
    this->setViewpointCenter(camera->getPosition());
}

bool HelloWorld::isKeyPressed(EventKeyboard::KeyCode keyCode)
{
    if(keys[keyCode]) {
        return true;
    } else {
        return false;
    }
}
void HelloWorld::keyPressedDuration(EventKeyboard::KeyCode code)
{
    int offsetX = 0, offsetY = 0;
    switch (code) {
        case EventKeyboard::KeyCode::KEY_A:
            if ((int)camera->getPositionX() > _screenWidth/2)
            {
                offsetX = -5;
            }
            else
            {
                offsetX = 0;
            }
            break;
        case EventKeyboard::KeyCode::KEY_D:
            if ((int)camera->getPositionX() < _tileMap->getMapSize().width * _tileMap->getTileSize().width - _screenWidth/2 - 10)
            {
                offsetX = 5;
            }
            else
            {
                offsetX = 0;
            }
            break;
        case EventKeyboard::KeyCode::KEY_W:
            if ((int)camera->getPositionY() < _tileMap->getMapSize().height * _tileMap->getTileSize().height - _screenHeight/2 - 10)
            {
                offsetY = 5;
            }
            else
            {
                offsetY = 0;
            }
            break;
        case EventKeyboard::KeyCode::KEY_S:
            if ((int)camera->getPositionY() > _screenHeight/2)
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
    camera->runAction(CameraMoveBy);
    auto RightMenuMoveBy = MoveBy::create(0, Vec2(offsetX, offsetY));
    RightMenuPicture->runAction(RightMenuMoveBy);
}
