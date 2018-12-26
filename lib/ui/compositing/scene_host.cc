// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "flutter/lib/ui/compositing/scene_host.h"

#ifdef OS_FUCHSIA
#include "dart-pkg/zircon/sdk_ext/handle.h"
#endif

namespace blink {

fml::RefPtr<SceneHost> SceneHost::create() {
  return fml::MakeRefCounted<SceneHost>();
}

SceneHost::SceneHost() {}

SceneHost::~SceneHost() {}

void SceneHost::dispose() {
}

}  // namespace blink
