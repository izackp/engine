// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "flutter/lib/ui/window/window.h"

#include "flutter/lib/ui/compositing/scene.h"
#include "flutter/lib/ui/ui_dart_state.h"
#include "flutter/lib/ui/window/platform_message_response.h"

namespace blink {
namespace {

std::string DefaultRouteName() {
  std::string routeName =
      UIDartState::Current()->window()->client()->DefaultRouteName();
  return routeName;
}

void ScheduleFrame() {
  UIDartState::Current()->window()->client()->ScheduleFrame();
}

void Render(Scene* scene) {
  UIDartState::Current()->window()->client()->Render(scene);
}

void UpdateSemantics(SemanticsUpdate* update) {
  UIDartState::Current()->window()->client()->UpdateSemantics(update);
}

void RespondToPlatformMessage(int response_id,
                              const std::vector<uint8_t>& data) {
  UIDartState::Current()->window()->CompletePlatformMessageResponse(
        response_id,
        data);
}

}  // namespace


WindowClient::~WindowClient() {}

Window::Window(WindowClient* client) : client_(client) {}

Window::~Window() {}

void Window::UpdateWindowMetrics(const ViewportMetrics& metrics) {
  viewport_metrics_ = metrics;
}

void Window::UpdateLocale(const std::string& language_code,
                          const std::string& country_code) {

}

void Window::UpdateUserSettingsData(const std::string& data) {

}

void Window::UpdateSemanticsEnabled(bool enabled) {

}

void Window::UpdateAccessibilityFeatures(int32_t values) {

}

void Window::DispatchPlatformMessage(fml::RefPtr<PlatformMessage> message) {
  int response_id = 0;
  if (auto response = message->response()) {
    response_id = next_response_id_++;
    pending_responses_[response_id] = response;
  }
}

void Window::DispatchPointerDataPacket(const PointerDataPacket& packet) {

}

void Window::DispatchSemanticsAction(int32_t id,
                                     SemanticsAction action,
                                     std::vector<uint8_t> args) {

}

void Window::BeginFrame(fml::TimePoint frameTime) {
  int64_t microseconds = (frameTime - fml::TimePoint()).ToMicroseconds();

  //DartInvokeField(library_.value(), "_beginFrame", {  Dart_NewInteger(microseconds), });
  //UIDartState::Current()->FlushMicrotasksNow();
  //DartInvokeField(library_.value(), "_drawFrame", {});
}

void Window::CompletePlatformMessageEmptyResponse(int response_id) {
  if (!response_id)
    return;
  auto it = pending_responses_.find(response_id);
  if (it == pending_responses_.end())
    return;
  auto response = std::move(it->second);
  pending_responses_.erase(it);
  response->CompleteEmpty();
}

void Window::CompletePlatformMessageResponse(int response_id,
                                             std::vector<uint8_t> data) {
  if (!response_id)
    return;
  auto it = pending_responses_.find(response_id);
  if (it == pending_responses_.end())
    return;
  auto response = std::move(it->second);
  pending_responses_.erase(it);
  response->Complete(std::make_unique<fml::DataMapping>(std::move(data)));
}
/*
void Window::RegisterNatives(tonic::DartLibraryNatives* natives) {
  natives->Register({
      {"Window_defaultRouteName", DefaultRouteName, 1, true},
      {"Window_scheduleFrame", ScheduleFrame, 1, true},
      {"Window_sendPlatformMessage", _SendPlatformMessage, 4, true},
      {"Window_respondToPlatformMessage", _RespondToPlatformMessage, 3, true},
      {"Window_render", Render, 2, true},
      {"Window_updateSemantics", UpdateSemantics, 2, true},
  });
}
*/
}  // namespace blink
