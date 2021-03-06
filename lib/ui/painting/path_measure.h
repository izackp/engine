// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef FLUTTER_LIB_UI_PAINTING_PATH_MEASURE_H_
#define FLUTTER_LIB_UI_PAINTING_PATH_MEASURE_H_

#include "flutter/lib/ui/painting/path.h"
#include "flutter/fml/memory/ref_counted.h"
#include "third_party/skia/include/core/SkPath.h"
#include "third_party/skia/include/core/SkPathMeasure.h"

// Be sure that the client doesn't modify a path on us before Skia finishes
// See AOSP's reasoning in PathMeasure.cpp

namespace blink {

class CanvasPathMeasure : public fml::RefCountedThreadSafe<CanvasPathMeasure> {

  FML_FRIEND_MAKE_REF_COUNTED(CanvasPathMeasure);

 public:
  ~CanvasPathMeasure();
  static fml::RefPtr<CanvasPathMeasure> Create(const CanvasPath* path,
                                               bool forceClosed);

  void setPath(const CanvasPath* path, bool isClosed);
  float getLength();
  bool getPosTan(float distance, SkPoint* position, SkVector* tangent);
  fml::RefPtr<CanvasPath> getSegment(float startD,
                                     float stopD,
                                     bool startWithMoveTo);
  bool isClosed();
  bool nextContour();

  const SkPathMeasure& pathMeasure() const { return *path_measure_; }

 private:
  CanvasPathMeasure();

  std::unique_ptr<SkPathMeasure> path_measure_;
};

}  // namespace blink

#endif  // FLUTTER_LIB_UI_PAINTING_PATH_MEASURE_H_
