// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "flutter/lib/ui/compositing/scene_builder.h"

#include "flutter/flow/layers/backdrop_filter_layer.h"
#include "flutter/flow/layers/clip_path_layer.h"
#include "flutter/flow/layers/clip_rect_layer.h"
#include "flutter/flow/layers/clip_rrect_layer.h"
#include "flutter/flow/layers/color_filter_layer.h"
#include "flutter/flow/layers/container_layer.h"
#include "flutter/flow/layers/layer.h"
#include "flutter/flow/layers/layer_tree.h"
#include "flutter/flow/layers/opacity_layer.h"
#include "flutter/flow/layers/performance_overlay_layer.h"
#include "flutter/flow/layers/physical_shape_layer.h"
#include "flutter/flow/layers/picture_layer.h"
#include "flutter/flow/layers/shader_mask_layer.h"
#include "flutter/flow/layers/texture_layer.h"
#include "flutter/flow/layers/transform_layer.h"
#include "flutter/fml/build_config.h"
#include "flutter/lib/ui/painting/matrix.h"
#include "flutter/lib/ui/painting/shader.h"
#include "flutter/lib/ui/ui_dart_state.h"
#include "flutter/lib/ui/window/window.h"
#include "third_party/skia/include/core/SkColorFilter.h"
#include "third_party/tonic/converter/dart_converter.h"
#include "third_party/tonic/dart_args.h"
#include "third_party/tonic/dart_binding_macros.h"
#include "third_party/tonic/dart_library_natives.h"

namespace blink {

static void SceneBuilder_constructor(Dart_NativeArguments args) {
  DartCallConstructor(&SceneBuilder::create, args);
}

IMPLEMENT_WRAPPERTYPEINFO(ui, SceneBuilder);

#define FOR_EACH_BINDING(V)                         \
  V(SceneBuilder, pushTransform)                    \
  V(SceneBuilder, pushClipRect)                     \
  V(SceneBuilder, pushClipRRect)                    \
  V(SceneBuilder, pushClipPath)                     \
  V(SceneBuilder, pushOpacity)                      \
  V(SceneBuilder, pushColorFilter)                  \
  V(SceneBuilder, pushBackdropFilter)               \
  V(SceneBuilder, pushShaderMask)                   \
  V(SceneBuilder, pushPhysicalShape)                \
  V(SceneBuilder, pop)                              \
  V(SceneBuilder, addPicture)                       \
  V(SceneBuilder, addTexture)                       \
  V(SceneBuilder, addChildScene)                    \
  V(SceneBuilder, addPerformanceOverlay)            \
  V(SceneBuilder, setRasterizerTracingThreshold)    \
  V(SceneBuilder, setCheckerboardOffscreenLayers)   \
  V(SceneBuilder, setCheckerboardRasterCacheImages) \
  V(SceneBuilder, build)

FOR_EACH_BINDING(DART_NATIVE_CALLBACK)

void SceneBuilder::RegisterNatives(tonic::DartLibraryNatives* natives) {
  natives->Register(
      {{"SceneBuilder_constructor", SceneBuilder_constructor, 1, true},
       FOR_EACH_BINDING(DART_REGISTER_NATIVE)});
}

static const SkRect kGiantRect = SkRect::MakeLTRB(-1E9F, -1E9F, 1E9F, 1E9F);

SceneBuilder::SceneBuilder() {
  cull_rects_.push(kGiantRect);
}

SceneBuilder::~SceneBuilder() = default;

void SceneBuilder::pushTransform(const tonic::Float64List& matrix4) {
  SkMatrix sk_matrix = ToSkMatrix(matrix4);
  SkMatrix inverse_sk_matrix;
  SkRect cullRect;
  // Perspective projections don't produce rectangles that are useful for
  // culling for some reason.
  if (!sk_matrix.hasPerspective() && sk_matrix.invert(&inverse_sk_matrix)) {
    inverse_sk_matrix.mapRect(&cullRect, cull_rects_.top());
  } else {
    cullRect = kGiantRect;
  }
  auto layer = std::make_unique<flow::TransformLayer>();
  layer->set_transform(sk_matrix);
  PushLayer(std::move(layer), cullRect);
}

void SceneBuilder::pushClipRect(double left,
                                double right,
                                double top,
                                double bottom,
                                int clipBehavior) {
  SkRect clipRect = SkRect::MakeLTRB(left, top, right, bottom);
  flow::Clip clip_behavior = static_cast<flow::Clip>(clipBehavior);
  SkRect cullRect;
  if (!cullRect.intersect(clipRect, cull_rects_.top())) {
    cullRect = SkRect::MakeEmpty();
  }
  auto layer = std::make_unique<flow::ClipRectLayer>(clip_behavior);
  layer->set_clip_rect(clipRect);
  PushLayer(std::move(layer), cullRect);
}

void SceneBuilder::pushClipRRect(const RRect& rrect, int clipBehavior) {
  flow::Clip clip_behavior = static_cast<flow::Clip>(clipBehavior);
  SkRect cullRect;
  if (!cullRect.intersect(rrect.sk_rrect.rect(), cull_rects_.top())) {
    cullRect = SkRect::MakeEmpty();
  }
  auto layer = std::make_unique<flow::ClipRRectLayer>(clip_behavior);
  layer->set_clip_rrect(rrect.sk_rrect);
  PushLayer(std::move(layer), cullRect);
}

void SceneBuilder::pushClipPath(const CanvasPath* path, int clipBehavior) {
  flow::Clip clip_behavior = static_cast<flow::Clip>(clipBehavior);
  FML_DCHECK(clip_behavior != flow::Clip::none);
  SkRect cullRect;
  if (!cullRect.intersect(path->path().getBounds(), cull_rects_.top())) {
    cullRect = SkRect::MakeEmpty();
  }
  auto layer = std::make_unique<flow::ClipPathLayer>(clip_behavior);
  layer->set_clip_path(path->path());
  PushLayer(std::move(layer), cullRect);
}

void SceneBuilder::pushOpacity(int alpha) {
  auto layer = std::make_unique<flow::OpacityLayer>();
  layer->set_alpha(alpha);
  PushLayer(std::move(layer), cull_rects_.top());
}

void SceneBuilder::pushColorFilter(int color, int blendMode) {
  auto layer = std::make_unique<flow::ColorFilterLayer>();
  layer->set_color(static_cast<SkColor>(color));
  layer->set_blend_mode(static_cast<SkBlendMode>(blendMode));
  PushLayer(std::move(layer), cull_rects_.top());
}

void SceneBuilder::pushBackdropFilter(ImageFilter* filter) {
  auto layer = std::make_unique<flow::BackdropFilterLayer>();
  layer->set_filter(filter->filter());
  PushLayer(std::move(layer), cull_rects_.top());
}

void SceneBuilder::pushShaderMask(Shader* shader,
                                  double maskRectLeft,
                                  double maskRectRight,
                                  double maskRectTop,
                                  double maskRectBottom,
                                  int blendMode) {
  SkRect rect = SkRect::MakeLTRB(maskRectLeft, maskRectTop, maskRectRight,
                                 maskRectBottom);
  auto layer = std::make_unique<flow::ShaderMaskLayer>();
  layer->set_shader(shader->shader());
  layer->set_mask_rect(rect);
  layer->set_blend_mode(static_cast<SkBlendMode>(blendMode));
  PushLayer(std::move(layer), cull_rects_.top());
}

void SceneBuilder::pushPhysicalShape(const CanvasPath* path,
                                     double elevation,
                                     int color,
                                     int shadow_color,
                                     int clipBehavior) {
  const SkPath& sk_path = path->path();
  flow::Clip clip_behavior = static_cast<flow::Clip>(clipBehavior);
  SkRect cullRect;
  if (!cullRect.intersect(sk_path.getBounds(), cull_rects_.top())) {
    cullRect = SkRect::MakeEmpty();
  }
  auto layer = std::make_unique<flow::PhysicalShapeLayer>(clip_behavior);
  layer->set_path(sk_path);
  layer->set_elevation(elevation);
  layer->set_color(static_cast<SkColor>(color));
  layer->set_shadow_color(static_cast<SkColor>(shadow_color));
  layer->set_device_pixel_ratio(
      UIDartState::Current()->window()->viewport_metrics().device_pixel_ratio);
  PushLayer(std::move(layer), cullRect);
}

void SceneBuilder::pop() {
  if (!current_layer_) {
    return;
  }
  cull_rects_.pop();
  current_layer_ = current_layer_->parent();
}

void SceneBuilder::addPicture(double dx,
                              double dy,
                              Picture* picture,
                              int hints) {
  if (!current_layer_) {
    return;
  }
  SkPoint offset = SkPoint::Make(dx, dy);
  SkRect pictureRect = picture->picture()->cullRect();
  pictureRect.offset(offset.x(), offset.y());
  if (!SkRect::Intersects(pictureRect, cull_rects_.top())) {
    return;
  }
  auto layer = std::make_unique<flow::PictureLayer>();
  layer->set_offset(offset);
  layer->set_picture(UIDartState::CreateGPUObject(picture->picture()));
  layer->set_is_complex(!!(hints & 1));
  layer->set_will_change(!!(hints & 2));
  current_layer_->Add(std::move(layer));
}

void SceneBuilder::addTexture(double dx,
                              double dy,
                              double width,
                              double height,
                              int64_t textureId,
                              bool freeze) {
  if (!current_layer_) {
    return;
  }
  auto layer = std::make_unique<flow::TextureLayer>();
  layer->set_offset(SkPoint::Make(dx, dy));
  layer->set_size(SkSize::Make(width, height));
  layer->set_texture_id(textureId);
  layer->set_freeze(freeze);
  current_layer_->Add(std::move(layer));
}

void SceneBuilder::addChildScene(double dx,
                                 double dy,
                                 double width,
                                 double height,
                                 SceneHost* sceneHost,
                                 bool hitTestable) {
#if defined(OS_FUCHSIA)
  if (!current_layer_) {
    return;
  }
  SkRect sceneRect = SkRect::MakeXYWH(dx, dy, width, height);
  if (!SkRect::Intersects(sceneRect, cull_rects_.top())) {
    return;
  }
  auto layer = std::make_unique<flow::ChildSceneLayer>();
  layer->set_offset(SkPoint::Make(dx, dy));
  layer->set_size(SkSize::Make(width, height));
  layer->set_export_node_holder(sceneHost->export_node_holder());
  layer->set_hit_testable(hitTestable);
  current_layer_->Add(std::move(layer));
#endif  // defined(OS_FUCHSIA)
}

void SceneBuilder::addPerformanceOverlay(uint64_t enabledOptions,
                                         double left,
                                         double right,
                                         double top,
                                         double bottom) {
  if (!current_layer_) {
    return;
  }
  SkRect rect = SkRect::MakeLTRB(left, top, right, bottom);
  auto layer = std::make_unique<flow::PerformanceOverlayLayer>(enabledOptions);
  layer->set_paint_bounds(rect);
  current_layer_->Add(std::move(layer));
}

void SceneBuilder::setRasterizerTracingThreshold(uint32_t frameInterval) {
  rasterizer_tracing_threshold_ = frameInterval;
}

void SceneBuilder::setCheckerboardRasterCacheImages(bool checkerboard) {
  checkerboard_raster_cache_images_ = checkerboard;
}

void SceneBuilder::setCheckerboardOffscreenLayers(bool checkerboard) {
  checkerboard_offscreen_layers_ = checkerboard;
}

fml::RefPtr<Scene> SceneBuilder::build() {
  fml::RefPtr<Scene> scene = Scene::create(
      std::move(root_layer_), rasterizer_tracing_threshold_,
      checkerboard_raster_cache_images_, checkerboard_offscreen_layers_);
  ClearDartWrapper();
  return scene;
}

void SceneBuilder::PushLayer(std::unique_ptr<flow::ContainerLayer> layer,
                             const SkRect& cullRect) {
  FML_DCHECK(layer);

  cull_rects_.push(cullRect);

  if (!root_layer_) {
    root_layer_ = std::move(layer);
    current_layer_ = root_layer_.get();
    return;
  }

  if (!current_layer_) {
    return;
  }

  flow::ContainerLayer* newLayer = layer.get();
  current_layer_->Add(std::move(layer));
  current_layer_ = newLayer;
}

}  // namespace blink
