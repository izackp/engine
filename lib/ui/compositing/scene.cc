// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "flutter/lib/ui/compositing/scene.h"

#include "flutter/fml/make_copyable.h"
#include "flutter/fml/trace_event.h"
#include "flutter/lib/ui/painting/image.h"
#include "third_party/skia/include/core/SkImageInfo.h"
#include "third_party/skia/include/core/SkSurface.h"

namespace blink {
/*
IMPLEMENT_WRAPPERTYPEINFO(ui, Scene);

#define FOR_EACH_BINDING(V) \
  V(Scene, toImage)         \
  V(Scene, dispose)

DART_BIND_ALL(Scene, FOR_EACH_BINDING)*/

fml::RefPtr<Scene> Scene::create(std::shared_ptr<flow::Layer> rootLayer,
                                 uint32_t rasterizerTracingThreshold,
                                 bool checkerboardRasterCacheImages,
                                 bool checkerboardOffscreenLayers) {
  return fml::MakeRefCounted<Scene>(
      std::move(rootLayer), rasterizerTracingThreshold,
      checkerboardRasterCacheImages, checkerboardOffscreenLayers);
}

Scene::Scene(std::shared_ptr<flow::Layer> rootLayer,
             uint32_t rasterizerTracingThreshold,
             bool checkerboardRasterCacheImages,
             bool checkerboardOffscreenLayers)
    : m_layerTree(new flow::LayerTree()) {
  m_layerTree->set_root_layer(std::move(rootLayer));
  m_layerTree->set_rasterizer_tracing_threshold(rasterizerTracingThreshold);
  m_layerTree->set_checkerboard_raster_cache_images(
      checkerboardRasterCacheImages);
  m_layerTree->set_checkerboard_offscreen_layers(checkerboardOffscreenLayers);
}

Scene::~Scene() {}

void Scene::dispose() {
}

char* Scene::toImage(uint32_t width,
                           uint32_t height,
                           ImageCallback raw_image_callback) {
  TRACE_EVENT0("flutter", "Scene::toImage");
  if (raw_image_callback == 0) {
    return "Image callback was invalid";
  }

  if (!m_layerTree) {
    return "Scene did not contain a layer tree.";
  }

  if (width == 0 || height == 0) {
    return "Image dimensions for scene were invalid.";
  }

  auto* dart_state = UIDartState::Current();
  auto unref_queue = dart_state->GetSkiaUnrefQueue();
  auto ui_task_runner = dart_state->GetTaskRunners().GetUITaskRunner();
  auto gpu_task_runner = dart_state->GetTaskRunners().GetGPUTaskRunner();
  auto snapshot_delegate = dart_state->GetSnapshotDelegate();

  // We can't create an image on this task runner because we don't have a
  // graphics context. Even if we did, it would be slow anyway. Also, this
  // thread owns the sole reference to the layer tree. So we flatten the layer
  // tree into a picture and use that as the thread transport mechanism.

  auto picture_bounds = SkISize::Make(width, height);
  auto picture = m_layerTree->Flatten(SkRect::MakeWH(width, height));

  if (!picture) {
    // Already in Dart scope.
    return "Could not flatten scene into a layer tree.";
  }
  
  auto ui_task = fml::MakeCopyable([ui_task_runner,
                                    image_callback = std::move(raw_image_callback),
                                    unref_queue](
                                       sk_sp<SkImage> raster_image) mutable {
    // Send the raster image back to the UI thread for submission to the
    // framework.
    ui_task_runner->PostTask(fml::MakeCopyable([raster_image,
                                                image_callback =
                                                    std::move(image_callback),
                                                unref_queue]() mutable {

      if (!raster_image) {
        image_callback(nullptr);
        return;
      }

      auto dart_image = CanvasImage::Create();
      dart_image->set_image({std::move(raster_image), std::move(unref_queue)});
      image_callback(dart_image);
    }));
  });

  auto gpu_task = fml::MakeCopyable([gpu_task_runner, picture, picture_bounds,
                                     snapshot_delegate, ui_task]() {
    gpu_task_runner->PostTask([snapshot_delegate, picture, picture_bounds,
                               ui_task]() {
      // Snapshot the picture on the GPU thread. This thread has access to the
      // GPU contexts that may contain the sole references to texture backed
      // images in the picture.
      ui_task(snapshot_delegate->MakeRasterSnapshot(picture, picture_bounds));
    });
  });

  // Kick things off on the GPU.
  gpu_task();

  return nullptr;
}

std::unique_ptr<flow::LayerTree> Scene::takeLayerTree() {
  return std::move(m_layerTree);
}

}  // namespace blink
