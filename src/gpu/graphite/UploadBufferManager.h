/*
 * Copyright 2022 Google LLC
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef skgpu_graphite_UploadBufferManager_DEFINED
#define skgpu_graphite_UploadBufferManager_DEFINED

#include <tuple>
#include <vector>

#include "include/core/SkRefCnt.h"
#include "src/gpu/BufferWriter.h"
#include "src/gpu/graphite/DrawTypes.h"

namespace skgpu::graphite {

class Buffer;
class Caps;
class CommandBuffer;
class Recording;
class ResourceProvider;

class UploadBufferManager {
public:
    UploadBufferManager(ResourceProvider*, const Caps*);
    ~UploadBufferManager();

    std::tuple<TextureUploadWriter, BindBufferInfo> getTextureUploadWriter(
            size_t requiredBytes, size_t requiredAlignment);

    // Finalizes all buffers and transfers ownership of them to a Recording.
    void transferToRecording(Recording*);
    void transferToCommandBuffer(CommandBuffer*);

private:
    friend class DrawBufferManager; // to access makeBindInfo
    friend class StaticBufferManager; // to access makeBindInfo


    std::tuple<void*/*mappedPtr*/, BindBufferInfo> makeBindInfo(size_t requiredBytes,
                                                                size_t requiredAlignment);

    ResourceProvider* fResourceProvider;

    sk_sp<Buffer> fReusedBuffer;
    size_t fMinAlignment;
    size_t fReusedBufferOffset = 0;

    std::vector<sk_sp<Buffer>> fUsedBuffers;
};

}  // namespace skgpu::graphite

#endif  // skgpu_graphite_UploadBufferManager_DEFINED
