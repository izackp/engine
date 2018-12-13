// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef FLUTTER_LIB_UI_TEXT_PARAGRAPH_BUILDER_H_
#define FLUTTER_LIB_UI_TEXT_PARAGRAPH_BUILDER_H_

#include <memory>
#include "flutter/lib/ui/dart_wrapper.h"
#include "flutter/lib/ui/text/paragraph.h"
#include "flutter/third_party/txt/src/txt/paragraph_builder.h"

namespace blink {

class Paragraph;

class ParagraphBuilder : public RefCountedDartWrappable<ParagraphBuilder> {

  FML_FRIEND_MAKE_REF_COUNTED(ParagraphBuilder);

 public:
  static fml::RefPtr<ParagraphBuilder> create(const txt::ParagraphStyle& style);

  ~ParagraphBuilder();

  void pushStyle(const txt::TextStyle& style);

  void pop();

  char* addText(const std::u16string& text);

  fml::RefPtr<Paragraph> build();

 private:
  explicit ParagraphBuilder(const txt::ParagraphStyle& style);

  std::unique_ptr<txt::ParagraphBuilder> m_paragraphBuilder;
};

}  // namespace blink

#endif  // FLUTTER_LIB_UI_TEXT_PARAGRAPH_BUILDER_H_
