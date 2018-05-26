
#ifndef SettingScene_h
#define SettingScene_h

#include "cocos2d.h"
#include "extensions/GUI/CCControlExtension/CCControlSlider.h"
#include "ui/CocosGUI.h"


USING_NS_CC;
using namespace cocos2d::extension;
using namespace ui;

class SettingScene : public cocos2d::Scene
{
private:
    cocos2d::Label* _displayValueLabel;
public:
    static cocos2d::Scene* createScene();
    
    virtual bool init();
    
    CREATE_FUNC(SettingScene);
    
    void menuReturnCallback(cocos2d::Ref* pSender);
    
    void valueChanged(Ref *sender, Control::EventType controlEvent);
    
};

#endif /* SettingScene_h */
