/*
    Plugin-SDK (Grand Theft Auto IV) source file
    Authors: GTA Community. See more here
    https://github.com/DK22Pac/plugin-sdk
    Do not delete this comment block. Respect others' work!
*/
#include "CFont.h"
#include "Patch.h"

CFontDetails& CFont::Details = *gpatternt(CFontDetails, "C7 05 ? ? ? ? ? ? ? ? C7 05 ? ? ? ? ? ? ? ? C7 05 ? ? ? ? ? ? ? ? C7 05 ? ? ? ? ? ? ? ? C7 05 ? ? ? ? ? ? ? ? C7 05 ? ? ? ? ? ? ? ? C3 CC CC CC F3 0F 10 05", 4);

int32_t CFont::DetailIndex() {
    return plugin::CallAndReturnDyn<int32_t>(gpattern("A1 ? ? ? ? 83 F8 FF 75 1E"));
}

float CFont::GetStringWidth(const wchar_t* str, bool spaces) {
    // Anchored past the first six bytes, which FusionFix 5.1 overwrites with an inline hook.
    return plugin::CallAndReturnDyn<float>(plugin::pattern::Get("75 03 D9 EE C3 89 44 24 04", -6), str, spaces);
}

void CFont::PrintString(float x, float y, const wchar_t* str, int32_t arg1, int32_t arg2) {
    // Anchored past the first six bytes, which FusionFix 5.1 overwrites with an inline hook.
    plugin::CallDyn(plugin::pattern::Get("83 EC 58 A1 ? ? ? ? 33 C4 89 44 24 54 56 57 8B 7D 10 | 83 EC 58 56 57 8B 7D 10 85 FF 0F 84", -6), x, y, str, arg1, arg2);
}

void CFont::PrintStringFromBottom(float x, float y, const wchar_t* str, int32_t arg1, int32_t arg2) {
    plugin::CallDyn(gpattern("57 8B 7C 24 10 85 FF 74 78"), x, y, str, arg1, arg2);
}

void CFont::SetOrientation(int32_t align) {
    plugin::CallDyn(gpattern("E8 ? ? ? ? 8D 0C C0 8B 44 24 04 89 04 CD ? ? ? ? C3 CC CC CC CC CC CC CC CC CC CC CC CC E8 ? ? ? ? F3 0F 10 44 24 | E8 ? ? ? ? 8B 4C 24 04 8D 04 C0 89 0C C5 ? ? ? ? C3 CC CC CC CC CC CC CC CC CC CC CC CC E8"), align);
}

void CFont::SetFontStyle(int32_t style) {
    plugin::CallDyn(gpattern("53 56 E8 ? ? ? ? 8B 5C 24 0C | E8 ? ? ? ? 8B 54 24 04 8D 0C C0"), style);
}

void CFont::SetColor(rage::Color32 const& col) {
    plugin::CallDyn(gpattern("E8 ? ? ? ? F3 0F 10 05 ? ? ? ? 8D 14 C0 8B 44 24 04 0F B6 0C D5 ? ? ? ? 66 0F 6E C9 0F 5B C9 89 04 D5 ? ? ? ? 0F 2F C1 76 35 | E8 ? ? ? ? 8B 4C 24 04 F3 0F 10 0D ? ? ? ? 8D 04 C0 03 C0 03 C0 0F B6 94 00 ? ? ? ? 03 C0 F3 0F 2A C2 0F 2F C8 89 88 ? ? ? ? 76 31"), col);
}

void CFont::SetDropColor(rage::Color32 const& col) {
    plugin::CallDyn(gpattern("E8 ? ? ? ? F3 0F 10 05 ? ? ? ? 8D 14 C0 8B 44 24 04 0F B6 0C D5 ? ? ? ? 66 0F 6E C9 0F 5B C9 89 04 D5 ? ? ? ? 0F 2F C1 76 33 | E8 ? ? ? ? 8B 4C 24 04 F3 0F 10 0D ? ? ? ? 8D 04 C0 03 C0 03 C0 0F B6 94 00 ? ? ? ? 03 C0 F3 0F 2A C2 0F 2F C8 89 88 ? ? ? ? 76 35"), col);
}

void CFont::SetEdge(float size) {
    plugin::CallDyn(gpattern("E8 ? ? ? ? F3 0F 10 44 24 ? F3 0F 5E 05 ? ? ? ? 8D 04 C0 C7 04 C5 ? ? ? ? ? ? ? ? F3 0F 11 04 C5 | E8 ? ? ? ? 0F 57 C0 8D 04 C0 03 C0 03 C0 03 C0 F3 0F 11 80 ? ? ? ? F3 0F 10"), size);
}

void CFont::SetScale(float w, float h) {
    plugin::CallDyn(gpattern("83 EC 08 56 E8 ? ? ? ? F3 0F 10 44 24"), w, h);
}

void CFont::SetWrapx(float x, float w) {
    plugin::CallDyn(gpattern("E8 ? ? ? ? F3 0F 10 44 24 ? 8D 04 C0 F3 0F 11 04 C5 ? ? ? ? F3 0F 10 44 24 | E8 ? ? ? ? F3 0F 10 44 24 04 8D 04 C0 03"), x, w);
}

void CFont::SetDropShadowPosition(float value) {
    plugin::CallDyn(gpattern("E8 ? ? ? ? F3 0F 10 44 24 ? F3 0F 5E 05 ? ? ? ? 8D 04 C0 C7 04 C5 ? ? ? ? ? ? ? ? C7 04 C5"), value);
}

void CFont::SetProportional(bool on) {
    plugin::CallDyn(gpattern("E8 ? ? ? ? 8D 0C C0 8A 44 24 04 88 04 CD ? ? ? ? C3 CC CC CC CC CC CC CC CC CC CC CC CC 83 EC 08 | E8 ? ? ? ? 8A 4C 24 04 8D 04 C0 88 0C C5 ? ? ? ? C3 CC CC CC CC CC CC CC CC CC CC CC CC E8 0B"), on);
}

void CFont::DrawFonts() {
    plugin::CallDyn(gpattern("56 57 E8 ? ? ? ? 8B F0 8B CE | 56 57 E8 ? ? ? ? 8B F0 C1 E0 0B"));
}

void CFont::SetBackground(bool enable, bool includeWrap) {
    plugin::CallDyn(gpattern("E8 ? ? ? ? 8D 0C C0 8A 44 24 04 88 04 CD ? ? ? ? 8A 44 24 08 | E8 ? ? ? ? 8A 4C 24 04 8A 54 24"), enable, includeWrap);
}

int32_t CFont::GetNumberLines(float x, float y, const wchar_t* str, int32_t arg1) {
    return plugin::CallAndReturnDyn<int32_t>(gpattern("8B 54 24 0C 83 EC 0C"), x, y, str, arg1);
}

void CFont::SetAlphaFade(uint8_t alpha) {
    plugin::CallDyn(gpattern("E8 ? ? ? ? 8D 0C C0 8A 44 24 04 88 04 CD ? ? ? ? C3 CC CC CC CC CC CC CC CC CC CC CC CC E8 ? ? ? ? 8D 0C C0 8A 44 24 04 88 04 CD ? ? ? ? 8A 44 24 08"), alpha);
}

void CFont::SetBackgroundColor(rage::Color32 const& col) {
    plugin::CallDyn(gpattern("E8 ? ? ? ? 8D 0C C0 8B 44 24 04 89 04 CD ? ? ? ? C3 CC CC CC CC CC CC CC CC CC CC CC CC E8 ? ? ? ? F3 0F 10 05"), col);
}

void CFont::GetTextRect(rage::Vector4* rect, float x, float y, const wchar_t* str) {
    plugin::CallDyn(gpattern("51 56 57 E8 ? ? ? ? F3 0F 10 44 24"), rect, x, y, str);
}

void CFont::SetLineHeight(float y) {
    plugin::CallDyn(gpattern("E8 ? ? ? ? F3 0F 10 44 24 ? 8D 04 C0 F3 0F 11 04 C5 ? ? ? ? C3 CC CC CC CC CC CC CC CC E8 ? ? ? ? 8D 0C C0 8B 44 24 04 | E8 ? ? ? ? F3 0F 10 44 24 04 8D 04 C0 F3 0F 11 04 C5 ? ? ? ? C3 CC CC CC CC CC CC CC CC E8 6B"), y);
}

void CFont::PrintString(float x, float y, const char* text) {
    _SWSTRING_INIT(text, 1);
    PrintString(x, y, _SWSTRING(1));
}
