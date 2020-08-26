#pragma once

#include "./graph/simplegrid.h"
#include "./agent/agent.h"
#include "./stn/stn.h"
#include "ofMain.h"
#include "ofxGui.h"

enum Focus { TIME_FREE, DISCRETE_TIME };

class ofApp : public ofBaseApp {
private:
  Graph* G;
  Agents A;
  STN* stn;
  Realization realization;
  std::mt19937* MT;
  Nodes V;
  int makespan;
  std::vector<States> sync_exec;

  bool stn_on;

  // model params
  std::string field_name;
  int num_agent;

  // drawing params
  Focus focus;
  int max_activation;
  int width;
  int height;
  int scale;
  float rad_agent;
  float rad_head;
  float rad_goal;
  int font_size;
  ofTrueTypeFont font;
  ofxPanel gui_step;
  ofxPanel gui_time;
  ofxIntSlider step_slider;
  ofxFloatSlider time_slider;
  ofxFloatSlider speed_slider;

  // interactive
  bool flg_autoplay;
  bool flg_print;
  bool flg_loop;
  bool flg_goal;
  bool flg_font;
  bool flg_line;

public:
  ofApp(Graph* _G, Agents _A, STN* _stn, std::mt19937* _MT);
  ofApp(Graph* _G, Agents _A, std::vector<States> exec, std::mt19937* _MT);  // for DP
  void setup();
  void update();
  void draw();
  void printState();

  void keyPressed(int key);
  void keyReleased(int key);
  void mouseMoved(int x, int y );
  void mouseDragged(int x, int y, int button);
  void mousePressed(int x, int y, int button);
  void mouseReleased(int x, int y, int button);
  void mouseEntered(int x, int y);
  void mouseExited(int x, int y);
  void windowResized(int w, int h);
  void dragEvent(ofDragInfo dragInfo);
  void gotMessage(ofMessage msg);
  void printKeys();
};
