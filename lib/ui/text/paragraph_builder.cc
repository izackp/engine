// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "flutter/lib/ui/text/paragraph_builder.h"

#include "flutter/common/settings.h"
#include "flutter/common/task_runners.h"
#include "flutter/fml/logging.h"
#include "flutter/fml/task_runner.h"
#include "flutter/lib/ui/text/font_collection.h"
#include "flutter/lib/ui/ui_dart_state.h"
#include "flutter/lib/ui/window/window.h"
#include "flutter/third_party/txt/src/txt/font_style.h"
#include "flutter/third_party/txt/src/txt/font_weight.h"
#include "flutter/third_party/txt/src/txt/paragraph_style.h"
#include "flutter/third_party/txt/src/txt/text_decoration.h"
#include "flutter/third_party/txt/src/txt/text_style.h"
#include "third_party/icu/source/common/unicode/ustring.h"

namespace blink {
namespace {

// TextStyle

const int tsColorIndex = 1;
const int tsTextDecorationIndex = 2;
const int tsTextDecorationColorIndex = 3;
const int tsTextDecorationStyleIndex = 4;
const int tsFontWeightIndex = 5;
const int tsFontStyleIndex = 6;
const int tsTextBaselineIndex = 7; 
const int tsFontFamilyIndex = 8;
const int tsFontSizeIndex = 9;
const int tsLetterSpacingIndex = 10;
const int tsWordSpacingIndex = 11;
const int tsHeightIndex = 12;
const int tsLocaleIndex = 13;
const int tsBackgroundIndex = 14; 
const int tsForegroundIndex = 15;
const int tsTextShadowsIndex = 16;

const int tsColorMask = 1 << tsColorIndex;
const int tsTextDecorationMask = 1 << tsTextDecorationIndex;
const int tsTextDecorationColorMask = 1 << tsTextDecorationColorIndex;
const int tsTextDecorationStyleMask = 1 << tsTextDecorationStyleIndex;
const int tsFontWeightMask = 1 << tsFontWeightIndex;
const int tsFontStyleMask = 1 << tsFontStyleIndex;
const int tsTextBaselineMask = 1 << tsTextBaselineIndex;
const int tsFontFamilyMask = 1 << tsFontFamilyIndex;
const int tsFontSizeMask = 1 << tsFontSizeIndex;
const int tsLetterSpacingMask = 1 << tsLetterSpacingIndex;
const int tsWordSpacingMask = 1 << tsWordSpacingIndex;
const int tsHeightMask = 1 << tsHeightIndex;
const int tsLocaleMask = 1 << tsLocaleIndex;
const int tsBackgroundMask = 1 << tsBackgroundIndex;
const int tsForegroundMask = 1 << tsForegroundIndex;
const int tsTextShadowsMask = 1 << tsTextShadowsIndex;

// ParagraphStyle

const int psTextAlignIndex = 1;
const int psTextDirectionIndex = 2;
const int psFontWeightIndex = 3;
const int psFontStyleIndex = 4;
const int psMaxLinesIndex = 5;
const int psFontFamilyIndex = 6;
const int psFontSizeIndex = 7;
const int psLineHeightIndex = 8;
const int psEllipsisIndex = 9;
const int psLocaleIndex = 10;

const int psTextAlignMask = 1 << psTextAlignIndex;
const int psTextDirectionMask = 1 << psTextDirectionIndex;
const int psFontWeightMask = 1 << psFontWeightIndex;
const int psFontStyleMask = 1 << psFontStyleIndex;
const int psMaxLinesMask = 1 << psMaxLinesIndex;
const int psFontFamilyMask = 1 << psFontFamilyIndex;
const int psFontSizeMask = 1 << psFontSizeIndex;
const int psLineHeightMask = 1 << psLineHeightIndex;
const int psEllipsisMask = 1 << psEllipsisIndex;
const int psLocaleMask = 1 << psLocaleIndex;

// TextShadows decoding

constexpr uint32_t kColorDefault = 0xFF000000;
constexpr uint32_t kBytesPerShadow = 16;
constexpr uint32_t kShadowPropertiesCount = 4;
constexpr uint32_t kColorOffset = 0;
constexpr uint32_t kXOffset = 1;
constexpr uint32_t kYOffset = 2;
constexpr uint32_t kBlurOffset = 3;

}  // namespace
/*
static void ParagraphBuilder_constructor(Dart_NativeArguments args) {
  DartCallConstructor(&ParagraphBuilder::create, args);
}

IMPLEMENT_WRAPPERTYPEINFO(ui, ParagraphBuilder);

#define FOR_EACH_BINDING(V)      \
  V(ParagraphBuilder, pushStyle) \
  V(ParagraphBuilder, pop)       \
  V(ParagraphBuilder, addText)   \
  V(ParagraphBuilder, build)

FOR_EACH_BINDING(DART_NATIVE_CALLBACK)

void ParagraphBuilder::RegisterNatives(tonic::DartLibraryNatives* natives) {
  natives->Register(
      {{"ParagraphBuilder_constructor", ParagraphBuilder_constructor, 7, true},
       FOR_EACH_BINDING(DART_REGISTER_NATIVE)});
}*/

fml::RefPtr<ParagraphBuilder> ParagraphBuilder::create(const txt::ParagraphStyle& style) {
  return fml::MakeRefCounted<ParagraphBuilder>(style);
}

ParagraphBuilder::ParagraphBuilder(const txt::ParagraphStyle& style) {
  FontCollection& font_collection =
      UIDartState::Current()->window()->client()->GetFontCollection();
  m_paragraphBuilder = std::make_unique<txt::ParagraphBuilder>(
      style, font_collection.GetFontCollection());
}  // namespace blink

ParagraphBuilder::~ParagraphBuilder() = default;

void ParagraphBuilder::pushStyle(const txt::TextStyle& style) {
  m_paragraphBuilder->PushStyle(style);
}

void ParagraphBuilder::pop() {
  m_paragraphBuilder->Pop();
}

char* ParagraphBuilder::addText(const std::u16string& text) {
  if (text.empty())
    return nullptr;

  // Use ICU to validate the UTF-16 input.  Calling u_strToUTF8 with a null
  // output buffer will return U_BUFFER_OVERFLOW_ERROR if the input is well
  // formed.
  const UChar* text_ptr = reinterpret_cast<const UChar*>(text.data());
  UErrorCode error_code = U_ZERO_ERROR;
  u_strToUTF8(nullptr, 0, nullptr, text_ptr, text.size(), &error_code);
  if (error_code != U_BUFFER_OVERFLOW_ERROR)
    return "string is not well-formed UTF-16";

  m_paragraphBuilder->AddText(text);

  return nullptr;
}

fml::RefPtr<Paragraph> ParagraphBuilder::build() {
  return Paragraph::Create(m_paragraphBuilder->Build());
}

}  // namespace blink
