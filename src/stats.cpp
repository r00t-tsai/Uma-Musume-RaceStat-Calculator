#include "../include/stats.h"
#include <cmath>
#include <algorithm>

/**
 * LOGIC:
 * - S/A Grades: 100% efficiency.
 * - B/C Grades: 90% efficiency.
 * - D/E/F Grades: 75% efficiency.
 * - G Grade: 50% efficiency (severe penalty).
 * Raw stats are converted to a 'Score' using a bracketed multiplier system (via calculateStatScore). 
 * Higher raw stats grant exponentially more score points (e.g., points above 1200 are 7x more valuable than points below 100).
 * In the race, the total score or specific sub-stats (Speed, Power, etc.) are multiplied by the Modifier corresponding to the track (Turf/Dirt), Distance (Sprint/Mile/etc.), and Strategy (Front Runner/Pace Chaser/etc.).
 */

namespace UmaCalculator {

Aptitudes::Aptitudes() 
    : turf(AptitudeGrade::B_C), dirt(AptitudeGrade::G),
      sprint(AptitudeGrade::B_C), mile(AptitudeGrade::S_A),
      medium(AptitudeGrade::B_C), long_distance(AptitudeGrade::B_C),
      front_runner(AptitudeGrade::B_C), pace_chaser(AptitudeGrade::S_A),
      late_surger(AptitudeGrade::B_C), end_closer(AptitudeGrade::D_E_F) {}

int StatCalculator::calculateStatScore(int stat_value) {
    
    if (stat_value <= 0) return 0;
    
    double score = 0.0;
    
    if (stat_value <= 100) {
        score = stat_value * 0.5;
    } else if (stat_value <= 400) {
        score = 50 + (stat_value - 100) * 1.0;
    } else if (stat_value <= 600) {
        score = 350 + (stat_value - 400) * 1.5;
    } else if (stat_value <= 800) {
        score = 650 + (stat_value - 600) * 2.0;
    } else if (stat_value <= 1000) {
        score = 1050 + (stat_value - 800) * 2.5;
    } else if (stat_value <= 1200) {
        score = 1550 + (stat_value - 1000) * 3.0;
    } else {
        score = 2150 + (stat_value - 1200) * 3.5;
    }
    
    return static_cast<int>(std::round(score));
}

int StatCalculator::getAptitudeModifier(AptitudeGrade grade) {
    switch (grade) {
        case AptitudeGrade::S_A:   return 100;
        case AptitudeGrade::B_C:   return 90;
        case AptitudeGrade::D_E_F: return 75;
        case AptitudeGrade::G:     return 50;
        default: return 0;
    }
}

std::string StatCalculator::aptitudeToString(AptitudeGrade grade) {
    switch (grade) {
        case AptitudeGrade::S_A:   return "S-A";
        case AptitudeGrade::B_C:   return "B-C";
        case AptitudeGrade::D_E_F: return "D-E-F";
        case AptitudeGrade::G:     return "G";
        default: return "Unknown";
    }
}

AptitudeGrade StatCalculator::stringToAptitude(const std::string& str) {
    if (str == "S-A" || str == "S" || str == "A") return AptitudeGrade::S_A;
    if (str == "B-C" || str == "B" || str == "C") return AptitudeGrade::B_C;
    if (str == "D-E-F" || str == "D" || str == "E" || str == "F") return AptitudeGrade::D_E_F;
    if (str == "G") return AptitudeGrade::G;
    return AptitudeGrade::B_C;
}

int Stats::calculateTotalScore() const {
    return StatCalculator::calculateStatScore(speed) +
           StatCalculator::calculateStatScore(stamina) +
           StatCalculator::calculateStatScore(power) +
           StatCalculator::calculateStatScore(guts) +
           StatCalculator::calculateStatScore(wit);
}

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
    // Oguri Cap
    Aptitudes oguri_cap;
    oguri_cap.turf = AptitudeGrade::S_A;
    oguri_cap.dirt = AptitudeGrade::D_E_F;
    oguri_cap.sprint = AptitudeGrade::D_E_F;
    oguri_cap.mile = AptitudeGrade::S_A;
    oguri_cap.medium = AptitudeGrade::S_A;
    oguri_cap.long_distance = AptitudeGrade::B_C;
    oguri_cap.front_runner = AptitudeGrade::D_E_F;
    oguri_cap.pace_chaser = AptitudeGrade::S_A;
    oguri_cap.late_surger = AptitudeGrade::S_A;
    oguri_cap.end_closer = AptitudeGrade::D_E_F;
    addCharacter(UmaCharacter("Oguri Cap", oguri_cap));
    //Spe-chan
    Aptitudes special_week;
    special_week.turf = AptitudeGrade::S_A;
    special_week.dirt = AptitudeGrade::G;
    special_week.sprint = AptitudeGrade::B_C;
    special_week.mile = AptitudeGrade::S_A;
    special_week.medium = AptitudeGrade::S_A;
    special_week.long_distance = AptitudeGrade::B_C;
    special_week.front_runner = AptitudeGrade::B_C;
    special_week.pace_chaser = AptitudeGrade::S_A;
    special_week.late_surger = AptitudeGrade::B_C;
    special_week.end_closer = AptitudeGrade::D_E_F;
    addCharacter(UmaCharacter("Special Week", special_week));
    //Suzuka
    Aptitudes silence_suzuka;
    silence_suzuka.turf = AptitudeGrade::S_A;
    silence_suzuka.dirt = AptitudeGrade::D_E_F;
    silence_suzuka.sprint = AptitudeGrade::B_C;
    silence_suzuka.mile = AptitudeGrade::S_A;
    silence_suzuka.medium = AptitudeGrade::S_A;
    silence_suzuka.long_distance = AptitudeGrade::D_E_F;
    silence_suzuka.front_runner = AptitudeGrade::S_A;
    silence_suzuka.pace_chaser = AptitudeGrade::B_C;
    silence_suzuka.late_surger = AptitudeGrade::D_E_F;
    silence_suzuka.end_closer = AptitudeGrade::G;
    addCharacter(UmaCharacter("Silence Suzuka", silence_suzuka));
    //Hachimi
    Aptitudes tokai_teio;
    tokai_teio.turf = AptitudeGrade::S_A;
    tokai_teio.dirt = AptitudeGrade::D_E_F;
    tokai_teio.sprint = AptitudeGrade::D_E_F;
    tokai_teio.mile = AptitudeGrade::S_A;
    tokai_teio.medium = AptitudeGrade::S_A;
    tokai_teio.long_distance = AptitudeGrade::B_C;
    tokai_teio.front_runner = AptitudeGrade::B_C;
    tokai_teio.pace_chaser = AptitudeGrade::S_A;
    tokai_teio.late_surger = AptitudeGrade::S_A;
    tokai_teio.end_closer = AptitudeGrade::D_E_F;
    addCharacter(UmaCharacter("Tokai Teio", tokai_teio));
	//Custom Apptitudes
	//You can add custom uma musume aptitude templates below
}

void CharacterDatabase::addCharacter(const UmaCharacter& character) {
    characters[character.name] = character;
}

const UmaCharacter* CharacterDatabase::findCharacter(const std::string& name) const {
    auto it = characters.find(name);
    if (it != characters.end()) {
        return &(it->second);
    }
    return nullptr;
}

std::vector<std::string> CharacterDatabase::getAllCharacterNames() const {
    std::vector<std::string> names;
    for (const auto& [name, _] : characters) {
        names.push_back(name);
    }
    std::sort(names.begin(), names.end());
    return names;
}

}
