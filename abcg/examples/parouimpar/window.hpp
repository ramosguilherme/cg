#ifndef WINDOW_HPP_
#define WINDOW_HPP_

#include "abcgOpenGL.hpp"
#include <string>

class Window : public abcg::OpenGLWindow {
protected:
  void onCreate() override;
  void onPaintUI() override;

private:
  static int const m_N{1}; // Board size is m_N x m_N

  enum class GameState { Play, Draw, WinX, WinO, Par, Impar };
  GameState m_gameState;

  bool m_XsTurn{true};
  std::array<int, m_N * m_N> m_board{}; // '\0', 'X' or 'O'

  ImFont *m_font{};

  void checkEndCondition();
  void jogarNovamente();
};

#endif