// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef FLUTTER_LIB_UI_COMPOSITING_SCENE_HOST_H_
#define FLUTTER_LIB_UI_COMPOSITING_SCENE_HOST_H_

#include <stdint.h>

#include "flutter/fml/memory/ref_counted.h"
#include "flutter/fml/build_config.h"

namespace blink {

class SceneHost : public fml::RefCountedThreadSafe<SceneHost> {
  FML_FRIEND_MAKE_REF_COUNTED(SceneHost);

 public:
  static fml::RefPtr<SceneHost> create();

  ~SceneHost();

  void dispose();

 private:
  explicit SceneHost();
};

}  // namespace blink

#endif  // FLUTTER_LIB_UI_COMPOSITING_SCENE_HOST_H_
