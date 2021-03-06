/* Copyright (C) 2013-2014 Michal Brzozowski (rusolis@poczta.fm)

   This file is part of KeeperRL.

   KeeperRL is free software; you can redistribute it and/or modify it under the terms of the
   GNU General Public License as published by the Free Software Foundation; either version 2
   of the License, or (at your option) any later version.

   KeeperRL is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
   even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License along with this program.
   If not, see http://www.gnu.org/licenses/ . */

#ifndef _WINDOW_VIEW
#define _WINDOW_VIEW

#include "util.h"
#include "view.h"
#include "map_layout.h"
#include "map_gui.h"
#include "minimap_gui.h"
#include "input_queue.h"
#include "animation.h"
#include "gui_builder.h"

class ViewIndex;
class Options;
class Clock;

/** See view.h for documentation.*/
class WindowView: public View {
  public:
  struct ViewParams {
    Renderer& renderer;
    GuiFactory& gui;
    bool useTiles;
    Options* options;
    Clock* clock;
  };
  static View* createDefaultView(ViewParams);
  static View* createLoggingView(OutputArchive& of, ViewParams);
  static View* createReplayView(InputArchive& ifs, ViewParams);

  WindowView(ViewParams); 
  virtual void initialize() override;
  virtual void reset() override;
  virtual void displaySplash(const ProgressMeter&, View::SplashType, function<void()> cancelFun) override;
  virtual void clearSplash() override;

  virtual void close() override;

  virtual void refreshView() override;
  virtual void updateView(const CreatureView*, bool noRefresh) override;
  virtual void drawLevelMap(const CreatureView*) override;
  virtual void resetCenter() override;
  virtual optional<int> chooseFromList(const string& title, const vector<ListElem>& options, int index = 0,
      MenuType = View::NORMAL_MENU, double* scrollPos = nullptr,
      optional<UserInputId> exitAction = none) override;
  virtual GameTypeChoice chooseGameType() override;
  virtual optional<Vec2> chooseDirection(const string& message) override;
  virtual bool yesOrNoPrompt(const string& message, bool defaultNo) override;
  virtual void animateObject(vector<Vec2> trajectory, ViewObject object) override;
  virtual void animation(Vec2 pos, AnimationId) override;
  virtual double getGameSpeed() override;

  virtual void presentText(const string& title, const string& text) override;
  virtual void presentList(const string& title, const vector<ListElem>& options, bool scrollDown = false,
      MenuType = NORMAL_MENU, optional<UserInputId> exitAction = none) override;
  virtual optional<int> getNumber(const string& title, int min, int max, int increments = 1) override;
  virtual optional<string> getText(const string& title, const string& value, int maxLength,
      const string& hint) override;

  virtual UserInput getAction() override;
  virtual bool travelInterrupt() override;
  virtual int getTimeMilli() override;
  virtual int getTimeMilliAbsolute() override;
  virtual void stopClock() override;
  virtual bool isClockStopped() override;
  virtual void continueClock() override;
  
  static Color getFireColor();
  static bool areTilesOk();


  private:

  Renderer& renderer;
  GuiFactory& gui;
  void processEvents();
  void displayMenuSplash2();
  void displayOldSplash();
  void updateMinimap(const CreatureView*);
  void mapLeftClickFun(Vec2);
  void mapCreatureClickFun(UniqueEntity<Creature>::Id);
  void mapRightClickFun(Vec2);
  Rectangle getTextInputPosition();
  optional<int> chooseFromListInternal(const string& title, const vector<ListElem>& options, int index, MenuType,
      double* scrollPos, optional<UserInputId> exitAction, optional<sf::Event::KeyEvent> exitKey,
      vector<sf::Event::KeyEvent> shortCuts);
  optional<UserInputId> getSimpleInput(sf::Event::KeyEvent key);
  void refreshViewInt(const CreatureView*, bool flipBuffer = true);
  PGuiElem drawGameChoices(optional<View::GameTypeChoice>& choice, optional<View::GameTypeChoice>& index);
  PGuiElem getTextContent(const string& title, const string& value, const string& hint);
  void rebuildGui();
  void drawMap();
  void propagateEvent(const Event& event, vector<GuiElem*>);
  void keyboardAction(Event::KeyEvent key);

  void drawList(const string& title, const vector<ListElem>& options, int hightlight, int setMousePos = -1);
  void refreshScreen(bool flipBuffer = true);
  void drawAndClearBuffer();
  void displayAutosaveSplash(const ProgressMeter&);

  void switchZoom();
  void zoom(bool out);
  void resize(int width, int height, vector<GuiElem*> gui);
  Rectangle getMapGuiBounds() const;
  Rectangle getMinimapBounds() const;
  void resetMapBounds();
  void switchTiles();

  bool considerResizeEvent(sf::Event&, vector<GuiElem*> gui);

  int messageInd = 0;
  std::deque<string> currentMessage = std::deque<string>(3, "");
  bool oldMessage = false;

  GameInfo gameInfo;

  MapLayout* mapLayout;
  MapGui* mapGui;
  MinimapGui* minimapGui;
  PGuiElem mapDecoration;
  PGuiElem minimapDecoration;
  vector<PGuiElem> tempGuiElems;
  vector<GuiElem*> getAllGuiElems();
  vector<GuiElem*> getClickableGuiElems();
  SyncQueue<UserInput> inputQueue;

  bool gameReady = false;
  bool uiLock = false;
  atomic<bool> refreshInput;
  atomic<bool> wasRendered;

  typedef std::unique_lock<std::recursive_mutex> RenderLock;

  struct TileLayouts {
    MapLayout normalLayout;
    MapLayout unzoomLayout;
    bool sprites;
  };
  TileLayouts asciiLayouts;
  TileLayouts spriteLayouts;
  TileLayouts currentTileLayout;

  function<void()> getButtonCallback(UserInput);

  std::recursive_mutex renderMutex;

  bool lockKeyboard = false;

  thread::id renderThreadId;
  function<void()> renderDialog;

  void addVoidDialog(function<void()>);

  template <class T>
  void addReturnDialog(SyncQueue<T>& q, function<T()> fun) {
    renderDialog = [&q, fun, this] {
      q.push(fun());
      renderDialog = nullptr;
    };
    if (currentThreadId() == renderThreadId)
      renderDialog();
  }
  atomic<bool> splashDone;
  bool useTiles;
  Options* options;
  Clock* clock;
  GuiBuilder guiBuilder;
  void drawMenuBackground(double barState, double mouthState);
  atomic<int> fullScreenTrigger;
};


#endif
