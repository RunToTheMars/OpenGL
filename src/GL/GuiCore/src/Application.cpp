#include "GL/Application.h"
#include "ApplicationPrivate.h"
#include "Common/Signal.h"
#include "GL/Window.h"
#include "GL/Monitor.h"
#include <GLFW/glfw3.h>

namespace
{
GL::Widget *gFocusWidget = nullptr;
GL::Window *gFocusWindow = nullptr;
Common::Signal<GL::Widget * /*old*/, GL::Widget * /*new*/> gFocusWidgetChanged;
Common::Signal<GL::Window * /*old*/, GL::Window * /*new*/> gFocusWindowChanged;
bool gExecuted = false;
}  // namespace

namespace GL
{
namespace Application
{

void init ()
{
  glfwInit ();

  return;
}

void exec ()
{
  if (gExecuted)
    return;

  gExecuted = true;
  while (gExecuted)
    {
      for (GL::Window *window: GL::ApplicationPrivate::allWindows ())
        {
          GLFWwindow* windowImpl = static_cast<GLFWwindow*> (window->mPimpl);
          glfwMakeContextCurrent (windowImpl);
          window->renderEvent ();
          glfwSwapBuffers (windowImpl);
        }
      glfwPollEvents ();
    }
}

void exit ()
{
  gExecuted = false;
}

GL::Widget *focusWidget ()
{
  return gFocusWidget;
}

void setFocusWidget (GL::Widget *widget)
{
  if (gFocusWidget == widget)
    return;

  GL::Widget *oldFocusWidget = gFocusWidget;
  gFocusWidget = widget;

  gFocusWidgetChanged.notify (oldFocusWidget, widget);
}

GL::Window *focusWindow ()
{
  return gFocusWindow;
}

void setFocusWindow (GL::Window *window)
{
  if (gFocusWindow == window)
    return;

  GL::Window *oldFocusWindow = gFocusWindow;
  gFocusWindow = window;

  gFocusWindowChanged.notify (oldFocusWindow, window);
}

Monitor primaryMonitor ()
{
  return Monitor (glfwGetPrimaryMonitor ());
}
}
}
