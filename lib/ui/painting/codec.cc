// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "flutter/lib/ui/painting/codec.h"

#include "flutter/common/task_runners.h"
#include "flutter/fml/logging.h"
#include "flutter/fml/make_copyable.h"
#include "flutter/fml/trace_event.h"
#include "flutter/lib/ui/painting/frame_info.h"
#include "third_party/skia/include/codec/SkCodec.h"
#include "third_party/skia/include/core/SkPixelRef.h"

namespace blink {

namespace {

static constexpr const char* kInitCodecTraceTag = "InitCodec";
static constexpr const char* kCodecNextFrameTraceTag = "CodecNextFrame";

static void InvokeCodecCallback(fml::RefPtr<Codec> codec,
                                void (*callback)(fml::RefPtr<Codec> codec),
                                size_t trace_id) {

  (*callback) (codec);
  TRACE_FLOW_END("flutter", kInitCodecTraceTag, trace_id);
}

static sk_sp<SkImage> DecodeImage(fml::WeakPtr<GrContext> context,
                                  sk_sp<SkData> buffer,
                                  size_t trace_id) {
  TRACE_FLOW_STEP("flutter", kInitCodecTraceTag, trace_id);
  TRACE_EVENT0("flutter", "DecodeImage");

  if (buffer == nullptr || buffer->isEmpty()) {
    return nullptr;
  }

  if (context) {
    // This indicates that we do not want a "linear blending" decode.
    sk_sp<SkColorSpace> dstColorSpace = nullptr;
    return SkImage::MakeCrossContextFromEncoded(
        context.get(), std::move(buffer), false, dstColorSpace.get(), true);
  } else {
    // Defer decoding until time of draw later on the GPU thread. Can happen
    // when GL operations are currently forbidden such as in the background
    // on iOS.
    return SkImage::MakeFromEncoded(std::move(buffer));
  }
}

fml::RefPtr<Codec> InitCodec(fml::WeakPtr<GrContext> context,
                             sk_sp<SkData> buffer,
                             fml::RefPtr<flow::SkiaUnrefQueue> unref_queue,
                             const float decodedCacheRatioCap,
                             size_t trace_id) {
  TRACE_FLOW_STEP("flutter", kInitCodecTraceTag, trace_id);
  TRACE_EVENT0("blink", "InitCodec");

  if (buffer == nullptr || buffer->isEmpty()) {
    FML_LOG(ERROR) << "InitCodec failed - buffer was empty ";
    return nullptr;
  }

  std::unique_ptr<SkCodec> skCodec = SkCodec::MakeFromData(buffer);
  if (!skCodec) {
    FML_LOG(ERROR) << "Failed decoding image. Data is either invalid, or it is "
                      "encoded using an unsupported format.";
    return nullptr;
  }

  if (skCodec->getFrameCount() > 1) {
    return fml::MakeRefCounted<MultiFrameCodec>(std::move(skCodec), decodedCacheRatioCap);
  }

  auto skImage = DecodeImage(context, buffer, trace_id);
  if (!skImage) {
    FML_LOG(ERROR) << "DecodeImage failed";
    return nullptr;
  }

  auto image = CanvasImage::Create();
  image->set_image({skImage, unref_queue});
  auto frameInfo = fml::MakeRefCounted<FrameInfo>(std::move(image), 0);
  return fml::MakeRefCounted<SingleFrameCodec>(std::move(frameInfo));
}

fml::RefPtr<Codec> InitCodecUncompressed(
    fml::WeakPtr<GrContext> context,
    sk_sp<SkData> buffer,
    ImageInfo image_info,
    fml::RefPtr<flow::SkiaUnrefQueue> unref_queue,
    const float decodedCacheRatioCap,
    size_t trace_id) {
  TRACE_FLOW_STEP("flutter", kInitCodecTraceTag, trace_id);
  TRACE_EVENT0("blink", "InitCodecUncompressed");

  if (buffer == nullptr || buffer->isEmpty()) {
    FML_LOG(ERROR) << "InitCodecUncompressed failed - buffer was empty";
    return nullptr;
  }

  sk_sp<SkImage> skImage;
  if (context) {
    SkPixmap pixmap(image_info.sk_info, buffer->data(), image_info.row_bytes);
    skImage = SkImage::MakeCrossContextFromPixmap(context.get(), pixmap, false,
                                                  nullptr, true);
  } else {
    skImage = SkImage::MakeRasterData(image_info.sk_info, std::move(buffer),
                                      image_info.row_bytes);
  }

  auto image = CanvasImage::Create();
  image->set_image({skImage, unref_queue});
  auto frameInfo = fml::MakeRefCounted<FrameInfo>(std::move(image), 0);
  return fml::MakeRefCounted<SingleFrameCodec>(std::move(frameInfo));
}

void InitCodecAndInvokeCodecCallback(
    fml::RefPtr<fml::TaskRunner> ui_task_runner,
    fml::WeakPtr<GrContext> context,
    fml::RefPtr<flow::SkiaUnrefQueue> unref_queue,
    void (*callback)(fml::RefPtr<Codec> codec),
    sk_sp<SkData> buffer,
    std::unique_ptr<ImageInfo> image_info,
    const float decodedCacheRatioCap,
    size_t trace_id) {
  fml::RefPtr<Codec> codec;
  if (image_info) {
    codec = InitCodecUncompressed(context, std::move(buffer), *image_info,
                                  std::move(unref_queue), decodedCacheRatioCap,
                                  trace_id);
  } else {
    codec = InitCodec(context, std::move(buffer), std::move(unref_queue),
                      decodedCacheRatioCap, trace_id);
  }
  ui_task_runner->PostTask(
      fml::MakeCopyable([callback = callback,
                         codec = std::move(codec), trace_id]() mutable {
        InvokeCodecCallback(std::move(codec), callback, trace_id);
      }));
}

char* InstantiateImageCodec(const uint8_t* list, int count, std::unique_ptr<ImageInfo> image_info, void (*callback)(fml::RefPtr<Codec> codec), const float decodedCacheRatioCap) {
  static size_t trace_counter = 1;
  const size_t trace_id = trace_counter++;
  TRACE_FLOW_BEGIN("flutter", kInitCodecTraceTag, trace_id);

  if (callback == nullptr) {
    TRACE_FLOW_END("flutter", kInitCodecTraceTag, trace_id);
    return "Callback must be a function";
  }

  if (image_info) {
    int expected_size = image_info->row_bytes * image_info->sk_info.height();
    if (count < expected_size) {
      TRACE_FLOW_END("flutter", kInitCodecTraceTag, trace_id);
      return "Pixel buffer size does not match image size";
    }
  }

  auto buffer = SkData::MakeWithCopy(list, count);

  auto* dart_state = UIDartState::Current();

  const auto& task_runners = dart_state->GetTaskRunners();
  task_runners.GetIOTaskRunner()->PostTask(fml::MakeCopyable(
      [callback = callback,
       buffer = std::move(buffer), trace_id, image_info = std::move(image_info),
       ui_task_runner = task_runners.GetUITaskRunner(),
       context = dart_state->GetResourceContext(),
       queue = UIDartState::Current()->GetSkiaUnrefQueue(),
       decodedCacheRatioCap = decodedCacheRatioCap]() mutable {
        InitCodecAndInvokeCodecCallback(std::move(ui_task_runner), context,
                                        std::move(queue), std::move(callback),
                                        std::move(buffer),
                                        std::move(image_info), 
                                        decodedCacheRatioCap, trace_id);
      }));
  return nullptr;
}

bool copy_to(SkBitmap* dst, SkColorType dstColorType, const SkBitmap& src) {
  SkPixmap srcPM;
  if (!src.peekPixels(&srcPM)) {
    return false;
  }

  SkBitmap tmpDst;
  SkImageInfo dstInfo = srcPM.info().makeColorType(dstColorType);
  if (!tmpDst.setInfo(dstInfo)) {
    return false;
  }

  if (!tmpDst.tryAllocPixels()) {
    return false;
  }

  SkPixmap dstPM;
  if (!tmpDst.peekPixels(&dstPM)) {
    return false;
  }

  if (!srcPM.readPixels(dstPM)) {
    return false;
  }

  dst->swap(tmpDst);
  return true;
}

void InvokeNextFrameCallback(fml::RefPtr<FrameInfo> frameInfo,
                             void (*callback)(fml::RefPtr<FrameInfo> frameIsssnfo),
                             size_t trace_id) {

  (*callback)(frameInfo);
  TRACE_FLOW_END("flutter", kCodecNextFrameTraceTag, trace_id);
}

}  // namespace
/*
IMPLEMENT_WRAPPERTYPEINFO(ui, Codec);

#define FOR_EACH_BINDING(V) \
  V(Codec, getNextFrame)    \
  V(Codec, frameCount)      \
  V(Codec, repetitionCount) \
  V(Codec, dispose)

FOR_EACH_BINDING(DART_NATIVE_CALLBACK)
*/
void Codec::dispose() {
  //ClearDartWrapper();
}

MultiFrameCodec::MultiFrameCodec(std::unique_ptr<SkCodec> codec,
                                 const float decodedCacheRatioCap)
    : codec_(std::move(codec)), decodedCacheRatioCap_(decodedCacheRatioCap) {
  repetitionCount_ = codec_->getRepetitionCount();
  frameInfos_ = codec_->getFrameInfo();
  compressedSizeBytes_ = codec_->getInfo().computeMinByteSize();
  frameBitmaps_.clear();
  decodedCacheSize_ = 0;
  // Initialize the frame cache, marking frames that are required for other
  // dependent frames to render.
  for (size_t frameIndex = 0; frameIndex < frameInfos_.size(); frameIndex++) {
    const auto& frameInfo = frameInfos_[frameIndex];
    if (frameInfo.fRequiredFrame != SkCodec::kNoFrame) {
      frameBitmaps_[frameInfo.fRequiredFrame] =
          std::make_unique<DecodedFrame>(/*required=*/true);
    }
    if (frameBitmaps_.count(frameIndex) < 1) {
      frameBitmaps_[frameIndex] =
          std::make_unique<DecodedFrame>(/*required=*/false);
    }
  }
  nextFrameIndex_ = 0;
}

MultiFrameCodec::~MultiFrameCodec() {}

int MultiFrameCodec::frameCount() {
  return frameInfos_.size();
}

int MultiFrameCodec::repetitionCount() {
  return repetitionCount_;
}

sk_sp<SkImage> MultiFrameCodec::GetNextFrameImage(
    fml::WeakPtr<GrContext> resourceContext) {
  // Populate this bitmap from the cache if it exists
  DecodedFrame& cacheEntry = *frameBitmaps_[nextFrameIndex_];
  SkBitmap bitmap =
      cacheEntry.bitmap_ != nullptr ? *cacheEntry.bitmap_ : SkBitmap();
  if (!bitmap.getPixels()) {  // We haven't decoded this frame yet
    const SkImageInfo info = codec_->getInfo().makeColorType(kN32_SkColorType);
    bitmap.allocPixels(info);

    SkCodec::Options options;
    options.fFrameIndex = nextFrameIndex_;
    const int requiredFrame = frameInfos_[nextFrameIndex_].fRequiredFrame;
    if (requiredFrame != SkCodec::kNoFrame) {
      const SkBitmap* requiredBitmap =
          frameBitmaps_[requiredFrame]->bitmap_.get();
      if (requiredBitmap == nullptr) {
        FML_LOG(ERROR) << "Frame " << nextFrameIndex_ << " depends on frame "
                       << requiredFrame << " which has not been cached.";
        return NULL;
      }

      if (requiredBitmap->getPixels() &&
          copy_to(&bitmap, requiredBitmap->colorType(), *requiredBitmap)) {
        options.fPriorFrame = requiredFrame;
      }
    }

    if (SkCodec::kSuccess != codec_->getPixels(info, bitmap.getPixels(),
                                               bitmap.rowBytes(), &options)) {
      FML_LOG(ERROR) << "Could not getPixels for frame " << nextFrameIndex_;
      return NULL;
    }

    // Cache the bitmap if this is a required frame or if we're still under our
    // ratio cap.
    const size_t cachedFrameSize = bitmap.computeByteSize();
    if (cacheEntry.required_ ||
        ((decodedCacheSize_ + cachedFrameSize) / compressedSizeBytes_) <=
            decodedCacheRatioCap_) {
      cacheEntry.bitmap_ = std::make_unique<SkBitmap>(bitmap);
      decodedCacheSize_ += cachedFrameSize;
    }
  }

  if (resourceContext) {
    SkPixmap pixmap(bitmap.info(), bitmap.pixelRef()->pixels(),
                    bitmap.pixelRef()->rowBytes());
    // This indicates that we do not want a "linear blending" decode.
    sk_sp<SkColorSpace> dstColorSpace = nullptr;
    return SkImage::MakeCrossContextFromPixmap(resourceContext.get(), pixmap,
                                               false, dstColorSpace.get());
  } else {
    // Defer decoding until time of draw later on the GPU thread. Can happen
    // when GL operations are currently forbidden such as in the background
    // on iOS.
    return SkImage::MakeFromBitmap(bitmap);
  }
}

void MultiFrameCodec::GetNextFrameAndInvokeCallback(
    void (*callback)(fml::RefPtr<FrameInfo> frameInfo),
    fml::RefPtr<fml::TaskRunner> ui_task_runner,
    fml::WeakPtr<GrContext> resourceContext,
    fml::RefPtr<flow::SkiaUnrefQueue> unref_queue,
    size_t trace_id) {
  fml::RefPtr<FrameInfo> frameInfo = NULL;
  sk_sp<SkImage> skImage = GetNextFrameImage(resourceContext);
  if (skImage) {
    fml::RefPtr<CanvasImage> image = CanvasImage::Create();
    image->set_image({skImage, std::move(unref_queue)});
    frameInfo = fml::MakeRefCounted<FrameInfo>(
        std::move(image), frameInfos_[nextFrameIndex_].fDuration);
  }
  nextFrameIndex_ = (nextFrameIndex_ + 1) % frameInfos_.size();

  ui_task_runner->PostTask(fml::MakeCopyable(
      [callback = callback, frameInfo, trace_id]() mutable {
        InvokeNextFrameCallback(frameInfo, callback, trace_id);
      }));

  TRACE_FLOW_END("flutter", kCodecNextFrameTraceTag, trace_id);
}

char* MultiFrameCodec::getNextFrame(void (*callback)(fml::RefPtr<FrameInfo> frameInfo)) {
  static size_t trace_counter = 1;
  const size_t trace_id = trace_counter++;
  TRACE_FLOW_BEGIN("flutter", kCodecNextFrameTraceTag, trace_id);

  if (callback == nullptr) {
    TRACE_FLOW_END("flutter", kCodecNextFrameTraceTag, trace_id);
    return "Callback is null";
  }

  auto* dart_state = UIDartState::Current();

  const auto& task_runners = dart_state->GetTaskRunners();

  task_runners.GetIOTaskRunner()->PostTask(fml::MakeCopyable(
      [callback = callback,
       this, trace_id, ui_task_runner = task_runners.GetUITaskRunner(),
       queue = UIDartState::Current()->GetSkiaUnrefQueue(),
       context = dart_state->GetResourceContext()]() mutable {
        GetNextFrameAndInvokeCallback(std::move(callback),
                                      std::move(ui_task_runner), context,
                                      std::move(queue), trace_id);
      }));

  return nullptr;
}

MultiFrameCodec::DecodedFrame::DecodedFrame(bool required)
    : required_(required) {}

MultiFrameCodec::DecodedFrame::~DecodedFrame() = default;

SingleFrameCodec::SingleFrameCodec(fml::RefPtr<FrameInfo> frame)
    : frame_(std::move(frame)) {}

SingleFrameCodec::~SingleFrameCodec() {}

int SingleFrameCodec::frameCount() {
  return 1;
}

int SingleFrameCodec::repetitionCount() {
  return 0;
}

char* SingleFrameCodec::getNextFrame(void (*callback)(fml::RefPtr<FrameInfo> frameInfo)) {
  if (callback == nullptr) {
    return "Callback is null";
  }

  (*callback) (frame_);
  return nullptr;
}

}  // namespace blink
