// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "flutter/lib/ui/painting/canvas.h"

#define _USE_MATH_DEFINES
#include <math.h>

#include "flutter/flow/layers/physical_shape_layer.h"
#include "flutter/lib/ui/painting/image.h"
#include "flutter/lib/ui/ui_dart_state.h"
#include "flutter/lib/ui/window/window.h"
#include "third_party/skia/include/core/SkBitmap.h"
#include "third_party/skia/include/core/SkCanvas.h"
#include "third_party/skia/include/core/SkRSXform.h"

namespace blink {
/*
static void Canvas_constructor(Dart_NativeArguments args) {
  DartCallConstructor(&Canvas::Create, args);
}

IMPLEMENT_WRAPPERTYPEINFO(ui, Canvas);

#define FOR_EACH_BINDING(V)         \
  V(Canvas, save)                   \
  V(Canvas, saveLayerWithoutBounds) \
  V(Canvas, saveLayer)              \
  V(Canvas, restore)                \
  V(Canvas, getSaveCount)           \
  V(Canvas, translate)              \
  V(Canvas, scale)                  \
  V(Canvas, rotate)                 \
  V(Canvas, skew)                   \
  V(Canvas, transform)              \
  V(Canvas, clipRect)               \
  V(Canvas, clipRRect)              \
  V(Canvas, clipPath)               \
  V(Canvas, drawColor)              \
  V(Canvas, drawLine)               \
  V(Canvas, drawPaint)              \
  V(Canvas, drawRect)               \
  V(Canvas, drawRRect)              \
  V(Canvas, drawDRRect)             \
  V(Canvas, drawOval)               \
  V(Canvas, drawCircle)             \
  V(Canvas, drawArc)                \
  V(Canvas, drawPath)               \
  V(Canvas, drawImage)              \
  V(Canvas, drawImageRect)          \
  V(Canvas, drawImageNine)          \
  V(Canvas, drawPicture)            \
  V(Canvas, drawPoints)             \
  V(Canvas, drawVertices)           \
  V(Canvas, drawAtlas)              \
  V(Canvas, drawShadow)

FOR_EACH_BINDING(DART_NATIVE_CALLBACK)

void Canvas::RegisterNatives(tonic::DartLibraryNatives* natives) {
  natives->Register({{"Canvas_constructor", Canvas_constructor, 6, true},
                     FOR_EACH_BINDING(DART_REGISTER_NATIVE)});
}
*/
fml::RefPtr<Canvas> Canvas::Create(PictureRecorder* recorder,
                                   double left,
                                   double top,
                                   double right,
                                   double bottom,
                                   char **error) {
  if (!recorder) {
    *error = "Canvas constructor called with non-genuine PictureRecorder.";
    return nullptr;
  }
  FML_DCHECK(!recorder->isRecording());  // verified by Dart code
  fml::RefPtr<Canvas> canvas = fml::MakeRefCounted<Canvas>(
      recorder->BeginRecording(SkRect::MakeLTRB(left, top, right, bottom)));
  recorder->set_canvas(canvas);
  return canvas;
}

Canvas::Canvas(SkCanvas* canvas) : canvas_(canvas) {}

Canvas::~Canvas() {}

void Canvas::save() {
  if (!canvas_)
    return;
  canvas_->save();
}

void Canvas::saveLayerWithoutBounds(const SkPaint& paint) {
  if (!canvas_)
    return;
  canvas_->saveLayer(nullptr, &paint);
}

void Canvas::saveLayer(double left,
                       double top,
                       double right,
                       double bottom,
                       const SkPaint& paint) {
  if (!canvas_)
    return;
  SkRect bounds = SkRect::MakeLTRB(left, top, right, bottom);
  canvas_->saveLayer(&bounds, &paint);
}

void Canvas::restore() {
  if (!canvas_)
    return;
  canvas_->restore();
}

int Canvas::getSaveCount() {
  if (!canvas_)
    return 0;
  return canvas_->getSaveCount();
}

void Canvas::translate(double dx, double dy) {
  if (!canvas_)
    return;
  canvas_->translate(dx, dy);
}

void Canvas::scale(double sx, double sy) {
  if (!canvas_)
    return;
  canvas_->scale(sx, sy);
}

void Canvas::rotate(double radians) {
  if (!canvas_)
    return;
  canvas_->rotate(radians * 180.0 / M_PI);
}

void Canvas::skew(double sx, double sy) {
  if (!canvas_)
    return;
  canvas_->skew(sx, sy);
}

void Canvas::transform(const SkMatrix& matrix4) {
  if (!canvas_)
    return;
  canvas_->concat(matrix4);
}

void Canvas::clipRect(double left,
                      double top,
                      double right,
                      double bottom,
                      SkClipOp clipOp,
                      bool doAntiAlias) {
  if (!canvas_)
    return;
  canvas_->clipRect(SkRect::MakeLTRB(left, top, right, bottom), clipOp,
                    doAntiAlias);
}

void Canvas::clipRRect(const SkRRect& rrect, bool doAntiAlias) {
  if (!canvas_)
    return;
  canvas_->clipRRect(rrect, doAntiAlias);
}

char* Canvas::clipPath(const CanvasPath* path, bool doAntiAlias) {
  if (!canvas_)
    return nullptr;
  if (!path)
    return "Canvas.clipPath called with non-genuine Path.";
  canvas_->clipPath(path->path(), doAntiAlias);
  return nullptr;
}

void Canvas::drawColor(SkColor color, SkBlendMode blend_mode) {
  if (!canvas_)
    return;
  canvas_->drawColor(color, blend_mode);
}

void Canvas::drawLine(double x1,
                      double y1,
                      double x2,
                      double y2,
                      const SkPaint& paint) {
  if (!canvas_)
    return;
  canvas_->drawLine(x1, y1, x2, y2, paint);
}

void Canvas::drawPaint(const SkPaint& paint) {
  if (!canvas_)
    return;
  canvas_->drawPaint(paint);
}

void Canvas::drawRect(double left,
                      double top,
                      double right,
                      double bottom,
                      const SkPaint& paint) {
  if (!canvas_)
    return;
  canvas_->drawRect(SkRect::MakeLTRB(left, top, right, bottom), paint);
}

void Canvas::drawRRect(const SkRRect& rrect,
                       const SkPaint& paint) {
  if (!canvas_)
    return;
  canvas_->drawRRect(rrect, paint);
}

void Canvas::drawDRRect(const SkRRect& outer,
                        const SkRRect& inner,
                        const SkPaint& paint) {
  if (!canvas_)
    return;
  canvas_->drawDRRect(outer, inner, paint);
}

void Canvas::drawOval(double left,
                      double top,
                      double right,
                      double bottom,
                      const SkPaint& paint) {
  if (!canvas_)
    return;
  canvas_->drawOval(SkRect::MakeLTRB(left, top, right, bottom), paint);
}

void Canvas::drawCircle(double x,
                        double y,
                        double radius,
                        const SkPaint& paint) {
  if (!canvas_)
    return;
  canvas_->drawCircle(x, y, radius, paint);
}

void Canvas::drawArc(double left,
                     double top,
                     double right,
                     double bottom,
                     double startAngle,
                     double sweepAngle,
                     bool useCenter,
                     const SkPaint& paint) {
  if (!canvas_)
    return;
  canvas_->drawArc(SkRect::MakeLTRB(left, top, right, bottom),
                   startAngle * 180.0 / M_PI, sweepAngle * 180.0 / M_PI,
                   useCenter, paint);
}

char* Canvas::drawPath(const CanvasPath* path,
                      const SkPaint& paint) {
  if (!canvas_)
    return nullptr;
  if (!path)
    return "Canvas.drawPath called with non-genuine Path.";
  canvas_->drawPath(path->path(), paint);
  return nullptr;
}

char* Canvas::drawImage(const CanvasImage* image,
                       double x,
                       double y,
                       const SkPaint& paint) {
  if (!canvas_)
    return nullptr;
  if (!image)
    return "Canvas.drawImage called with non-genuine Image.";
  canvas_->drawImage(image->image(), x, y, &paint);
  return nullptr;
}

char* Canvas::drawImageRect(const CanvasImage* image,
                           double src_left,
                           double src_top,
                           double src_right,
                           double src_bottom,
                           double dst_left,
                           double dst_top,
                           double dst_right,
                           double dst_bottom,
                           const SkPaint& paint) {
  if (!canvas_)
    return nullptr;
  if (!image)
    return "Canvas.drawImageRect called with non-genuine Image.";
  SkRect src = SkRect::MakeLTRB(src_left, src_top, src_right, src_bottom);
  SkRect dst = SkRect::MakeLTRB(dst_left, dst_top, dst_right, dst_bottom);
  canvas_->drawImageRect(image->image(), src, dst, &paint,
                         SkCanvas::kFast_SrcRectConstraint);
  return nullptr;
}

char* Canvas::drawImageNine(const CanvasImage* image,
                           double center_left,
                           double center_top,
                           double center_right,
                           double center_bottom,
                           double dst_left,
                           double dst_top,
                           double dst_right,
                           double dst_bottom,
                           const SkPaint& paint) {
  if (!canvas_)
    return nullptr;
  if (!image)
    return "Canvas.drawImageNine called with non-genuine Image.";
  SkRect center =
      SkRect::MakeLTRB(center_left, center_top, center_right, center_bottom);
  SkIRect icenter;
  center.round(&icenter);
  SkRect dst = SkRect::MakeLTRB(dst_left, dst_top, dst_right, dst_bottom);
  canvas_->drawImageNine(image->image(), icenter, dst, &paint);
  return nullptr;
}

char* Canvas::drawPicture(Picture* picture) {
  if (!canvas_)
    return nullptr;
  if (!picture)
    return "Canvas.drawPicture called with non-genuine Picture.";
  canvas_->drawPicture(picture->picture().get());
  return nullptr;
}

void Canvas::drawPoints(const SkPaint& paint,
                        SkCanvas::PointMode point_mode,
                        size_t count,
                        const SkPoint pts[]) {
  if (!canvas_)
    return;

  static_assert(sizeof(SkPoint) == sizeof(float) * 2,
                "SkPoint doesn't use floats.");

  canvas_->drawPoints(point_mode,
                      count,  // SkPoints have two floats.
                      pts,
                      paint);
}

char* Canvas::drawVertices(const SkVertices* vertices,
                          SkBlendMode blend_mode,
                          const SkPaint& paint) {
  if (!canvas_)
    return nullptr;
  if (!vertices)
    return "Canvas.drawVertices called with non-genuine Vertices.";

  canvas_->drawVertices(vertices, blend_mode, paint);
  return nullptr;
}

char* Canvas::drawAtlas(const SkPaint& paint,
                       CanvasImage* atlas,
                       const SkRSXform transforms[],
                       const SkRect rects[],
                       const SkColor colors[],
                       int count,
                       SkBlendMode blend_mode,
                       const SkRect* cull_rect) {
  if (!canvas_)
    return nullptr;
  if (!atlas)
    return "Canvas.drawAtlas or Canvas.drawRawAtlas called with "
               "non-genuine Image.";

  sk_sp<SkImage> skImage = atlas->image();

  static_assert(sizeof(SkRSXform) == sizeof(float) * 4,
                "SkRSXform doesn't use floats.");
  static_assert(sizeof(SkRect) == sizeof(float) * 4,
                "SkRect doesn't use floats.");

  canvas_->drawAtlas(
      skImage.get(), transforms,
      rects,
      colors,
      count,  // SkRect have four floats.
      blend_mode, cull_rect,
      &paint);
  return nullptr;
}

char* Canvas::drawShadow(const CanvasPath* path,
                        SkColor color,
                        double elevation,
                        bool transparentOccluder) {
  if (!path)
    return "Canvas.drawShader called with non-genuine Path.";
  SkScalar dpr =
      UIDartState::Current()->window()->viewport_metrics().device_pixel_ratio;
  flow::PhysicalShapeLayer::DrawShadow(canvas_, path->path(), color, elevation,
                                       transparentOccluder, dpr);
  return nullptr;
}

void Canvas::Clear() {
  canvas_ = nullptr;
}

bool Canvas::IsRecording() const {
  return !!canvas_;
}

}  // namespace blink
