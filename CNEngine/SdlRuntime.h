// SdlRuntime.h - RAII wrapper for SDL_Init / TTF_Init and shutdown.
#ifndef SDLRUNTIME_H
#define SDLRUNTIME_H

class SdlRuntime {
public:
    SdlRuntime();
    ~SdlRuntime();

private:
    SdlRuntime(const SdlRuntime&);
    SdlRuntime& operator=(const SdlRuntime&);
};

#endif // SDLRUNTIME_H