#include "window.hpp"
#include <bits/stdc++.h>

void Window::onCreate() {
  // Load font with bigger size for the X's and O's
  auto const filename{abcg::Application::getAssetsPath() +
                      "Inconsolata-Medium.ttf"};
  m_font = ImGui::GetIO().Fonts->AddFontFromFileTTF(filename.c_str(), 72.0f);
  if (m_font == nullptr) {
    throw abcg::RuntimeError{"Cannot load font file"};
  }

  jogarNovamente();
}

int geraNumeroAleatorio()
{
  int numero = 1 + (rand() % 100);
  return numero;
}

bool isPar(int numero){
  bool par = numero % 2;
  if (par == 0)
  {
    return true;
  }
  else{
    return false;
  }
}

void Window::jogarNovamente() {
  int numero = geraNumeroAleatorio();
  m_board.fill(numero);
  bool par = isPar(numero);
  m_gameState = par ? GameState::Par : GameState::Impar;
  
}

void Window::onPaintUI() {
  // Get size of application's window
  auto const appWindowWidth{gsl::narrow<float>(getWindowSettings().width)};
  auto const appWindowHeight{gsl::narrow<float>(getWindowSettings().height)};

  // "Tic-Tac-Toe" window
  {
    ImGui::SetNextWindowSize(ImVec2(appWindowWidth, appWindowHeight));
    ImGui::SetNextWindowPos(ImVec2(0, 0));

    auto const flags{ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoResize |
                     ImGuiWindowFlags_NoCollapse};
    ImGui::Begin("Par ou Impar", nullptr, flags);

    // TODO: Add Menu
        // Menu
    {
      bool restartSelected{};
      if (ImGui::BeginMenuBar()) {
        if (ImGui::BeginMenu("Menu")) {
          ImGui::MenuItem("Jogar Novamente", nullptr, &restartSelected);
          ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
      }
      if (restartSelected) {
        jogarNovamente();
      }
    }
    // TODO: Add static text showing current turn or win/draw messages
    {
      std::string text;
      switch (m_gameState) {
      case GameState::Play:
        text = fmt::format("{}'s turn", m_XsTurn ? 'X' : 'O');
        break;
      case GameState::Draw:
        text = "Draw!";
        break;
      case GameState::WinX:
        text = "X's player wins!";
        break;
      case GameState::WinO:
        text = "O's player wins!";
        break;
      case GameState::Par:
        text = fmt::format("Eh par");
        break;
      case GameState::Impar:
        text = fmt::format("Eh impar");
        break;
      }
      // Center text
      ImGui::SetCursorPosX(
          (appWindowWidth - ImGui::CalcTextSize(text.c_str()).x) / 2);
      ImGui::Text("%s", text.c_str());
      ImGui::Spacing();
    }

    
  
    // TODO: Add game board
        // Game board
    {
      auto const gridHeight{appWindowHeight - 22 - 60 - (m_N * 10) - 60};
      auto const buttonHeight{gridHeight / m_N};

      // Use custom font
      ImGui::PushFont(m_font);
      if (ImGui::BeginTable("Game board", m_N)) {
        for (auto i : iter::range(m_N)) {
          ImGui::TableNextRow();
          for (auto j : iter::range(m_N)) {
            ImGui::TableSetColumnIndex(j);
            auto const offset{i * m_N + j};

            // Get current character
            auto ch{m_board.at(offset)};

            // Replace null character with whitespace because the button label
            // cannot be an empty string
            if (ch == 0) {
              ch = ' ';
            }

            // Button text is ch followed by an ID in the format ##ij
            auto buttonText{fmt::format("{}##{}{}", ch, i, j)};
            if (ImGui::Button(buttonText.c_str(), ImVec2(-1, buttonHeight))) {
              if (m_gameState == GameState::Play && ch == ' ') {
                m_board.at(offset) = m_XsTurn ? 'X' : 'O';
                checkEndCondition();
                m_XsTurn = !m_XsTurn;
              }
            }
          }
          ImGui::Spacing();
        }
        ImGui::EndTable();
      }
      ImGui::PopFont();
    }

    ImGui::Spacing();
    // TODO: Add "Restart game" button
        // "Restart game" button
    {
      if (ImGui::Button("Jogar game", ImVec2(-1, 50))) {
        jogarNovamente();
      }
    }
    ImGui::End();
  }
}

void Window::checkEndCondition() {
  if (m_gameState != GameState::Play) {
    return;
  }

  



  // Lambda expression that checks if a string contains only Xs or Os. If so, it
  // changes the game state to WinX or WinO accordingly and returns true.
  // Otherwise, returns false.
  auto allXsOrOs{[&](std::string_view str) {
    if (str == std::string(m_N, 'X')) {
      m_gameState = GameState::WinX;
      return true;
    }
    if (str == std::string(m_N, 'O')) {
      m_gameState = GameState::WinO;
      return true;
    }
    return false;
  }};

  // Check rows
  for (auto const i : iter::range(m_N)) {
    std::string concatenation;
    for (auto const j : iter::range(m_N)) {
      concatenation += m_board.at(i * m_N + j);
    }
    if (allXsOrOs(concatenation)) {
      return;
    }
  }

  // Check columns
  for (auto const j : iter::range(m_N)) {
    std::string concatenation;
    for (auto const i : iter::range(m_N)) {
      concatenation += m_board.at(i * m_N + j);
    }
    if (allXsOrOs(concatenation)) {
      return;
    }
  }

  // Check main diagonal
  {
    std::string concatenation;
    for (auto const i : iter::range(m_N)) {
      concatenation += m_board.at(i * m_N + i);
    }
    if (allXsOrOs(concatenation)) {
      return;
    }
  }

  // Check inverse diagonal
  {
    std::string concatenation;
    for (auto const i : iter::range(m_N)) {
      concatenation += m_board.at(i * m_N + (m_N - i - 1));
    }
    if (allXsOrOs(concatenation)) {
      return;
    }
  }

  // Check draw
  if (std::find(m_board.begin(), m_board.end(), '\0') == m_board.end()) {
    m_gameState = GameState::Draw;
  }
}