// Copyright 2017 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef FLUTTER_LIB_UI_COMPOSITING_SCENE_HOST_H_
#define FLUTTER_LIB_UI_COMPOSITING_SCENE_HOST_H_

#include <stdint.h>

#include "flutter/lib/ui/dart_wrapper.h"
#include "flutter/fml/build_config.h"

namespace blink {

class SceneHost : public RefCountedDartWrappable<SceneHost> {
  FML_FRIEND_MAKE_REF_COUNTED(SceneHost);

 public:
  static fml::RefPtr<SceneHost> create();

  ~SceneHost() override;

  void dispose();

 private:
  explicit SceneHost();
};

}  // namespace blink

#endif  // FLUTTER_LIB_UI_COMPOSITING_SCENE_HOST_H_
