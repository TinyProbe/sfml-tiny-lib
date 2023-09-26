#pragma once
#ifndef SCRMANAGER_HPP
#define SCRMANAGER_HPP

# if _WIN32
#  include <Windows.h>
# elif __APPLE__
#  include <CoreGraphics/CoreGraphics.h>
# elif __linux__
#  include <X11/Xlib.h>
# endif

using usize = unsigned long;

class ScrManager {

public:
  ScrManager() {}
  ScrManager(ScrManager const &rhs) { *this = rhs; }
  virtual ScrManager &operator=(ScrManager const &rhs) {
    if (this == &rhs) { return *this; }
    return *this;
  }
  virtual ~ScrManager() {}

  virtual void changeResolution(usize const &wid, usize const &hei) {
# if _WIN32
    DEVMODE dm({
      .dmSize = sizeof(DEVMODE),
      .dmPelsWidth = wid,
      .dmPelsHeight = hei,
      .dmBitsPerPel = 32,
      .dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT,
    });
    if (ChangeDisplaySettings(&dm, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL) {
    }
# elif __APPLE__
    CGDisplayConfigRef config;
    CGBeginDisplayConfiguration(&config);
    CGDirectDisplayID displayID = kCGDirectMainDisplay;
    CFDictionaryRef resolutionMode
      = CGDisplayBestModeForParameters(displayID, 32, wid, hei, NULL);
    CGConfigureDisplayMode(config, displayID, resolutionMode);
    CGCompleteDisplayConfiguration(config, kCGConfigureForSession);
# elif __linux__
    XRRScreenConfiguration* screenConfig
      = XRRGetScreenInfo(display, RootWindow(display, 0));
    if (screenConfig) {
      XRRSetScreenConfig(
        display, screenConfig, RootWindow(display, 1), wid, hei, CurrentTime);
      XRRFreeScreenConfigInfo(screenConfig);
    }
# endif
  }

};

#endif
