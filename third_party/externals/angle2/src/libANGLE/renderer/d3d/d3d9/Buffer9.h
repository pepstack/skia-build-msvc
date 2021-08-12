//
// Copyright 2014 The ANGLE Project Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
//

// Buffer9.h: Defines the rx::Buffer9 class which implements rx::BufferImpl via rx::BufferD3D.

#ifndef LIBANGLE_RENDERER_D3D_D3D9_BUFFER9_H_
#define LIBANGLE_RENDERER_D3D_D3D9_BUFFER9_H_

#include "common/MemoryBuffer.h"
#include "libANGLE/angletypes.h"
#include "libANGLE/renderer/d3d/BufferD3D.h"

namespace rx
{
class Renderer9;

class Buffer9 : public BufferD3D
{
  public:
    Buffer9(const gl::BufferState &state, Renderer9 *renderer);
    virtual ~Buffer9();

    // BufferD3D implementation
    virtual size_t getSize() const { return mSize; }
    virtual bool supportsDirectBinding() const { return false; }
    gl::Error getData(const uint8_t **outData) override;

    // BufferImpl implementation
    gl::Error setData(const gl::Context *context,
                      GLenum target,
                      const void *data,
                      size_t size,
                      GLenum usage) override;
    gl::Error setSubData(const gl::Context *context,
                         GLenum target,
                         const void *data,
                         size_t size,
                         size_t offset) override;
    gl::Error copySubData(const gl::Context *context,
                          BufferImpl *source,
                          GLintptr sourceOffset,
                          GLintptr destOffset,
                          GLsizeiptr size) override;
    gl::Error map(const gl::Context *context, GLenum access, void **mapPtr) override;
    gl::Error mapRange(const gl::Context *context,
                       size_t offset,
                       size_t length,
                       GLbitfield access,
                       void **mapPtr) override;
    gl::Error unmap(const gl::Context *context, GLboolean *result) override;
    gl::Error markTransformFeedbackUsage() override;

  private:
    angle::MemoryBuffer mMemory;
    size_t mSize;
};

}  // namespace rx

#endif  // LIBANGLE_RENDERER_D3D_D3D9_BUFFER9_H_
