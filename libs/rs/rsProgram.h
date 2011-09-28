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

#ifndef ANDROID_RS_PROGRAM_H
#define ANDROID_RS_PROGRAM_H

#include "rsObjectBase.h"
#include "rsElement.h"

// ---------------------------------------------------------------------------
namespace android {
namespace renderscript {
class ShaderCache;

#define RS_SHADER_INTERNAL "//rs_shader_internal\n"
#define RS_SHADER_ATTR "ATTRIB_"
#define RS_SHADER_UNI "UNI_"

class Program : public ObjectBase {
public:

    Program(Context *);
    Program(Context *, const char * shaderText, uint32_t shaderLength,
                       const uint32_t * params, uint32_t paramLength);
    virtual ~Program();

    void bindAllocation(Context *, Allocation *, uint32_t slot);
    virtual void createShader();

    bool isUserProgram() const {return !mIsInternal;}

    void bindTexture(Context *, uint32_t slot, Allocation *);
    void bindSampler(Context *, uint32_t slot, Sampler *);

    uint32_t getShaderID() const {return mShaderID;}
    void setShader(const char *, uint32_t len);

    uint32_t getAttribCount() const {return mAttribCount;}
    uint32_t getUniformCount() const {return mUniformCount;}
    const String8 & getAttribName(uint32_t i) const {return mAttribNames[i];}
    const String8 & getUniformName(uint32_t i) const {return mUniformNames[i];}
    uint32_t getUniformArraySize(uint32_t i) const {return mUniformArraySizes[i];}

    String8 getGLSLInputString() const;
    String8 getGLSLOutputString() const;
    String8 getGLSLConstantString() const;

    bool isValid() const {return mIsValid;}
    void forceDirty() const {mDirty = true;}

protected:
    // Components not listed in "in" will be passed though
    // unless overwritten by components in out.
    ObjectBaseRef<Element> *mInputElements;
    ObjectBaseRef<Element> *mOutputElements;
    ObjectBaseRef<Type> *mConstantTypes;
    ObjectBaseRef<Allocation> *mConstants;
    uint32_t mInputCount;
    uint32_t mOutputCount;
    uint32_t mConstantCount;
    bool mIsValid;
    bool mIsInternal;

    // Applies to vertex and fragment shaders only
    void appendUserConstants();
    void setupUserConstants(Context *rsc, ShaderCache *sc, bool isFragment);
    void initAddUserElement(const Element *e, String8 *names, uint32_t *arrayLengths, uint32_t *count, const char *prefix);

    void initAttribAndUniformArray();

    mutable bool mDirty;
    String8 mShader;
    String8 mUserShader;
    uint32_t mShaderID;

    uint32_t mTextureCount;
    uint32_t mAttribCount;
    uint32_t mUniformCount;
    String8 *mAttribNames;
    String8 *mUniformNames;
    uint32_t *mUniformArraySizes;

    void logUniform(const Element *field, const float *fd, uint32_t arraySize );
    void setUniform(Context *rsc, const Element *field, const float *fd, int32_t slot, uint32_t arraySize );
    void initMemberVars();

    // The difference between Textures and Constants is how they are accessed
    // Texture lookups go though a sampler which in effect converts normalized
    // coordinates into type specific.  Multiple samples may also be taken
    // and filtered.
    //
    // Constants are strictly accessed by programetic loads.
    ObjectBaseRef<Allocation> *mTextures;
    ObjectBaseRef<Sampler> *mSamplers;
    RsTextureTarget *mTextureTargets;
    bool loadShader(Context *, uint32_t type);
};

}
}
#endif



