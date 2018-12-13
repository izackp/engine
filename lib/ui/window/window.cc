// Copyright 2013 The Flutter Authors. All rights reserved.
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

<<<<<<< HEAD
void RespondToPlatformMessage(int response_id,
                              const std::vector<uint8_t>& data) {
  UIDartState::Current()->window()->CompletePlatformMessageResponse(
=======
void SetIsolateDebugName(Dart_NativeArguments args) {
  Dart_Handle exception = nullptr;
  const std::string name =
      tonic::DartConverter<std::string>::FromArguments(args, 1, exception);
  if (exception) {
    Dart_ThrowException(exception);
    return;
  }
  UIDartState::Current()->SetDebugName(name);
}

Dart_Handle SendPlatformMessage(Dart_Handle window,
                                const std::string& name,
                                Dart_Handle callback,
                                const tonic::DartByteData& data) {
  UIDartState* dart_state = UIDartState::Current();

  if (!dart_state->window()) {
    // Must release the TypedData buffer before allocating other Dart objects.
    data.Release();
    return ToDart("Platform messages can only be sent from the main isolate");
  }

  fml::RefPtr<PlatformMessageResponse> response;
  if (!Dart_IsNull(callback)) {
    response = fml::MakeRefCounted<PlatformMessageResponseDart>(
        tonic::DartPersistentValue(dart_state, callback),
        dart_state->GetTaskRunners().GetUITaskRunner());
  }
  if (Dart_IsNull(data.dart_handle())) {
    dart_state->window()->client()->HandlePlatformMessage(
        fml::MakeRefCounted<PlatformMessage>(name, response));
  } else {
    const uint8_t* buffer = static_cast<const uint8_t*>(data.data());

    dart_state->window()->client()->HandlePlatformMessage(
        fml::MakeRefCounted<PlatformMessage>(
            name, std::vector<uint8_t>(buffer, buffer + data.length_in_bytes()),
            response));
  }

  return Dart_Null();
}

void _SendPlatformMessage(Dart_NativeArguments args) {
  tonic::DartCallStatic(&SendPlatformMessage, args);
}

void RespondToPlatformMessage(Dart_Handle window,
                              int response_id,
                              const tonic::DartByteData& data) {
  if (Dart_IsNull(data.dart_handle())) {
    UIDartState::Current()->window()->CompletePlatformMessageEmptyResponse(
        response_id);
  } else {
    // TODO(engine): Avoid this copy.
    const uint8_t* buffer = static_cast<const uint8_t*>(data.data());
    UIDartState::Current()->window()->CompletePlatformMessageResponse(
>>>>>>> master
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

void Window::UpdateLocales(const std::vector<std::string>& locales) {

}

void Window::UpdateUserSettingsData(const std::string& data) {

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
      {"Window_setIsolateDebugName", SetIsolateDebugName, 2, true},
  });
}
*/
}  // namespace blink
