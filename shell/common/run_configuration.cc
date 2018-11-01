// Copyright 2017 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "flutter/shell/common/run_configuration.h"

#include <sstream>

#include "flutter/assets/directory_asset_bundle.h"
#include "flutter/fml/file.h"

namespace shell {

RunConfiguration RunConfiguration::InferFromSettings(
    const blink::Settings& settings) {
  auto asset_manager = fml::MakeRefCounted<blink::AssetManager>();

  asset_manager->PushBack(std::make_unique<blink::DirectoryAssetBundle>(
      fml::Duplicate(settings.assets_dir)));

  asset_manager->PushBack(
      std::make_unique<blink::DirectoryAssetBundle>(fml::OpenFile(
          settings.assets_path.c_str(), fml::OpenPermission::kRead, true)));

  return {asset_manager};
}

RunConfiguration::RunConfiguration()
    : RunConfiguration(fml::MakeRefCounted<blink::AssetManager>()) {}

RunConfiguration::RunConfiguration(
    fml::RefPtr<blink::AssetManager> asset_manager)
    : asset_manager_(std::move(asset_manager)) {}

RunConfiguration::RunConfiguration(RunConfiguration&&) = default;

RunConfiguration::~RunConfiguration() = default;

bool RunConfiguration::IsValid() const {
  return (asset_manager_.get() != nullptr);
}

bool RunConfiguration::AddAssetResolver(
    std::unique_ptr<blink::AssetResolver> resolver) {
  if (!resolver || !resolver->IsValid()) {
    return false;
  }

  asset_manager_->PushBack(std::move(resolver));
  return true;
}

void RunConfiguration::SetEntrypoint(std::string entrypoint) {
  entrypoint_ = std::move(entrypoint);
}

void RunConfiguration::SetEntrypointAndLibrary(std::string entrypoint,
                                               std::string library) {
  SetEntrypoint(entrypoint);
  entrypoint_library_ = std::move(library);
}

fml::RefPtr<blink::AssetManager> RunConfiguration::GetAssetManager() const {
  return asset_manager_;
}

const std::string& RunConfiguration::GetEntrypoint() const {
  return entrypoint_;
}

const std::string& RunConfiguration::GetEntrypointLibrary() const {
  return entrypoint_library_;
}

}  // namespace shell
