// Copyright 2017 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "flutter/lib/ui/compositing/scene_host.h"

//#include "flutter/lib/ui/ui_dart_state.h"
//#include "third_party/tonic/dart_args.h"
//#include "third_party/tonic/dart_binding_macros.h"
//#include "third_party/tonic/dart_library_natives.h"

#ifdef OS_FUCHSIA
#include "dart-pkg/zircon/sdk_ext/handle.h"
#endif

namespace blink {
/*
static void SceneHost_constructor(Dart_NativeArguments args) {
  DartCallConstructor(&SceneHost::create, args);
}

IMPLEMENT_WRAPPERTYPEINFO(ui, SceneHost);

#define FOR_EACH_BINDING(V) V(SceneHost, dispose)

FOR_EACH_BINDING(DART_NATIVE_CALLBACK)

void SceneHost::RegisterNatives(tonic::DartLibraryNatives* natives) {
  natives->Register({{"SceneHost_constructor", SceneHost_constructor, 2, true},
                     FOR_EACH_BINDING(DART_REGISTER_NATIVE)});
}
*/
fml::RefPtr<SceneHost> SceneHost::create() {
  return fml::MakeRefCounted<SceneHost>();
}

SceneHost::SceneHost() {}

SceneHost::~SceneHost() {}

void SceneHost::dispose() {
  //ClearDartWrapper();
}

}  // namespace blink
