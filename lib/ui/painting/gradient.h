// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef FLUTTER_LIB_UI_PAINTING_GRADIENT_H_
#define FLUTTER_LIB_UI_PAINTING_GRADIENT_H_

#include <vector>

#include "flutter/lib/ui/dart_wrapper.h"
#include "flutter/lib/ui/painting/shader.h"
#include "third_party/skia/include/effects/SkGradientShader.h"

namespace blink {

// TODO: update this if/when Skia adds Decal mode skbug.com/7638
static_assert(SkShader::kTileModeCount >= 3, "Need to update tile mode enum");

class CanvasGradient : public Shader {
  FML_FRIEND_MAKE_REF_COUNTED(CanvasGradient);

 public:
  ~CanvasGradient();
  static fml::RefPtr<CanvasGradient> Create();

  void initLinear(const SkPoint pts[2],
                  const SkColor colors[],
                  const SkScalar pos[],
                  int colorCount,
                  SkShader::TileMode tile_mode);

  void initRadial(SkScalar center_x,
                  SkScalar center_y,
                  SkScalar radius,
                  const SkColor colors[],
                  const SkScalar pos[],
                  int colorCount,
                  SkShader::TileMode tile_mode,
                  const SkMatrix* localMatrix);

  void initSweep(SkScalar center_x,
                 SkScalar center_y,
                 const SkColor colors[],
                 const SkScalar pos[],
                 int colorCount,
                 SkShader::TileMode tile_mode,
                 SkScalar start_angle,
                 SkScalar end_angle,
                 const SkMatrix* localMatrix);

  void initTwoPointConical(SkScalar start_x,
                           SkScalar start_y,
                           SkScalar start_radius,
                           SkScalar end_x,
                           SkScalar end_y,
                           SkScalar end_radius,
                           const SkColor colors[],
                           const SkScalar pos[],
                           int colorCount,
                           SkShader::TileMode tile_mode,
                           const SkMatrix* localMatrix);

 private:
  CanvasGradient();
};

}  // namespace blink

#endif  // FLUTTER_LIB_UI_PAINTING_GRADIENT_H_
