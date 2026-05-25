# BMI Calculator System

**Final Project — Computer Programming 2**

A console-based **C++17** application that calculates Body Mass Index (BMI), classifies results using **WHO (World Health Organization)** guidelines, and stores user records in a local **PSV** (pipe-separated values) database. This repository contains our **final project submission** for **Computer Programming 2**, demonstrating object-oriented design, layered architecture, input validation, and file-based persistence.

## Live Presentation
Instead of a standard PowerPoint file, you can browse our project details and code blocks directly in your browser:

**[Open Presentation Website](https://bertwork.github.io/bmi-calculator-presentation/)**

*A clean, static presentation highlighting our class layout, memory rules, and local file storage mechanics.*

---

> **For full technical detail**, see **[DOCUMENTATION.md](DOCUMENTATION.md)**.  
> **For every class and function** (pointers, structs, file handling), see **[CLASS_REFERENCE.md](CLASS_REFERENCE.md)**.
> **For console screens and menu flows**, see **[UI_FLOW.md](UI_FLOW.md)**.
> **For BMI categories, advice, and risk text**, see **[BMI_CLASSIFICATION.md](BMI_CLASSIFICATION.md)**.

---

## About the Project

This system is the **final project** for **Computer Programming 2**. It was developed to meet course requirements for structured C++ programming, including classes, file handling, user input validation, and a complete console application with multiple features.

The BMI Calculator System automates a common health metric workflow: collect height and weight (metric or imperial), compute BMI, assign a standardized category with health guidance, and optionally save results for later review. It is suitable as a **course project** or **portfolio sample** demonstrating:

- Modular C++ design (`User`, `BMIService`, `FileManager`, `UI`, `App`)
- Separation of presentation, application, domain, and data layers
- Validated console input and persistent storage without a database server

**BMI formula used:** `BMI = weight (kg) / [height (m)]²`

---

## Features

| Feature | Description |
|---------|-------------|
| Quick BMI | Calculate BMI without saving (anonymous profile) |
| Save record | Store name, gender, age, height, weight, and classification; warns on duplicate name |
| View records | List all saved entries; optional sort by BMI, name, or age; includes summary |
| Search | Find records by partial name (case-insensitive) |
| Delete | Remove a record with confirmation |
| Edit | Update one or more fields on a saved record; BMI recalculated on save |
| Unit support | Centimeters/feet, kilograms/pounds |
| Classification | Six WHO categories with advice and risk messages |
| Data limit | Up to 500 records in `database/records.psv` |

---

## Project Structure

```
bmi-calculator/
├── headers/                 Class declarations
│   ├── app.h                  Menu orchestration
│   ├── bmi_service.h          BMI calculation & classification
│   ├── colors.h               ANSI color macros
│   ├── file_manager.h         PSV read/write
│   ├── input_utility.h        Numeric input validation
│   ├── ui.h                   Console interface
│   └── user.h                 Record model
├── src/                     Implementations
├── build/                   Executable output (after build)
├── database/                records.psv (created at runtime)
├── BMI_CLASSIFICATION.md    WHO category reference
├── CLASS_REFERENCE.md       Classes and functions reference
├── DOCUMENTATION.md         Full system documentation
├── IMPROVEMENTS.md          Feature backlog and ideas
├── PLANNED_FEATURES.md      Features in progress and planned
├── UI_FLOW.md               Console UI and menu flows
└── README.md                This file
```

---

## Requirements

- **Compiler:** g++ with **C++17** support
- **Platform:** Windows (MSYS2/MinGW), Linux, or macOS
- **VSCode** (optional) — build task included in `.vscode/tasks.json`

---

## Build and Run

### Option 1 — VSCode

1. Open the project folder in VSCode.
2. Press **Ctrl+Shift+B** and select **"Build bmi-calculator"**.
3. Run the executable:

```powershell
.\build\bmi_calculator.exe
```

### Option 2 — Command line (Windows PowerShell - recommended)

Run this from the **project root**:

```powershell
mkdir build

g++ -std=c++17 -Iheaders src/main.cpp src/app.cpp src/file_manager.cpp src/ui.cpp src/user.cpp src/bmi_service.cpp -o build/bmi_calculator.exe

.\build\bmi_calculator.exe
```

On first launch, the app creates `database/records.psv` automatically.

---

## Architecture (Summary)

```
main → App → UI (menus & input)
          → BMIService (BMI & WHO classification)
          → FileManager → records.psv
          → User (data model)
```

See **[DOCUMENTATION.md](DOCUMENTATION.md)** for architecture flows, module tables, validation rules, sample test scenarios, and limitations.

---

## Documentation Index

| File | Purpose |
|------|---------|
| [DOCUMENTATION.md](DOCUMENTATION.md) | Complete project documentation |
| [CLASS_REFERENCE.md](CLASS_REFERENCE.md) | Classes, functions, pointers, struct, and file I/O |
| [UI_FLOW.md](UI_FLOW.md) | Console screens, prompts, and feature flows |
| [BMI_CLASSIFICATION.md](BMI_CLASSIFICATION.md) | WHO BMI ranges, advice, and risk definitions |
| [PLANNED_FEATURES.md](PLANNED_FEATURES.md) | Features in progress and planned |
| [IMPROVEMENTS.md](IMPROVEMENTS.md) | Feature backlog and ideas |
| [README.md](README.md) | Quick start and overview |

---

## Course Submission

| Item | Detail |
|------|--------|
| Subject | Computer Programming 2 |
| Deliverable | Final Project |
| Application | BMI Calculator System |
| Documentation | [DOCUMENTATION.md](DOCUMENTATION.md) |

---

## Disclaimer

This application is for **educational and informational purposes only**. BMI is one health indicator; results do not replace advice from a qualified healthcare professional.