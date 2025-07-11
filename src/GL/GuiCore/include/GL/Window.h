#pragma once

#include "Common/Signal.h"
#include "GL/MouseButtonEvent.h"
#include "GL/KeyEvent.h"
#include "Geometry/Point.h"
#include "Geometry/Rect.h"
#include "Geometry/Size.h"
#include <optional>

namespace Application
{
void exec ();
}

namespace GL
{
class Monitor;
class VideoMode;

enum class Profile
{
  Any    = 0,          /* GLFW_OPENGL_ANY_PROFILE    */
  Core   = 0x00032001, /* GLFW_OPENGL_CORE_PROFILE   */
  Compat = 0x00032002  /* GLFW_OPENGL_COMPAT_PROFILE */
};

struct WindowHints
{
  Profile profile = Profile::Core;
  int contextVersionMajor = 3;
  int contextVersionMinor = 3;

  std::optional<int> samples;

  bool resizable         = true;
  bool decorated         = true;
  bool focused           = true;
  bool autoIconify       = true;
  bool floating          = false;
  bool maximized         = false;
  bool visible           = true;
  bool centerCursor      = true;
  bool focusOnShow       = true;
  bool mousePassthrough  = false;
  bool doublebuffer      = true;
  bool transparentFrameBuffer = false;

  std::optional<Geometry::Point> pos;

  std::optional<int> refreshRate;

  int redBits     = 8;
  int greenBits   = 8;
  int blueBits    = 8;
  int alphaBits   = 8;
  int depthBits   = 24;
  int stencilBits = 8;
};

class Window
{
public:
  Window (const Geometry::Size &size, const char *title, const WindowHints &hints = {}) noexcept;
  Window (const Geometry::Size &resolution, const char *title, const Monitor &monitor,  const WindowHints &hints = {}) noexcept;
  Window (const char *title, const Monitor &monitor, const GL::VideoMode &videoMode) noexcept;
  Window (const char *title, const Monitor &monitor) noexcept;

  ~Window () noexcept;

  Window (const Window &) = delete;
  Window &operator= (const Window &) = delete;

  Window (Window &&) = delete;
  Window &operator= (Window &&) = delete;

  void setMonitor (const Monitor &monitor, const Geometry::Size &resolution, std::optional<int> refreshRate = std::nullopt);
  void setWindowed (const Geometry::Point &pos, const Geometry::Size &size);

  // void makeCurrent ();
  // void swapBuffers ();

  bool shouldClose ();
  void setShouldClose (bool shouldClose);

  const char *title () const;
  void setTitle (const char *title);

  Geometry::Point pos () const;
  void setPos (const Geometry::Point &pos);

  Geometry::Size size () const;
  void setSize (const Geometry::Size &size);

  std::optional<Geometry::Size> minSize () const;
  std::optional<Geometry::Size> maxSize () const;
  void setSizeLimits (const std::optional<Geometry::Size> &minSize, const std::optional<Geometry::Size> &maxSize);

  std::optional<Geometry::Size> aspectRatio () const;
  void setAspectRatio (const std::optional<Geometry::Size> &ratio);

  Geometry::Size frameBufferSize () const;

  Geometry::Rect frameRect () const;

  void contentScale (float *xScale, float *yScale) const;

  float opacity () const;
  void setOpacity (float opacity);

  bool isIconified () const;
  void setIconified (bool iconified);
  void iconify ();
  void restore ();

  bool isVisible () const;
  void setVislble (bool visible);
  void show ();
  void hide ();

  bool isMaximized () const;
  void maximize ();

  void requestAttention ();

  bool isFocused () const;
  void focus ();

  bool isAutoIconified () const;
  void setAutoIconified (bool autoIconified);

  bool isResizable () const;
  void setResizable (bool resizable);

  bool isDecorated () const;
  void setDecorated (bool decorated);

  bool isFloating () const;
  void setFloating (bool floating);

  bool isFocusedOnShow () const;
  void setFocusedOnShow (bool focusedOnShow);

  bool isMousePassthrough () const;
  void setMousePassthrough (bool mousePassthrough);

  bool isHovered () const;

  bool hasTransparentFramebuffer () const;

  bool hasDoubleBuffer () const;

  enum class CursorInputMode
  {
    Normal   = 0x00034001, /* GLFW_CURSOR_NORMAL   */
    Hidden   = 0x00034002, /* GLFW_CURSOR_HIDDEN   */
    Disabled = 0x00034003, /* GLFW_CURSOR_DISABLED */
    Captured = 0x00034004  /* GLFW_CURSOR_CAPTURED */
  };

  CursorInputMode cursorInputMode () const;
  void setCursorInputMode (CursorInputMode mode);

  bool stickKeysEnabled () const;
  void setStickKeysEnabled (bool enabled);

  bool stickMouseButtonsEnabled () const;
  void setStickMouseButtonsEnabled (bool enabled);

  bool lockKeyModsEnabled () const;
  void setLockKeyModsEnabled (bool enabled);

  bool rawMouseMotionEnabled () const;
  void setRawMouseMotionEnabled (bool enabled);
  bool rawMouseMotionSupported () const;

  MouseButtonAction lastMouseButtonAction (MouseButton button) const;

  Geometry::PointF cursorPos () const;
  void setCursorPos (const Geometry::PointF &pos);

  /// signals
public:
  Common::Signal<Geometry::Point> posChanged;
  Common::Signal<Geometry::Size> sizeChanged;
  Common::Signal<> closeNeeded;
  Common::Signal<> refreshNeeded;
  Common::Signal<bool> focusChanged;
  Common::Signal<bool> iconified;
  Common::Signal<bool> maximized;
  Common::Signal<Geometry::Size> framebufferSizeChanged;
  Common::Signal<float, float> contentScaleChanged;

  Common::Signal<GL::KeyEvent> keyEvent;
  Common::Signal<GL::MouseButtonEvent> mouseButtonEvent;
  Common::Signal<Geometry::PointF> cursorPosChanged;
  Common::Signal<bool> cursorEnterChanged;
  Common::Signal<Geometry::PointF> scrollEvent;

private:
  virtual void renderEvent ();

  friend void Application::exec ();

  std::optional<Geometry::Size> mMinSize;
  std::optional<Geometry::Size> mMaxSize;
  std::optional<Geometry::Size> mAspectRatio;

  void *mPimpl = nullptr;
};
}  // namespace GL
