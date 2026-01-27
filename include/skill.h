#ifndef SKILL_H
#define SKILL_H

#include <string>
#include <vector>
#include <map>

namespace UmaCalculator {

enum class SkillRarity {
    GOLD,
    GREEN,
    BLUE,
    YELLOW,
    RED,
    PURPLE,
    INHERITED_UNIQUE
};

enum class DistanceType {
    SPRINT,
    MILE,
    MEDIUM,
    LONG,
    NONE
};

enum class RunningStyle {
    FRONT,
    PACE,
    LATE,
    END,
    NONE
};

struct SkillScores {
    int base_score;
    std::map<std::string, int> grade_scores; // S-A, B-C, D-E-F, G
};

class Skill {
private:
    std::string name;
    SkillRarity rarity;
    SkillScores scores;
    DistanceType distance_req;
    RunningStyle style_req;
    
public:
    Skill(const std::string& n, SkillRarity r, int base, 
          DistanceType dist = DistanceType::NONE,
          RunningStyle style = RunningStyle::NONE);
    
    void setGradeScores(const std::map<std::string, int>& grades);
    
    std::string getName() const { return name; }
    SkillRarity getRarity() const { return rarity; }
    int getBaseScore() const { return scores.base_score; }
    int getGradeScore(const std::string& grade) const;
    DistanceType getDistanceReq() const { return distance_req; }
    RunningStyle getStyleReq() const { return style_req; }
    
    bool meetsRequirements(DistanceType distance, RunningStyle style) const;
};

class SkillDatabase {
private:
    std::map<SkillRarity, std::vector<Skill>> skills_by_rarity;
    
public:
    void loadSkills();
    void addSkill(const Skill& skill);
    
    const std::vector<Skill>& getSkillsByRarity(SkillRarity rarity) const;
    const Skill* findSkill(const std::string& name) const;
    std::vector<Skill> getAvailableSkills(DistanceType distance, RunningStyle style) const;
};

} 

#endif
