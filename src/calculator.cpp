#include "../include/calculator.h"
#include <iostream>
#include <sstream>
#include <iomanip>

/**
 * SUMMARY:
 * SCALING: 
 * - Lower rarity (1-2*) characters gain 120 pts per unique level.
 * - Higher rarity (3-5*) characters gain 170 pts per unique level.
 * EVALUATION:
 * - Total Score = StatScore (Piecewise) + Skills (Base) + Unique.
 * RANKING:
 * - Maps Total Score to a naming convention from G Rank to Ug6.
 * - PointsToNextRank calculates the delta to the next min_score threshold.
 */

namespace UmaCalculator {

RaceCalculator::RaceCalculator() 
    : unique_skill_level(4), star_level(StarLevel::THREE_STAR) {
    initializeRankThresholds();
}

void RaceCalculator::initializeRankThresholds() {
    rank_thresholds = {
        RankInfo("G Rank", 0, 299),
        RankInfo("G+ Rank", 300, 599),
        RankInfo("F Rank", 600, 899),
        RankInfo("F+ Rank", 900, 1299),
        RankInfo("E Rank", 1300, 1799),
        RankInfo("E+ Rank", 1800, 2299),
        RankInfo("D Rank", 2300, 2899),
        RankInfo("D+ Rank", 2900, 3499),
        RankInfo("C Rank", 3500, 4899),
        RankInfo("C+ Rank", 4900, 6499),
        RankInfo("B Rank", 6500, 8199),
        RankInfo("B+ Rank", 8200, 9999),
        RankInfo("A Rank", 10000, 12099),
        RankInfo("A+ Rank", 12100, 14499),
        RankInfo("S Rank", 14500, 15899),
        RankInfo("S+ Rank", 15900, 17499),
        RankInfo("SS Rank", 17500, 19199),
        RankInfo("SS+ Rank", 19200, 19599),
        RankInfo("Ug⁰ Rank", 19600, 19999),
        RankInfo("Ug¹ Rank", 20000, 20399),
        RankInfo("Ug² Rank", 20400, 20799),
        RankInfo("Ug³ Rank", 20800, 21199),
        RankInfo("Ug⁴ Rank", 21200, 21599),
        RankInfo("Ug⁵ Rank", 21600, 22099),
        RankInfo("Ug⁶ Rank", 22100, 999999)
    };
}

void RaceCalculator::setStats(const Stats& stats) {
    current_stats = stats;
}

void RaceCalculator::setAptitudes(const Aptitudes& apt) {
    current_aptitudes = apt;
}

void RaceCalculator::addSkill(const Skill& skill) {
    equipped_skills.push_back(skill);
}

void RaceCalculator::clearSkills() {
    equipped_skills.clear();
}

void RaceCalculator::setUniqueSkillLevel(int level) {
    unique_skill_level = level;
}

void RaceCalculator::setStarLevel(StarLevel level) {
    star_level = level;
}

int RaceCalculator::calculateStatsScore() const {
    return current_stats.calculateTotalScore();
}

int RaceCalculator::calculateSkillScore() const {
    int total = 0;
    for (const auto& skill : equipped_skills) {
        total += skill.getBaseScore();
    }
    return total;
}

int RaceCalculator::calculateUniqueSkillScore() const {
    int base_score = 0;
    
    switch (star_level) {
        case StarLevel::ONE_STAR:
        case StarLevel::TWO_STAR:
            base_score = unique_skill_level * 120;
            break;
        case StarLevel::THREE_STAR:
        case StarLevel::FOUR_STAR:
        case StarLevel::FIVE_STAR:
            base_score = unique_skill_level * 170;
            break;
    }
    
    return base_score;
}

int RaceCalculator::calculateTotalScore() const {
    return calculateStatsScore() + calculateSkillScore() + calculateUniqueSkillScore();
}

std::string RaceCalculator::getRank(int total_score) const {
    for (const auto& rank : rank_thresholds) {
        if (total_score >= rank.min_score && total_score <= rank.max_score) {
            return rank.rank_name;
        }
    }
    return "Unknown Rank";
}

int RaceCalculator::getPointsToNextRank(int total_score) const {
    for (size_t i = 0; i < rank_thresholds.size(); ++i) {
        if (total_score >= rank_thresholds[i].min_score && 
            total_score <= rank_thresholds[i].max_score) {
            if (i + 1 < rank_thresholds.size()) {
                return rank_thresholds[i + 1].min_score - total_score;
            }
            return 0; 
        }
    }
    return 0;
}

std::string RaceCalculator::generateReport() const {
    std::ostringstream report;
    const std::string nl = "\r\n"; 
    
    report << "" << nl;
    report << "                RESULTS                 " << nl;
    report << "" << nl << nl;
    
    report << "| STATS BREAKDOWN |" << nl;
    report << "  Speed:   " << std::setw(4) << current_stats.speed   << " (Score: " << StatCalculator::calculateStatScore(current_stats.speed) << ")" << nl;
    report << "  Stamina: " << std::setw(4) << current_stats.stamina << " (Score: " << StatCalculator::calculateStatScore(current_stats.stamina) << ")" << nl;
    report << "  Power:   " << std::setw(4) << current_stats.power   << " (Score: " << StatCalculator::calculateStatScore(current_stats.power) << ")" << nl;
    report << "  Guts:    " << std::setw(4) << current_stats.guts    << " (Score: " << StatCalculator::calculateStatScore(current_stats.guts) << ")" << nl;
    report << "  Wit:     " << std::setw(4) << current_stats.wit     << " (Score: " << StatCalculator::calculateStatScore(current_stats.wit) << ")" << nl;
    report << "  --------------------------------------" << nl;
    report << "  Stats Total Score: " << calculateStatsScore() << nl << nl;
    
    report << "| EQUIPPED SKILLS (" << equipped_skills.size() << ") |" << nl;
    if (equipped_skills.empty()) {
        report << "  (No skills equipped)" << nl;
    } else {
        int skill_idx = 1;
        for (const auto& skill : equipped_skills) {
            report << "  " << skill_idx++ << ". " << std::left << std::setw(18) << skill.getName() 
                   << " [+" << skill.getBaseScore() << "]" << nl;
        }
    }
    report << "  Skills Total Score: " << calculateSkillScore() << nl << nl;
    
    report << "| UNIQUE SKILL |" << nl;
    report << "  Level:      " << unique_skill_level << nl;
    report << "  Star Level: ";
    switch (star_level) {
        case StarLevel::ONE_STAR:   report << "1 Star"; break;
        case StarLevel::TWO_STAR:   report << "2 Stars"; break;
        case StarLevel::THREE_STAR: report << "3 Stars"; break;
        case StarLevel::FOUR_STAR:  report << "4 Stars"; break;
        case StarLevel::FIVE_STAR:  report << "5 Stars"; break;
        default:                    report << "Unknown";
    }
    report << nl << "  Unique Score: " << calculateUniqueSkillScore() << nl << nl;
    
    int total = calculateTotalScore();
    report << "-----------------------------------------" << nl;
    report << "  TOTAL SCORE: " << total << nl;
    report << "  RANK:        " << getRank(total) << nl;
    
    int points_to_next = getPointsToNextRank(total);
    if (points_to_next > 0) {
        report << "  Next Rank:   +" << points_to_next << " pts" << nl;
    } else {
        report << "  MAX RANK ACHIEVED" << nl;
    }
    report << "-----------------------------------------" << nl;
    
    return report.str();
}

void RaceCalculator::printSummary() const {
    std::cout << generateReport();
}

}
