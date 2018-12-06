// Copyright 2017 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef FLUTTER_LIB_UI_PAINTING_CODEC_H_
#define FLUTTER_LIB_UI_PAINTING_CODEC_H_

#include "flutter/lib/ui/dart_wrapper.h"
#include "flutter/lib/ui/painting/frame_info.h"
#include "third_party/skia/include/codec/SkCodec.h"
#include "third_party/skia/include/core/SkBitmap.h"
#include "third_party/skia/include/core/SkImage.h"

namespace blink {

//refer to ConvertImageInfo on how to populate this data
struct ImageInfo {
  SkImageInfo sk_info;
  size_t row_bytes;
};

// A handle to an SkCodec object.
//
// Doesn't mirror SkCodec's API but provides a simple sequential access API.
class Codec : public RefCountedDartWrappable<Codec> {
  //DEFINE_WRAPPERTYPEINFO();

 public:
  virtual int frameCount() = 0;
  virtual int repetitionCount() = 0;
  virtual char* getNextFrame(void (*callback)(fml::RefPtr<FrameInfo> frameInfo)) = 0;
  void dispose();

  //static void RegisterNatives(tonic::DartLibraryNatives* natives);
};

class MultiFrameCodec : public Codec {
 public:
  int frameCount() { return frameInfos_.size(); }
  int repetitionCount() { return repetitionCount_; }
  char* getNextFrame(void (*callback)(fml::RefPtr<FrameInfo> frameInfo));

 private:
  MultiFrameCodec(std::unique_ptr<SkCodec> codec);

  ~MultiFrameCodec() {}

  sk_sp<SkImage> GetNextFrameImage(fml::WeakPtr<GrContext> resourceContext);

  void GetNextFrameAndInvokeCallback(
      void (*callback)(fml::RefPtr<FrameInfo> frameInfo),
      fml::RefPtr<fml::TaskRunner> ui_task_runner,
      fml::WeakPtr<GrContext> resourceContext,
      fml::RefPtr<flow::SkiaUnrefQueue> unref_queue,
      size_t trace_id);

  const std::unique_ptr<SkCodec> codec_;
  int repetitionCount_;
  int nextFrameIndex_;

  std::vector<SkCodec::FrameInfo> frameInfos_;
  std::vector<SkBitmap> frameBitmaps_;

  FML_FRIEND_MAKE_REF_COUNTED(MultiFrameCodec);
  FML_FRIEND_REF_COUNTED_THREAD_SAFE(MultiFrameCodec);
};

class SingleFrameCodec : public Codec {
 public:
  int frameCount() { return 1; }
  int repetitionCount() { return 0; }
  char* getNextFrame(void (*callback)(fml::RefPtr<FrameInfo> frameInfo));

 private:
  SingleFrameCodec(fml::RefPtr<FrameInfo> frame) : frame_(std::move(frame)) {}
  ~SingleFrameCodec() {}

  fml::RefPtr<FrameInfo> frame_;

  FML_FRIEND_MAKE_REF_COUNTED(SingleFrameCodec);
  FML_FRIEND_REF_COUNTED_THREAD_SAFE(SingleFrameCodec);
};

//image_info can be null. Null seems to imply that the data is compressed (png)
char* InstantiateImageCodec(
  const uint8_t* list, 
  int count, 
  std::unique_ptr<ImageInfo> image_info, 
  void (*callback)(fml::RefPtr<Codec> codec));

}  // namespace blink

#endif  // FLUTTER_LIB_UI_PAINTING_CODEC_H_
