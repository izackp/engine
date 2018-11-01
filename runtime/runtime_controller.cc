// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "flutter/runtime/runtime_controller.h"

#include "flutter/fml/message_loop.h"
#include "flutter/fml/trace_event.h"
#include "flutter/lib/ui/compositing/scene.h"
#include "flutter/lib/ui/ui_dart_state.h"
#include "flutter/lib/ui/window/window.h"
#include "flutter/runtime/runtime_delegate.h"

#ifdef ERROR
#undef ERROR
#endif

namespace blink {

RuntimeController::RuntimeController(
    RuntimeDelegate& p_client,
    TaskRunners p_task_runners,
    fml::WeakPtr<GrContext> p_resource_context,
    fml::RefPtr<flow::SkiaUnrefQueue> p_unref_queue)
    : RuntimeController(p_client,
                        std::move(p_task_runners),
                        std::move(p_resource_context),
                        std::move(p_unref_queue),
                        WindowData{/* default window data */}) {}

RuntimeController::RuntimeController(
    RuntimeDelegate& p_client,
    TaskRunners p_task_runners,
    fml::WeakPtr<GrContext> p_resource_context,
    fml::RefPtr<flow::SkiaUnrefQueue> p_unref_queue,
    WindowData p_window_data)
    : client_(p_client),
      task_runners_(p_task_runners),
      resource_context_(p_resource_context),
      unref_queue_(p_unref_queue),
      window_data_(p_window_data),//TODO: was std::Move 
      root_isolate_(
                    UIDartState(
                                task_runners_,
                                nullptr,
                                nullptr,
                                resource_context_,
                                unref_queue_,
                                nullptr)) {
  //std::shared_ptr<UIDartState> root_isolate = root_isolate_.lock();
  if (auto window = GetWindowIfAvailable()) {
    window->DidCreateIsolate();
    
    if (!FlushRuntimeStateToIsolate()) {
      FML_DLOG(ERROR) << "Could not setup intial isolate state.";
    }
  } else {
    FML_DCHECK(false) << "RuntimeController created without window binding.";
  }
}

RuntimeController::~RuntimeController() {
}

bool RuntimeController::IsRootIsolateRunning() const {
  return true;
}

std::unique_ptr<RuntimeController> RuntimeController::Clone() const {
  return std::unique_ptr<RuntimeController>(new RuntimeController(
      client_,                      //
      task_runners_,                //
      resource_context_,            //
      unref_queue_,                 //
      window_data_                  //
      ));
}

bool RuntimeController::FlushRuntimeStateToIsolate() {
  return SetViewportMetrics(window_data_.viewport_metrics) &&
         SetLocale(window_data_.language_code, window_data_.country_code) &&
         SetSemanticsEnabled(window_data_.semantics_enabled) &&
         SetAccessibilityFeatures(window_data_.accessibility_feature_flags_);
}

bool RuntimeController::SetViewportMetrics(const ViewportMetrics& metrics) {
  window_data_.viewport_metrics = metrics;

  if (auto window = GetWindowIfAvailable()) {
    window->UpdateWindowMetrics(metrics);
    return true;
  }
  return false;
}

bool RuntimeController::SetLocale(const std::string& language_code,
                                  const std::string& country_code) {
  window_data_.language_code = language_code;
  window_data_.country_code = country_code;

  if (auto window = GetWindowIfAvailable()) {
    window->UpdateLocale(window_data_.language_code, window_data_.country_code);
    return true;
  }

  return false;
}

bool RuntimeController::SetUserSettingsData(const std::string& data) {
  window_data_.user_settings_data = data;

  if (auto window = GetWindowIfAvailable()) {
    window->UpdateUserSettingsData(window_data_.user_settings_data);
    return true;
  }

  return false;
}

bool RuntimeController::SetSemanticsEnabled(bool enabled) {
  window_data_.semantics_enabled = enabled;

  if (auto window = GetWindowIfAvailable()) {
    window->UpdateSemanticsEnabled(window_data_.semantics_enabled);
    return true;
  }

  return false;
}

bool RuntimeController::SetAccessibilityFeatures(int32_t flags) {
  window_data_.accessibility_feature_flags_ = flags;
  if (auto window = GetWindowIfAvailable()) {
    window->UpdateAccessibilityFeatures(
        window_data_.accessibility_feature_flags_);
    return true;
  }

  return false;
}

bool RuntimeController::BeginFrame(fml::TimePoint frame_time) {
  if (auto window = GetWindowIfAvailable()) {
    window->BeginFrame(frame_time);
    return true;
  }
  return false;
}

bool RuntimeController::NotifyIdle(int64_t deadline) {
  return true;
}

bool RuntimeController::DispatchPlatformMessage(
    fml::RefPtr<PlatformMessage> message) {
  if (auto window = GetWindowIfAvailable()) {
    TRACE_EVENT1("flutter", "RuntimeController::DispatchPlatformMessage",
                 "mode", "basic");
    window->DispatchPlatformMessage(std::move(message));
    return true;
  }
  return false;
}

bool RuntimeController::DispatchPointerDataPacket(
    const PointerDataPacket& packet) {
  if (auto window = GetWindowIfAvailable()) {
    TRACE_EVENT1("flutter", "RuntimeController::DispatchPointerDataPacket",
                 "mode", "basic");
    window->DispatchPointerDataPacket(packet);
    return true;
  }
  return false;
}

bool RuntimeController::DispatchSemanticsAction(int32_t id,
                                                SemanticsAction action,
                                                std::vector<uint8_t> args) {
  TRACE_EVENT1("flutter", "RuntimeController::DispatchSemanticsAction", "mode",
               "basic");
  if (auto window = GetWindowIfAvailable()) {
    window->DispatchSemanticsAction(id, action, std::move(args));
    return true;
  }
  return false;
}

Window* RuntimeController::GetWindowIfAvailable() {
  //std::shared_ptr<UIDartState> root_isolate = root_isolate_.lock();
  return root_isolate_.window();
}

std::string RuntimeController::DefaultRouteName() {
  return client_.DefaultRouteName();
}

void RuntimeController::ScheduleFrame() {
  client_.ScheduleFrame();
}

void RuntimeController::Render(Scene* scene) {
  client_.Render(scene->takeLayerTree());
}

void RuntimeController::UpdateSemantics(SemanticsUpdate* update) {
  if (window_data_.semantics_enabled) {
    client_.UpdateSemantics(update->takeNodes(), update->takeActions());
  }
}

void RuntimeController::HandlePlatformMessage(
    fml::RefPtr<PlatformMessage> message) {
  client_.HandlePlatformMessage(std::move(message));
}

FontCollection& RuntimeController::GetFontCollection() {
  return client_.GetFontCollection();
}

}  // namespace blink
