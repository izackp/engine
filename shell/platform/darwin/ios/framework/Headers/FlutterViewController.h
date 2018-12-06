// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef FLUTTER_FLUTTERVIEWCONTROLLER_H_
#define FLUTTER_FLUTTERVIEWCONTROLLER_H_

#import <UIKit/UIKit.h>
#include <sys/cdefs.h>

#include "FlutterBinaryMessenger.h"
#include "FlutterDartProject.h"
#include "FlutterMacros.h"
#include "FlutterPlugin.h"
#include "FlutterTexture.h"

FLUTTER_EXPORT
@interface FlutterViewController
    : UIViewController <FlutterBinaryMessenger, FlutterTextureRegistry>

- (instancetype)initWithNibName:(NSString*)nibNameOrNil
                         bundle:(NSBundle*)nibBundleOrNil NS_DESIGNATED_INITIALIZER;

- (void)handleStatusBarTouches:(UIEvent*)event;


/**
 Specifies the view to use as a splash screen. Flutter's rendering is asynchronous, so the first
 frame rendered by the Flutter application might not immediately appear when the Flutter view is
 initially placed in the view hierarchy. The splash screen view will be used as a replacement
 until the first frame is rendered.

 The view used should be appropriate for multiple sizes; an autoresizing mask to have a flexible
 width and height will be applied automatically.

 If not specified, uses a view generated from `UILaunchStoryboardName` from the main bundle's
 `Info.plist` file.
 */
@property(strong, nonatomic) UIView* splashScreenView;

@end

#endif  // FLUTTER_FLUTTERVIEWCONTROLLER_H_
