#include "GL/Application.h"
#include "GL/Monitor.h"
#include "GL/Window.h"
#include "Geometry/PointF.h"
#include <GL/glew.h>

int main ()
{
  GL::Application::init ();

  glewInit ();

  GL::Window window ("Hello Window", GL::Application::primaryMonitor ());

  Common::Signal<Geometry::Point>::Connection posChangedConnect = window.posChanged.connect ([&] (Geometry::Point pos) {
    (void) pos;
  });

  Common::Signal<Geometry::Size>::Connection sizeChangedConnect = window.sizeChanged.connect ([&] (Geometry::Size size) {
    (void) size;
  });

  Common::Signal<bool>::Connection iconofiedConnect = window.iconified.connect ([&] (bool iconified) {
    (void) iconified;
  });

  Common::Signal<bool>::Connection maximizedConnect = window.maximized.connect ([&] (bool maximized) {
    (void) maximized;
  });

  // Common::Signal<>::Connection closeNeededConnect = window1.closeNeeded.connect ([&] () {
  //   window1.setMonitor (GL::Application::primaryMonitor (), {200, 100}, 144);
  // });

  GL::Application::exec ();

  return 0;
}
