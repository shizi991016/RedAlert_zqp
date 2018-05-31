#include "GameScene.h"
#include "SimpleAudioEngine.h"
#include "CountryChoiceScene.h"
#include "../Army/AllArmy.h"
//#include "../PublicData.h"
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
    
    auto VisibleSize = Director::getInstance()->getVisibleSize();
    Vec2 Origin = Director::getInstance()->getVisibleOrigin();
    
    
    ScreenWidth = VisibleSize.width;
    ScreenHeight = VisibleSize.height;
    
    TileMap = TMXTiledMap::create("RedAlertMap.tmx");
    TileMap->setAnchorPoint(Vec2::ZERO);
    TileMap->setPosition(Vec2::ZERO);
    this->addChild(TileMap,-1);
    
    
    Camera = Sprite::create("Common/camera.png");
    Camera->setPosition(Vec2(ScreenWidth/2,ScreenHeight/2));
    this->addChild(Camera);
    
    RightMenuPicture = Sprite::create("MenuPicture/RightMenu.png");
    RightMenuPicture->setPosition(Vec2(ScreenWidth-100, ScreenHeight/2));
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
    
    
    
    /*******************************ButtonText***************************************
    Textbutton = Button::create("GamePicture/Barracks.png");
    Textbutton->setPosition(Vec2(VisibleSize.width - 150, VisibleSize.height - 28));
    
    //WarFactoryButton = Button::create("GamePicture/Barracks.png");
    //WarFactoryButtonPicture = Sprite::create("GamePicture/Barracks.png");
    
    
    Textbutton->addTouchEventListener([=](Ref* pSender,Widget::TouchEventType type)
                                      {
                                          switch (type)
                                          {
                                              case Widget::TouchEventType::ENDED:
                                              {
                                                  auto TextPic = Sprite::create("GamePicture/Barracks.png");
                                                  TextPic->setPosition(Vec2(Camera->getPosition().x,Camera->getPosition().y));
                                                  //TextPic->setScale(2.0f);
                                                  this->addChild(TextPic,1);
                                                  break;
                                              }
                                                  
                                              default:
                                                  break;
                                          }
                                      });
    this->addChild(Textbutton,1);
    */
    
    switch (CountryChoice)
    {
        case 1:
        {
            //*******************************ButtonText***************************************
            Textbutton = Button::create("GamePicture/Cannon.png");
            Textbutton->setPosition(Vec2(VisibleSize.width - 150, VisibleSize.height - 28));
            
            //WarFactoryButton = Button::create("GamePicture/Barracks.png");
            //WarFactoryButtonPicture = Sprite::create("GamePicture/Barracks.png");
            
            
            Textbutton->addTouchEventListener([=](Ref* pSender,Widget::TouchEventType type)
                                              {
                                                  switch (type)
                                                  {
                                                      case Widget::TouchEventType::ENDED:
                                                      {
                                                          auto TextPic = Sprite::create("GamePicture/Cannon.png");
                                                          TextPic->setPosition(Vec2(Camera->getPosition().x,Camera->getPosition().y));
                                                          //TextPic->setScale(2.0f);
                                                          this->addChild(TextPic,1);
                                                          break;
                                                      }
                                                          
                                                      default:
                                                          break;
                                                  }
                                              });
            this->addChild(Textbutton,1);
            //*******************************ButtonText***************************************
            
            break;
        }
            
        case 2:
        {
            //*******************************ButtonText***************************************
            Textbutton = Button::create("GamePicture/Rtank.png");
            Textbutton->setPosition(Vec2(VisibleSize.width - 150, VisibleSize.height - 28));
            
            //WarFactoryButton = Button::create("GamePicture/Barracks.png");
            //WarFactoryButtonPicture = Sprite::create("GamePicture/Barracks.png");
            
            
            Textbutton->addTouchEventListener([=](Ref* pSender,Widget::TouchEventType type)
                                              {
                                                  switch (type)
                                                  {
                                                      case Widget::TouchEventType::ENDED:
                                                      {
                                                          auto TextPic = Sprite::create("GamePicture/Rtank.png");
                                                          TextPic->setPosition(Vec2(Camera->getPosition().x,Camera->getPosition().y));
                                                          //TextPic->setScale(2.0f);
                                                          this->addChild(TextPic,1);
                                                          break;
                                                      }
                                                          
                                                      default:
                                                          break;
                                                  }
                                              });
            this->addChild(Textbutton,1);
            //*******************************ButtonText***************************************
            
            break;
        }
        
        case 3:
        {
            //*******************************ButtonText***************************************
            Textbutton = Button::create("GamePicture/Barracks.png");
            Textbutton->setPosition(Vec2(VisibleSize.width - 150, VisibleSize.height - 28));
            
            //WarFactoryButton = Button::create("GamePicture/Barracks.png");
            //WarFactoryButtonPicture = Sprite::create("GamePicture/Barracks.png");
            
            
            Textbutton->addTouchEventListener([=](Ref* pSender,Widget::TouchEventType type)
                                              {
                                                  switch (type)
                                                  {
                                                      case Widget::TouchEventType::ENDED:
                                                      {
                                                          auto TextPic = Sprite::create("GamePicture/Barracks.png");
                                                          TextPic->setPosition(Vec2(Camera->getPosition().x,Camera->getPosition().y));
                                                          //TextPic->setScale(2.0f);
                                                          this->addChild(TextPic,1);
                                                          break;
                                                      }
                                                          
                                                      default:
                                                          break;
                                                  }
                                              });
            this->addChild(Textbutton,1);
            //*******************************ButtonText***************************************
            
            break;
        }
        
        default:
        {
            //*******************************ButtonText***************************************
            Textbutton = Button::create("GamePicture/Ship.png");
            Textbutton->setPosition(Vec2(VisibleSize.width - 150, VisibleSize.height - 28));
            
            //WarFactoryButton = Button::create("GamePicture/Barracks.png");
            //WarFactoryButtonPicture = Sprite::create("GamePicture/Barracks.png");
            
            
            Textbutton->addTouchEventListener([=](Ref* pSender,Widget::TouchEventType type)
                                              {
                                                  switch (type)
                                                  {
                                                      case Widget::TouchEventType::ENDED:
                                                      {
                                                          auto TextPic = Sprite::create("GamePicture/Ship.png");
                                                          TextPic->setPosition(Vec2(Camera->getPosition().x,Camera->getPosition().y));
                                                          //TextPic->setScale(2.0f);
                                                          this->addChild(TextPic,1);
                                                          break;
                                                      }
                                                          
                                                      default:
                                                          break;
                                                  }
                                              });
            this->addChild(Textbutton,1);
            //*******************************ButtonText***************************
            break;
        }
            
    }
    
    
    
    
    
   
    
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
}








