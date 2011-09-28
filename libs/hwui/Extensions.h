/*
 * Copyright (C) 2010 The Android Open Source Project
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

#ifndef ANDROID_HWUI_EXTENSIONS_H
#define ANDROID_HWUI_EXTENSIONS_H

#include <utils/SortedVector.h>
#include <utils/String8.h>

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

namespace android {
namespace uirenderer {

///////////////////////////////////////////////////////////////////////////////
// Defines
///////////////////////////////////////////////////////////////////////////////

// Debug
#define DEBUG_EXTENSIONS 0

// Debug
#if DEBUG_EXTENSIONS
    #define EXT_LOGD(...) LOGD(__VA_ARGS__)
#else
    #define EXT_LOGD(...)
#endif

class Extensions {
public:
    Extensions() {
        const char* buffer = (const char*) glGetString(GL_EXTENSIONS);
        const char* current = buffer;
        const char* head = current;
        EXT_LOGD("Available GL extensions:");
        do {
            head = strchr(current, ' ');
            String8 s(current, head ? head - current : strlen(current));
            if (s.length()) {
                mExtensionList.add(s);
                EXT_LOGD("  %s", s.string());
            }
            current = head + 1;
        } while (head);

        mHasNPot = hasExtension("GL_OES_texture_npot");
        mHasDrawPath = hasExtension("GL_NV_draw_path");
        mHasCoverageSample = hasExtension("GL_NV_coverage_sample");
        mHasFramebufferFetch = hasExtension("GL_NV_shader_framebuffer_fetch");

        mExtensions = buffer;
    }

    inline bool hasNPot() const { return mHasNPot; }
    inline bool hasDrawPath() const { return mHasDrawPath; }
    inline bool hasCoverageSample() const { return mHasCoverageSample; }
    inline bool hasFramebufferFetch() const { return mHasFramebufferFetch; }

    bool hasExtension(const char* extension) const {
        const String8 s(extension);
        return mExtensionList.indexOf(s) >= 0;
    }

    void dump() {
        LOGD("Supported extensions:\n%s", mExtensions);
    }

private:
    SortedVector<String8> mExtensionList;

    const char* mExtensions;

    bool mHasNPot;
    bool mHasDrawPath;
    bool mHasCoverageSample;
    bool mHasFramebufferFetch;
}; // class Extensions

}; // namespace uirenderer
}; // namespace android

#endif // ANDROID_HWUI_EXTENSIONS_H
