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
#include <GLES/glext.h>
#endif //ANDROID_RS_SERIALIZE

#include "rsSampler.h"


using namespace android;
using namespace android::renderscript;


Sampler::Sampler(Context *rsc) : ObjectBase(rsc) {
    // Should not get called.
    rsAssert(0);
}

Sampler::Sampler(Context *rsc,
                 RsSamplerValue magFilter,
                 RsSamplerValue minFilter,
                 RsSamplerValue wrapS,
                 RsSamplerValue wrapT,
                 RsSamplerValue wrapR,
                 float aniso) : ObjectBase(rsc) {
    mMagFilter = magFilter;
    mMinFilter = minFilter;
    mWrapS = wrapS;
    mWrapT = wrapT;
    mWrapR = wrapR;
    mAniso = aniso;
}

Sampler::~Sampler() {
}

void Sampler::setupGL(const Context *rsc, const Allocation *tex) {
    GLenum trans[] = {
        GL_NEAREST, //RS_SAMPLER_NEAREST,
        GL_LINEAR, //RS_SAMPLER_LINEAR,
        GL_LINEAR_MIPMAP_LINEAR, //RS_SAMPLER_LINEAR_MIP_LINEAR,
        GL_REPEAT, //RS_SAMPLER_WRAP,
        GL_CLAMP_TO_EDGE, //RS_SAMPLER_CLAMP
        GL_LINEAR_MIPMAP_NEAREST, //RS_SAMPLER_LINEAR_MIP_NEAREST
    };

    GLenum transNP[] = {
        GL_NEAREST, //RS_SAMPLER_NEAREST,
        GL_LINEAR, //RS_SAMPLER_LINEAR,
        GL_LINEAR, //RS_SAMPLER_LINEAR_MIP_LINEAR,
        GL_CLAMP_TO_EDGE, //RS_SAMPLER_WRAP,
        GL_CLAMP_TO_EDGE, //RS_SAMPLER_CLAMP
        GL_LINEAR, //RS_SAMPLER_LINEAR_MIP_NEAREST,
    };

    // This tells us the correct texture type
    GLenum target = (GLenum)tex->getGLTarget();

    if (!rsc->ext_OES_texture_npot() && tex->getType()->getIsNp2()) {
        if (tex->getHasGraphicsMipmaps() &&
            (rsc->ext_GL_NV_texture_npot_2D_mipmap() || rsc->ext_GL_IMG_texture_npot())) {
            if (rsc->ext_GL_NV_texture_npot_2D_mipmap()) {
                glTexParameteri(target, GL_TEXTURE_MIN_FILTER, trans[mMinFilter]);
            } else {
                switch (trans[mMinFilter]) {
                case GL_LINEAR_MIPMAP_LINEAR:
                    glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
                    break;
                default:
                    glTexParameteri(target, GL_TEXTURE_MIN_FILTER, trans[mMinFilter]);
                    break;
                }
            }
        } else {
            glTexParameteri(target, GL_TEXTURE_MIN_FILTER, transNP[mMinFilter]);
        }
        glTexParameteri(target, GL_TEXTURE_MAG_FILTER, transNP[mMagFilter]);
        glTexParameteri(target, GL_TEXTURE_WRAP_S, transNP[mWrapS]);
        glTexParameteri(target, GL_TEXTURE_WRAP_T, transNP[mWrapT]);
    } else {
        if (tex->getHasGraphicsMipmaps()) {
            glTexParameteri(target, GL_TEXTURE_MIN_FILTER, trans[mMinFilter]);
        } else {
            glTexParameteri(target, GL_TEXTURE_MIN_FILTER, transNP[mMinFilter]);
        }
        glTexParameteri(target, GL_TEXTURE_MAG_FILTER, trans[mMagFilter]);
        glTexParameteri(target, GL_TEXTURE_WRAP_S, trans[mWrapS]);
        glTexParameteri(target, GL_TEXTURE_WRAP_T, trans[mWrapT]);
    }

    float anisoValue = rsMin(rsc->ext_texture_max_aniso(), mAniso);
    if (rsc->ext_texture_max_aniso() > 1.0f) {
        glTexParameterf(target, GL_TEXTURE_MAX_ANISOTROPY_EXT, anisoValue);
    }

    rsc->checkError("Sampler::setupGL2 tex env");
}

void Sampler::bindToContext(SamplerState *ss, uint32_t slot) {
    ss->mSamplers[slot].set(this);
    mBoundSlot = slot;
}

void Sampler::unbindFromContext(SamplerState *ss) {
    int32_t slot = mBoundSlot;
    mBoundSlot = -1;
    ss->mSamplers[slot].clear();
}

void Sampler::serialize(OStream *stream) const {
}

Sampler *Sampler::createFromStream(Context *rsc, IStream *stream) {
    return NULL;
}

////////////////////////////////

namespace android {
namespace renderscript {


void rsi_SamplerBegin(Context *rsc) {
    SamplerState * ss = &rsc->mStateSampler;

    ss->mMagFilter = RS_SAMPLER_LINEAR;
    ss->mMinFilter = RS_SAMPLER_LINEAR;
    ss->mWrapS = RS_SAMPLER_WRAP;
    ss->mWrapT = RS_SAMPLER_WRAP;
    ss->mWrapR = RS_SAMPLER_WRAP;
    ss->mAniso = 1.0f;
}

void rsi_SamplerSet(Context *rsc, RsSamplerParam param, RsSamplerValue value) {
    SamplerState * ss = &rsc->mStateSampler;

    switch (param) {
    case RS_SAMPLER_MAG_FILTER:
        ss->mMagFilter = value;
        break;
    case RS_SAMPLER_MIN_FILTER:
        ss->mMinFilter = value;
        break;
    case RS_SAMPLER_WRAP_S:
        ss->mWrapS = value;
        break;
    case RS_SAMPLER_WRAP_T:
        ss->mWrapT = value;
        break;
    case RS_SAMPLER_WRAP_R:
        ss->mWrapR = value;
        break;
    default:
        LOGE("Attempting to set invalid value on sampler");
        break;
    }
}

void rsi_SamplerSet2(Context *rsc, RsSamplerParam param, float value) {
    SamplerState * ss = &rsc->mStateSampler;

    switch (param) {
    case RS_SAMPLER_ANISO:
        ss->mAniso = value;
        break;
    default:
        LOGE("Attempting to set invalid value on sampler");
        break;
    }
}

RsSampler rsi_SamplerCreate(Context *rsc) {
    SamplerState * ss = &rsc->mStateSampler;

    Sampler * s = new Sampler(rsc,
                              ss->mMagFilter,
                              ss->mMinFilter,
                              ss->mWrapS,
                              ss->mWrapT,
                              ss->mWrapR,
                              ss->mAniso);
    s->incUserRef();
    return s;
}

}}
