// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef FLUTTER_LIB_UI_PAINTING_ENGINE_LAYER_H_
#define FLUTTER_LIB_UI_PAINTING_ENGINE_LAYER_H_

#include "flutter/flow/layers/layer.h"
#include "flutter/fml/memory/ref_counted.h"

namespace blink {

class EngineLayer;

class EngineLayer : public fml::RefCountedThreadSafe<EngineLayer> {
  //DEFINE_WRAPPERTYPEINFO();

 public:
  ~EngineLayer();

  size_t GetAllocationSize();

  static fml::RefPtr<EngineLayer> MakeRetained(
      std::shared_ptr<flow::ContainerLayer> layer) {
    return fml::MakeRefCounted<EngineLayer>(layer);
  }

  std::shared_ptr<flow::ContainerLayer> Layer() const { return layer_; }

 private:
  explicit EngineLayer(std::shared_ptr<flow::ContainerLayer> layer);
  std::shared_ptr<flow::ContainerLayer> layer_;

  FML_FRIEND_MAKE_REF_COUNTED(EngineLayer);
};

}  // namespace blink

#endif  // FLUTTER_LIB_UI_PAINTING_ENGINE_LAYER_H_
