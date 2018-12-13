// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef FLUTTER_LIB_UI_PAINTING_IMAGE_H_
#define FLUTTER_LIB_UI_PAINTING_IMAGE_H_

#include "flutter/flow/skia_gpu_object.h"
#include "flutter/lib/ui/dart_wrapper.h"
#include "flutter/lib/ui/ui_dart_state.h"
#include "third_party/skia/include/core/SkImage.h"
#include "flutter/lib/ui/painting/image_encoding.h"

namespace blink {

class CanvasImage final : public RefCountedDartWrappable<CanvasImage> {
  //DEFINE_WRAPPERTYPEINFO();
  FML_FRIEND_MAKE_REF_COUNTED(CanvasImage);

 public:
  ~CanvasImage();
  static fml::RefPtr<CanvasImage> Create() {
    return fml::MakeRefCounted<CanvasImage>();
  }

  int width() { return image_.get()->width(); }

  int height() { return image_.get()->height(); }

  char* toByteData(ImageByteFormat format, void (*callback)(sk_sp<SkData>));

  void dispose();

  sk_sp<SkImage> image() const { return image_.get(); }
  void set_image(flow::SkiaGPUObject<SkImage> image) {
    image_ = std::move(image);
  }

  virtual size_t GetAllocationSize();

 private:
  CanvasImage();

  flow::SkiaGPUObject<SkImage> image_;
};

}  // namespace blink

#endif  // FLUTTER_LIB_UI_PAINTING_IMAGE_H_
