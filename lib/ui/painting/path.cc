// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "flutter/lib/ui/painting/path.h"

#define _USE_MATH_DEFINES
#include <math.h>

namespace blink {

typedef CanvasPath Path;
/*
static void Path_constructor(Dart_NativeArguments args) {
  DartCallConstructor(&CanvasPath::Create, args);
}

IMPLEMENT_WRAPPERTYPEINFO(ui, Path);

#define FOR_EACH_BINDING(V)          \
  V(Path, addArc)                    \
  V(Path, addOval)                   \
  V(Path, addPath)                   \
  V(Path, addPolygon)                \
  V(Path, addRect)                   \
  V(Path, addRRect)                  \
  V(Path, arcTo)                     \
  V(Path, arcToPoint)                \
  V(Path, close)                     \
  V(Path, conicTo)                   \
  V(Path, contains)                  \
  V(Path, cubicTo)                   \
  V(Path, extendWithPath)            \
  V(Path, extendWithPathAndMatrix)   \
  V(Path, getFillType)               \
  V(Path, lineTo)                    \
  V(Path, moveTo)                    \
  V(Path, quadraticBezierTo)         \
  V(Path, relativeArcToPoint)        \
  V(Path, relativeConicTo)           \
  V(Path, relativeCubicTo)           \
  V(Path, relativeLineTo)            \
  V(Path, relativeMoveTo)            \
  V(Path, relativeQuadraticBezierTo) \
  V(Path, reset)                     \
  V(Path, setFillType)               \
  V(Path, shift)                     \
  V(Path, transform)                 \
  V(Path, getBounds)                 \
  V(Path, addPathWithMatrix)         \
  V(Path, op)                        \
  V(Path, clone)

FOR_EACH_BINDING(DART_NATIVE_CALLBACK)

void CanvasPath::RegisterNatives(tonic::DartLibraryNatives* natives) {
  natives->Register({{"Path_constructor", Path_constructor, 1, true},
                     FOR_EACH_BINDING(DART_REGISTER_NATIVE)});
}*/

CanvasPath::CanvasPath() {}

CanvasPath::~CanvasPath() {}

int CanvasPath::getFillType() {
  return path_.getFillType();
}

void CanvasPath::setFillType(int fill_type) {
  path_.setFillType(static_cast<SkPath::FillType>(fill_type));
}

void CanvasPath::moveTo(float x, float y) {
  path_.moveTo(x, y);
}

void CanvasPath::relativeMoveTo(float x, float y) {
  path_.rMoveTo(x, y);
}

void CanvasPath::lineTo(float x, float y) {
  path_.lineTo(x, y);
}

void CanvasPath::relativeLineTo(float x, float y) {
  path_.rLineTo(x, y);
}

void CanvasPath::quadraticBezierTo(float x1, float y1, float x2, float y2) {
  path_.quadTo(x1, y1, x2, y2);
}

void CanvasPath::relativeQuadraticBezierTo(float x1,
                                           float y1,
                                           float x2,
                                           float y2) {
  path_.rQuadTo(x1, y1, x2, y2);
}

void CanvasPath::cubicTo(float x1,
                         float y1,
                         float x2,
                         float y2,
                         float x3,
                         float y3) {
  path_.cubicTo(x1, y1, x2, y2, x3, y3);
}

void CanvasPath::relativeCubicTo(float x1,
                                 float y1,
                                 float x2,
                                 float y2,
                                 float x3,
                                 float y3) {
  path_.rCubicTo(x1, y1, x2, y2, x3, y3);
}

void CanvasPath::conicTo(float x1, float y1, float x2, float y2, float w) {
  path_.conicTo(x1, y1, x2, y2, w);
}

void CanvasPath::relativeConicTo(float x1,
                                 float y1,
                                 float x2,
                                 float y2,
                                 float w) {
  path_.rConicTo(x1, y1, x2, y2, w);
}

void CanvasPath::arcTo(float left,
                       float top,
                       float right,
                       float bottom,
                       float startAngle,
                       float sweepAngle,
                       bool forceMoveTo) {
  path_.arcTo(SkRect::MakeLTRB(left, top, right, bottom),
              startAngle * 180.0 / M_PI, sweepAngle * 180.0 / M_PI,
              forceMoveTo);
}

void CanvasPath::arcToPoint(float arcEndX,
                            float arcEndY,
                            float radiusX,
                            float radiusY,
                            float xAxisRotation,
                            bool isLargeArc,
                            bool isClockwiseDirection) {
  const auto arcSize = isLargeArc ? SkPath::ArcSize::kLarge_ArcSize
                                  : SkPath::ArcSize::kSmall_ArcSize;
  const auto direction = isClockwiseDirection
                             ? SkPath::Direction::kCW_Direction
                             : SkPath::Direction::kCCW_Direction;

  path_.arcTo(radiusX, radiusY, xAxisRotation, arcSize, direction, arcEndX,
              arcEndY);
}

void CanvasPath::relativeArcToPoint(float arcEndDeltaX,
                                    float arcEndDeltaY,
                                    float radiusX,
                                    float radiusY,
                                    float xAxisRotation,
                                    bool isLargeArc,
                                    bool isClockwiseDirection) {
  const auto arcSize = isLargeArc ? SkPath::ArcSize::kLarge_ArcSize
                                  : SkPath::ArcSize::kSmall_ArcSize;
  const auto direction = isClockwiseDirection
                             ? SkPath::Direction::kCW_Direction
                             : SkPath::Direction::kCCW_Direction;
  path_.rArcTo(radiusX, radiusY, xAxisRotation, arcSize, direction,
               arcEndDeltaX, arcEndDeltaY);
}

void CanvasPath::addRect(float left, float top, float right, float bottom) {
  path_.addRect(SkRect::MakeLTRB(left, top, right, bottom));
}

void CanvasPath::addOval(float left, float top, float right, float bottom) {
  path_.addOval(SkRect::MakeLTRB(left, top, right, bottom));
}

void CanvasPath::addArc(float left,
                        float top,
                        float right,
                        float bottom,
                        float startAngle,
                        float sweepAngle) {
  path_.addArc(SkRect::MakeLTRB(left, top, right, bottom),
               startAngle * 180.0 / M_PI, sweepAngle * 180.0 / M_PI);
}

void CanvasPath::addPolygon(const SkPoint* points, int count, bool close) {
  path_.addPoly(points, count, close);
}

void CanvasPath::addRRect(const SkRRect& rrect) {
  path_.addRRect(rrect);
}

char* CanvasPath::addPath(CanvasPath* path, double dx, double dy) {
  if (!path)
    return "Path.addPath called with non-genuine Path.";
  path_.addPath(path->path(), dx, dy, SkPath::kAppend_AddPathMode);
  return nullptr;
}

char* CanvasPath::addPathWithMatrix(CanvasPath* path,
                                   double dx,
                                   double dy,
                                   const SkMatrix& matrix) {
  if (!path) {
    return "Path.addPathWithMatrix called with non-genuine Path.";
  }

  SkMatrix matrixCopy = SkMatrix(matrix);
  matrixCopy.setTranslateX(matrixCopy.getTranslateX() + dx);
  matrixCopy.setTranslateY(matrixCopy.getTranslateY() + dy);
  path_.addPath(path->path(), matrixCopy, SkPath::kAppend_AddPathMode);
  return nullptr;
}

char* CanvasPath::extendWithPath(CanvasPath* path, double dx, double dy) {
  if (!path)
    return "Path.extendWithPath called with non-genuine Path.";
  path_.addPath(path->path(), dx, dy, SkPath::kExtend_AddPathMode);
  return nullptr;
}

char* CanvasPath::extendWithPathAndMatrix(CanvasPath* path,
                                          double dx,
                                          double dy,
                                          const SkMatrix& matrix) {
  if (!path) {
    return "Path.addPathWithMatrix called with non-genuine Path.";
  }

  SkMatrix matrixCopy = SkMatrix(matrix);
  matrixCopy.setTranslateX(matrixCopy.getTranslateX() + dx);
  matrixCopy.setTranslateY(matrixCopy.getTranslateY() + dy);
  path_.addPath(path->path(), matrixCopy, SkPath::kAppend_AddPathMode);
  return nullptr;
}

void CanvasPath::close() {
  path_.close();
}

void CanvasPath::reset() {
  path_.reset();
}

bool CanvasPath::contains(double x, double y) {
  return path_.contains(x, y);
}

fml::RefPtr<CanvasPath> CanvasPath::shift(double dx, double dy) {
  fml::RefPtr<CanvasPath> path = CanvasPath::Create();
  path_.offset(dx, dy, &path->path_);
  return path;
}

fml::RefPtr<CanvasPath> CanvasPath::transform(const SkMatrix& matrix) {
  fml::RefPtr<CanvasPath> path = CanvasPath::Create();
  path_.transform(matrix, &path->path_);
  return path;
}

SkRect CanvasPath::getBounds() {
  const SkRect& bounds = path_.getBounds();
  return bounds;
}

bool CanvasPath::op(CanvasPath* path1, CanvasPath* path2, int operation) {
  return Op(path1->path(), path2->path(), (SkPathOp)operation, &path_);
}

fml::RefPtr<CanvasPath> CanvasPath::clone() {
  fml::RefPtr<CanvasPath> path = CanvasPath::Create();
  // per Skia docs, this will create a fast copy
  // data is shared until the source path or dest path are mutated
  path->path_ = path_;
  return path;
}

}  // namespace blink
