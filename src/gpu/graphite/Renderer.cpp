/*
 * Copyright 2022 Google LLC
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#include "src/gpu/graphite/Renderer.h"

namespace skgpu::graphite {

static uint32_t next_id() {
    static std::atomic<int32_t> nextID{0};
    // Not worried about overflow since each Context won't have that many RenderSteps, so even if
    // it wraps back to 0, that RenderStep will not be in the same Context as the original 0.
    return nextID.fetch_add(1, std::memory_order_relaxed);
}

RenderStep::RenderStep(std::string_view className,
               std::string_view variantName,
               SkEnumBitMask<Flags> flags,
               std::initializer_list<SkUniform> uniforms,
               PrimitiveType primitiveType,
               DepthStencilSettings depthStencilSettings,
               std::initializer_list<Attribute> vertexAttrs,
               std::initializer_list<Attribute> instanceAttrs,
               std::initializer_list<Varying> varyings)
        : fUniqueID(next_id())
        , fFlags(flags)
        , fPrimitiveType(primitiveType)
        , fDepthStencilSettings(depthStencilSettings)
        , fUniforms(uniforms)
        , fVertexAttrs(vertexAttrs)
        , fInstanceAttrs(instanceAttrs)
        , fVaryings(varyings)
        , fVertexStride(0)
        , fInstanceStride(0)
        , fName(className) {
    for (auto v : this->vertexAttributes()) {
        fVertexStride += v.sizeAlign4();
    }
    for (auto i : this->instanceAttributes()) {
        fInstanceStride += i.sizeAlign4();
    }
    if (variantName.size() > 0) {
        fName += "[";
        fName += variantName;
        fName += "]";
    }
}

} // namespace skgpu::graphite
