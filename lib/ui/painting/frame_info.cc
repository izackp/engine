
// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "flutter/lib/ui/painting/frame_info.h"

namespace blink {
FrameInfo::FrameInfo(fml::RefPtr<CanvasImage> image, int durationMillis)
    : image_(std::move(image)), durationMillis_(durationMillis) {}

FrameInfo::~FrameInfo(){};
}  // namespace blink
