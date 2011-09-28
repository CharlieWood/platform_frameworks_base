/*
 * Copyright (C) 2009 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "rsContext.h"
#ifndef ANDROID_RS_SERIALIZE
#include <GLES/gl.h>
#include <GLES2/gl2.h>
#endif

using namespace android;
using namespace android::renderscript;

VertexArray::VertexArray(const Attrib *attribs, uint32_t numAttribs) {
    mAttribs = attribs;
    mCount = numAttribs;
}

VertexArray::~VertexArray() {
}

VertexArray::Attrib::Attrib() {
    clear();
}

void VertexArray::Attrib::clear() {
    buffer = 0;
    offset = 0;
    type = 0;
    size = 0;
    stride = 0;
    ptr = NULL;
    normalized = false;
    name.setTo("");
}

void VertexArray::Attrib::set(uint32_t type, uint32_t size, uint32_t stride,
                              bool normalized, uint32_t offset,
                              const char *name) {
    clear();
    this->type = type;
    this->size = size;
    this->offset = offset;
    this->normalized = normalized;
    this->stride = stride;
    this->name.setTo(name);
}

void VertexArray::logAttrib(uint32_t idx, uint32_t slot) const {
    if (idx == 0) {
        LOGV("Starting vertex attribute binding");
    }
    LOGV("va %i: slot=%i name=%s buf=%i ptr=%p size=%i  type=0x%x  stride=0x%x  norm=%i  offset=0x%x",
         idx, slot,
         mAttribs[idx].name.string(),
         mAttribs[idx].buffer,
         mAttribs[idx].ptr,
         mAttribs[idx].size,
         mAttribs[idx].type,
         mAttribs[idx].stride,
         mAttribs[idx].normalized,
         mAttribs[idx].offset);
}

void VertexArray::setupGL2(const Context *rsc,
                           class VertexArrayState *state,
                           ShaderCache *sc) const {
    rsc->checkError("VertexArray::setupGL2 start");
    uint32_t maxAttrs = state->mAttrsEnabledSize;
    for (uint32_t ct=1; ct < maxAttrs; ct++) {
        if(state->mAttrsEnabled[ct]) {
            glDisableVertexAttribArray(ct);
            state->mAttrsEnabled[ct] = false;
        }
    }

    rsc->checkError("VertexArray::setupGL2 disabled");
    for (uint32_t ct=0; ct < mCount; ct++) {
        int32_t slot = sc->vtxAttribSlot(mAttribs[ct].name);
        if (rsc->props.mLogShadersAttr) {
            logAttrib(ct, slot);
        }
        if (slot < 0 || slot >= (int32_t)maxAttrs) {
            continue;
        }
        glEnableVertexAttribArray(slot);
        state->mAttrsEnabled[slot] = true;
        glBindBuffer(GL_ARRAY_BUFFER, mAttribs[ct].buffer);
        glVertexAttribPointer(slot,
                              mAttribs[ct].size,
                              mAttribs[ct].type,
                              mAttribs[ct].normalized,
                              mAttribs[ct].stride,
                              mAttribs[ct].ptr + mAttribs[ct].offset);
    }
    rsc->checkError("VertexArray::setupGL2 done");
}
////////////////////////////////////////////
VertexArrayState::VertexArrayState() {
    mAttrsEnabled = NULL;
    mAttrsEnabledSize = 0;
}

VertexArrayState::~VertexArrayState() {
    if (mAttrsEnabled) {
        delete[] mAttrsEnabled;
        mAttrsEnabled = NULL;
    }
}
void VertexArrayState::init(Context *rsc) {
    mAttrsEnabledSize = rsc->getMaxVertexAttributes();
    mAttrsEnabled = new bool[mAttrsEnabledSize];
    for (uint32_t ct = 0; ct < mAttrsEnabledSize; ct++) {
        mAttrsEnabled[ct] = false;
    }
}

