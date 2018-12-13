// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef FLUTTER_LIB_UI_PAINTING_CANVAS_H_
#define FLUTTER_LIB_UI_PAINTING_CANVAS_H_

#include "flutter/lib/ui/dart_wrapper.h"
#include "flutter/lib/ui/painting/path.h"
#include "flutter/lib/ui/painting/picture.h"
#include "flutter/lib/ui/painting/picture_recorder.h"
#include "third_party/skia/include/core/SkCanvas.h"
#include "third_party/skia/include/utils/SkShadowUtils.h"

namespace blink {
class CanvasImage;

class Canvas : public RefCountedDartWrappable<Canvas> {
  //DEFINE_WRAPPERTYPEINFO();
  FML_FRIEND_MAKE_REF_COUNTED(Canvas);

 public:
  static fml::RefPtr<Canvas> Create(PictureRecorder* recorder,
                                    double left,
                                    double top,
                                    double right,
                                    double bottom,
                                    char **error);

  ~Canvas();

  void save();
  void saveLayerWithoutBounds(const SkPaint& paint);
  void saveLayer(double left,
                 double top,
                 double right,
                 double bottom,
                 const SkPaint& paint);
  void restore();
  int getSaveCount();

  void translate(double dx, double dy);
  void scale(double sx, double sy);
  void rotate(double radians);
  void skew(double sx, double sy);
  void transform(const SkMatrix& matrix4);

  void clipRect(double left,
                double top,
                double right,
                double bottom,
                SkClipOp clipOp,
                bool doAntiAlias = true);
  void clipRRect(const SkRRect& rrect, bool doAntiAlias = true);
  char* clipPath(const CanvasPath* path, bool doAntiAlias = true);

  void drawColor(SkColor color, SkBlendMode blend_mode);
  void drawLine(double x1,
                double y1,
                double x2,
                double y2,
                const SkPaint& paint);
  void drawPaint(const SkPaint& paint);
  void drawRect(double left,
                double top,
                double right,
                double bottom,
                const SkPaint& paint);
  void drawRRect(const SkRRect& rrect,
                 const SkPaint& paint);
  void drawDRRect(const SkRRect& outer,
                  const SkRRect& inner,
                  const SkPaint& paint);
  void drawOval(double left,
                double top,
                double right,
                double bottom,
                const SkPaint& paint);
  void drawCircle(double x,
                  double y,
                  double radius,
                  const SkPaint& paint);
  void drawArc(double left,
               double top,
               double right,
               double bottom,
               double startAngle,
               double sweepAngle,
               bool useCenter,
               const SkPaint& paint);
  char* drawPath(const CanvasPath* path,
                 const SkPaint& paint);
  char* drawImage(const CanvasImage* image,
                  double x,
                  double y,
                  const SkPaint& paint);
  char* drawImageRect(const CanvasImage* image,
                      double src_left,
                      double src_top,
                      double src_right,
                      double src_bottom,
                      double dst_left,
                      double dst_top,
                      double dst_right,
                      double dst_bottom,
                      const SkPaint& paint);
  char* drawImageNine(const CanvasImage* image,
                      double center_left,
                      double center_top,
                      double center_right,
                      double center_bottom,
                      double dst_left,
                      double dst_top,
                      double dst_right,
                      double dst_bottom,
                      const SkPaint& paint);
  char* drawPicture(Picture* picture);

  // The paint argument is first for the following functions because Paint
  // unwraps a number of C++ objects. Once we create a view unto a
  // Float32List, we cannot re-enter the VM to unwrap objects. That means we
  // either need to process the paint argument first.

  void drawPoints(const SkPaint& paint,
                  SkCanvas::PointMode point_mode,
                  size_t count,
                  const SkPoint pts[]);

  char* drawVertices(const SkVertices* vertices,
                     SkBlendMode blend_mode,
                     const SkPaint& paint);

  char* drawAtlas(const SkPaint& paint,
                  CanvasImage* atlas,
                  const SkRSXform transforms[],
                  const SkRect rects[],
                  const SkColor colors[],
                  int count,
                  SkBlendMode blend_mode,
                  const SkRect* cull_rect);

  char* drawShadow(const CanvasPath* path,
                   SkColor color,
                   double elevation,
                   bool transparentOccluder);

  SkCanvas* canvas() const { return canvas_; }
  void Clear();
  bool IsRecording() const;

 private:
  explicit Canvas(SkCanvas* canvas);

  // The SkCanvas is supplied by a call to SkPictureRecorder::beginRecording,
  // which does not transfer ownership.  For this reason, we hold a raw
  // pointer and manually set to null in Clear.
  SkCanvas* canvas_;
};

}  // namespace blink

#endif  // FLUTTER_LIB_UI_PAINTING_CANVAS_H_
