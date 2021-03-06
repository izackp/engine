// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef FLUTTER_LIB_UI_SEMANTICS_SEMANTICS_UPDATE_H_
#define FLUTTER_LIB_UI_SEMANTICS_SEMANTICS_UPDATE_H_

#include "flutter/fml/memory/ref_counted.h"
#include "flutter/lib/ui/semantics/custom_accessibility_action.h"
#include "flutter/lib/ui/semantics/semantics_node.h"

namespace blink {

class SemanticsUpdate : public fml::RefCountedThreadSafe<SemanticsUpdate> {

  FML_FRIEND_MAKE_REF_COUNTED(SemanticsUpdate);

 public:
  ~SemanticsUpdate();
  static fml::RefPtr<SemanticsUpdate> create(
      SemanticsNodeUpdates nodes,
      CustomAccessibilityActionUpdates actions);

  SemanticsNodeUpdates takeNodes();

  CustomAccessibilityActionUpdates takeActions();

 private:
  explicit SemanticsUpdate(SemanticsNodeUpdates nodes,
                           CustomAccessibilityActionUpdates updates);

  SemanticsNodeUpdates nodes_;
  CustomAccessibilityActionUpdates actions_;
};

}  // namespace blink

#endif  // FLUTTER_LIB_UI_SEMANTICS_SEMANTICS_UPDATE_H_
