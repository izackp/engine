// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef FLUTTER_LIB_UI_PAINTING_IMAGE_SHADER_H_
#define FLUTTER_LIB_UI_PAINTING_IMAGE_SHADER_H_

#include "flutter/lib/ui/dart_wrapper.h"
#include "flutter/lib/ui/painting/gradient.h"
#include "flutter/lib/ui/painting/image.h"
#include "flutter/lib/ui/painting/shader.h"
#include "third_party/skia/include/core/SkMatrix.h"
#include "third_party/skia/include/core/SkShader.h"

namespace blink {

class ImageShader : public Shader {

  FML_FRIEND_MAKE_REF_COUNTED(ImageShader);

 public:
  ~ImageShader();
  static fml::RefPtr<ImageShader> Create();

  char* initWithImage(CanvasImage* image,
                     SkShader::TileMode tmx,
                     SkShader::TileMode tmy,
                     const SkMatrix& matrix);

 private:
  ImageShader();
};

}  // namespace blink

#endif  // FLUTTER_LIB_UI_PAINTING_IMAGE_SHADER_H_
