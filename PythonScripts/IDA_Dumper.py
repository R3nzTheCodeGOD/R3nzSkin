from idc import BADADDR, SEARCH_DOWN
import idc
import idaapi
import datetime


def MakeNamespace(enumName, offsetArray):
    print("namespace %s {" % enumName)
    for offset in offsetArray:
        if len(offset[0]) == 0:
            print("")
            continue
        if type(offset[1]) is str:
            print("   constexpr std::uint32_t %s = %s;" % (offset[0], offset[1]))
            continue
        fncValue = offset[1] if offset[1] != -1 else 0x0
        isMismatch = fncValue == BADADDR or fncValue == 0
        if (fncValue >= idaapi.get_imagebase() and not isMismatch):
            idc.set_name(fncValue, offset[0])
            fncValue = fncValue - idaapi.get_imagebase()
        print("   constexpr std::uint32_t %s{ 0x%X }; %s" % (offset[0], fncValue, "// broken pattern" if isMismatch else ""))
    print("};\r")


def FindOffsetByString(name, offset, operandValue):
    address = idc.find_binary(0, SEARCH_DOWN, "\"" + name + "\"")
    dword = -1
    if address == BADADDR:
        return BADADDR
    xrefs = XrefsTo(address)
    for xref in xrefs:
        dword = xref.frm + offset
    if dword == BADADDR:
        return BADADDR
    return idc.get_operand_value(dword, operandValue)


def FindFuncCall(Pattern):
    addr = idc.find_binary(0, SEARCH_DOWN, Pattern)
    if addr == BADADDR: return 0
    return idc.get_operand_value(addr, 0)


def FindFunctionByPatternStartEA(pattern):
	address = idc.find_binary(0, SEARCH_DOWN, pattern)
	if address == BADADDR:
		return BADADDR
	try:
		return idaapi.get_func(address).start_ea
	except Exception:
		return -1


def FindOffsetPattern(Pattern, Operand):
    addr = idc.find_binary(0, SEARCH_DOWN, Pattern)
    if addr == BADADDR: return 0
    return idc.get_operand_value(addr, Operand)


def FindFunctionFirstXRef(name):
	address = idc.find_binary(0, SEARCH_DOWN, "\"" + name + "\"")
	dword = BADADDR
	if address == BADADDR:
		return BADADDR
	xrefs = XrefsTo(address)
	for xref in xrefs:
		dword = xref.frm
	try:
		return idaapi.get_func(dword).start_ea
	except Exception:
		return -1


def main():
    print("// [*] League of Legends Update Tool")
    print("// [*] By R3nzTheCodeGOD")
    print("// [*] Started at: %s" % datetime.datetime.now())
    MakeNamespace("Functions", [
        ["fnLogger", FindFunctionByPatternStartEA("8D 44 24 14 50 FF 74 24 14 FF")],
        ["fnCastSpell", FindFunctionFirstXRef("ERROR: Client Tried to cast a spell from")],
        ["fnGetPing", FindFunctionByPatternStartEA("55 8B EC 83 EC 08 0F B6")], # FindFuncCall("E8 ? ? ? ? 68 ? ? ? ? 8D 4D E0 66")
        ["fnSendChat", FindFunctionByPatternStartEA("A1 ? ? ? ? 56 6A FF")],
        ["fnGetTimerExpiry", FindFuncCall("E8 ? ? ? ? 51 D9 1C 24 E8 ? ? ? ? 8B")],
        ["fnHealthBarPosition", FindFuncCall("E8 ? ? ? ? 8B 4E ? 8D 54 ? ? 52 8B 01 FF ? ? 5E 83 ? ? C3")],
        ["fnHud_OnDisconnect", FindFunctionFirstXRef("game_messagebox_caption_disconnect")],
        ["fnHud_OnAfk", FindFunctionFirstXRef("game_messagebox_text_afkwarningcaption")],
        ["fnOnCreateObject", FindFuncCall("E8 ? ? ? ? E8 ? ? ? ? 84 C0 74 ? 32 C9 EB")],
        ["fnOnDeleteObject", FindFuncCall("E8 ? ? ? ? 8B 4E 20 85 C9 74 0F")],
        ["fntranslateString_UNSAFE_DONOTUSE", FindFuncCall("E8 ? ? ? ? 8B 0D ? ? ? ? 83 C4 04 8B F0 6A 0B")], # FindFunctionFirstXRef("-176")
        ["fnCharacterDataStack_Push", FindFunctionByPatternStartEA("83 EC 50 53 55 56 57 8B F9 8B 47 04")],
        ["fnCharacterDataStack_Update", FindFuncCall("E8 ? ? ? ? 5F 5E 5B 83 C4 44")],
        ["fnCharacterData_GetCharacterPackage", FindFunctionFirstXRef("Load Package - %s:%u")],
        ["fnSetBaseCharacterData", FindFuncCall("83 EC 10 80 7C 24 ? ? 53 55")],
        ["fnOnprocessSpell", FindFuncCall("E8 ? ? ? ? 8B CE E8 ? ? ? ? 80 BE ? ? ? ? ? D8")],
        ["fnGetGoldRedirectTarget", FindFuncCall("E8 ? ? ? ? 39 44 24 1C 5F")],
        ["fnGetFirstObject", FindFuncCall("E8 ? ? ? ? 8B F0 85 F6 74 1E 66 90")],
        ["fnGetNextObject", FindFunctionByPatternStartEA("8B 44 24 04 56 8B 71 18")],
        ["fnDrawCircle", FindFunctionByPatternStartEA("33 C4 89 84 24 ? ? ? ? F3 0F 10 84 24 ? ? ? ? 8D 0C")],
        ["fnWorldToScreen", FindFunctionByPatternStartEA("83 EC 10 56 E8 ? ? ? ? 8B 08")],
        ["fnr3dProjectToScreen", FindFunctionByPatternStartEA("0F 2F 05 ? ? ? ? 73 ? 0F 2F DA 77")],
        ["fnRiot_Renderer_MaterialRegistry_GetSingletonPtr", FindFunctionByPatternStartEA("A1 ? ? ? ? 85 C0 75 0B 8B 0D ? ? ? ? 8B 01 FF 60 14")],
        ["fnDrawTurretRange", FindFuncCall("E8 ? ? ? ? 84 C0 75 1B 83")],
        ["fnBaseDrawPosition", FindFunctionByPatternStartEA("30 44 14 10 42 3B D1 72 F0 8B 74 24 14 83 7C 24 ? ? 74")],
        ["fnGetBasicAttack", FindFunctionByPatternStartEA("53 8B D9 B8 ? ? ? ? 8B 93")],
        ["fnGetAttackDelay", FindFunctionByPatternStartEA("8B 44 24 04 51 F3")],
        ["fnGetAttackCastDelay", FindFunctionByPatternStartEA("83 EC 0C 53 8B 5C 24 14 8B CB 56 57 8B 03 FF 90")],
        ["fnIsNotWall", FindFuncCall("E8 ? ? ? ? 8B 0F 0F 57 C0")],
        ["fnIsAlive", FindFunctionByPatternStartEA("56 8B F1 8B 06 8B 80 ? ? ? ? FF D0 84 C0 74 19")],
        ["fnIsInhib", FindFuncCall("E8 ? ? ? ? 83 C4 04 84 C0 74 38")],
        ["fnIsNexus", FindFuncCall("E8 ? ? ? ? 55 88 44 24 20")],
        ["fnIsTurret", FindFuncCall("E8 ? ? ? ? 83 C4 04 84 C0 74 09 5F")],
        ["fnIsMinion", FindFuncCall("E8 ? ? ? ? 83 C4 04 84 C0 74 16 8B 0F")],
        ["fnIsBaron", FindFunctionFirstXRef("SRU_Baron")],
        ["fnIsHero", FindFuncCall("E8 ? ? ? ? 83 C4 04 84 C0 74 7E")],
        ["fnIsTroy", FindFuncCall("E8 ? ? ? ? 33 C9 83 C4 04 84 C0 0F 45 4C 24 ? 8B C1 C3 CC CC CC CC CC CC CC CC FF 74 24 04 E8 ? ? ? ? 33 C9 83 C4 04 84 C0 0F 45 4C 24 ? 8B C1 C3 CC CC CC CC CC CC CC CC 56")],
        ["fnIsMissile", FindFuncCall("E8 ? ? ? ? 83 C4 04 84 C0 74 60")],
        ["fnpwOpen", FindFunctionFirstXRef("game_console_chatcommand_allchat_1")],
        ["fnBuffAddRemove", FindFunctionFirstXRef("SpellToggleSlot")],
        ["fnPlaySound", FindFunctionFirstXRef("AudioManager::PlaySoundEvent: Failed to play sound event %s.")]
    ])

    MakeNamespace("Offsets", [
        ["oRiotgWindow", FindOffsetPattern("3B 05 ? ? ? ? 75 72", 1)],
        ["oIssueOrder", FindOffsetPattern("C7 05 ? ? ? ? ? ? ? ? E8 ? ? ? ? 8B 44 24 20 C7 05 ? ? ? ? ? ? ? ?", 0)],
        ["oGfxWinMsgProc", FindOffsetPattern("A1 ? ? ? ? 55 57 53", 1)],
        ["oD3DDevice", FindOffsetPattern("8B 86 ? ? ? ? 89 4C 24 08", 1)],
        ["oSwapChain", FindOffsetPattern("8B 8E ? ? ? ? 52 57", 1)],
        ["oRenderer", FindOffsetPattern("8B 15 ? ? ? ? 83 EC 08", 1)],
        ["or3dRendererInstance", FindOffsetPattern("8B 15 ? ? ? ? 83 EC 08 F3", 1)],
        ["oViewMatrix", FindOffsetPattern("8D 4A ? 51 81 C2 ? ? ? ? 52", 1)],
        ["oProjection", FindOffsetPattern("81 C2 ? ? ? ? 52 FF 74 24 38", 1)],
        ["oMainHUD", FindOffsetPattern("83 3D ? ? ? ? ? 0F 84 ? ? ? ? 57 E8 ? ? ? ? 83 C4 04", 0)],
        ["oChatClientPtr", FindOffsetPattern("8B 35 ? ? ? ? 8D 44 24 14 53 8B 1D ? ? ? ? 8B CF", 1)],
        ["oZoomInstance", FindOffsetPattern("A3 ? ? ? ? 83 FA 10 72 32", 0)],
        ["oGameClient", FindOffsetPattern("A1 ? ? ? ? 83 78 08 02 0F 94", 1)],
        ["oHudInstance", FindOffsetPattern("8B 0D ? ? ? ? 6A 00 8B 49 34 E8 ? ? ? ? B0 01 C2", 1)],
        ["oUnderMouseObject", FindOffsetPattern("8B 0D ? ? ? ? 89 0D", 1)],
        ["oGameTime", FindOffsetPattern("F3 0F 11 05 ? ? ? ? 8B 49", 0)],
        ["oChampionManager", FindOffsetPattern("89 1D ? ? ? ? 57 8D 4B 08", 0)],
        ["oCharacterDataStack", FindOffsetPattern("8D 8E ? ? ? ? FF 74 24 4C", 1)],
        ["oSkinId", FindOffsetPattern("80 BE ? ? ? ? ? 75 4D 0F 31", 0)],
        ["oObjManager", FindOffsetPattern("8B 0D ? ? ? ? E8 ? ? ? ? FF 77 08", 1)],
        ["oLocalPlayer", FindOffsetPattern("A1 ? ? ? ? 85 C0 74 07 05 ? ? ? ? EB 02 33 C0 56", 1)],
        ["oManagerTemplate_Minions", FindOffsetPattern("A1 ?? ?? ?? ?? 53 55 8B 6C 24 1C", 1)],
        ["oManagerTemplate_Heros", FindOffsetPattern("8B 0D ?? ?? ?? ?? 50 8D 44 24 18", 1)],
        ["oManagerTemplate_Turrets", FindOffsetPattern("A1 ? ? ? ? 53 56 8B 70 04", 1)],
    ])

    MakeNamespace("GameObject", [
        ["ID", FindOffsetPattern("8B 40 20 C3 33 C0 C3 CC CC", 1)],
        ["TeamID", FindOffsetPattern("8B 40 4C C3 33 C0", 1)],
        ["Name", FindOffsetPattern("8D 4E 6C E8 ? ? ? ? 8D 4E 60 E8 ? ? ? ? 8D 4E 54 E8 ? ? ? ? 8D 4E 48 E8 ? ? ? ?", 1)],
        ["NetworkID", FindOffsetPattern("8D 83 ? ? ? ? 50 8D 44 24 34 50 E8 ? ? ? ? 8B 00 89 83 ? ? ? ? 8B 8B ? ? ? ?", 1)],
        ["Position", FindOffsetPattern("8D 86 ? ? ? ? 8B 74 24 08", 1)],
        ["Mana", FindOffsetPattern("8D B3 ? ? ? ? F3 0F 11 83 ? ? ? ? BF ? ? ? ? C6 83 ? ? ? ? ?", 1)],
        ["MaxMana", FindOffsetPattern("8D B3 ? ? ? ? F3 0F 11 83 ? ? ? ? BF ? ? ? ? C6 83 ? ? ? ? ?", 1) + 0x10],
        ["Health", FindOffsetPattern("8D 8B ? ? ? ? C6 83 ? ? ? ? ? 50 8D 83 ? ? ? ? C7 83 ? ? ? ? ? ? ? ? 50", 1)],
        ["MaxHealth", FindOffsetByString("mMaxHP", 0x5, 1)],
        ["Gold", FindOffsetByString("mGold", 0x5, 1)],
        ["Experience", FindOffsetByString("mExp", -0x6, 1)],
        ["Level", FindOffsetByString("mLevelRef", -0x10, 1)],
        ["HeroInventory", FindOffsetPattern("8D 8B ? ? ? ? E8 ? ? ? ? 8D B3 ? ? ? ? C7 06 ? ? ? ? 89 5E 04 6A 18", 1)],
    ])


if __name__ == "__main__":
    main()