// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "flutter/lib/ui/painting/image.h"

namespace blink {

typedef CanvasImage Image;
/*
IMPLEMENT_WRAPPERTYPEINFO(ui, Image);

#define FOR_EACH_BINDING(V) \
  V(Image, width)           \
  V(Image, height)          \
  V(Image, toByteData)      \
  V(Image, dispose)

FOR_EACH_BINDING(DART_NATIVE_CALLBACK)

void CanvasImage::RegisterNatives(tonic::DartLibraryNatives* natives) {
  natives->Register({FOR_EACH_BINDING(DART_REGISTER_NATIVE)});
}*/

CanvasImage::CanvasImage() = default;

CanvasImage::~CanvasImage() = default;

char* CanvasImage::toByteData(ImageByteFormat format, void (*callback)(sk_sp<SkData>)) {
  return EncodeImage(this, format, callback);
}

void CanvasImage::dispose() {
}

size_t CanvasImage::GetAllocationSize() {
  if (auto image = image_.get()) {
    return image->width() * image->height() * 4;
  } else {
    return sizeof(CanvasImage);
  }
}

}  // namespace blink
