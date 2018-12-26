// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef FLUTTER_LIB_UI_PAINTING_IMAGE_FILTER_H_
#define FLUTTER_LIB_UI_PAINTING_IMAGE_FILTER_H_

#include "flutter/lib/ui/painting/image.h"
#include "flutter/lib/ui/painting/picture.h"
#include "flutter/fml/memory/ref_counted.h"
#include "third_party/skia/include/core/SkImageFilter.h"

namespace blink {

class ImageFilter : public fml::RefCountedThreadSafe<ImageFilter> {
  FML_FRIEND_MAKE_REF_COUNTED(ImageFilter);

 public:
  ~ImageFilter();
  static fml::RefPtr<ImageFilter> Create();

  void initImage(CanvasImage* image);
  void initPicture(Picture*);
  void initBlur(double sigma_x, double sigma_y);
  void initMatrix(const SkMatrix& matrix4, int filter_quality);

  const sk_sp<SkImageFilter>& filter() { return filter_; }

 private:
  ImageFilter();

  sk_sp<SkImageFilter> filter_;
};

}  // namespace blink

#endif  // FLUTTER_LIB_UI_PAINTING_IMAGE_FILTER_H_
