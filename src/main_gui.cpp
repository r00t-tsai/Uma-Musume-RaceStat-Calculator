#include "../include/calculator.h"
#include "../include/skill.h"
#include "../include/stats.h"
#include <windows.h>
#include <commctrl.h>
#include <string>
#include <sstream>
#include <vector>

#pragma comment(lib, "comctl32.lib")
#pragma comment(linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

using namespace UmaCalculator;

HINSTANCE g_hInst;
HWND g_hMainWnd;
RaceCalculator g_calculator;
SkillDatabase g_skillDB;
CharacterDatabase g_charDB;

#define ID_SPEED_EDIT       1001
#define ID_STAMINA_EDIT     1002
#define ID_POWER_EDIT       1003
#define ID_GUTS_EDIT        1004
#define ID_WIT_EDIT         1005
#define ID_CHAR_COMBO       1006
#define ID_SKILL_LIST       1007
#define ID_ADD_SKILL_BTN    1008
#define ID_REMOVE_SKILL_BTN 1009
#define ID_CALCULATE_BTN    1010
#define ID_RESULT_EDIT      1011
#define ID_UNIQUE_LEVEL     1012
#define ID_STAR_LEVEL       1013
#define ID_SKILL_COMBO      1014
#define ID_CLEAR_SKILLS_BTN 1015

HWND hSpeedEdit, hStaminaEdit, hPowerEdit, hGutsEdit, hWitEdit;
HWND hCharCombo, hSkillList, hSkillCombo;
HWND hResultEdit;
HWND hUniqueLevelSpin, hStarLevelSpin;
HWND hUniqueLevelEdit, hStarLevelEdit;

std::string GetEditText(HWND hEdit) {
    int len = GetWindowTextLength(hEdit);
    if (len == 0) return "";

    std::vector<char> buffer(len + 1);
    GetWindowTextA(hEdit, buffer.data(), len + 1);
    return std::string(buffer.data());
}

int GetEditInt(HWND hEdit) {
    std::string text = GetEditText(hEdit);
    if (text.empty()) return 0;
    try {
        return std::stoi(text);
    } catch (...) {
        return 0;
    }
}

void SetEditText(HWND hEdit, const std::string& text) {
    SetWindowTextA(hEdit, text.c_str());
}

void LoadCharacterTemplate() {
    int index = SendMessage(hCharCombo, CB_GETCURSEL, 0, 0);
    if (index == CB_ERR) return;

    char buffer[256];
    SendMessageA(hCharCombo, CB_GETLBTEXT, index, (LPARAM)buffer);

    const auto* character = g_charDB.findCharacter(buffer);
    if (character) {
        g_calculator.setAptitudes(character->aptitudes);

        std::string msg = "Loaded character template: " + character->name + "\r\n\r\n";
        msg += "Aptitudes have been set.\r\n";
        msg += "Now enter stats and add skills, then click CALCULATE.";
        SetEditText(hResultEdit, msg);
    }
}

std::vector<std::string> g_skillNames;

void AddSkill() {
    int index = SendMessage(hSkillCombo, CB_GETCURSEL, 0, 0);
    if (index == CB_ERR || index < 0 || index >= (int)g_skillNames.size()) return;

    const Skill* skill = g_skillDB.findSkill(g_skillNames[index]);
    if (skill) {
        g_calculator.addSkill(*skill);

        std::string skillText = skill->getName() + " [+" + std::to_string(skill->getBaseScore()) + "]";
        SendMessageA(hSkillList, LB_ADDSTRING, 0, (LPARAM)skillText.c_str());
    }
}

void RemoveSkill() {
    int index = SendMessage(hSkillList, LB_GETCURSEL, 0, 0);
    if (index != LB_ERR) {
        SendMessage(hSkillList, LB_DELETESTRING, index, 0);

        std::string msg = "Skill removed from list.\r\n\r\n";
        msg += "Note: To fully update the calculation,\r\n";
        msg += "please click CALCULATE again.";
        SetEditText(hResultEdit, msg);
    }
}

void ClearAllSkills() {
    g_calculator.clearSkills();
    SendMessage(hSkillList, LB_RESETCONTENT, 0, 0);

    std::string msg = "All skills cleared.\r\n\r\n";
    msg += "Add new skills and click CALCULATE.";
    SetEditText(hResultEdit, msg);
}

void Calculate() {

    Stats stats;
    stats.speed = GetEditInt(hSpeedEdit);
    stats.stamina = GetEditInt(hStaminaEdit);
    stats.power = GetEditInt(hPowerEdit);
    stats.guts = GetEditInt(hGutsEdit);
    stats.wit = GetEditInt(hWitEdit);

    g_calculator.setStats(stats);

    int uniqueLevel = GetEditInt(hUniqueLevelEdit);
    int starLevel = GetEditInt(hStarLevelEdit);

    if (uniqueLevel < 1) uniqueLevel = 1;
    if (uniqueLevel > 4) uniqueLevel = 4;
    if (starLevel < 1) starLevel = 1;
    if (starLevel > 5) starLevel = 5;

    g_calculator.setUniqueSkillLevel(uniqueLevel);

    StarLevel star;
    switch (starLevel) {
        case 1: star = StarLevel::ONE_STAR; break;
        case 2: star = StarLevel::TWO_STAR; break;
        case 3: star = StarLevel::THREE_STAR; break;
        case 4: star = StarLevel::FOUR_STAR; break;
        case 5: star = StarLevel::FIVE_STAR; break;
        default: star = StarLevel::THREE_STAR;
    }
    g_calculator.setStarLevel(star);

    g_calculator.clearSkills();
    int skillCount = SendMessage(hSkillList, LB_GETCOUNT, 0, 0);
    for (int i = 0; i < skillCount; i++) {
        char buffer[256];
        SendMessageA(hSkillList, LB_GETTEXT, i, (LPARAM)buffer);
        std::string skillText(buffer);

        size_t bracketPos = skillText.find(" [+");
        if (bracketPos != std::string::npos) {
            std::string skillName = skillText.substr(0, bracketPos);
            const Skill* skill = g_skillDB.findSkill(skillName);
            if (skill) {
                g_calculator.addSkill(*skill);
            }
        }
    }

    std::string report = g_calculator.generateReport();
    SetEditText(hResultEdit, report);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
        case WM_COMMAND:
            switch (LOWORD(wParam)) {
                case ID_CHAR_COMBO:
                    if (HIWORD(wParam) == CBN_SELCHANGE) {
                        LoadCharacterTemplate();
                    }
                    break;

                case ID_ADD_SKILL_BTN:
                    AddSkill();
                    break;

                case ID_REMOVE_SKILL_BTN:
                    RemoveSkill();
                    break;

                case ID_CLEAR_SKILLS_BTN:
                    ClearAllSkills();
                    break;

                case ID_CALCULATE_BTN:
                    Calculate();
                    break;
            }
            break;

        case WM_DESTROY:
            PostQuitMessage(0);
            break;

        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

HWND CreateMainWindow(HINSTANCE hInstance) {

    WNDCLASSEX wcex = {};
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.hInstance = hInstance;
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszClassName = "UmaCalculatorClass";

    RegisterClassEx(&wcex);

    HWND hWnd = CreateWindowEx(
        0,
        "UmaCalculatorClass",
        "Uma Musume Race Calculator",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        680, 650,
        NULL, NULL, hInstance, NULL
    );

    return hWnd;
}

void CreateControls(HWND hWnd) {
    int x = 20, y = 20;

    CreateWindowA("STATIC", "STATS", WS_VISIBLE | WS_CHILD | SS_LEFT,
                 x, y, 200, 20, hWnd, NULL, g_hInst, NULL);
    y += 25;

    CreateWindowA("STATIC", "Speed:", WS_VISIBLE | WS_CHILD | SS_LEFT,
                 x, y, 80, 20, hWnd, NULL, g_hInst, NULL);
    hSpeedEdit = CreateWindowA("EDIT", "800", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_NUMBER,
                               x + 85, y, 100, 20, hWnd, (HMENU)ID_SPEED_EDIT, g_hInst, NULL);
    y += 25;

    CreateWindowA("STATIC", "Stamina:", WS_VISIBLE | WS_CHILD | SS_LEFT,
                 x, y, 80, 20, hWnd, NULL, g_hInst, NULL);
    hStaminaEdit = CreateWindowA("EDIT", "600", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_NUMBER,
                                 x + 85, y, 100, 20, hWnd, (HMENU)ID_STAMINA_EDIT, g_hInst, NULL);
    y += 25;

    CreateWindowA("STATIC", "Power:", WS_VISIBLE | WS_CHILD | SS_LEFT,
                 x, y, 80, 20, hWnd, NULL, g_hInst, NULL);
    hPowerEdit = CreateWindowA("EDIT", "750", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_NUMBER,
                               x + 85, y, 100, 20, hWnd, (HMENU)ID_POWER_EDIT, g_hInst, NULL);
    y += 25;

    CreateWindowA("STATIC", "Guts:", WS_VISIBLE | WS_CHILD | SS_LEFT,
                 x, y, 80, 20, hWnd, NULL, g_hInst, NULL);
    hGutsEdit = CreateWindowA("EDIT", "500", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_NUMBER,
                              x + 85, y, 100, 20, hWnd, (HMENU)ID_GUTS_EDIT, g_hInst, NULL);
    y += 25;

    CreateWindowA("STATIC", "Wit:", WS_VISIBLE | WS_CHILD | SS_LEFT,
                 x, y, 80, 20, hWnd, NULL, g_hInst, NULL);
    hWitEdit = CreateWindowA("EDIT", "400", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_NUMBER,
                             x + 85, y, 100, 20, hWnd, (HMENU)ID_WIT_EDIT, g_hInst, NULL);
    y += 40;

    CreateWindowA("STATIC", "Character Template:", WS_VISIBLE | WS_CHILD | SS_LEFT,
                 x, y, 150, 20, hWnd, NULL, g_hInst, NULL);
    y += 25;

    hCharCombo = CreateWindowA("COMBOBOX", "", WS_VISIBLE | WS_CHILD | CBS_DROPDOWNLIST | WS_VSCROLL,
                               x, y, 200, 200, hWnd, (HMENU)ID_CHAR_COMBO, g_hInst, NULL);

    auto charNames = g_charDB.getAllCharacterNames();
    for (const auto& name : charNames) {
        SendMessageA(hCharCombo, CB_ADDSTRING, 0, (LPARAM)name.c_str());
    }

    y += 40;

    CreateWindowA("STATIC", "Unique Skill Level:", WS_VISIBLE | WS_CHILD | SS_LEFT,
                 x, y, 120, 20, hWnd, NULL, g_hInst, NULL);
    hUniqueLevelEdit = CreateWindowA("EDIT", "4", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_NUMBER,
                                     x + 125, y, 40, 20, hWnd, (HMENU)ID_UNIQUE_LEVEL, g_hInst, NULL);
    y += 25;

    CreateWindowA("STATIC", "Star Level:", WS_VISIBLE | WS_CHILD | SS_LEFT,
                 x, y, 120, 20, hWnd, NULL, g_hInst, NULL);
    hStarLevelEdit = CreateWindowA("EDIT", "3", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_NUMBER,
                                   x + 125, y, 40, 20, hWnd, (HMENU)ID_STAR_LEVEL, g_hInst, NULL);

    x = 250;
    y = 20;

    CreateWindowA("STATIC", "SKILLS", WS_VISIBLE | WS_CHILD | SS_LEFT,
                 x, y, 200, 20, hWnd, NULL, g_hInst, NULL);
    y += 25;

    CreateWindowA("STATIC", "Available Skills:", WS_VISIBLE | WS_CHILD | SS_LEFT,
                 x, y, 150, 20, hWnd, NULL, g_hInst, NULL);
    y += 25;

    hSkillCombo = CreateWindowA("COMBOBOX", "", WS_VISIBLE | WS_CHILD | CBS_DROPDOWNLIST | WS_VSCROLL,
                                x, y, 300, 200, hWnd, (HMENU)ID_SKILL_COMBO, g_hInst, NULL);

    g_skillNames.clear();
    for (int rarity = 0; rarity <= 6; rarity++) {
        SkillRarity r = static_cast<SkillRarity>(rarity);
        const auto& skills = g_skillDB.getSkillsByRarity(r);
        for (const auto& skill : skills) {
            std::string skillText = skill.getName() + " [+" + std::to_string(skill.getBaseScore()) + "]";
            SendMessageA(hSkillCombo, CB_ADDSTRING, 0, (LPARAM)skillText.c_str());
            g_skillNames.push_back(skill.getName()); 

        }
    }

    y += 30;

    CreateWindowA("BUTTON", "Add Skill", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
                 x, y, 100, 25, hWnd, (HMENU)ID_ADD_SKILL_BTN, g_hInst, NULL);

    CreateWindowA("BUTTON", "Remove", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
                 x + 110, y, 90, 25, hWnd, (HMENU)ID_REMOVE_SKILL_BTN, g_hInst, NULL);

    CreateWindowA("BUTTON", "Clear All", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
                 x + 210, y, 90, 25, hWnd, (HMENU)ID_CLEAR_SKILLS_BTN, g_hInst, NULL);

    y += 35;

    CreateWindowA("STATIC", "Equipped Skills:", WS_VISIBLE | WS_CHILD | SS_LEFT,
                 x, y, 150, 20, hWnd, NULL, g_hInst, NULL);
    y += 25;

    hSkillList = CreateWindowA("LISTBOX", "", WS_VISIBLE | WS_CHILD | WS_BORDER | WS_VSCROLL | LBS_NOTIFY,
                               x, y, 400, 150, hWnd, (HMENU)ID_SKILL_LIST, g_hInst, NULL);

    y += 160;

    CreateWindowA("BUTTON", "CALCULATE", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
                 x, y, 400, 35, hWnd, (HMENU)ID_CALCULATE_BTN, g_hInst, NULL);

    y += 45;
    x = 20;

    CreateWindowA("STATIC", "RESULTS", WS_VISIBLE | WS_CHILD | SS_LEFT,
                 x, y, 200, 20, hWnd, NULL, g_hInst, NULL);
    y += 25;

    hResultEdit = CreateWindowA("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_MULTILINE | ES_READONLY | WS_VSCROLL | ES_AUTOVSCROLL,
                                x, y, 630, 200, hWnd, (HMENU)ID_RESULT_EDIT, g_hInst, NULL);

	HFONT hFont = CreateFont(16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
                         DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                         DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Consolas");

    SendMessage(hResultEdit, WM_SETFONT, (WPARAM)hFont, TRUE);

    std::string welcome = "Uma Musume Race Calculator\r\n\r\n";
    welcome += "Instructions:\r\n";
    welcome += "1. Enter your character's stats\r\n";
    welcome += "2. (Optional) Select a character template\r\n";
    welcome += "3. Select skills from the dropdown and click Add Skill\r\n";
    welcome += "4. Set unique skill level and star level\r\n";
    welcome += "5. Click CALCULATE to see your results!\r\n";
    SetWindowTextA(hResultEdit, welcome.c_str());
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    (void)hPrevInstance;
    (void)lpCmdLine;

    g_hInst = hInstance;

    INITCOMMONCONTROLSEX icex;
    icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
    icex.dwICC = ICC_STANDARD_CLASSES;
    InitCommonControlsEx(&icex);

    g_skillDB.loadSkills();
    g_charDB.loadCharacters();

    g_hMainWnd = CreateMainWindow(hInstance);
    if (!g_hMainWnd) {
        MessageBoxA(NULL, "Window creation failed!", "Error", MB_OK | MB_ICONERROR);
        return 1;
    }

    CreateControls(g_hMainWnd);

    ShowWindow(g_hMainWnd, nCmdShow);
    UpdateWindow(g_hMainWnd);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}

