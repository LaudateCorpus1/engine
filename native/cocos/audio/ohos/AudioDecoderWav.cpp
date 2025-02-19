/****************************************************************************
 Copyright (c) 2016 Chukong Technologies Inc.
 Copyright (c) 2017-2022 Xiamen Yaji Software Co., Ltd.

 http://www.cocos.com

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated engine source code (the "Software"), a limited,
 worldwide, royalty-free, non-assignable, revocable and non-exclusive license
 to use Cocos Creator solely to develop games on your target platforms. You shall
 not use Cocos Creator software for developing other software or tools that's
 used for developing games. You are not granted to publish, distribute,
 sublicense, and/or sell copies of Cocos Creator.

 The software or tools in this License Agreement are licensed, not sold.
 Xiamen Yaji Software Co., Ltd. reserves all rights not expressly granted to you.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
****************************************************************************/

#define LOG_TAG "AudioDecoderWav"

#include "audio/ohos/AudioDecoderWav.h"
#include "audio/ohos/FsCallback.h"
#include "base/Log.h"

namespace {
snd_callbacks wavCallbacks = {
    .open  = cc::ohosOpen,
    .read  = cc::ohosRead,
    .seek  = cc::ohosSeek,
    .close = cc::ohosClose,
    .tell  = cc::ohosTell,
};
}

namespace cc {

AudioDecoderWav::AudioDecoderWav() {
    CC_LOG_DEBUG("Create AudioDecoderWav");
}

AudioDecoderWav::~AudioDecoderWav() {
    close();
}

bool AudioDecoderWav::open(const char *path) {
    _sndHandle = sf_open_read(path, &_sndInfo, &wavCallbacks, this);
    _isOpened  = (_sndHandle != nullptr) && _sndInfo.frames > 0;
    if (!_isOpened) return false;

    _channelCount  = _sndInfo.channels;
    _sampleRate    = _sndInfo.samplerate;
    _bytesPerFrame = 2 * _channelCount; // short
    _totalFrames   = _sndInfo.frames;
    return true;
}

void AudioDecoderWav::close() {
    if (_sndHandle) {
        sf_close(_sndHandle);
        _sndHandle = nullptr;
    }
    _isOpened = false;
}

uint32_t AudioDecoderWav::read(uint32_t framesToRead, char *pcmBuf) {
    auto *output       = reinterpret_cast<int16_t *>(pcmBuf);
    auto  actualFrames = sf_readf_short(_sndHandle, output, framesToRead);
    return actualFrames;
}

bool AudioDecoderWav::seek(uint32_t frameOffset) {
    off_t offset = sf_seek(_sndHandle, frameOffset, SEEK_SET);
    return offset >= 0 && offset == frameOffset;
}

uint32_t AudioDecoderWav::tell() const {
    return sf_tell(_sndHandle);
}

} // namespace cc