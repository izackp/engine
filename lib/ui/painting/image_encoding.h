// Copyright 2018 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef FLUTTER_LIB_UI_PAINTING_IMAGE_ENCODING_H_
#define FLUTTER_LIB_UI_PAINTING_IMAGE_ENCODING_H_

#include "third_party/skia/include/core/SkImage.h"

namespace blink {

class CanvasImage;

//TODO: Use available skia options
enum ImageByteFormat {
  kRawRGBA,
  kRawUnmodified,
  kPNG,
};

char* EncodeImage(CanvasImage* canvas_image,
                  ImageByteFormat format,
                  void (*callback)(sk_sp<SkData>));

}  // namespace blink

#endif  // FLUTTER_LIB_UI_PAINTING_IMAGE_ENCODING_H_
