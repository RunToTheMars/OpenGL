#include "GL/Window.h"
#include "Gl/Monitor.h"
#include "ApplicationPrivate.h"
#include <GLFW/glfw3.h>

namespace
{
GLFWwindow* toGLFWwindow (void *pImpl)
{
  return static_cast<GLFWwindow*> (pImpl);
}

GL::Window* toGLwindow (void *pImpl)
{
  return static_cast<GL::Window*> (pImpl);
}

void WindowPosChangedHandle (GLFWwindow* window, int xpos, int ypos)
{
  GL::Window *GLwindow = toGLwindow (glfwGetWindowUserPointer (window));
  GLwindow->posChanged.notify (Geometry::Point (xpos, ypos));
}

void WindowSizeChangedHandle (GLFWwindow* window, int width, int height)
{
  GL::Window *GLwindow = toGLwindow (glfwGetWindowUserPointer (window));
  GLwindow->sizeChanged.notify (Geometry::Size (width, height));
}

void WindowClosedHandle (GLFWwindow* window)
{
  GL::Window *GLwindow = toGLwindow (glfwGetWindowUserPointer (window));
  GLwindow->closeNeeded.notify ();
}

void WindowRefreshHandle (GLFWwindow* window)
{
  GL::Window *GLwindow = toGLwindow (glfwGetWindowUserPointer (window));
  GLwindow->refreshNeeded.notify ();
}

void WindowFocusChangedHandle (GLFWwindow* window, int focused)
{
  GL::Window *GLwindow = toGLwindow (glfwGetWindowUserPointer (window));
  GLwindow->focusChanged.notify (focused == GLFW_TRUE);
}

void WindowIconifiedHandle (GLFWwindow* window, int iconified)
{
  GL::Window *GLwindow = toGLwindow (glfwGetWindowUserPointer (window));
  GLwindow->iconified.notify (iconified == GLFW_TRUE);
}

void WindowMaximizedHandle (GLFWwindow* window, int maximized)
{
  GL::Window *GLwindow = toGLwindow (glfwGetWindowUserPointer (window));
  GLwindow->maximized.notify (maximized == GLFW_TRUE);
}

void WindowFramebufferSizeChangedHandle (GLFWwindow* window, int width, int height)
{
  GL::Window *GLwindow = toGLwindow (glfwGetWindowUserPointer (window));
  GLwindow->framebufferSizeChanged.notify (Geometry::Size (width, height));
}

void WindowContentScaleChangedHandle (GLFWwindow* window, float xscale, float yscale)
{
  GL::Window *GLwindow = toGLwindow (glfwGetWindowUserPointer (window));
  GLwindow->contentScaleChanged.notify (xscale, yscale);
}

void WindowKeyEventHandle (GLFWwindow* window, int key, int scancode, int action, int mods)
{
  GL::Window *GLwindow = toGLwindow (glfwGetWindowUserPointer (window));
  GLwindow->keyEvent.notify (GL::KeyEvent (static_cast<GL::Key> (key), static_cast<GL::KeyAction> (action)));
}

void WindowMouseButtonEventHandle (GLFWwindow* window, int button, int action, int mods)
{
  GL::Window *GLwindow = toGLwindow (glfwGetWindowUserPointer (window));
  GLwindow->mouseButtonEvent.notify (GL::MouseButtonEvent (static_cast<GL::MouseButton> (button), static_cast<GL::MouseButtonAction> (action)));
}

void WindowCursorPosChangedHandle (GLFWwindow* window, double xpos, double ypos)
{
  GL::Window *GLwindow = toGLwindow (glfwGetWindowUserPointer (window));
  GLwindow->cursorPosChanged.notify (Geometry::PointF (xpos,  ypos));
}

void WindowCursorEnterChangedHandle (GLFWwindow* window, int entered)
{
  GL::Window *GLwindow = toGLwindow (glfwGetWindowUserPointer (window));
  GLwindow->cursorEnterChanged.notify (entered == GLFW_TRUE);
}

void WindowScrollEventHandle (GLFWwindow* window, double xoffset, double yoffset)
{
  GL::Window *GLwindow = toGLwindow (glfwGetWindowUserPointer (window));
  GLwindow->scrollEvent.notify (Geometry::PointF (xoffset, yoffset));
}

void *createWindowImpl (GL::Window *window, const Geometry::Size &size, const char *title, GLFWmonitor *monitor, const GL::WindowHints &hints)
{
  glfwWindowHint (GLFW_OPENGL_PROFILE       , static_cast<int> (hints.profile));
  glfwWindowHint (GLFW_CONTEXT_VERSION_MAJOR, hints.contextVersionMinor);
  glfwWindowHint (GLFW_CONTEXT_VERSION_MINOR, hints.contextVersionMajor);
  glfwWindowHint (GLFW_REFRESH_RATE         , hints.refreshRate.value_or (GLFW_DONT_CARE));

  glfwWindowHint (GLFW_SAMPLES, hints.samples.value_or (GLFW_DONT_CARE));

  glfwWindowHint (GLFW_RESIZABLE              , hints.resizable        ? GLFW_TRUE : GLFW_FALSE);
  glfwWindowHint (GLFW_DECORATED              , hints.decorated        ? GLFW_TRUE : GLFW_FALSE);
  glfwWindowHint (GLFW_FOCUSED                , hints.focused          ? GLFW_TRUE : GLFW_FALSE);
  glfwWindowHint (GLFW_AUTO_ICONIFY           , hints.autoIconify      ? GLFW_TRUE : GLFW_FALSE);
  glfwWindowHint (GLFW_FLOATING               , hints.floating         ? GLFW_TRUE : GLFW_FALSE);
  glfwWindowHint (GLFW_MAXIMIZED              , hints.maximized        ? GLFW_TRUE : GLFW_FALSE);
  glfwWindowHint (GLFW_VISIBLE                , hints.visible          ? GLFW_TRUE : GLFW_FALSE);
  glfwWindowHint (GLFW_CENTER_CURSOR          , hints.centerCursor     ? GLFW_TRUE : GLFW_FALSE);
  glfwWindowHint (GLFW_FOCUS_ON_SHOW          , hints.focusOnShow      ? GLFW_TRUE : GLFW_FALSE);
  glfwWindowHint (GLFW_MOUSE_PASSTHROUGH      , hints.mousePassthrough ? GLFW_TRUE : GLFW_FALSE);
  glfwWindowHint (GLFW_DOUBLEBUFFER           , hints.doublebuffer     ? GLFW_TRUE : GLFW_FALSE);
  glfwWindowHint (GLFW_TRANSPARENT_FRAMEBUFFER, hints.transparentFrameBuffer ? GLFW_TRUE : GLFW_FALSE);

  if (hints.pos.has_value ())
    {
      glfwWindowHint (GLFW_POSITION_X, hints.pos->x ());
      glfwWindowHint (GLFW_POSITION_Y, hints.pos->y ());
    }
  else
    {
      glfwWindowHint (GLFW_POSITION_X, GLFW_ANY_POSITION);
      glfwWindowHint (GLFW_POSITION_Y, GLFW_ANY_POSITION);
    }

  glfwWindowHint (GLFW_RED_BITS    , hints.redBits);
  glfwWindowHint (GLFW_GREEN_BITS  , hints.greenBits);
  glfwWindowHint (GLFW_BLUE_BITS   , hints.blueBits);
  glfwWindowHint (GLFW_ALPHA_BITS  , hints.alphaBits);
  glfwWindowHint (GLFW_DEPTH_BITS  , hints.depthBits);
  glfwWindowHint (GLFW_STENCIL_BITS, hints.stencilBits);

  GLFWwindow *impl = glfwCreateWindow (size.width (), size.height (), title, monitor, nullptr);

  glfwSetWindowUserPointer          (impl, window);
  glfwSetWindowPosCallback          (impl, &WindowPosChangedHandle);
  glfwSetWindowSizeCallback         (impl, &WindowSizeChangedHandle);
  glfwSetWindowCloseCallback        (impl, &WindowClosedHandle);
  glfwSetWindowRefreshCallback      (impl, &WindowRefreshHandle);
  glfwSetWindowFocusCallback        (impl, &WindowFocusChangedHandle);
  glfwSetWindowIconifyCallback      (impl, &WindowIconifiedHandle);
  glfwSetWindowMaximizeCallback     (impl, &WindowMaximizedHandle);
  glfwSetFramebufferSizeCallback    (impl, &WindowFramebufferSizeChangedHandle);
  glfwSetWindowContentScaleCallback (impl, &WindowContentScaleChangedHandle);

  glfwSetKeyCallback         (impl, &WindowKeyEventHandle);
  glfwSetMouseButtonCallback (impl, &WindowMouseButtonEventHandle);
  glfwSetCursorPosCallback   (impl, &WindowCursorPosChangedHandle);
  glfwSetCursorEnterCallback (impl, &WindowCursorEnterChangedHandle);
  glfwSetScrollCallback      (impl, &WindowScrollEventHandle);

  GL::ApplicationPrivate::insertWindow (window);

  return impl;
}

void *createWindowImpl (GL::Window *window, const char *title, GLFWmonitor *monitor, const GL::VideoMode &videoMode)
{
  GL::WindowHints hints;
  hints.redBits = videoMode.redBits ();
  hints.greenBits = videoMode.greenBits ();
  hints.blueBits = videoMode.blueBits ();
  hints.refreshRate = videoMode.refreshRate ();
  return createWindowImpl (window, {videoMode.width (), videoMode.height ()}, title, monitor, hints);
}
}

namespace GL
{
Window::Window (const Geometry::Size &size, const char *title, const WindowHints &hints) noexcept
{
  mPimpl = createWindowImpl (this, size, title, nullptr, hints);
}

Window::Window (const Geometry::Size &resolution, const char *title, const Monitor &monitor, const WindowHints &hints) noexcept
{
  mPimpl = createWindowImpl (this, resolution, title, static_cast<GLFWmonitor*> (monitor.mPimpl), hints);
}

Window::Window (const char *title, const Monitor &monitor, const GL::VideoMode &videoMode) noexcept
{
  mPimpl = createWindowImpl (this, title, static_cast<GLFWmonitor*> (monitor.mPimpl), videoMode);
}

Window::Window (const char *title, const Monitor &monitor) noexcept
{
  GL::VideoModeList videoModeList = monitor.videoModeList ();
  GL::VideoMode videoMode = videoModeList[videoModeList.count () - 1];
  mPimpl = createWindowImpl (this, title, static_cast<GLFWmonitor *> (monitor.mPimpl), videoMode);
}

Window::~Window () noexcept
{
  GL::ApplicationPrivate::eraseWindow (this);
  glfwDestroyWindow (toGLFWwindow (mPimpl));
}

// void Window::makeCurrent ()
// {
//   return glfwMakeContextCurrent (toGLFWwindow (mPimpl));
// }

// void Window::swapBuffers ()
// {
//   return glfwSwapBuffers (toGLFWwindow (mPimpl));
// }

void Window::setMonitor (const Monitor &monitor, const Geometry::Size &resolution, std::optional<int> refreshRate)
{
  glfwSetWindowMonitor (toGLFWwindow (mPimpl), static_cast<GLFWmonitor*> (monitor.mPimpl), 0, 0, resolution.width (), resolution.height (), refreshRate.value_or (GL_DONT_CARE));
}

void Window::setWindowed (const Geometry::Point &pos, const Geometry::Size &size)
{
  glfwSetWindowMonitor (toGLFWwindow (mPimpl), nullptr, pos.x (), pos.y (), size.width (), size.height (), GL_DONT_CARE);
}

bool Window::shouldClose ()
{
  return glfwWindowShouldClose (toGLFWwindow (mPimpl)) == GLFW_TRUE;
}

void Window::setShouldClose (bool shouldClose)
{
  glfwSetWindowShouldClose (toGLFWwindow (mPimpl), shouldClose ? GLFW_TRUE : GL_FALSE);
}

const char *Window::title () const
{
  return glfwGetWindowTitle (toGLFWwindow (mPimpl));
}

void Window::setTitle (const char *title)
{
  glfwSetWindowTitle (toGLFWwindow (mPimpl), title);
}

Geometry::Point Window::pos () const
{
  Geometry::Point pos;
  glfwGetWindowPos (toGLFWwindow (mPimpl), &pos.rx (), &pos.ry ());
  return pos;
}

void Window::setPos (const Geometry::Point &pos)
{
  glfwSetWindowPos (toGLFWwindow (mPimpl), pos.x (), pos.y ());
}

Geometry::Size Window::size () const
{
  Geometry::Size size;
  glfwGetWindowSize (toGLFWwindow (mPimpl), &size.rwidth (), &size.rheight ());
  return size;
}

void Window::setSize (const Geometry::Size &size)
{
  glfwSetWindowSize (toGLFWwindow (mPimpl), size.width (), size.height ());
}

std::optional<Geometry::Size> Window::minSize () const
{
  return mMinSize;
}

std::optional<Geometry::Size> Window::maxSize () const
{
  return mMaxSize;
}

void Window::setSizeLimits (const std::optional<Geometry::Size> &minSize, const std::optional<Geometry::Size> &maxSize)
{
  int minWidth;
  int minHeight;
  int maxWidth;
  int maxHeight;

  if (minSize)
    {
      minWidth = minSize->width ();
      minHeight = minSize->height ();
    }
  else
    {
      minWidth = minHeight = GLFW_DONT_CARE;
    }

  if (maxSize)
    {
      maxWidth = maxSize->width ();
      maxHeight = maxSize->height ();
    }
  else
    {
      maxWidth = maxHeight = GLFW_DONT_CARE;
    }

  mMinSize = minSize;
  mMaxSize = maxSize;
  glfwSetWindowSizeLimits (toGLFWwindow (mPimpl), minWidth, minHeight, maxWidth, maxHeight);
}

std::optional<Geometry::Size> Window::aspectRatio () const
{
  return mAspectRatio;
}

void Window::setAspectRatio (const std::optional<Geometry::Size> &ratio)
{
  mAspectRatio = ratio;
  if (ratio)
    glfwSetWindowAspectRatio (toGLFWwindow (mPimpl), ratio->width (), ratio->height ());
  else
    glfwSetWindowAspectRatio (toGLFWwindow (mPimpl), GLFW_DONT_CARE, GLFW_DONT_CARE);
}

Geometry::Size Window::frameBufferSize () const
{
  Geometry::Size size;
  glfwGetFramebufferSize (toGLFWwindow (mPimpl), &size.rwidth (), &size.rheight ());
  return size;
}

Geometry::Rect Window::frameRect () const
{
  int left;
  int top;
  int right;
  int bottom;
  glfwGetWindowFrameSize (toGLFWwindow (mPimpl), &left, &top, &right, &bottom);
  return Geometry::Rect (Geometry::Point (left, bottom), Geometry::Size (right - left + 1, top - bottom + 1));
}

void Window::contentScale (float *xScale, float *yScale) const
{
  glfwGetWindowContentScale (toGLFWwindow (mPimpl), xScale, yScale);
}

float Window::opacity () const
{
  return glfwGetWindowOpacity (toGLFWwindow (mPimpl));
}

void Window::setOpacity (float opacity)
{
  glfwSetWindowOpacity (toGLFWwindow (mPimpl), opacity);
}

bool Window::isIconified () const
{
  return glfwGetWindowAttrib (toGLFWwindow (mPimpl), GLFW_ICONIFIED) == GLFW_TRUE;
}

void Window::setIconified (bool iconified)
{
  if (iconified)
    return iconify ();
  else
    return restore ();
}

void Window::iconify ()
{
  glfwIconifyWindow (toGLFWwindow (mPimpl));
}

void Window::restore ()
{
  glfwRestoreWindow (toGLFWwindow (mPimpl));
}

bool Window::isVisible () const
{
  return glfwGetWindowAttrib (toGLFWwindow (mPimpl), GLFW_VISIBLE) == GLFW_TRUE;
}

void Window::setVislble (bool visible)
{
  if (visible)
    show ();
  else
    hide ();
}

void Window::show ()
{
  glfwShowWindow (toGLFWwindow (mPimpl));
}

void Window::hide ()
{
  glfwHideWindow (toGLFWwindow (mPimpl));
}

bool Window::isMaximized () const
{
  return glfwGetWindowAttrib (toGLFWwindow (mPimpl), GLFW_MAXIMIZED) == GLFW_TRUE;
}

void Window::maximize ()
{
  glfwMaximizeWindow (toGLFWwindow (mPimpl));
}

void Window::requestAttention ()
{
  glfwRequestWindowAttention (toGLFWwindow (mPimpl));
}

bool Window::isFocused () const
{
  return glfwGetWindowAttrib (toGLFWwindow (mPimpl), GLFW_FOCUSED) == GLFW_TRUE;
}

void Window::focus ()
{
  glfwFocusWindow (toGLFWwindow (mPimpl));
}

bool Window::isAutoIconified () const
{
  return glfwGetWindowAttrib (toGLFWwindow (mPimpl), GLFW_AUTO_ICONIFY) == GLFW_TRUE;
}

void Window::setAutoIconified (bool autoIconified)
{
  glfwSetWindowAttrib (toGLFWwindow (mPimpl), GLFW_AUTO_ICONIFY, autoIconified ? GLFW_TRUE : GL_FALSE);
}

bool Window::isResizable () const
{
  return glfwGetWindowAttrib (toGLFWwindow (mPimpl), GLFW_RESIZABLE) == GLFW_TRUE;
}

void Window::setResizable (bool resizable)
{
  glfwSetWindowAttrib (toGLFWwindow (mPimpl), GLFW_RESIZABLE, resizable ? GLFW_TRUE : GL_FALSE);
}

bool Window::isDecorated () const
{
  return glfwGetWindowAttrib (toGLFWwindow (mPimpl), GLFW_DECORATED) == GLFW_TRUE;
}

void Window::setDecorated (bool decorated)
{
  glfwSetWindowAttrib (toGLFWwindow (mPimpl), GLFW_DECORATED, decorated ? GLFW_TRUE : GL_FALSE);
}

bool Window::isFloating () const
{
  return glfwGetWindowAttrib (toGLFWwindow (mPimpl), GLFW_FLOATING) == GLFW_TRUE;
}

void Window::setFloating (bool floating)
{
  glfwSetWindowAttrib (toGLFWwindow (mPimpl), GLFW_FLOATING, floating ? GLFW_TRUE : GL_FALSE);
}

bool Window::isFocusedOnShow () const
{
  return glfwGetWindowAttrib (toGLFWwindow (mPimpl), GLFW_FOCUS_ON_SHOW) == GLFW_TRUE;
}

void Window::setFocusedOnShow (bool focusedOnShow)
{
  glfwSetWindowAttrib (toGLFWwindow (mPimpl), GLFW_FOCUS_ON_SHOW, focusedOnShow ? GLFW_TRUE : GL_FALSE);
}

bool Window::isMousePassthrough () const
{
  return glfwGetWindowAttrib (toGLFWwindow (mPimpl), GLFW_MOUSE_PASSTHROUGH) == GLFW_TRUE;
}

void Window::setMousePassthrough (bool mousePassthrough)
{
  glfwSetWindowAttrib (toGLFWwindow (mPimpl), GLFW_MOUSE_PASSTHROUGH, mousePassthrough ? GLFW_TRUE : GL_FALSE);
}

bool Window::isHovered () const
{
  return glfwGetWindowAttrib (toGLFWwindow (mPimpl), GLFW_HOVERED) == GLFW_TRUE;
}

bool Window::hasTransparentFramebuffer () const
{
  return glfwGetWindowAttrib (toGLFWwindow (mPimpl), GLFW_TRANSPARENT_FRAMEBUFFER) == GLFW_TRUE;
}

bool Window::hasDoubleBuffer () const
{
  return glfwGetWindowAttrib (toGLFWwindow (mPimpl), GLFW_DOUBLEBUFFER) == GLFW_TRUE;
}

Window::CursorInputMode Window::cursorInputMode () const
{
  return static_cast<Window::CursorInputMode> (glfwGetInputMode (toGLFWwindow (mPimpl), GLFW_CURSOR));
}

void Window::setCursorInputMode (CursorInputMode mode)
{
  glfwSetInputMode (toGLFWwindow (mPimpl), GLFW_CURSOR, static_cast<int> (mode));
}

bool Window::stickKeysEnabled () const
{
  return glfwGetInputMode (toGLFWwindow (mPimpl), GLFW_STICKY_KEYS) == GLFW_TRUE;
}

void Window::setStickKeysEnabled (bool enabled)
{
  glfwSetInputMode (toGLFWwindow (mPimpl), GLFW_STICKY_KEYS, enabled ? GLFW_TRUE : GL_FALSE);
}

bool Window::stickMouseButtonsEnabled () const
{
  return glfwGetInputMode (toGLFWwindow (mPimpl), GLFW_STICKY_MOUSE_BUTTONS) == GLFW_TRUE;
}

void Window::setStickMouseButtonsEnabled (bool enabled)
{
  glfwSetInputMode (toGLFWwindow (mPimpl), GLFW_STICKY_MOUSE_BUTTONS, enabled ? GLFW_TRUE : GL_FALSE);
}

bool Window::lockKeyModsEnabled () const
{
  return glfwGetInputMode (toGLFWwindow (mPimpl), GLFW_LOCK_KEY_MODS) == GLFW_TRUE;
}

void Window::setLockKeyModsEnabled (bool enabled)
{
  glfwSetInputMode (toGLFWwindow (mPimpl), GLFW_LOCK_KEY_MODS, enabled ? GLFW_TRUE : GL_FALSE);
}

bool Window::rawMouseMotionEnabled () const
{
  return glfwGetInputMode (toGLFWwindow (mPimpl), GLFW_RAW_MOUSE_MOTION) == GLFW_TRUE;
}

void Window::setRawMouseMotionEnabled (bool enabled)
{
  glfwSetInputMode (toGLFWwindow (mPimpl), GLFW_RAW_MOUSE_MOTION, enabled ? GLFW_TRUE : GL_FALSE);
}

bool Window::rawMouseMotionSupported () const
{
  return glfwRawMouseMotionSupported () == GLFW_TRUE;
}

MouseButtonAction Window::lastMouseButtonAction (MouseButton button) const
{
  return static_cast<MouseButtonAction> (glfwGetMouseButton (toGLFWwindow (mPimpl), static_cast<int> (button)));
}

Geometry::PointF Window::cursorPos () const
{
  Geometry::PointF pos;
  glfwGetCursorPos (toGLFWwindow (mPimpl), &pos.rx (), &pos.ry ());
  return pos;
}

void Window::setCursorPos (const Geometry::PointF &pos)
{
  glfwSetCursorPos (toGLFWwindow (mPimpl), pos.x (), pos.y ());
}

void Window::renderEvent ()
{

}

}
