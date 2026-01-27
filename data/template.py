import pandas as pd

output_file = "data.xlsx"
skill_template = pd.DataFrame({
    "Name": [
        "Example Skill A",
        "Example Skill B"
    ],
    "Notes (ignored)": [
        "",
        ""
    ],
    "Score": [
        100,
        50
    ]
})

aptitudes_template = pd.DataFrame({
    "Name": ["Special Week", "Silence Suzuka"],
    "Turf": ["S-A", "S-A"],
    "Dirt": ["G", "G"],
    "Sprint": ["B-C", "S-A"],
    "Mile": ["S-A", "S-A"],
    "Medium": ["B-C", "B-C"],
    "Long": ["B-C", "G"],
    "Front": ["B-C", "S-A"],
    "Pace": ["S-A", "B-C"],
    "Late": ["B-C", "G"],
    "End": ["D-E-F", "G"]
})

with pd.ExcelWriter(output_file, engine="openpyxl") as writer:
    skill_template.to_excel(writer, sheet_name="Gold", index=False)
    skill_template.to_excel(writer, sheet_name="Blue", index=False)
    skill_template.to_excel(writer, sheet_name="Yellow", index=False)
    skill_template.to_excel(writer, sheet_name="Green", index=False)
    skill_template.to_excel(writer, sheet_name="Red", index=False)
    skill_template.to_excel(writer, sheet_name="Purple", index=False)
    skill_template.to_excel(writer, sheet_name="Inherited Unique Skill", index=False)
    aptitudes_template.to_excel(writer, sheet_name="Aptitudes", index=False)

print(f"Template generated: {output_file}")
