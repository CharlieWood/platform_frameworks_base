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

#define LOG_TAG "OpenGLRenderer"

#include <utils/StopWatch.h>

#include "OpenGLDebugRenderer.h"

namespace android {
namespace uirenderer {

void OpenGLDebugRenderer::prepareDirty(float left, float top,
        float right, float bottom, bool opaque) {
    mPrimitivesCount = 0;
    LOGD("========= Frame start =========");
    OpenGLRenderer::prepareDirty(left, top, right, bottom, opaque);
}

void OpenGLDebugRenderer::finish() {
    LOGD("========= Frame end =========");
    LOGD("Primitives draw count = %d", mPrimitivesCount);
    OpenGLRenderer::finish();
}

void OpenGLDebugRenderer::composeLayer(sp<Snapshot> current, sp<Snapshot> previous) {
    mPrimitivesCount++;
    StopWatch w("composeLayer");
    return OpenGLRenderer::composeLayer(current, previous);
}

int OpenGLDebugRenderer::saveLayer(float left, float top, float right, float bottom,
        SkPaint* p, int flags) {
    mPrimitivesCount++;
    StopWatch w("saveLayer");
    return OpenGLRenderer::saveLayer(left, top, right, bottom, p, flags);
}

bool OpenGLDebugRenderer::drawDisplayList(DisplayList* displayList, uint32_t level) {
    mPrimitivesCount++;
    StopWatch w("drawDisplayList");
    return OpenGLRenderer::drawDisplayList(displayList);
}

void OpenGLDebugRenderer::drawLayer(Layer* layer, float x, float y, SkPaint* paint) {
    mPrimitivesCount++;
    StopWatch w("drawLayer");
    OpenGLRenderer::drawLayer(layer, x, y, paint);
}

void OpenGLDebugRenderer::drawBitmap(SkBitmap* bitmap, float left, float top,
        SkPaint* paint) {
    mPrimitivesCount++;
    StopWatch w("drawBitmap");
    OpenGLRenderer::drawBitmap(bitmap, left, top, paint);
}

void OpenGLDebugRenderer::drawBitmap(SkBitmap* bitmap, SkMatrix* matrix,
        SkPaint* paint) {
    mPrimitivesCount++;
    StopWatch w("drawBitmapMatrix");
    OpenGLRenderer::drawBitmap(bitmap, matrix, paint);
}

void OpenGLDebugRenderer::drawBitmap(SkBitmap* bitmap, float srcLeft, float srcTop,
        float srcRight, float srcBottom, float dstLeft, float dstTop,
        float dstRight, float dstBottom, SkPaint* paint) {
    mPrimitivesCount++;
    StopWatch w("drawBitmapRect");
    OpenGLRenderer::drawBitmap(bitmap, srcLeft, srcTop, srcRight, srcBottom,
            dstLeft, dstTop, dstRight, dstBottom, paint);
}

void OpenGLDebugRenderer::drawPatch(SkBitmap* bitmap, const int32_t* xDivs, const int32_t* yDivs,
        const uint32_t* colors, uint32_t width, uint32_t height, int8_t numColors,
        float left, float top, float right, float bottom, SkPaint* paint) {
    mPrimitivesCount++;
    StopWatch w("drawPatch");
    OpenGLRenderer::drawPatch(bitmap, xDivs, yDivs, colors, width, height, numColors,
            left, top, right, bottom, paint);
}

void OpenGLDebugRenderer::drawColor(int color, SkXfermode::Mode mode) {
    mPrimitivesCount++;
    StopWatch w("drawColor");
    OpenGLRenderer::drawColor(color, mode);
}

void OpenGLDebugRenderer::drawRect(float left, float top, float right, float bottom,
        SkPaint* paint) {
    mPrimitivesCount++;
    StopWatch w("drawRect");
    OpenGLRenderer::drawRect(left, top, right, bottom, paint);
}

void OpenGLDebugRenderer::drawRoundRect(float left, float top, float right, float bottom,
        float rx, float ry, SkPaint* paint) {
    mPrimitivesCount++;
    StopWatch w("drawRoundRect");
    OpenGLRenderer::drawRoundRect(left, top, right, bottom, rx, ry, paint);
}

void OpenGLDebugRenderer::drawCircle(float x, float y, float radius, SkPaint* paint) {
    mPrimitivesCount++;
    StopWatch w("drawCircle");
    OpenGLRenderer::drawCircle(x, y, radius, paint);
}

void OpenGLDebugRenderer::drawOval(float left, float top, float right, float bottom,
        SkPaint* paint) {
    mPrimitivesCount++;
    StopWatch w("drawOval");
    OpenGLRenderer::drawOval(left, top, right, bottom, paint);
}

void OpenGLDebugRenderer::drawArc(float left, float top, float right, float bottom,
        float startAngle, float sweepAngle, bool useCenter, SkPaint* paint) {
    mPrimitivesCount++;
    StopWatch w("drawArc");
    OpenGLRenderer::drawArc(left, top, right, bottom, startAngle, sweepAngle, useCenter, paint);
}

void OpenGLDebugRenderer::drawPath(SkPath* path, SkPaint* paint) {
    mPrimitivesCount++;
    StopWatch w("drawPath");
    OpenGLRenderer::drawPath(path, paint);
}

void OpenGLDebugRenderer::drawLines(float* points, int count, SkPaint* paint) {
    mPrimitivesCount++;
    StopWatch w("drawLines");
    OpenGLRenderer::drawLines(points, count, paint);
}

void OpenGLDebugRenderer::drawText(const char* text, int bytesCount, int count, float x, float y,
        SkPaint* paint) {
    mPrimitivesCount++;
    StopWatch w("drawText");
    OpenGLRenderer::drawText(text, bytesCount, count, x, y, paint);
}

}; // namespace uirenderer
}; // namespace android
