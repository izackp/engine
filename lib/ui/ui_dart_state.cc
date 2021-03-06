// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "flutter/lib/ui/ui_dart_state.h"

#include "flutter/fml/message_loop.h"
#include "flutter/lib/ui/window/window.h"

namespace blink {

static UIDartState* lastInstance_ = nullptr; //TODO: We subsituted a reference from DartState which held a shared pointer reference to its self. Implying it kept itself alive which may or may not be necessary (avoid possible crash?)s

UIDartState::UIDartState(TaskRunners task_runners,
                         TaskObserverAdd add_callback,
                         TaskObserverRemove remove_callback,
                         fml::WeakPtr<SnapshotDelegate> snapshot_delegate,
                         fml::WeakPtr<GrContext> resource_context,
                         fml::RefPtr<flow::SkiaUnrefQueue> skia_unref_queue,
                         std::string logger_prefix)
    : task_runners_(std::move(task_runners)),
      add_callback_(std::move(add_callback)),
      remove_callback_(std::move(remove_callback)),
      snapshot_delegate_(std::move(snapshot_delegate)),
      resource_context_(std::move(resource_context)),
      logger_prefix_(std::move(logger_prefix)),
      skia_unref_queue_(std::move(skia_unref_queue)) {
  AddOrRemoveTaskObserver(true /* add */);
  lastInstance_ = this;
}

UIDartState::~UIDartState() {
  AddOrRemoveTaskObserver(false /* remove */);
  if (lastInstance_ == this) {
    lastInstance_ = nullptr;
  }
}

UIDartState* UIDartState::Current() {
  return lastInstance_;
}

void UIDartState::SetWindow(std::unique_ptr<Window> window) {
  window_ = std::move(window);
}

const TaskRunners& UIDartState::GetTaskRunners() const {
  return task_runners_;
}

fml::RefPtr<flow::SkiaUnrefQueue> UIDartState::GetSkiaUnrefQueue() const {
  return skia_unref_queue_;
}

void UIDartState::AddOrRemoveTaskObserver(bool add) {
  auto task_runner = task_runners_.GetUITaskRunner();
  if (!task_runner) {
    // This may happen in case the isolate has no thread affinity (for example,
    // the service isolate).
    return;
  }
  FML_DCHECK(add_callback_ && remove_callback_);
  if (add) {
    add_callback_(reinterpret_cast<intptr_t>(this),
                  [this]() { });
  } else {
    remove_callback_(reinterpret_cast<intptr_t>(this));
  }
}

fml::WeakPtr<SnapshotDelegate> UIDartState::GetSnapshotDelegate() const {
  return snapshot_delegate_;
}

fml::WeakPtr<GrContext> UIDartState::GetResourceContext() const {
  return resource_context_;
}

}  // namespace blink
