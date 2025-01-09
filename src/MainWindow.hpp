#pragma once
#include <FL/Fl_Group.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Int_Input.H>
#include <FL/Fl_Text_Display.H>
#include <FL/Fl_Window.H>

#include <vector>

#include "PointDatabase.hpp"

class MainWindow {
 public:
  MainWindow();
  void show();

 private:
  static void start_cb(Fl_Widget*, void*);
  static void add_cb(Fl_Widget*, void*);
  static void find_cb(Fl_Widget*, void*);
  void updateDisplay();
  void createMainInterface(int dimensions);
  void showInitialSetup();

  Fl_Window* window;
  Fl_Group* setupGroup;
  Fl_Group* mainGroup;
  Fl_Text_Display* display;
  Fl_Text_Buffer* buffer;
  Fl_Int_Input* dimInput;
  std::vector<Fl_Input*> inputs;
  PointDatabase* db;
  bool isInitialized;
};