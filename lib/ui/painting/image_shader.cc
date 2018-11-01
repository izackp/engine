// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "flutter/lib/ui/painting/image_shader.h"

#include "flutter/lib/ui/ui_dart_state.h"

namespace blink {
/*
static void ImageShader_constructor(Dart_NativeArguments args) {
  DartCallConstructor(&ImageShader::Create, args);
}

IMPLEMENT_WRAPPERTYPEINFO(ui, ImageShader);

#define FOR_EACH_BINDING(V) V(ImageShader, initWithImage)

FOR_EACH_BINDING(DART_NATIVE_CALLBACK)

void ImageShader::RegisterNatives(tonic::DartLibraryNatives* natives) {
  natives->Register(
      {{"ImageShader_constructor", ImageShader_constructor, 1, true},
       FOR_EACH_BINDING(DART_REGISTER_NATIVE)});
}*/

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
}

ImageShader::ImageShader() = default;

ImageShader::~ImageShader() = default;

}  // namespace blink
