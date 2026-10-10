/*
    Plugin-SDK (Grand Theft Auto IV) source file
    Authors: GTA Community. See more here
    https://github.com/DK22Pac/plugin-sdk
    Do not delete this comment block. Respect others' work!
*/
#include "CTxdStore.h"
#include "Patch.h"

CPool<TxdDef>*& CTxdStore::ms_pTxdPool = *gpatternt(CPool<TxdDef>*, "A3 ? ? ? ? C3 C7 05 ? ? ? ? ? ? ? ? C3 56", 1);
rage::pgDictionary<rage::pgRef<rage::grcTexturePC>>*& CTxdStore::ms_pStoredTxd = *gpatternt(rage::pgDictionary<rage::pgRef<rage::grcTexturePC>>*, "8B 3D ? ? ? ? 85 FF 74 21 | 8B 35 ? ? ? ? 85 F6 74 1F 6A 00", 2);

int32_t CTxdStore::AddTxdSlot(const char* name) {
    return plugin::CallAndReturnDyn<int32_t>(gpattern("8B 0D ? ? ? ? 56 E8 ? ? ? ? FF 74 24 08 8B F0 C7 06 ? ? ? ? C7 46 ? ? ? ? ? E8 ? ? ? ? 8B 0D | 8B 0D ? ? ? ? 56 E8 ? ? ? ? 8B F0 8B 44 24 08 50 C7 06 00 00 00 00 C7 46 04 00 00 00 00 E8 ? ? ? ? 8B"), name);
}

void CTxdStore::AddRef(int32_t slot) {
    plugin::CallDyn(gpattern("8B 0D ? ? ? ? 8B 54 24 04 8B 41 04 F6 04 02 80 74 06 33 C0 FF 40 04 C3 8B 41 0C 0F AF C2 03 01 FF 40 04 C3 CC CC CC CC CC CC CC CC CC CC CC 8B 0D ? ? ? ? 56 8B 41 04 | E9 ? ? ? ? CC CC CC CC CC CC CC CC CC CC 8B 0D ? ? ? ? 8B @15"), slot);
}

int32_t CTxdStore::FindTxdSlot(const char* name) {
    return plugin::CallAndReturnDyn<int32_t>(gpattern("FF 74 24 04 E8 ? ? ? ? 83 C4 04 89 44 24 04 E9 ? ? ? ? CC CC CC CC CC CC CC CC CC CC CC 8B 0D | 8B 44 24 04 50 E8 ? ? ? ? 83 C4 04 89 44 24 04 E9 ? ? ? ? CC CC CC CC CC CC CC CC CC CC 8B 0D ? ? ? ? 8B"), name);
}

void CTxdStore::PushCurrentTxd() {
    plugin::CallDyn(gpattern("A1 ? ? ? ? FF 05 ? ? ? ? A3 ? ? ? ? 85 C0 | A1 ? ? ? ? B9 01 00 00 00 01 0D ? ? ? ? 85"));
}

void CTxdStore::SetCurrentTxd(int32_t slot) {
    plugin::CallDyn(gpattern("8B 4C 24 04 83 F9 FF 74 31 | 8B 54 24 04 83 FA FF 74 32 8B 0D ? ? ? ? 8B"), slot);
}

void CTxdStore::PopCurrentTxd() {
    plugin::CallDyn(gpattern("8B 0D ? ? ? ? FF 0D ? ? ? ? 8B 15 | A1 ? ? ? ? 83 2D ? ? ? ? 01 85 C0 8B"));
}

void CTxdStore::Initialise() {
    plugin::CallDyn(gpattern("6A 1C E8 ? ? ? ? 83 C4 04 85 C0 74 17 6A 10 FF 74 24 0C 8B C8 FF 74 24 0C E8 ? ? ? ? A3 ? ? ? ? C3 C7 05 ? ? ? ? ? ? ? ? C3 56"));
}

bool CTxdStore::LoadTxd(int32_t slot, const char* name) {
    return plugin::CallAndReturnDyn<bool>(gpattern("8B 0D ? ? ? ? 56 8B 41 04 57 8B 7C 24 0C F6 04 07 80 74 04 33 F6 EB 08 8B 71 0C 0F AF F7 03 31 E8 | A1 ? ? ? ? 8B 48 04 53 8B 5C 24"), slot, name);
}

void CTxdStore::RemoveTxdSlot(int32_t slot) {
    plugin::CallDyn(gpattern("8B 0D ? ? ? ? 56 8B 41 04 8B 74 24 08 F6 04 06 80 75 2F 8B 41 0C 0F AF C6 03 01 74 25 FF 48 04 83 78 04 00 7F 1C FF 35 ? ? ? ? 56 E8 ? ? ? ? 83 C4 08 84 C0 75 09 56 E8 ? ? ? ? 83 C4 04 5E C3 CC CC CC CC CC CC CC CC CC CC CC 83 EC 08 | E9 ? ? ? ? CC CC CC CC CC CC CC CC CC CC 8B 0D ? ? ? ? 8B 41 04 8B 54 24 04 F6 04 02 80 74 07 33 C0 83 40 04 01 C3 8B 41 0C 0F AF C2 03 01 83 40 04 01 C3 CC CC CC CC CC CC CC CC CC 8B @63"), slot);
}

