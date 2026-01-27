#!/usr/bin/env python3
import pandas as pd
import sys

def generate_skill_code(excel_file):
    
    gold_df = pd.read_excel(excel_file, sheet_name='Gold')
    blue_df = pd.read_excel(excel_file, sheet_name='Blue')
    yellow_df = pd.read_excel(excel_file, sheet_name='Yellow')
    green_df = pd.read_excel(excel_file, sheet_name='Green')
    red_df = pd.read_excel(excel_file, sheet_name='Red')
    purple_df = pd.read_excel(excel_file, sheet_name='Purple')
    unique_df = pd.read_excel(excel_file, sheet_name='Inherited Unique Skill')
    
    code = []
    code.append("// Auto-generated skill data from Excel\n")
    code.append("void SkillDatabase::loadSkills() {\n")
    code.append("    // Gold Skills\n")
    for _, row in gold_df.iterrows():
        if pd.notna(row.iloc[0]) and pd.notna(row.iloc[2]):
            name = str(row.iloc[0]).strip()
            score = int(row.iloc[2])
            code.append(f'    addSkill(Skill("{name}", SkillRarity::GOLD, {score}));\n')
    code.append("\n    // Blue Skills\n")
    for _, row in blue_df.iterrows():
        if pd.notna(row.iloc[0]) and pd.notna(row.iloc[2]):
            name = str(row.iloc[0]).strip()
            score = int(row.iloc[2])
            code.append(f'    addSkill(Skill("{name}", SkillRarity::BLUE, {score}));\n')
    code.append("\n    // Yellow Skills\n")
    for _, row in yellow_df.iterrows():
        if pd.notna(row.iloc[0]) and pd.notna(row.iloc[2]):
            name = str(row.iloc[0]).strip()
            score = int(row.iloc[2])
            code.append(f'    addSkill(Skill("{name}", SkillRarity::YELLOW, {score}));\n')
    code.append("\n    // Green Skills\n")
    for _, row in green_df.iterrows():
        if pd.notna(row.iloc[0]) and pd.notna(row.iloc[2]):
            name = str(row.iloc[0]).strip()
            score = int(row.iloc[2])
            code.append(f'    addSkill(Skill("{name}", SkillRarity::GREEN, {score}));\n')
    code.append("\n    // Red Skills\n")
    for _, row in red_df.iterrows():
        if pd.notna(row.iloc[0]) and pd.notna(row.iloc[2]):
            name = str(row.iloc[0]).strip()
            score = int(row.iloc[2])
            code.append(f'    addSkill(Skill("{name}", SkillRarity::RED, {score}));\n')
    code.append("\n    // Purple Skills\n")
    for _, row in purple_df.iterrows():
        if pd.notna(row.iloc[0]) and pd.notna(row.iloc[2]):
            name = str(row.iloc[0]).strip()
            score = int(row.iloc[2])
            code.append(f'    addSkill(Skill("{name}", SkillRarity::PURPLE, {score}));\n')
    code.append("\n    // Inherited Unique Skills\n")
    for _, row in unique_df.iterrows():
        if pd.notna(row.iloc[0]) and pd.notna(row.iloc[2]):
            name = str(row.iloc[0]).strip()
            score = int(row.iloc[2])
            code.append(f'    addSkill(Skill("{name}", SkillRarity::INHERITED_UNIQUE, {score}));\n')
    
    code.append("}\n")
    
    return ''.join(code)

def generate_character_code(excel_file):
    
    apt_df = pd.read_excel(excel_file, sheet_name='Aptitudes')
    
    code = []
    code.append("// Auto-generated character data from Excel\n")
    code.append("void CharacterDatabase::loadCharacters() {\n")
    
    apt_map = {
        'S-A': 'AptitudeGrade::S_A',
        'B-C': 'AptitudeGrade::B_C',
        'D-E-F': 'AptitudeGrade::D_E_F',
        'G': 'AptitudeGrade::G'
    }
    
    for _, row in apt_df.iterrows():
        if pd.notna(row['Name']):
            name = str(row['Name']).strip()
            var_name = name.lower().replace(' ', '_').replace('-', '_')
            
            code.append(f"\n    Aptitudes {var_name};\n")
            code.append(f"    {var_name}.turf = {apt_map.get(row['Turf'], 'AptitudeGrade::B_C')};\n")
            code.append(f"    {var_name}.dirt = {apt_map.get(row['Dirt'], 'AptitudeGrade::G')};\n")
            code.append(f"    {var_name}.sprint = {apt_map.get(row['Sprint'], 'AptitudeGrade::B_C')};\n")
            code.append(f"    {var_name}.mile = {apt_map.get(row['Mile'], 'AptitudeGrade::S_A')};\n")
            code.append(f"    {var_name}.medium = {apt_map.get(row['Medium'], 'AptitudeGrade::B_C')};\n")
            code.append(f"    {var_name}.long_distance = {apt_map.get(row['Long'], 'AptitudeGrade::B_C')};\n")
            code.append(f"    {var_name}.front_runner = {apt_map.get(row['Front'], 'AptitudeGrade::B_C')};\n")
            code.append(f"    {var_name}.pace_chaser = {apt_map.get(row['Pace'], 'AptitudeGrade::S_A')};\n")
            code.append(f"    {var_name}.late_surger = {apt_map.get(row['Late'], 'AptitudeGrade::B_C')};\n")
            code.append(f"    {var_name}.end_closer = {apt_map.get(row['End'], 'AptitudeGrade::D_E_F')};\n")
            code.append(f'    addCharacter(UmaCharacter("{name}", {var_name}));\n')
    
    code.append("}\n")
    
    return ''.join(code)

def main():
    if len(sys.argv) < 2:
        print("Usage: python import_data.py <excel_file>")
        return
    
    excel_file = sys.argv[1]
    
    print("Generating skill code...")
    skill_code = generate_skill_code(excel_file)
    with open('generated_skills.cpp', 'w') as f:
        f.write(skill_code)
    print("Saved to generated_skills.cpp")
    
    print("\nGenerating character code...")
    char_code = generate_character_code(excel_file)
    with open('generated_characters.cpp', 'w') as f:
        f.write(char_code)
    print("Saved to generated_characters.cpp")
    
    print("\nDone! You can now copy the generated code into skill.cpp and stats.cpp")

if __name__ == '__main__':
    main()
