#ifndef STATS_H
#define STATS_H

#include <string>
#include <map>
#include <vector>

namespace UmaCalculator {

enum class AptitudeGrade {
    S_A,    // S or A
    B_C,    // B or C
    D_E_F,  // D, E, or F
    G       // G
};

struct Stats {
    int speed;
    int stamina;
    int power;
    int guts;
    int wit;
    
    Stats() : speed(0), stamina(0), power(0), guts(0), wit(0) {}
    Stats(int spd, int sta, int pwr, int gut, int wt) 
        : speed(spd), stamina(sta), power(pwr), guts(gut), wit(wt) {}
    
    int calculateTotalScore() const;
};

struct Aptitudes {
    AptitudeGrade turf;
    AptitudeGrade dirt;
    AptitudeGrade sprint;
    AptitudeGrade mile;
    AptitudeGrade medium;
    AptitudeGrade long_distance;
    AptitudeGrade front_runner;
    AptitudeGrade pace_chaser;
    AptitudeGrade late_surger;
    AptitudeGrade end_closer;
    
    Aptitudes();
};

class StatCalculator {
public:
    static int calculateStatScore(int stat_value);
    static int getAptitudeModifier(AptitudeGrade grade);
    static std::string aptitudeToString(AptitudeGrade grade);
    static AptitudeGrade stringToAptitude(const std::string& str);
};

struct UmaCharacter {
    std::string name;
    Aptitudes aptitudes;
    
    UmaCharacter() : name("Custom") {}
    UmaCharacter(const std::string& n, const Aptitudes& apt) 
        : name(n), aptitudes(apt) {}
};

class CharacterDatabase {
private:
    std::map<std::string, UmaCharacter> characters;
    
public:
    void loadCharacters();
    void addCharacter(const UmaCharacter& character);
    const UmaCharacter* findCharacter(const std::string& name) const;
    std::vector<std::string> getAllCharacterNames() const;
};

} 

#endif
