# Uma Musume Race Stats Calculator
~~View the HTML Version Here. Note that the HTML Version is NOT UPDATED and will not be supported (unless you fork it yourself, then my repository is fine with it).~~
Removed because it is outdated.
## Logic

### Stat-to-Score Conversion
Raw stats are transformed into a `StatScore` based on the following bracketed logic:

| Stat Value Range | Points Calculation Logic |
| :--- | :--- |
| **1 - 100** | $stat \times 0.5$ |
| **101 - 400** | $50 + (stat - 100) \times 1.0$ |
| **401 - 600** | $350 + (stat - 400) \times 1.5$ |
| **601 - 800** | $650 + (stat - 600) \times 2.0$ |
| **801 - 1000** | $1050 + (stat - 800) \times 2.5$ |
| **1001 - 1200** | $1550 + (stat - 1000) \times 3.0$ |
| **1201+** | $2150 + (stat - 1200) \times 3.5$ |

### Unique Skill Scaling
The rarity of the character determines the efficiency of their unique level:
* **Lower Rarity (1-2★)**: 120 pts per unique level.
* **Higher Rarity (3-5★)**: 170 pts per unique level.

### Aptitude Modifiers
Aptitude grades impact performance efficiency:
* **S/A Grades**: 100% efficiency.
* **B/C Grades**: 90% efficiency.
* **D/E/F Grades**: 75% efficiency.
* **G Grade**: 50% efficiency (Severe penalty).

---
## Compilation & Build

Microsoft C++ compiler:
```bash
build_gui_msvc.bat
```

MinGW Compiler:
```bash
build_gui_mingw.bat
```


## Manual Usage

### Setting up your own defaults:

```cpp
#include "calculator.h"

using namespace UmaCalculator;

int main() {
    RaceCalculator calc;

    // Define Stats
    Stats myStats;
    myStats.speed = 1200;
    myStats.stamina = 800;
    calc.setStats(myStats);

    // Set Rarity
    calc.setStarLevel(StarLevel::THREE_STAR);
    calc.setUniqueSkillLevel(4);

    // Print Results
    calc.printSummary();

    return 0;
}
```
### Adding your own Uma Musume Template in `stats.cpp`:
```cpp
void CharacterDatabase::loadCharacters() {
    // Agnes Digital
    Aptitudes agnes_digital;
    agnes_digital.turf = AptitudeGrade::S_A;
    agnes_digital.dirt = AptitudeGrade::S_A;
    agnes_digital.sprint = AptitudeGrade::D_E_F;
    agnes_digital.mile = AptitudeGrade::S_A;
    agnes_digital.medium = AptitudeGrade::S_A;
    agnes_digital.long_distance = AptitudeGrade::G;
    agnes_digital.front_runner = AptitudeGrade::G;
    agnes_digital.pace_chaser = AptitudeGrade::S_A;
    agnes_digital.late_surger = AptitudeGrade::S_A;
    agnes_digital.end_closer = AptitudeGrade::B_C;
    addCharacter(UmaCharacter("Agnes Digital", agnes_digital));
    //Add below here the same template
```
### Easy Update (Skills, Characters, Aptitudes, etc.)

This is handled by the script **`import_data.py`**.

#### Usage:
* Run `template.py` to generate the `data.xlsx` template.
* Edit the excel file values (add your new character templates, skills, etc.)
* Run `import_data.py` after. It will generate .cpp files with the correct data format so that you can manually plug-in inside `skills.cpp` and `stats.cpp`.
>
> NOTE
> 1. The output cpp files DOES NOT replace the existing files in the src folder. You must manually import them.
> 2. The output cpp files NEEDS RECOMPILATION. Which means that you need to recompile the executable after adding them.
---
# License
This is licensed under MIT. See **LICENSE.md**


