// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef FLUTTER_LIB_UI_COMPOSITING_SCENE_H_
#define FLUTTER_LIB_UI_COMPOSITING_SCENE_H_

#include <stdint.h>
#include <memory>

#include "flutter/flow/layers/layer_tree.h"
#include "flutter/lib/ui/dart_wrapper.h"
#include "flutter/lib/ui/painting/image.h"
#include "third_party/skia/include/core/SkPicture.h"

namespace blink {

typedef void (*ImageCallback) (fml::RefPtr<CanvasImage>);

class Scene : public RefCountedDartWrappable<Scene> {
  FML_FRIEND_MAKE_REF_COUNTED(Scene);

 public:
  ~Scene();
  static fml::RefPtr<Scene> create(std::unique_ptr<flow::Layer> rootLayer,
                                   uint32_t rasterizerTracingThreshold,
                                   bool checkerboardRasterCacheImages,
                                   bool checkerboardOffscreenLayers);

  std::unique_ptr<flow::LayerTree> takeLayerTree();

  char* toImage(uint32_t width,
                uint32_t height,
                ImageCallback image_callback);

  void dispose();

 private:
  explicit Scene(std::unique_ptr<flow::Layer> rootLayer,
                 uint32_t rasterizerTracingThreshold,
                 bool checkerboardRasterCacheImages,
                 bool checkerboardOffscreenLayers);

  std::unique_ptr<flow::LayerTree> m_layerTree;
};

}  // namespace blink

#endif  // FLUTTER_LIB_UI_COMPOSITING_SCENE_H_
