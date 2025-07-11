#pragma once

#include "Geometry/Point.h"
#include "Geometry/Rect.h"
#include "Geometry/Size.h"

namespace GL
{
class Monitor;
class MonitorList;
class Window;
class VideoMode;
class VideoModeList;
namespace Application
{
GL::Monitor primaryMonitor ();
}

class Monitor
{
public:
  ~Monitor ();

  Geometry::Point pos () const;
  Geometry::Rect workRect () const;
  Geometry::Size physicalSize () const;
  void contentScale (float *xScale, float *yScale) const;
  const char *name () const;

  GL::VideoMode videoMode () const;
  GL::VideoModeList videoModeList () const;

  bool operator== (const Monitor &rhs) const;

private:
  friend GL::Monitor GL::Application::primaryMonitor ();
  friend class GL::MonitorList;
  friend class GL::Window;

  Monitor (void *impl);

  Monitor (const Monitor &) = delete;
  Monitor &operator= (const Monitor &) = delete;

  void *mPimpl = nullptr;
};

class MonitorList
{
public:
  ~MonitorList ();

  int count () const;
  Monitor operator[] (int i) const;

private:
  MonitorList ();

  void *mPimpl = nullptr;
  int mCount;
};

class VideoMode
{
public:
  ~VideoMode ();

  int width () const;
  int height () const;

  int redBits () const;
  int greenBits () const;
  int blueBits () const;

  int refreshRate () const;

private:
  friend class Monitor;
  friend class VideoModeList;
  VideoMode (const void *impl);

  const void *mPimpl = nullptr;
};

class VideoModeList
{
public:
  ~VideoModeList ();

  int count () const;
  VideoMode operator[] (int i) const;

private:
  friend class Monitor;
  VideoModeList (void *impl);

  const void *mPimpl = nullptr;
  int mCount;
};
}  // namespace GL
