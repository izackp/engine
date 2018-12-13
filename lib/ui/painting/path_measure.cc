// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "flutter/lib/ui/painting/path_measure.h"

#define _USE_MATH_DEFINES
#include <math.h>

namespace blink {

typedef CanvasPathMeasure PathMeasure;
/*
static void PathMeasure_constructor(Dart_NativeArguments args) {
  DartCallConstructor(&CanvasPathMeasure::Create, args);
}

IMPLEMENT_WRAPPERTYPEINFO(ui, PathMeasure);

#define FOR_EACH_BINDING(V)  \
  V(PathMeasure, setPath)    \
  V(PathMeasure, getLength)  \
  V(PathMeasure, getPosTan)  \
  V(PathMeasure, getSegment) \
  V(PathMeasure, isClosed)   \
  V(PathMeasure, nextContour)

FOR_EACH_BINDING(DART_NATIVE_CALLBACK)

void CanvasPathMeasure::RegisterNatives(tonic::DartLibraryNatives* natives) {
  natives->Register(
      {{"PathMeasure_constructor", PathMeasure_constructor, 3, true},
       FOR_EACH_BINDING(DART_REGISTER_NATIVE)});
}*/

fml::RefPtr<CanvasPathMeasure> CanvasPathMeasure::Create(const CanvasPath* path,
                                                         bool forceClosed) {
  fml::RefPtr<CanvasPathMeasure> pathMeasure =
      fml::MakeRefCounted<CanvasPathMeasure>();
  if (path) {
    const SkPath skPath = path->path();
    SkScalar resScale = 1;
    pathMeasure->path_measure_ =
        std::make_unique<SkPathMeasure>(skPath, forceClosed, resScale);
  } else {
    pathMeasure->path_measure_ = std::make_unique<SkPathMeasure>();
  }
  return pathMeasure;
}

CanvasPathMeasure::CanvasPathMeasure() {}

CanvasPathMeasure::~CanvasPathMeasure() {}

void CanvasPathMeasure::setPath(const CanvasPath* path, bool isClosed) {
  const SkPath* skPath = &(path->path());
  path_measure_->setPath(skPath, isClosed);
}

float CanvasPathMeasure::getLength() {
  return path_measure_->getLength();
}

bool CanvasPathMeasure::getPosTan(float distance,
                                  SkPoint* position,
                                  SkVector* tangent) {
  bool success = path_measure_->getPosTan(distance, position, tangent);
  return success;
}

fml::RefPtr<CanvasPath> CanvasPathMeasure::getSegment(float startD,
                                                      float stopD,
                                                      bool startWithMoveTo) {
  SkPath dst;
  bool success =
      path_measure_->getSegment(startD, stopD, &dst, startWithMoveTo);
  if (!success) {
    return CanvasPath::Create();
  } else {
    return CanvasPath::CreateFrom(dst);
  }
}

bool CanvasPathMeasure::isClosed() {
  return path_measure_->isClosed();
}

bool CanvasPathMeasure::nextContour() {
  return path_measure_->nextContour();
}

}  // namespace blink
