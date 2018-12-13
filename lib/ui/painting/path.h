// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef FLUTTER_LIB_UI_PAINTING_PATH_H_
#define FLUTTER_LIB_UI_PAINTING_PATH_H_

#include "flutter/lib/ui/dart_wrapper.h"
#include "third_party/skia/include/core/SkPath.h"
#include "third_party/skia/include/pathops/SkPathOps.h"

namespace blink {

class CanvasPath : public RefCountedDartWrappable<CanvasPath> {

  FML_FRIEND_MAKE_REF_COUNTED(CanvasPath);

 public:
  ~CanvasPath();
  static fml::RefPtr<CanvasPath> Create() {
    return fml::MakeRefCounted<CanvasPath>();
  }

  static fml::RefPtr<CanvasPath> CreateFrom(const SkPath& src) {
    fml::RefPtr<CanvasPath> path = CanvasPath::Create();
    path->path_ = src;
    return path;
  }

  int getFillType();
  void setFillType(int fill_type);

  void moveTo(float x, float y);
  void relativeMoveTo(float x, float y);
  void lineTo(float x, float y);
  void relativeLineTo(float x, float y);
  void quadraticBezierTo(float x1, float y1, float x2, float y2);
  void relativeQuadraticBezierTo(float x1, float y1, float x2, float y2);
  void cubicTo(float x1, float y1, float x2, float y2, float x3, float y3);
  void relativeCubicTo(float x1,
                       float y1,
                       float x2,
                       float y2,
                       float x3,
                       float y3);
  void conicTo(float x1, float y1, float x2, float y2, float w);
  void relativeConicTo(float x1, float y1, float x2, float y2, float w);
  void arcTo(float left,
             float top,
             float right,
             float bottom,
             float startAngle,
             float sweepAngle,
             bool forceMoveTo);
  void arcToPoint(float arcEndX,
                  float arcEndY,
                  float radiusX,
                  float radiusY,
                  float xAxisRotation,
                  bool isLargeArc,
                  bool isClockwiseDirection);
  void relativeArcToPoint(float arcEndDeltaX,
                          float arcEndDeltaY,
                          float radiusX,
                          float radiusY,
                          float xAxisRotation,
                          bool isLargeArc,
                          bool isClockwiseDirection);
  void addRect(float left, float top, float right, float bottom);
  void addOval(float left, float top, float right, float bottom);
  void addArc(float left,
              float top,
              float right,
              float bottom,
              float startAngle,
              float sweepAngle);
  void addPolygon(const SkPoint* points, int count, bool close);
  void addRRect(const SkRRect& rrect);
  char* addPath(CanvasPath* path, double dx, double dy);
  char* addPathWithMatrix(CanvasPath* path,
                         double dx,
                         double dy,
                         const SkMatrix& matrix);
  char* extendWithPath(CanvasPath* path, double dx, double dy);
  char* extendWithPathAndMatrix(CanvasPath* path,
                               double dx,
                               double dy,
                               const SkMatrix& matrix);
  void close();
  void reset();
  bool contains(double x, double y);
  fml::RefPtr<CanvasPath> shift(double dx, double dy);
  fml::RefPtr<CanvasPath> transform(const SkMatrix& matrix);
  SkRect getBounds();
  bool op(CanvasPath* path1, CanvasPath* path2, int operation);
  fml::RefPtr<CanvasPath> clone();

  const SkPath& path() const { return path_; }

 private:
  CanvasPath();

  SkPath path_;
};

}  // namespace blink

#endif  // FLUTTER_LIB_UI_PAINTING_PATH_H_
