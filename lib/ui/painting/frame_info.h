// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef FLUTTER_LIB_UI_PAINTING_FRAME_INFO_H_
#define FLUTTER_LIB_UI_PAINTING_FRAME_INFO_H_

#include "flutter/lib/ui/painting/image.h"
#include "flutter/fml/memory/ref_counted.h"

namespace blink {

// A single animation frame.
class FrameInfo final : public fml::RefCountedThreadSafe<FrameInfo> {

 public:
  int durationMillis() { return durationMillis_; }
  fml::RefPtr<CanvasImage> image() { return image_; }

 private:
  FrameInfo(fml::RefPtr<CanvasImage> image, int durationMillis);

  ~FrameInfo();

  const fml::RefPtr<CanvasImage> image_;
  const int durationMillis_;

  FML_FRIEND_MAKE_REF_COUNTED(FrameInfo);
  FML_FRIEND_REF_COUNTED_THREAD_SAFE(FrameInfo);
};

}  // namespace blink

#endif  // FLUTTER_LIB_UI_PAINTING_FRAME_INFO_H_
