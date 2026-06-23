#include <ft2build.h>
#include FT_FREETYPE_H

#include "src/tint/api/tint.h"

int main() {
    FT_Library freetype = nullptr;
    if (FT_Init_FreeType(&freetype) != 0) {
        return 1;
    }
    FT_Done_FreeType(freetype);
    tint::Initialize();
    return 0;
}
