#include "MainWindow.hpp"

int main(int argc, char* argv[]) {
  MainWindow* window = new MainWindow();
  window->show();
  return Fl::run();
}
