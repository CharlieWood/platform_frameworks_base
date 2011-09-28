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

#ifndef ANDROID_HWUI_OPENGL_DEBUG_RENDERER_H
#define ANDROID_HWUI_OPENGL_DEBUG_RENDERER_H

#include "OpenGLRenderer.h"

namespace android {
namespace uirenderer {

///////////////////////////////////////////////////////////////////////////////
// Renderer
///////////////////////////////////////////////////////////////////////////////

class OpenGLDebugRenderer: public OpenGLRenderer {
public:
    OpenGLDebugRenderer(): mPrimitivesCount(0) {
    }

    ~OpenGLDebugRenderer() {
    }

    void prepareDirty(float left, float top, float right, float bottom, bool opaque);
    void finish();

    int saveLayer(float left, float top, float right, float bottom,
            SkPaint* p, int flags);

    bool drawDisplayList(DisplayList* displayList, uint32_t level = 0);
    void drawLayer(Layer* layer, float x, float y, SkPaint* paint);
    void drawBitmap(SkBitmap* bitmap, float left, float top, SkPaint* paint);
    void drawBitmap(SkBitmap* bitmap, SkMatrix* matrix, SkPaint* paint);
    void drawBitmap(SkBitmap* bitmap, float srcLeft, float srcTop,
            float srcRight, float srcBottom, float dstLeft, float dstTop,
            float dstRight, float dstBottom, SkPaint* paint);
    void drawPatch(SkBitmap* bitmap, const int32_t* xDivs, const int32_t* yDivs,
            const uint32_t* colors, uint32_t width, uint32_t height, int8_t numColors,
            float left, float top, float right, float bottom, SkPaint* paint);
    void drawColor(int color, SkXfermode::Mode mode);
    void drawRect(float left, float top, float right, float bottom, SkPaint* paint);
    void drawRoundRect(float left, float top, float right, float bottom,
            float rx, float ry, SkPaint* paint);
    void drawCircle(float x, float y, float radius, SkPaint* paint);
    void drawOval(float left, float top, float right, float bottom, SkPaint* paint);
    void drawArc(float left, float top, float right, float bottom,
            float startAngle, float sweepAngle, bool useCenter, SkPaint* paint);
    void drawPath(SkPath* path, SkPaint* paint);
    void drawLines(float* points, int count, SkPaint* paint);
    void drawText(const char* text, int bytesCount, int count, float x, float y,
            SkPaint* paint);

protected:
    void composeLayer(sp<Snapshot> current, sp<Snapshot> previous);

private:
    uint32_t mPrimitivesCount;

}; // class OpenGLDebugRenderer

}; // namespace uirenderer
}; // namespace android

#endif // ANDROID_HWUI_OPENGL_DEBUG_RENDERER_H
