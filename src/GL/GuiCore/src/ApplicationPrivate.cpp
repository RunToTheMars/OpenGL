#include "ApplicationPrivate.h"

namespace
{
// std::unordered_set<GL::Widget *> gAllWidgets;
std::unordered_set<GL::Window *> gAllWindows;
}

namespace GL
{
namespace ApplicationPrivate
{
// const std::unordered_set<GL::Widget *> &allWidgets ()
// {
//   return gAllWidgets;
// }

const std::unordered_set<GL::Window *> &allWindows ()
{
  return gAllWindows;
}

// void insertWidget (GL::Widget *widget)
// {
//   gAllWidgets.insert (widget);
// }

// void removeWidget (GL::Widget *widget)
// {
//   gAllWidgets.erase (gAllWidgets.find (widget));
// }

void insertWindow (GL::Window *window)
{
  gAllWindows.insert (window);
}

void eraseWindow (GL::Window *window)
{
  gAllWindows.erase (window);
}
}  // namespace ApplicationPrivate
}  // namespace GL
