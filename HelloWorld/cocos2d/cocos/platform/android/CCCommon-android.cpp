/****************************************************************************
Copyright (c) 2010-2012 cocos2d-x.org
Copyright (c) 2013-2017 Chukong Technologies Inc.

http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/

#include "platform/CCPlatformConfig.h"
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID

#include "platform/CCCommon.h"
#include "platform/android/jni/JniHelper.h"
#include <android/log.h>
#include <stdio.h>
#include <jni.h>




NS_CC_BEGIN


void MessageBox(const char * msg, const char * title)
{
    showDialogJNI(msg, title, 0); // 回调地址为0 则不回调
}

void MessageBox(const char * msg, const char * title, const MessageBox_Callback & callback)
{
    MessageBox_Callback * Callback = new MessageBox_Callback(); // 待回调后 delete
    *Callback = callback;                                       // 备份对象
    
    showDialogJNI(msg, title, (long long)Callback); // 将地址作为64位整型传给Java(防止日后将c++代码编译为64位版本)
}

extern "C"
{
    // 供Java回调
    JNIEXPORT void JNICALL Java_org_cocos2dx_lib_Cocos2dxHandler_onDialogButtonClicked(JNIEnv* env, jobject thiz, jlong callback)
    {
        if (0 != callback)
        {
            MessageBox_Callback * Callback = (MessageBox_Callback *)callback;
            (*Callback)(); // 回调之
            
            delete Callback; // 删除对象
        }
    }
}  


#define MAX_LEN         (cocos2d::kMaxLogLen + 1)

void MessageBox(const char * pszMsg, const char * pszTitle)
{
    JniHelper::callStaticVoidMethod("org/cocos2dx/lib/Cocos2dxHelper", "showDialog", pszTitle, pszMsg);
}

void LuaLog(const char * pszFormat)
{
    __android_log_write(ANDROID_LOG_DEBUG, "cocos2d-x debug info", pszFormat);
}

NS_CC_END

#endif // CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID

