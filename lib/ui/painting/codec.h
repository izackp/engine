// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef FLUTTER_LIB_UI_PAINTING_CODEC_H_
#define FLUTTER_LIB_UI_PAINTING_CODEC_H_

#include "flutter/lib/ui/painting/frame_info.h"
#include "flutter/fml/memory/ref_counted.h"
#include "third_party/skia/include/codec/SkCodec.h"
#include "third_party/skia/include/core/SkBitmap.h"
#include "third_party/skia/include/core/SkImage.h"
#include <map>

namespace blink {

//refer to ConvertImageInfo on how to populate this data
struct ImageInfo {
  SkImageInfo sk_info;
  size_t row_bytes;
};

// A handle to an SkCodec object.
//
// Doesn't mirror SkCodec's API but provides a simple sequential access API.
class Codec : public fml::RefCountedThreadSafe<Codec> {
  //DEFINE_WRAPPERTYPEINFO();

 public:
  virtual int frameCount() = 0;
  virtual int repetitionCount() = 0;
  virtual char* getNextFrame(void (*callback)(fml::RefPtr<FrameInfo> frameInfo)) = 0;
  void dispose();

};

class MultiFrameCodec : public Codec {
 public:
  int frameCount() override;
  int repetitionCount() override;
  char* getNextFrame(void (*callback)(fml::RefPtr<FrameInfo> frameInfo)) override;

 private:
  MultiFrameCodec(std::unique_ptr<SkCodec> codec,
                  const float decodedCacheRatioCap);

  ~MultiFrameCodec();

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
  // The default max amount of memory to use for caching decoded animated image
  // frames compared to total undecoded size.
  const float decodedCacheRatioCap_;
  size_t compressedSizeBytes_;
  size_t decodedCacheSize_;

  std::vector<SkCodec::FrameInfo> frameInfos_;
  // A struct linking the bitmap of a frame to whether it's required to render
  // other dependent frames.
  struct DecodedFrame {
    std::unique_ptr<SkBitmap> bitmap_ = nullptr;
    const bool required_;

    DecodedFrame(bool required);
    ~DecodedFrame();
  };

  // A cache of previously loaded bitmaps, indexed by the frame they belong to.
  // Always holds at least the frames marked as required for reuse by
  // [SkCodec::getFrameInfo()]. Will cache other non-essential frames until
  // [decodedCacheSize_] : [compressedSize_] exceeds [decodedCacheRatioCap_].
  std::map<int, std::unique_ptr<DecodedFrame>> frameBitmaps_;

  FML_FRIEND_MAKE_REF_COUNTED(MultiFrameCodec);
  FML_FRIEND_REF_COUNTED_THREAD_SAFE(MultiFrameCodec);
};

class SingleFrameCodec : public Codec {
 public:
  int frameCount() override;
  int repetitionCount() override;
  char* getNextFrame(void (*callback)(fml::RefPtr<FrameInfo> frameInfo)) override;

 private:
  SingleFrameCodec(fml::RefPtr<FrameInfo> frame);
  ~SingleFrameCodec();

  fml::RefPtr<FrameInfo> frame_;

  FML_FRIEND_MAKE_REF_COUNTED(SingleFrameCodec);
  FML_FRIEND_REF_COUNTED_THREAD_SAFE(SingleFrameCodec);
};

//image_info can be null. Null seems to imply that the data is compressed (png)
char* InstantiateImageCodec(
  const uint8_t* list, 
  int count, 
  std::unique_ptr<ImageInfo> image_info, 
  void (*callback)(fml::RefPtr<Codec> codec),
  const float decodedCacheRatioCap);

}  // namespace blink

#endif  // FLUTTER_LIB_UI_PAINTING_CODEC_H_
