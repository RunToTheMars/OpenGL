#pragma once

namespace Common
{
template<typename... Args>
class Signal;
}

namespace GL
{
class Widget;
class Window;
class Monitor;
class MonitorList;

namespace Application
{
void init ();

void exec ();
void exit ();

GL::Widget *focusWidget ();
void setFocusWidget (GL::Widget *widget);

GL::Window *focusWindow ();
void setFocusWindow (GL::Window *window);

Monitor primaryMonitor ();
MonitorList monitorList ();

/// signals
Common::Signal<GL::Widget * /*old*/, GL::Widget * /*new*/> &focusWidgetChanged ();
Common::Signal<GL::Window * /*old*/, GL::Window * /*new*/> &focusWindowChanged ();
}  // namespace Application
}  // namespace GL
