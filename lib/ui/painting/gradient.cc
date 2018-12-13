// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#define _USE_MATH_DEFINES

#include "flutter/lib/ui/painting/gradient.h"

namespace blink {

typedef CanvasGradient
    Gradient;  // Because the C++ name doesn't match the Dart name.
/*
static void Gradient_constructor(Dart_NativeArguments args) {
  DartCallConstructor(&CanvasGradient::Create, args);
}

IMPLEMENT_WRAPPERTYPEINFO(ui, Gradient);

#define FOR_EACH_BINDING(V) \
  V(Gradient, initLinear)   \
  V(Gradient, initRadial)   \
  V(Gradient, initSweep)    \
  V(Gradient, initTwoPointConical)

FOR_EACH_BINDING(DART_NATIVE_CALLBACK)

void CanvasGradient::RegisterNatives(tonic::DartLibraryNatives* natives) {
  natives->Register({{"Gradient_constructor", Gradient_constructor, 1, true},
                     FOR_EACH_BINDING(DART_REGISTER_NATIVE)});
}*/

fml::RefPtr<CanvasGradient> CanvasGradient::Create() {
  return fml::MakeRefCounted<CanvasGradient>();
}

void CanvasGradient::initLinear(const SkPoint pts[2],
                                const SkColor colors[],
                                const SkScalar pos[],
                                int colorCount,
                                SkShader::TileMode tile_mode) {

  set_shader(UIDartState::CreateGPUObject(SkGradientShader::MakeLinear(
      pts,
      colors, pos,
      colorCount, tile_mode)));
}

void CanvasGradient::initRadial(SkScalar center_x,
                                SkScalar center_y,
                                SkScalar radius,
                                const SkColor colors[],
                                const SkScalar pos[],
                                int colorCount,
                                SkShader::TileMode tile_mode,
                                const SkMatrix* localMatrix) {

  set_shader(UIDartState::CreateGPUObject(SkGradientShader::MakeRadial(
      SkPoint::Make(center_x, center_y), radius,
      colors, pos,
      colorCount, tile_mode, 0, localMatrix)));
}


void CanvasGradient::initSweep(SkScalar center_x,
                               SkScalar center_y,
                               const SkColor colors[],
                               const SkScalar pos[],
                               int colorCount,
                               SkShader::TileMode tile_mode,
                               SkScalar start_angle,
                               SkScalar end_angle,
                               const SkMatrix* localMatrix) {

  set_shader(UIDartState::CreateGPUObject(SkGradientShader::MakeSweep(
      center_x, center_y, colors,
      pos, colorCount, tile_mode,
      start_angle * 180.0 / M_PI, end_angle * 180.0 / M_PI, 0,
      localMatrix)));
}

void CanvasGradient::initTwoPointConical(SkScalar start_x,
                                         SkScalar start_y,
                                         SkScalar start_radius,
                                         SkScalar end_x,
                                         SkScalar end_y,
                                         SkScalar end_radius,
                                         const SkColor colors[],
                                         const SkScalar pos[],
                                         int colorCount,
                                         SkShader::TileMode tile_mode,
                                         const SkMatrix* localMatrix) {

  set_shader(UIDartState::CreateGPUObject(SkGradientShader::MakeTwoPointConical(
      SkPoint::Make(start_x, start_y), start_radius,
      SkPoint::Make(end_x, end_y), end_radius,
      colors, pos,
      colorCount, tile_mode, 0, localMatrix)));
}

CanvasGradient::CanvasGradient() = default;

CanvasGradient::~CanvasGradient() = default;

}  // namespace blink
