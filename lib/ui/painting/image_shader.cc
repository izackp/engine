// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "flutter/lib/ui/painting/image_shader.h"

#include "flutter/lib/ui/ui_dart_state.h"

namespace blink {

fml::RefPtr<ImageShader> ImageShader::Create() {
  return fml::MakeRefCounted<ImageShader>();
}

char* ImageShader::initWithImage(CanvasImage* image,
                                SkShader::TileMode tmx,
                                SkShader::TileMode tmy,
                                const SkMatrix& matrix) {
  if (!image) {
    return "ImageShader constructor called with non-genuine Image.";
  }
  set_shader(UIDartState::CreateGPUObject(
      image->image()->makeShader(tmx, tmy, &matrix)));
  return nullptr;
}

ImageShader::ImageShader() = default;

ImageShader::~ImageShader() = default;

}  // namespace blink
