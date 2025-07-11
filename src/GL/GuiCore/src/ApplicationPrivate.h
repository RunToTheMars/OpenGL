#pragma once

#include <GLFW/glfw3.h>
#include <unordered_set>

namespace GL
{
class Widget;
class Window;
namespace ApplicationPrivate
{
// const std::unordered_set<GL::Widget *> &allWidgets ();
const std::unordered_set<GL::Window *> &allWindows ();

// void insertWidget (GL::Widget *widget);
// void removeWidget (GL::Widget *widget);

void insertWindow (GL::Window *window);
void eraseWindow (GL::Window *window);
}
}
