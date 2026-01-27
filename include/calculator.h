#ifndef CALCULATOR_H
#define CALCULATOR_H

#include "skill.h"
#include "stats.h"
#include <vector>
#include <string>

namespace UmaCalculator {

enum class StarLevel {
    ONE_STAR,
    TWO_STAR,
    THREE_STAR,
    FOUR_STAR,
    FIVE_STAR
};

struct RankInfo {
    std::string rank_name;
    int min_score;
    int max_score;
    
    RankInfo(const std::string& name, int min, int max)
        : rank_name(name), min_score(min), max_score(max) {}
};

class RaceCalculator {
private:
    Stats current_stats;
    std::vector<Skill> equipped_skills;
    Aptitudes current_aptitudes;
    int unique_skill_level;
    StarLevel star_level;
    
    std::vector<RankInfo> rank_thresholds;
    
    void initializeRankThresholds();
    
public:
    RaceCalculator();
    
    void setStats(const Stats& stats);
    void setAptitudes(const Aptitudes& apt);
    void addSkill(const Skill& skill);
    void clearSkills();
    void setUniqueSkillLevel(int level);
    void setStarLevel(StarLevel level);
    
    int calculateStatsScore() const;
    int calculateSkillScore() const;
    int calculateUniqueSkillScore() const;
    int calculateTotalScore() const;
    
    std::string getRank(int total_score) const;
    int getPointsToNextRank(int total_score) const;
    
    void printSummary() const;
    std::string generateReport() const;
};

} 

#endif 