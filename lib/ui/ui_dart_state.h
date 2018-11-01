// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef FLUTTER_LIB_UI_UI_DART_STATE_H_
#define FLUTTER_LIB_UI_UI_DART_STATE_H_

#include <memory>
#include <string>
#include <utility>
#include <functional>

#include "flutter/common/settings.h"
#include "flutter/common/task_runners.h"
#include "flutter/flow/skia_gpu_object.h"
#include "flutter/fml/build_config.h"
#include "flutter/fml/memory/weak_ptr.h"
#include "third_party/skia/include/gpu/GrContext.h"

namespace blink {
class FontSelector;
class Window;

class UIDartState : public std::enable_shared_from_this<UIDartState> {
 public:
  static UIDartState* Current();

  const std::string& debug_name() const { return debug_name_; }

  const std::string& logger_prefix() const { return logger_prefix_; }

  Window* window() const { return window_.get(); }

  const TaskRunners& GetTaskRunners() const;

  fml::RefPtr<flow::SkiaUnrefQueue> GetSkiaUnrefQueue() const;

  fml::WeakPtr<GrContext> GetResourceContext() const;

  template <class T>
  static flow::SkiaGPUObject<T> CreateGPUObject(sk_sp<T> object) {
    if (!object) {
      return {};
    }
    auto state = UIDartState::Current();
    FML_DCHECK(state);
    auto queue = state->GetSkiaUnrefQueue();
    return {std::move(object), std::move(queue)};
  };

  UIDartState(TaskRunners task_runners,
              TaskObserverAdd add_callback,
              TaskObserverRemove remove_callback,
              fml::WeakPtr<GrContext> resource_context,
              fml::RefPtr<flow::SkiaUnrefQueue> skia_unref_queue,
              std::string logger_prefix);

  ~UIDartState();

  void SetWindow(std::unique_ptr<Window> window);
  
 private:

  const TaskRunners task_runners_;
  const TaskObserverAdd add_callback_;
  const TaskObserverRemove remove_callback_;
  fml::WeakPtr<GrContext> resource_context_;
  const std::string logger_prefix_;
  std::string debug_name_;
  std::unique_ptr<Window> window_;
  fml::RefPtr<flow::SkiaUnrefQueue> skia_unref_queue_;

  void AddOrRemoveTaskObserver(bool add);
};

}  // namespace blink

#endif  // FLUTTER_LIB_UI_UI_DART_STATE_H_
