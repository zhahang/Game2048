//
//  hardware_input.m
//  2048
//
//  Created by zhahang on 2022/6/29.
//

#include <gainput/gainput.h>

#import <Cocoa/Cocoa.h>


// Define your user buttons
enum Button
{
    ButtonMenu,
    ButtonConfirm,
    MouseX,
    MouseY,
    KeyUp = 0,
    KeyDown = 1,
    KeyLeft = 2,
    KeyRight = 3,
    KeyQ = 4,
    KeyR = 5
};


const char* windowName = "Gainput basic sample";
const int width = 300;
const int height = 300;

gainput::InputManager* manager;
gainput::InputMap* map;
gainput::DeviceId mouseId;
gainput::DeviceId keyboardId;
gainput::DeviceId padId;

typedef void(^OnKeyboardDown)(Button btn_type);

@interface Updater : NSObject
@property (nonatomic, strong) OnKeyboardDown onKeyboardDown;
@end

@implementation Updater
- (void)update:(NSTimer *)theTimer
{
    manager->Update();
    
    if (map->GetBoolWasDown(KeyUp)) {
        if (_onKeyboardDown) _onKeyboardDown(KeyUp);
    }
    if (map->GetBoolWasDown(KeyDown)) {
        if (_onKeyboardDown) _onKeyboardDown(KeyDown);
    }
    if (map->GetBoolWasDown(KeyLeft)) {
        if (_onKeyboardDown) _onKeyboardDown(KeyLeft);
    }
    if (map->GetBoolWasDown(KeyRight)) {
        if (_onKeyboardDown) _onKeyboardDown(KeyRight);
    }
    if (map->GetBoolWasDown(KeyQ)) {
        if (_onKeyboardDown) _onKeyboardDown(KeyQ);
    }
    if (map->GetBoolWasDown(KeyR)) {
        if (_onKeyboardDown) _onKeyboardDown(KeyR);
    }

//    // Check button states
//    if (map->GetBoolWasDown(ButtonConfirm))
//    {
//        gainput::InputDevicePad* pad = static_cast<gainput::InputDevicePad*>(manager->GetDevice(padId));
//        pad->Vibrate(1.0f, 0.0f);
//    }
//    if (map->GetBoolWasDown(ButtonMenu))
//    {
//        gainput::InputDevicePad* pad = static_cast<gainput::InputDevicePad*>(manager->GetDevice(padId));
//        pad->Vibrate(0.0f, 0.0f);
//    }
//
//    if (map->GetBoolWasDown(ButtonMenu))
//    {
//        std::cout << "Open Menu!!" << std::endl;
//    }
//    if (map->GetBoolWasDown(ButtonConfirm))
//    {
//        std::cout << "Confirmed!!" << std::endl;
//    }
//    if (map->GetBool(ButtonConfirm))
//    {
//        std::cout << "LM down" << std::endl;
//    }
//
//    if (map->GetFloatDelta(MouseX) != 0.0f || map->GetFloatDelta(MouseY) != 0.0f)
//    {
//        std::cout << "Mouse:" << map->GetFloat(MouseX) << ", " << map->GetFloat(MouseY) << std::endl;
//    }
}
@end

int scanKeyboard(void (*callback)(int type))
{
    [NSApplication sharedApplication];
    [NSApp setActivationPolicy:NSApplicationActivationPolicyRegular];
    id window = [[NSWindow alloc] initWithContentRect:NSMakeRect(0, 0, width, height)
        styleMask:NSTitledWindowMask|NSClosableWindowMask backing:NSBackingStoreBuffered defer:NO];
    [window cascadeTopLeftFromPoint:NSMakePoint(20,20)];
    [window setTitle: @"Gainput Basic Sample"];
    [window makeKeyAndOrderFront:nil];

    manager = new gainput::InputManager;
    manager->SetDisplaySize(width, height);
//    mouseId = manager->CreateDevice<gainput::InputDeviceMouse>();
    keyboardId = manager->CreateDevice<gainput::InputDeviceKeyboard>();
//    padId = manager->CreateDevice<gainput::InputDevicePad>();

    map = new gainput::InputMap(*manager);
//    map->MapBool(ButtonMenu, keyboardId, gainput::KeyReturn);
//    map->MapBool(ButtonConfirm, mouseId, gainput::MouseButtonLeft);
//    map->MapFloat(MouseX, mouseId, gainput::MouseAxisX);
//    map->MapFloat(MouseY, mouseId, gainput::MouseAxisY);
//    map->MapBool(ButtonConfirm, padId, gainput::PadButtonA);
    map->MapBool(KeyUp, keyboardId, gainput::KeyUp);
    map->MapBool(KeyDown, keyboardId, gainput::KeyDown);
    map->MapBool(KeyLeft, keyboardId, gainput::KeyLeft);
    map->MapBool(KeyRight, keyboardId, gainput::KeyRight);
    map->MapBool(KeyQ, keyboardId, gainput::KeyQ);
    map->MapBool(KeyR, keyboardId, gainput::KeyR);

    Updater* updater = [[Updater alloc] init];
    updater.onKeyboardDown = ^(Button btn_type) {
        int type = (int)btn_type;
        callback(type);
    };

    [NSTimer scheduledTimerWithTimeInterval:0.016
        target:updater
        selector:@selector(update:)
        userInfo:nil
        repeats:YES];

//    [NSApp activateIgnoringOtherApps:YES];
    [NSApp run];
    return 0;
}
