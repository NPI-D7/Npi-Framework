#include <3ds.h>
#include <Npi.hpp>

namespace Npi {
void Init() {
  aptInit();
  romfsInit();
  cfguInit();
  gfxInitDefault();
  C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
}

void Exit() {
  C3D_Fini();
  gfxExit();
  aptExit();
  romfsExit();
  cfguExit();
}

bool MainLoop()
{
    return aptMainLoop();
}
} // namespace Npi