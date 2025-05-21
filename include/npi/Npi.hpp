#pragma once
#include <npi/renderer/Renderer.hpp>
#include <npi/renderer/roc/Cube.hpp>
#include <npi/renderer/roc/Model.hpp>
#include <npi/renderer/roc/Text.hpp>

namespace Npi {
void Init();
void Exit();
bool MainLoop();
}  // namespace Npi