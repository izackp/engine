// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "flutter/fml/trace_event.h"

//REMOVED_DART: Refer to git history if needed
namespace fml {
namespace tracing {

void TraceCounter(TraceArg category_group, TraceArg name, TraceIDArg count) {
  auto count_string = std::to_string(count);
  const char* arg_names[] = {name};
  const char* arg_values[] = {count_string.c_str()};
  Dart_TimelineEvent(name,                         // label
                     Dart_TimelineGetMicros(),     // timestamp0
                     0,                            // timestamp1_or_async_id
                     Dart_Timeline_Event_Counter,  // event type
                     1,                            // argument_count
                     arg_names,                    // argument_names
                     arg_values                    // argument_values
  );
}

void TraceEvent0(TraceArg category_group, TraceArg name) {

}

void TraceEvent1(TraceArg category_group,
                 TraceArg name,
                 TraceArg arg1_name,
                 TraceArg arg1_val) {
  const char* arg_names[] = {arg1_name};
  const char* arg_values[] = {arg1_val};

}

void TraceEvent2(TraceArg category_group,
                 TraceArg name,
                 TraceArg arg1_name,
                 TraceArg arg1_val,
                 TraceArg arg2_name,
                 TraceArg arg2_val) {
  const char* arg_names[] = {arg1_name, arg2_name};
  const char* arg_values[] = {arg1_val, arg2_val};

}

void TraceEventEnd(TraceArg name) {

}

void TraceEventAsyncBegin0(TraceArg category_group,
                           TraceArg name,
                           TraceIDArg id) {

}

void TraceEventAsyncEnd0(TraceArg category_group,
                         TraceArg name,
                         TraceIDArg id) {

}

void TraceEventAsyncBegin1(TraceArg category_group,
                           TraceArg name,
                           TraceIDArg id,
                           TraceArg arg1_name,
                           TraceArg arg1_val) {
  const char* arg_names[] = {arg1_name};
  const char* arg_values[] = {arg1_val};

}

void TraceEventAsyncEnd1(TraceArg category_group,
                         TraceArg name,
                         TraceIDArg id,
                         TraceArg arg1_name,
                         TraceArg arg1_val) {
  const char* arg_names[] = {arg1_name};
  const char* arg_values[] = {arg1_val};

}

void TraceEventInstant0(TraceArg category_group, TraceArg name) {

}

void TraceEventFlowBegin0(TraceArg category_group,
                          TraceArg name,
                          TraceIDArg id) {

}

void TraceEventFlowStep0(TraceArg category_group,
                         TraceArg name,
                         TraceIDArg id) {

}

void TraceEventFlowEnd0(TraceArg category_group, TraceArg name, TraceIDArg id) {

}

}  // namespace tracing
}  // namespace fml
