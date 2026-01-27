#include "../include/skill.h"
#include <stdexcept>
#include <algorithm>

/**
 * LOGIC:
 * meetsRequirements() filters skills based on DistanceType (Sprint, Mile, etc.) and RunningStyle (Front, Pace, Late).
 * Skills can have a 'Grade Map'. If a character has a specific Aptitude (e.g., "S-A"), the skill uses a boosted value.
 * If no Grade Map is provided, the base_score is used.
 * Positive scores represent buffs (Gold/Yellow/Blue/Green), while negative scores represent debuffs (Purple).
 */

namespace UmaCalculator {

Skill::Skill(const std::string& n, SkillRarity r, int base, 
             DistanceType dist, RunningStyle style)
    : name(n), rarity(r), distance_req(dist), style_req(style) {
    scores.base_score = base;
}

void Skill::setGradeScores(const std::map<std::string, int>& grades) {
    scores.grade_scores = grades;
}

int Skill::getGradeScore(const std::string& grade) const {
    auto it = scores.grade_scores.find(grade);
    if (it != scores.grade_scores.end()) {
        return it->second;
    }
    return scores.base_score;
}

bool Skill::meetsRequirements(DistanceType distance, RunningStyle style) const {
    bool distance_match = (distance_req == DistanceType::NONE || distance_req == distance);
    bool style_match = (style_req == RunningStyle::NONE || style_req == style);
    return distance_match && style_match;
}

void SkillDatabase::loadSkills() {
   
    addSkill(Skill("Burning Spirit SPD", SkillRarity::GOLD, 633));
    addSkill(Skill("Burning Spirit STA", SkillRarity::GOLD, 633));
    addSkill(Skill("Burning Spirit PWR", SkillRarity::GOLD, 633));
    addSkill(Skill("Burning Spirit GUT", SkillRarity::GOLD, 633));
    addSkill(Skill("Burning Spirit WIT", SkillRarity::GOLD, 633));
    
    Skill runaway("Runaway", SkillRarity::GOLD, 296, DistanceType::NONE, RunningStyle::FRONT);
    runaway.setGradeScores({{"S-A", 407}, {"B-C", 333}, {"D-E-F", 296}, {"G", 259}});
    addSkill(runaway);
    
    addSkill(Skill("Super Lucky Seven", SkillRarity::GOLD, 334));
    addSkill(Skill("Fall Frenzy", SkillRarity::GOLD, 461));
    addSkill(Skill("Spring Spectacle", SkillRarity::GOLD, 461));
    addSkill(Skill("Right-Handed Demon", SkillRarity::GOLD, 461));
    addSkill(Skill("Yodo Invicta", SkillRarity::GOLD, 461));
    addSkill(Skill("Firm Course Menace", SkillRarity::GOLD, 461));
    addSkill(Skill("Swinging Maestro", SkillRarity::GOLD, 508));
    addSkill(Skill("Breath of Fresh Air", SkillRarity::GOLD, 508));
    addSkill(Skill("Iron Will", SkillRarity::GOLD, 508));
    addSkill(Skill("Indomitable", SkillRarity::GOLD, 508));
    addSkill(Skill("Unruffled", SkillRarity::GOLD, 508));
    
    Skill calm_collected("Calm and Collected", SkillRarity::GOLD, 559, DistanceType::NONE, RunningStyle::PACE);
    calm_collected.setGradeScores({{"S-A", 559}, {"B-C", 457}, {"D-E-F", 406}, {"G", 356}});
    addSkill(calm_collected);
    
    Skill be_still("Be Still", SkillRarity::BLUE, 239, DistanceType::NONE, RunningStyle::LATE);
    be_still.setGradeScores({{"S-A", 239}, {"B-C", 195}, {"D-E-F", 174}, {"G", 152}});
    addSkill(be_still);
    
    addSkill(Skill("Familiar Ground", SkillRarity::BLUE, 217));
    addSkill(Skill("Free-Spirited", SkillRarity::BLUE, 195, DistanceType::LONG));
    addSkill(Skill("Ignited Spirit STA", SkillRarity::BLUE, 263));
    addSkill(Skill("Corner Recovery ○", SkillRarity::BLUE, 217));
    addSkill(Skill("Straightaway Recovery", SkillRarity::BLUE, 217));
    addSkill(Skill("Lay Low", SkillRarity::BLUE, 217));
    addSkill(Skill("Pace Strategy", SkillRarity::BLUE, 217));
    addSkill(Skill("Calm in a Crowd", SkillRarity::BLUE, 217));
    
    Skill stamina_spare("Stamina to Spare", SkillRarity::BLUE, 239, DistanceType::NONE, RunningStyle::PACE);
    stamina_spare.setGradeScores({{"S-A", 239}, {"B-C", 195}, {"D-E-F", 174}, {"G", 152}});
    addSkill(stamina_spare);
    
    addSkill(Skill("Triple 7s", SkillRarity::BLUE, 217));
    
    addSkill(Skill("Ignited Spirit SPD", SkillRarity::YELLOW, 263));
    addSkill(Skill("Ignited Spirit PWR", SkillRarity::YELLOW, 263));
    addSkill(Skill("Ignited Spirit GUT", SkillRarity::YELLOW, 263));
    addSkill(Skill("Ignited Spirit WIT", SkillRarity::YELLOW, 263));
    addSkill(Skill("Corner Adept ○", SkillRarity::YELLOW, 217));
    addSkill(Skill("Straightaway Adept", SkillRarity::YELLOW, 217));
    addSkill(Skill("Ramp Up", SkillRarity::YELLOW, 217));
    addSkill(Skill("Homestretch Haste", SkillRarity::YELLOW, 217));
    
    Skill fast_paced("Fast-Paced", SkillRarity::YELLOW, 174, DistanceType::NONE, RunningStyle::FRONT);
    fast_paced.setGradeScores({{"S-A", 239}, {"B-C", 195}, {"D-E-F", 174}, {"G", 152}});
    addSkill(fast_paced);
    
    Skill sprint_straight_gold("Sprint Straightaways ◎", SkillRarity::YELLOW, 210, DistanceType::SPRINT);
    sprint_straight_gold.setGradeScores({{"S-A", 288}, {"B-C", 236}, {"D-E-F", 210}, {"G", 183}});
    addSkill(sprint_straight_gold);
    
    Skill mile_straight_gold("Mile Straightaways ◎", SkillRarity::YELLOW, 288, DistanceType::MILE);
    mile_straight_gold.setGradeScores({{"S-A", 288}, {"B-C", 236}, {"D-E-F", 210}, {"G", 183}});
    addSkill(mile_straight_gold);
    
    addSkill(Skill("Right-Handed ◎", SkillRarity::GREEN, 174));
    addSkill(Skill("Right-Handed ○", SkillRarity::GREEN, 129));
    addSkill(Skill("Left-Handed ◎", SkillRarity::GREEN, 174));
    addSkill(Skill("Left-Handed ○", SkillRarity::GREEN, 129));
    addSkill(Skill("Spring Runner ◎", SkillRarity::GREEN, 174));
    addSkill(Skill("Spring Runner ○", SkillRarity::GREEN, 129));
    addSkill(Skill("Summer Runner ◎", SkillRarity::GREEN, 174));
    addSkill(Skill("Summer Runner ○", SkillRarity::GREEN, 129));
    addSkill(Skill("Fall Runner ◎", SkillRarity::GREEN, 174));
    addSkill(Skill("Fall Runner ○", SkillRarity::GREEN, 129));
    addSkill(Skill("Winter Runner ◎", SkillRarity::GREEN, 174));
    addSkill(Skill("Winter Runner ○", SkillRarity::GREEN, 129));
    addSkill(Skill("Firm Conditions ◎", SkillRarity::GREEN, 174));
    addSkill(Skill("Firm Conditions ○", SkillRarity::GREEN, 129));
    addSkill(Skill("Wet Conditions ◎", SkillRarity::GREEN, 174));
    addSkill(Skill("Wet Conditions ○", SkillRarity::GREEN, 129));
    addSkill(Skill("Intimidate", SkillRarity::RED, 210, DistanceType::SPRINT));
    addSkill(Skill("Speed Eater", SkillRarity::RED, 239, DistanceType::MILE));
    addSkill(Skill("Tether", SkillRarity::RED, 239, DistanceType::MEDIUM));
    addSkill(Skill("Right-Handed ×", SkillRarity::PURPLE, -129));
    addSkill(Skill("Left-Handed ×", SkillRarity::PURPLE, -129));
    addSkill(Skill("Corner Recovery ×", SkillRarity::PURPLE, -262));
    addSkill(Skill("Corner Adept ×", SkillRarity::PURPLE, -262));
    addSkill(Skill("U=ma2", SkillRarity::INHERITED_UNIQUE, 180));
    addSkill(Skill("Pure Heart", SkillRarity::INHERITED_UNIQUE, 180));
    addSkill(Skill("Super-Duper Climax", SkillRarity::INHERITED_UNIQUE, 180));
    addSkill(Skill("Certain Victory", SkillRarity::INHERITED_UNIQUE, 180));
    addSkill(Skill("Legacy of the Strong", SkillRarity::INHERITED_UNIQUE, 180));
    addSkill(Skill("Shooting Star", SkillRarity::INHERITED_UNIQUE, 180));
}

void SkillDatabase::addSkill(const Skill& skill) {
    skills_by_rarity[skill.getRarity()].push_back(skill);
}

const std::vector<Skill>& SkillDatabase::getSkillsByRarity(SkillRarity rarity) const {
    static std::vector<Skill> empty;
    auto it = skills_by_rarity.find(rarity);
    if (it != skills_by_rarity.end()) {
        return it->second;
    }
    return empty;
}

const Skill* SkillDatabase::findSkill(const std::string& name) const {
    for (const auto& [rarity, skills] : skills_by_rarity) {
        for (const auto& skill : skills) {
            if (skill.getName() == name) {
                return &skill;
            }
        }
    }
    return nullptr;
}

std::vector<Skill> SkillDatabase::getAvailableSkills(DistanceType distance, RunningStyle style) const {
    std::vector<Skill> available;
    
    for (const auto& [rarity, skills] : skills_by_rarity) {
        for (const auto& skill : skills) {
            if (skill.meetsRequirements(distance, style)) {
                available.push_back(skill);
            }
        }
    }
    
    return available;
}

}
