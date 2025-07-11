#include "GL/Monitor.h"
#include <GLFW/glfw3.h>

GLFWmonitor* toGLFWmonitor (void *pImpl)
{
  return static_cast<GLFWmonitor*> (pImpl);
}


namespace GL
{

Monitor::Monitor (void *impl): mPimpl (impl)
{
}

Monitor::~Monitor ()
{
}

Geometry::Point Monitor::pos () const
{
  Geometry::Point pos;
  glfwGetMonitorPos (toGLFWmonitor (mPimpl), &pos.rx (), &pos.ry ());
  return pos;
}

Geometry::Rect Monitor::workRect () const
{
  Geometry::Rect workRect;
  glfwGetMonitorWorkarea (toGLFWmonitor (mPimpl),
                          &workRect.rleftBottom ().rx (),
                          &workRect.rleftBottom ().ry (),
                          &workRect.rsize ().rwidth (),
                          &workRect.rsize ().rheight ());
  return workRect;
}

Geometry::Size Monitor::physicalSize () const
{
  Geometry::Size size;
  glfwGetMonitorPhysicalSize (toGLFWmonitor (mPimpl),
                              &size.rwidth (),
                              &size.rheight ());
  return size;
}

void Monitor::contentScale (float *xScale, float *yScale) const
{
  glfwGetMonitorContentScale (toGLFWmonitor (mPimpl), xScale, yScale);
}

const char *Monitor::name () const
{
  return glfwGetMonitorName (toGLFWmonitor (mPimpl));
}

GL::VideoMode Monitor::videoMode () const
{
  return GL::VideoMode (glfwGetVideoMode (toGLFWmonitor (mPimpl)));
}

GL::VideoModeList Monitor::videoModeList () const
{
  return GL::VideoModeList (toGLFWmonitor (mPimpl));
}

bool Monitor::operator== (const Monitor &rhs) const
{
  return mPimpl == rhs.mPimpl;
}

MonitorList::MonitorList ()
{
  mPimpl = glfwGetMonitors (&mCount);
}

MonitorList::~MonitorList () = default;

int MonitorList::count () const
{
  return mCount;
}

Monitor MonitorList::operator[] (int i) const
{
  return Monitor (*(static_cast<GLFWmonitor**> (mPimpl) + i));
}

VideoMode::~VideoMode () = default;

int VideoMode::width () const
{
  return static_cast<const GLFWvidmode *> (mPimpl)->width;
}

int VideoMode::height () const
{
  return static_cast<const GLFWvidmode *> (mPimpl)->height;
}

int VideoMode::redBits () const
{
  return static_cast<const GLFWvidmode *> (mPimpl)->redBits;
}

int VideoMode::greenBits () const
{
  return static_cast<const GLFWvidmode *> (mPimpl)->greenBits;
}

int VideoMode::blueBits () const
{
  return static_cast<const GLFWvidmode *> (mPimpl)->blueBits;
}

int VideoMode::refreshRate () const
{
  return static_cast<const GLFWvidmode *> (mPimpl)->refreshRate;
}

VideoMode::VideoMode (const void *impl): mPimpl (impl)
{
}

VideoModeList::VideoModeList (void *impl): mPimpl (glfwGetVideoModes (toGLFWmonitor (impl), &mCount))
{
}

VideoModeList::~VideoModeList () = default;

int VideoModeList::count () const
{
  return mCount;
}

VideoMode VideoModeList::operator[] (int i) const
{
  return VideoMode (static_cast<const GLFWvidmode*> (mPimpl) + i);
}

}  // namespace GL
