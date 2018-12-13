// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "flutter/lib/ui/painting/engine_layer.h"

#include "flutter/flow/layers/container_layer.h"

namespace blink {

EngineLayer::EngineLayer(std::shared_ptr<flow::ContainerLayer> layer)
    : layer_(layer) {}

EngineLayer::~EngineLayer() = default;

size_t EngineLayer::GetAllocationSize() {
  // Provide an approximation of the total memory impact of this object to the
  // Dart GC.  The ContainerLayer may hold references to a tree of other layers,
  // which in turn may contain Skia objects.
  return 3000;
};

}  // namespace blink
