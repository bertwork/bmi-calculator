# BMI Calculator System — Project Documentation

This document describes the **BMI Calculator System**, a console-based C++ application developed for calculating Body Mass Index (BMI), classifying results using World Health Organization (WHO) guidelines, and managing user health records with file-based persistence.

**Course:** Computer Programming 2  
**Project type:** Final Project  

This documentation provides a complete technical description of the system architecture, modules, data flow, and implementation details.

---

## 1. Project Overview

### 1.1 Academic Context

This application is submitted as the **final project** for **Computer Programming 2**. The project applies course topics such as:

- Object-oriented programming (classes, encapsulation, composition)
- Control structures and menu-driven program flow
- Functions and modular code organization across multiple source files
- File input/output (PSV persistence)
- Input validation and error handling
- Practical application development from design through documentation

### 1.2 Purpose

The application helps users:

- Compute BMI from height and weight (with support for metric and imperial units).
- Receive a health category, practical advice, and risk information based on WHO BMI ranges.
- Save, view, search, and delete personal BMI records stored locally in a PSV database.

### 1.3 Problem Statement

Manual BMI calculation and record-keeping are error-prone and inconsistent. This system automates the formula, applies standardized classification rules, validates user input, and persists records so users can track measurements over time without a full database server.

### 1.4 Target Users

- Individuals monitoring personal health metrics.
- Students or developers learning object-oriented design in C++.
- Demonstration projects for introductory programming or software design courses.

### 1.5 Technology Stack

| Component | Choice |
|-----------|--------|
| Language | C++17 |
| Interface | Console (text-based menu) |
| Persistence | PSV file (`database/records.psv`) |
| Build tool | g++ (MinGW / MSYS2 on Windows) |
| IDE support | Visual Studio Code (tasks, launch configs) |

---

## 2. Objectives

| # | Objective | How it is met |
|---|-----------|----------------|
| 1 | Accurate BMI computation | `BMIService::calculateBMI()` using weight (kg) and height (meters) |
| 2 | WHO-based classification | `BMIService::classifyBMI()` with six categories (see [BMI_CLASSIFICATION.md](BMI_CLASSIFICATION.md)) |
| 3 | User-friendly interaction | `UI` class handles menus, prompts, and formatted output |
| 4 | Data persistence | `FileManager` reads/writes PSV records |
| 5 | Input validation | `getInput()` template and UI range constants prevent invalid entries |
| 6 | Maintainable structure | Separation of concerns across `User`, `BMIService`, `FileManager`, `UI`, and `App` |

---

## 3. System Architecture

The project uses a **layered architecture**: each layer has one job, and control flows **downward** from entry → application → specialized modules. `App` **owns** `FileManager` and `UI`; `BMIService` is **stateless** (static methods only).

**How to read the diagrams below**

| Symbol | Meaning |
|--------|---------|
| `│` `▼` `┌` `└` | Control or data flow direction |
| `owns` | Object lifetime tied to parent (member variable) |
| `uses` | Calls methods; does not own the object |
| `──►` | One step in a sequence |
| `[n]` | Step number in a flow |

### 3.1 Architecture Overview

| Layer | # | Component | Source files | Primary role |
|-------|---|-----------|--------------|--------------|
| Entry | 1 | `main` | `main.cpp` | Start program; create `App`; call `run()` |
| Application | 2 | `App` | `app.h`, `app.cpp` | Menu loop; route features to other modules |
| Presentation | 3 | `UI`, `getInput()` | `ui.h`, `ui.cpp`, `input_utility.h` | Menus, prompts, validation, screen output |
| Business | 4 | `BMIService` | `bmi_service.h`, `bmi_service.cpp` | BMI math, WHO classification, unit conversion |
| Data model | 5 | `User` | `user.h`, `user.cpp` | One record’s fields; PSV serialize/parse (`to_psv` / `from_psv`) |
| Persistence | 6 | `FileManager` | `file_manager.h`, `file_manager.cpp` | In-memory record list + `records.psv` |

### 3.2 Layered Structure (Vertical Stack)

Top = first code that runs. Lower layers are **used by** layers above them.

```
┌────────────────────────────────────────────────────────────────────────────┐
│ L1  ENTRY                                                                  │
│     main.cpp                                                               │
│       │  App app("database");                                              │
│       │  app.run();                                                        │
│       ▼                                                                    │
├────────────────────────────────────────────────────────────────────────────┤
│ L2  APPLICATION                                                            │
│     ┌──────────────────────────────────────────────────────────────┐       │
│     │  App                                                         │       │
│     │    run() ──► handleMenuChoice() ──► feature methods          │       │
│     └───────────────┬────────────────────────────┬─────────────────┘       │
│                     │ owns                       │ owns                    │
│         ┌───────────▼───────────┐     ┌──────────▼────────────┐            │
│         │  FileManager (L6)     │     │  UI (L3)              │            │
│         │  vector<User*>        │     │  menus & prompts      │            │
│         │  records.psv ◄──────► │     └──────────┬────────────┘            │
│         └───────────┬───────────┘                │ uses                    │
├─────────────────────┼────────────────────────────┼─────────────────────────┤
│ L3  PRESENTATION    │                            │                         │
│                     │         ┌──────────────────▼──────────────────┐      │
│                     │         │  getInput()  — numeric validation   │      │
│                     │         │  BMIService  — feet/lb conversion   │      │
│                     │         │  User        — display only (const&)│      │
├─────────────────────┼─────────┴────────────────────────────────────────────┤
│ L4  BUSINESS        │                                                      │
│                     │         ┌──────────────────────────────────────┐     │
│                     │         │  BMIService (static)                 │     │
│                     │         │    calculateBMI / classifyBMI        │     │
│                     │         │    applyToUser(User&)                │     │
│                     │         └──────────────────┬───────────────────┘     │
├─────────────────────┼────────────────────────────┼─────────────────────────┤
│ L5  DATA MODEL      │                            │                         │
│                     │         ┌──────────────────▼───────────────────┐     │
│                     └────────►│  User  (id, name, height, bmi, …)    │     │
│                               │  to_psv() / from_psv()               │     │
│                               └──────────────────────────────────────┘     │
├────────────────────────────────────────────────────────────────────────────┤
│ L6  PERSISTENCE     database/records.psv  ◄── read/write ── FileManager    │
└────────────────────────────────────────────────────────────────────────────┘
```

### 3.3 Module Dependency Map

**Who calls whom** (runtime). Rows = caller; columns = callee.

| Caller → | `UI` | `FileManager` | `BMIService` | `User` | `getInput()` | `records.psv` |
|----------|:----:|:-------------:|:------------:|:------:|:------------:|:-------------:|
| `main` | — | — | — | — | — | — |
| `App` | ✓ | ✓ | ✓ | ✓ (stack) | ✓ (delete) | — |
| `UI` | — | — | ✓ (units) | ✓ (display) | ✓ | — |
| `FileManager` | — | — | — | ✓ (heap) | — | ✓ |
| `BMIService` | — | — | — | ✓ (via `applyToUser`) | — | — |

**Dependency diagram** (arrows = “uses”; no arrow = no direct link):

```
                    ┌─────────────┐
                    │  main.cpp   │
                    └──────┬──────┘
                           │ creates
                           ▼
                    ┌─────────────┐
              ┌────►│     App     │◄────┐
              │     └──────┬──────┘     │
              │            │            │
         owns │      owns  │            │ uses
              │            │            │
    ┌─────────▼───┐  ┌─────▼─────┐      │
    │ FileManager │  │    UI     │──────┘ getInput()
    └──────┬──────┘  └─────┬─────┘
           │               │
           │ uses          │ uses
           ▼               ├──────────────────┐
    ┌─────────────┐        ▼                  ▼
    │    User     │◄── BMIService        getInput()
    └──────▲──────┘
           │
           │ read / write
           ▼
    database/records.psv
```

**Note:** `UI` does **not** own `User` records. It only formats data passed from `App` or pointers from `FileManager::read_all()`.

### 3.4 Component Ownership

| Component | Owned by | Lifetime | Notes |
|-----------|----------|----------|-------|
| `App` | `main` | Stack in `main()` | Single application controller |
| `FileManager` | `App` | Member of `App` | Loads PSV file on construct |
| `UI` | `App` | Member of `App` | Stateless display/input helpers |
| `BMIService` | — | N/A | No instances; static methods only |
| `User` records | `FileManager` | Heap (`new` / `delete`) | Stored in `FileManager::records` vector |
| `getInput()` | — | N/A | Header-only template function |

### 3.5 Layer Responsibilities

| Layer | Component(s) | Responsibility |
|-------|----------------|----------------|
| Entry | `main` | Create `App` with database folder path; invoke `run()` |
| Application | `App` | Main menu loop; delegate to `UI`, `FileManager`, and `BMIService` |
| Presentation | `UI`, `getInput()` | Menus, prompts, validation ranges, formatted output |
| Business | `BMIService` | BMI formula, unit conversion, WHO classification, `applyToUser()` |
| Data model | `User` | Record fields; PSV serialization (`to_psv` / `from_psv`) |
| Persistence | `FileManager` | Create, read, delete records; read/write `records.psv` |

### 3.6 Program Startup Flow

**Phase A — Construction** (runs once at launch)

```
[1] main()
      │
      ▼
[2] App("database") constructed
      │
      ├──► [3] FileManager constructor
      │         └──► read_from_file()     ──► load User* into memory (if PSV file exists)
      │
      └──► [4] init_database()
                └──► create database/ + records.psv header (if missing)
```

**Phase B — Main loop** (repeats until Exit)

```
[5] App::run()
      │
      └──► loop until menu choice == 7:
              │
              ├─► [5a] UI::displayMenu(record count)
              ├─► [5b] UI::menuChoice()          ──► getInput()
              ├─► [5c] App::handleMenuChoice()   ──► feature method
              └─► [5d] UI::pauseScreen()         (skipped on Exit)

[6] UI::displayHeader("Goodbye!")  ──► program ends
```

### 3.7 Main Menu Routing

Each row is one menu path. Follow **App → helpers → persistence** left to right.

| # | Menu label | `App` method | `UI` | `BMIService` | `FileManager` | `getInput()` | Saved? |
|---|------------|--------------|------|--------------|---------------|--------------|--------|
| 1 | Quick BMI Calculation | `quickCalculate()` | `collectHeightWeight`, `displayBMIResult` | `applyToUser` | — | (via UI) | No |
| 2 | Save BMI Record | `saveRecord()` | profile prompts, `displayBMIResult` | `applyToUser` | `create` | (via UI) | Yes |
| 3 | View All Records | `viewRecords()` | `promptSortOption`, `displayRecordList`, `displayBMISummary` | — | `read_all` | sort 1–4 | — |
| 4 | Search Record | `searchRecord()` | `promptLine`, `nameMatches`, `displayBMIResult` | — | `read_all` | — | — |
| 5 | Delete Record | `deleteRecord()` | `displayRecordList`, `confirm` | — | `delete_by_id` | record # | — |
| 6 | Edit Record | `editRecord()` | `displayRecordList`, `promptEditField`, field prompts, `confirm` | `applyToUser` | `update` | record #, field | Yes |
| 7 | Exit | (loop ends) | `displayHeader("Goodbye!")` | — | — | — | — |

**Routing tree** (same information, branch view):

```
                    ┌─────────────────┐
                    │  User: 1 – 7    │
                    └────────┬────────┘
                             │
         ┌───────────────────┼───────────────────┐
         │                   │                   │
    [1] Quick            [2] Save            [3] View
         │                   │                   │
         ▼                   ▼                   ▼
  quickCalculate()      saveRecord()        viewRecords()
         │                   │                   │
    UI: height/weight    UI: full profile    FM: read_all()
    BMI: applyToUser     BMI: applyToUser    UI: sort prompt
    UI: result card      UI: result card     UI: list + summary
                         FM: create ──► PSV + backup

    [4] Search     [5] Delete     [6] Edit          [7] Exit
         │              │              │                │
         ▼              ▼              ▼                ▼
  searchRecord()   deleteRecord()  editRecord()      Goodbye
    FM: read_all()   FM: read_all()  FM: read_all()
    UI: filter       UI: confirm     UI: field menu
    UI: cards        FM: delete      BMI: applyToUser
                                      FM: update ──► PSV + backup
```

### 3.8 Example Flow: Save BMI Record

**Simplified pipeline** (analyze logic first):

```
User picks [2]
    │
    ▼
App::saveRecord()
    │
    ├─► UI ──► name, gender, age, height, weight  (validated; metric stored)
    │
    ├─► User (stack object filled)
    │
    ├─► BMIService::applyToUser(user)  ──► BMI + category + advice + risk
    │
    ├─► UI::displayBMIResult(user)
    │
    └─► FileManager::create(user)
            ├─► assign ID
            ├─► new User (heap copy)
            ├─► write_to_file() ──► records.psv
            └─► backup() ──► database/backup/records_YYYY-MM-DD_HH-MM-SS.psv (keeps last 3)
```

**Sequence diagram** (participants across the top; time flows downward):

```
  USER          APP            UI          BMIService      User         FileManager      PSV
   │             │              │              │            │               │            │
   │ [2] Save    │              │              │            │               │            │
   │────────────►│              │              │            │               │            │
   │             │ prompt name  │              │            │               │            │
   │             │─────────────►│              │            │               │            │
   │◄────────────│◄─────────────│              │            │               │            │
   │ enter data  │              │              │            │               │            │
   │────────────►│ gender,age   │              │            │               │            │
   │             │─────────────►│              │            │               │            │
   │             │ height/weight│              │            │               │            │
   │             │─────────────►│ convert?     │            │               │            │
   │             │              │─────────────►│ (units)    │               │            │
   │             │ fill User    │              │            │               │            │
   │             │─────────────────────────────────────────►│               │            │
   │             │ applyToUser  │              │            │               │            │
   │             │────────────────────────────►│───────────►│ set bmi,etc.  │            │
   │             │ show result  │              │            │               │            │
   │             │─────────────►│              │            │               │            │
   │◄────────────│◄─────────────│              │            │               │            │
   │             │ create(user) │              │            │               │            │
   │             │─────────────────────────────────────────────────────────►│            │
   │             │              │              │            │ assign ID     │            │
   │             │              │              │            │ write file    │───────────►│
   │◄ saved msg ─│              │              │            │               │            │
```

| Step | Actor | Action |
|------|-------|--------|
| 1 | User | Chooses menu option 2 |
| 2 | `App` | Calls `UI` for name, gender, age, height, weight |
| 3 | `UI` | Validates input; converts feet/pounds if needed |
| 4 | `App` | Builds stack `User`; calls `BMIService::applyToUser()` |
| 5 | `UI` | Shows full BMI result card |
| 6 | `FileManager` | `create(user)` -> ID, unique_ptr copy, rewrite PSV file |
| 7 | User | Sees confirmation (e.g. `Record saved! (ID: n)`) |

### 3.9 Include Dependencies (Compile-Time)

Understanding header includes clarifies coupling:

| File | Includes |
|------|----------|
| `main.cpp` | `app.h` |
| `app.h` | `file_manager.h`, `ui.h` |
| `app.cpp` | `app.h`, `bmi_service.h` |
| `ui.h` | `bmi_service.h`, `input_utility.h`, `user.h` |
| `file_manager.h` | `user.h` |
| `bmi_service.h` | `user.h` |

`App` does not include `user.h` or `bmi_service.h` in its header; it uses them only in `app.cpp`, which keeps the application header dependent on presentation and persistence—not directly on the full model API.

### 3.10 Design Principles Applied

- **Single Responsibility** — Each class has one primary role (`BMIService` does not handle file I/O or console output).
- **Separation of Concerns** — Formatting and prompts live in `UI`; health rules in `BMIService`; storage in `FileManager`.
- **Low Coupling** — `App` orchestrates workflows without implementing BMI formulas or PSV parsing.
- **Encapsulation** — `User` fields are private; validation constants are scoped inside `UI`.

---

## 4. Module Reference — Classes and Functions

This section explains **what each class and function does**, how they interact, and what happens step by step during execution. For additional detail on pointers, the `BMIResult` struct, and file I/O mechanics, see **[CLASS_REFERENCE.md](CLASS_REFERENCE.md)**. For every console screen and prompt sequence, see **[UI_FLOW.md](UI_FLOW.md)**.

### 4.1 Program entry — `main()` (`src/main.cpp`)

| Function | Role |
|----------|------|
| `main()` | Creates one `App` object with the database folder `"database"`, calls `App::run()` to start the menu loop, and returns `0` when the program ends normally. |

`main()` does not perform BMI logic or file access directly. It only bootstraps the application controller.

---

### 4.2 Class `User` — Data model (`headers/user.h`, `src/user.cpp`)

**Purpose:** `User` is the **data model** for one BMI record. It stores profile information (name, gender, age), physical measurements (height, weight), and computed health fields (BMI, category, advice, risk). All fields are **private**; other classes access them through getters and setters. This is **encapsulation** — the internal representation can change without affecting callers as long as the public interface stays the same.

**Storage convention:** Height is always stored in **centimeters** and weight in **kilograms**, even when the user enters feet or pounds in the UI. That keeps `BMIService` and PSV storage consistent.

#### Data members

| Member | Type | Meaning |
|--------|------|---------|
| `id` | `int` | Unique record ID; `0` until assigned by `FileManager::create()` |
| `name` | `string` | Person’s name, or `"Anonymous"` for quick BMI |
| `gender` | `string` | `"Male"`, `"Female"`, `"Prefer not to say"`, or `"-"` for quick BMI |
| `age` | `int` | Age in years; `0` for quick (anonymous) calculation |
| `height` | `double` | Height in centimeters |
| `weight` | `double` | Weight in kilograms |
| `bmi` | `double` | Body Mass Index after `BMIService::applyToUser()` |
| `category` | `string` | WHO label (e.g. Normal weight) |
| `advice` | `string` | Short health recommendation for that category |
| `risk` | `string` | Risk description for that category |

#### Constructors

| Method | What it does |
|--------|--------------|
| `User()` | Default constructor: sets all fields to empty strings or zero. Used when building a new record before the user fills in data. |
| `User(id, name, gender, age, height, weight, bmi, category, advice, risk)` | Full constructor: initializes every field at once. Used when copying from a PSV line (`from_psv`) or when `FileManager` duplicates a record on the heap. |

#### Getters and setters

Each private field has a matching `get_*()` and `set_*()` pair. Getters are marked `const` because they do not modify the object. Setters allow `App`, `UI`, and `BMIService` to fill or update a record without breaking encapsulation.

Typical usage:

- `App::saveRecord()` uses setters after prompting the user.
- `BMIService::applyToUser()` uses setters to write BMI and classification results.
- `FileManager::create()` reads the ID back via `get_id()` after assigning it.

#### Serialization methods

| Method | What it does |
|--------|--------------|
| `to_psv()` | Builds one pipe-separated line for PSV storage: `id\|name\|gender\|age\|height\|weight\|bmi\|category\|advice\|risk`. Used when `FileManager` writes `records.psv`. |
| `from_psv(psvLine)` | **Static** factory: parses one PSV data line into a `User` object using `std::istringstream` and `\|` delimiters. Used when loading records from disk. Returns a fully populated `User` (including ID and saved BMI fields). |

**Design note:** `from_psv` is static because parsing does not require an existing `User` instance — it **creates** one from text.

---

### 4.3 Struct `BMIResult` and class `BMIService` — Business logic (`headers/bmi_service.h`, `src/bmi_service.cpp`)

**Purpose:** `BMIService` contains all **BMI calculation and WHO classification** logic. Every method is `static`, so no `BMIService` object is created. The class acts as a **utility** — a namespace-like group of related functions.

#### `BMIResult` (struct)

| Field | Meaning |
|-------|---------|
| `category` | WHO BMI category name |
| `advice` | Practical recommendation text |
| `risk` | Associated health risk description |

`classifyBMI()` fills this struct and returns it. `applyToUser()` then copies the three strings into a `User` record. A struct is used here because the data is a simple grouped result with no behavior.

#### Constants

| Constant | Value | Used for |
|----------|-------|----------|
| `POUND_TO_KILOGRAM` | 0.453592 | Converting pounds to kilograms (and reverse) |
| `FEET_TO_METER` | 0.3048 | Converting feet to meters |
| `CM_TO_METERS` | 100.0 | Dividing height in cm to get meters for the BMI formula |

BMI threshold boundaries are stored as **private** `constexpr` constants (`UNDERWEIGHT_THRESHOLD = 18.5`, `NORMAL_WEIGHT_THRESHOLD = 25.0`, etc.) -- implementation details of `classifyBMI` not exposed in the public API.

#### Methods

| Method | Parameters | Returns | Detailed behavior |
|--------|------------|---------|-------------------|
| `calculateBMI` | `weightKg`, `heightMeters` | `double` | Computes `weight / (height x height)`. If height or weight is zero or negative, returns `0.0` to avoid division errors. |
| `classifyBMI` | `bmi` | `BMIResult` | Compares BMI against the named private threshold constants. Sets category, advice, and risk strings for each band. See [BMI_CLASSIFICATION.md](BMI_CLASSIFICATION.md). |
| `convertMass` | `mass`, `isPoundToKg` | `double` | If `isPoundToKg` is true (default), multiplies by `POUND_TO_KILOGRAM`; otherwise divides -- converting pounds to/from kilograms. |
| `convertHeightToCm` | `feet` | `double` | Converts a height in feet to centimeters using `feet * FEET_TO_METER * CM_TO_METERS`. Called by `UI::collectHeight`. |
| `applyToUser` | `User &user` | `void` | Converts stored height from cm to meters, calls `calculateBMI`, calls `classifyBMI`, then updates the user's `bmi`, `category`, `advice`, and `risk` fields via setters. |

---

### 4.4 Class `FileManager` — Persistence (`headers/file_manager.h`, `src/file_manager.cpp`)

**Purpose:** `FileManager` is the **persistence layer**. It keeps all saved records in an in-memory `std::vector<std::unique_ptr<User>>` and synchronizes that list with `database/records.psv`. Its public API follows a **Create-Read-Update-Delete** pattern.

| Operation | Public method | Behavior |
|-----------|---------------|----------|
| Create | `create(const User &)` | Add a new record |
| Read | `read_all()` | Return non-owning pointers to all records |
| Update | `update(const User &)` | Replace one record by ID (same ID preserved) |
| Delete | `delete_by_id(int id)` | Remove one record by ID |

#### Private members

| Member | Purpose |
|--------|---------|
| `db_folder` | Folder path (default `"database"`) |
| `db_file_path` | Full path to `records.psv` |
| `records` | `vector<unique_ptr<User>>` -- owns all heap-allocated records |

#### Constructor

| Method | What it does |
|--------|--------------|
| `FileManager(folder)` | Stores paths, then immediately calls `read_from_file()` so existing PSV data is loaded into memory when the app starts. No destructor is needed -- `unique_ptr` automatically frees all `User` objects when `FileManager` is destroyed. |

#### Public methods

| Method | What it does |
|--------|--------------|
| `init_database()` | Uses `std::filesystem` to create the `database/` folder if missing. If `records.psv` does not exist, creates it and writes the header row only. If the file already exists, prints that the database is ready. |
| `getRecordCount()` | Returns `records.size()` as an `int` for the menu display and save limit check. |
| `create(const User &user)` | Makes a heap copy via `make_unique<User>(user)`, assigns the next ID to the stored copy, moves the `unique_ptr` into `records`, calls `write_to_file()`, then `backup()` (silent timestamped copy under `database/backup/`, retaining only the three newest backups), and prints a save confirmation with the new ID. |
| `update(const User &user)` | Finds the record with matching `get_id()`, replaces it in memory, calls `write_to_file()` and `backup()`, prints `Record updated! (ID: n)`, and returns `true`. If ID not found, prints an error and returns `false`. |
| `read_all()` | Returns a vector of **non-owning** `const User *` observer pointers extracted via `.get()`. Ownership remains with `FileManager`. |
| `delete_by_id(id)` | Finds the record with matching ID, erases the `unique_ptr` (automatically freeing the `User`), rewrites the PSV file, and returns `true`. If ID not found, prints an error and returns `false`. |

#### Private methods (file I/O core)

| Method | What it does |
|--------|--------------|
| `read_from_file()` | Opens `records.psv` with `ifstream` if it exists. Skips the header line, reads each non-empty line, parses with `User::from_psv`, allocates via `make_unique<User>`, and appends to `records`. |
| `write_to_file()` | Opens `records.psv` with `ofstream` (overwrites entire file). Writes the header using `|` as delimiter, then one `to_psv()` line per record. |
| `get_next_id()` | Scans all records for the maximum ID and returns `maxId + 1` so new records never reuse an ID. |
| `backup()` | After each successful save: copies `records.psv` to `database/backup/records_YYYY-MM-DD_HH-MM-SS.psv`. If more than three backup files exist, deletes the oldest by last-write time. No user-facing output. |

**Automatic backup:** Every `create()` triggers a full PSV snapshot in `database/backup/`. Only the three most recent backups are kept; older files are removed automatically when a fourth is created.

**Persistence pattern:** Load all data at startup → modify memory on create, update, or delete → rewrite the full file after each change. `create` and `update` also trigger `backup()`. Simple and reliable for a small local database.

---

### 4.5 Class `UI` — Console interface (`headers/ui.h`, `src/ui.cpp`)

> Full screen-by-screen flows: **[UI_FLOW.md](UI_FLOW.md)**

**Purpose:** `UI` handles **everything the user sees and types** — menus, prompts, validation ranges, formatted output, and search matching. It does **not** save data or compute BMI formulas directly; it delegates conversions to `BMIService` and numeric validation to `getInput()`.

#### Public constants and enums

| Name | Purpose |
|------|---------|
| `MAX_RECORDS` | `500` — upper limit enforced before save |
| `LIST_DISPLAY_OFFSET` | `1` — list numbers shown to the user start at 1, not 0 |
| `MenuOption` | Enum mapping menu keys 1–7 to actions (Quick BMI, Save, View, Search, Delete, Edit, Exit) |
| `EditFieldChoice` | Enum for edit submenu: Name, Gender, Age, Height, Weight, All, Cancel (7) |
| `SortOption` | Enum for view sort: InsertionOrder, Bmi, Name, Age (1–4) |

Private enums `GenderChoice`, `HeightUnit`, and `WeightUnit` map numeric menu choices to strings or conversion paths.

#### Display and flow methods

| Method | What it does |
|--------|--------------|
| `displayHeader(header)` | Prints a 60-character border, centers the title string, prints another border. |
| `displayMenu(count)` | Shows the main title, current record count (`count / MAX_RECORDS`), and numbered menu options from the internal `menu` vector. |
| `printLine(ch)` | Prints a line of repeated characters (`=` or `-`) for visual separation. |
| `pauseScreen()` | Clears input state and waits for Enter before returning to the main menu (used after every action except Exit). |
| `menuChoice(choice)` | Prompts for a menu option between 1 and 7 using `getInput()`; stores result in the `int &choice` reference parameter. |
| `displayBMIResult(user)` | Prints a full result card: name, gender, age, height (cm), weight (kg), then BMI, category, advice, and risk with two decimal places. |
| `displayRecordList(records)` | Iterates `vector<const User *>`, calls `displayRecordLine` for each entry with a 1-based index. |
| `displayBMISummary(records)` | After the list in View All Records: prints a `SUMMARY` block with total records, average BMI (`CYAN`), lowest and highest BMI with holder name and severity-colored category, and the most common category with count. |
| `displayRecordLine(index, user)` | **Private** — one compact line: list number, ID, name, gender, age, BMI, category. |

#### Input methods

| Method | What it does |
|--------|--------------|
| `promptLine(prompt)` | Reads a full line with `getline`; repeats until non-empty (used for name and search query). |
| `promptGender()` | Shows gender submenu, validates 1–3, returns `"Male"`, `"Female"`, or `"Prefer not to say"`. |
| `promptAge()` | Prompts for age between 2 and 120 using `getInput()`. |
| `collectHeightWeight(h, w)` | Calls `collectHeight` then `collectWeight`; both output parameters are filled in centimeters and kilograms. |
| `collectHeight(heightCm)` | User picks cm or feet. Feet: converted via `BMIService::convertHeightToCm(feet)`. |
| `collectWeight(weightKg)` | User picks kg or pounds. Pounds: converted via `BMIService::convertMass(pounds, true)`. |
| `promptEditField()` | Shows edit field submenu (1–7); returns validated choice (**7** = Cancel). |
| `promptSortOption()` | Shows sort submenu (1–4): insertion order, BMI, name, age. |
| `confirm(prompt)` | Reads a line; returns `true` if first character is `y` (case-insensitive), `false` if `n`; otherwise asks again. |
| `nameMatches(name, query)` | Lowercases both strings and checks if `query` appears anywhere inside `name` — enables partial search (e.g. `"man"` matches `"Mandy"`). |

---

### 4.6 Class `App` — Application controller (`headers/app.h`, `src/app.cpp`)

**Purpose:** `App` is the **orchestrator**. It owns `FileManager` and `UI` as member objects (composition), runs the main menu loop, and implements each feature by calling the right combination of UI, `BMIService`, and `FileManager`.

#### Member objects

| Member | Role |
|--------|------|
| `file_manager` | Loads and saves PSV records; constructed with the folder from `App`’s constructor |
| `ui` | All console interaction |

#### Constants (quick BMI placeholders)

| Constant | Value | Use |
|----------|-------|-----|
| `QUICK_CALC_AGE` | `0` | Age shown for anonymous quick calculation |
| `ANONYMOUS_NAME` | `"Anonymous"` | Name for quick BMI |
| `UNKNOWN_GENDER` | `"-"` | Gender placeholder for quick BMI |

#### Methods

| Method | What it does |
|--------|--------------|
| `App(db_folder)` | Initializes `file_manager` with the folder path and calls `init_database()` so the PSV file exists before the menu runs. |
| `run()` | Repeatedly displays the menu, reads the user’s choice, calls `handleMenuChoice`, and loops until Exit (option 7). |
| `handleMenuChoice(choice)` | `switch` on `MenuOption`: runs the matching feature, calls `pauseScreen()` except on Exit (shows goodbye header instead). |
| `quickCalculate()` | Collects height/weight only; builds a temporary `User` with anonymous placeholders; `BMIService::applyToUser()`; displays result — **not saved** to PSV. |
| `saveRecord()` | Checks record count against `MAX_RECORDS`; prompts name, gender, age, height, weight; computes BMI; displays result; passes stack `User` by const reference to `file_manager.create(user)` for persistence. |
| `viewRecords()` | Calls `read_all()`; if empty, prints `No records found.` Otherwise prompts sort order via `promptSortOption()`, sorts the display list with `sortRecordsForDisplay()` (display-only; PSV unchanged), then shows `ALL RECORDS` header, `displayRecordList()`, and `displayBMISummary()`. |
| `searchRecord()` | Loads all records, prompts search text, loops with `nameMatches`, displays full BMI card for each match. |
| `deleteRecord()` | Lists records, asks for list number via `getInput()`, resolves pointer from vector index, asks `confirm()`, calls `delete_by_id` with the record’s stored ID. |
| `editRecord()` | Lists records, selects by list number, shows current BMI card, `promptEditField()`, updates chosen field(s), `applyToUser()`, preview card, `confirm()` save, then `update()` (keeps same ID). Empty database prints `No records found.` |

---

### 4.7 Function `getInput()` — Validated numeric input (`headers/input_utility.h`)

**Type:** Function **template** (not a class member).

**Signature:** `void getInput(prompt, T &out, T min, T max)`

| Step | Behavior |
|------|----------|
| 1 | Prints the prompt and reads input as `double` first (works for `int` and `double` types). |
| 2 | For integer types (`int`), rejects values with a fractional part. |
| 3 | Checks the value is within `[min, max]` before casting to `T`. |
| 4 | On success, stores in `out` (reference), clears the rest of the input line, and returns. |
| 5 | On failure, clears error flags, discards bad input, prints an error, and loops until valid. |

**Used by:** `UI` (menu, gender, age, height, weight, edit field options) and `App` (delete/edit record number selection).

---

## 5. BMI Calculation and Classification

### 5.1 Formula

All internal calculations use **metric units**:

```
BMI = weight (kg) / [height (m)]²
```

Height entered in centimeters is converted: `height_m = height_cm / 100`.

**Example:** 164 cm, 54 kg → height = 1.64 m → BMI = 54 / (1.64 × 1.64) ≈ **20.08** → **Normal weight**.

### 5.2 WHO Classification

Categories, advice, and risk strings follow [BMI_CLASSIFICATION.md](BMI_CLASSIFICATION.md):

| BMI Range | Category |
|-----------|----------|
| Below 18.5 | Underweight |
| 18.5 – 24.9 | Normal weight |
| 25.0 – 29.9 | Overweight |
| 30.0 – 34.9 | Obese Class I |
| 35.0 – 39.9 | Obese Class II |
| 40.0 and above | Obese Class III |

Each category includes tailored **advice** and **risk** messages displayed to the user.

### 5.3 Unit Conversion

| Input unit | Conversion |
|------------|------------|
| Feet | `cm = feet × 0.3048 × 100` |
| Pounds | `kg = pounds × 0.453592` |

---

## 6. Data Persistence

### 6.1 Storage Location

```
database/records.psv
```

All saved BMI records live in this single PSV file. The program does not use a database server; `FileManager` acts as a small file-based store with in-memory caching.

### 6.1.1 Persistence operations (Create, Read, Update, Delete)

`FileManager` exposes four operations that mirror common data-management patterns:

| Operation | Method | When it runs |
|-----------|--------|--------------|
| **Create** | `create(const User &)` | User chooses Save BMI Record; assigns ID, adds to memory, rewrites PSV file, backup |
| **Read** | `read_all()` | View, Search, Delete, or Edit — returns pointers to in-memory records |
| **Update** | `update(const User &)` | User confirms edit save; replaces record by ID, rewrites PSV file, backup |
| **Delete** | `delete_by_id(int id)` | User confirms deletion; removes from memory and rewrites PSV file |

Behind the public API, **Read** also happens at startup via private `read_from_file()`, and **Create**, **Update**, and **Delete** trigger private `write_to_file()` to keep the file in sync with memory. **Create** and **Update** also call `backup()`.

### 6.2 PSV Format

**Header row:**

```
id|name|gender|age|height|weight|bmi|category|advice|risk
```

**Sample row:**

```
1|mandy|Female|21|156.00|53.00|21.78|Normal weight|Maintain current habits...|Low risk - keep it up!
```

### 6.3 Record Limits

- Maximum **500** records (`UI::MAX_RECORDS`).
- IDs are auto-incremented based on the highest existing ID.

---

## 7. Menu and User Flows

For detailed flow diagrams, submenus (height/weight units), and validation per screen, see **[UI_FLOW.md](UI_FLOW.md)**.

### 7.1 Main Menu

```
============================================================
                  BMI CALCULATOR SYSTEM
============================================================
Total Records: N / 500

MENU OPTIONS:
[1] Quick BMI Calculation
[2] Save BMI Record
[3] View All Records
[4] Search Record
[5] Delete Record
[6] Edit Record
[7] Exit
```

### 7.2 Input Validation Summary

| Input | Valid range / rules |
|-------|---------------------|
| Menu option | 1 – 7 |
| Gender | 1 – 3 |
| Age (save) | 2 – 120 years |
| Height (cm) | 50 – 272 |
| Height (feet) | 1.6 – 9 |
| Weight (kg) | 2 – 500 |
| Weight (lb) | 4 – 1102 |
| Name / search | Non-empty string |
| Delete confirm | `y` or `n` |
| Edit field | 1 – 7 (**7** = Cancel) |
| Edit save confirm | `y` or `n` |
| View sort | 1 – 4 |

Invalid numeric input is rejected with a clear message; the user is prompted again.

**Empty database:** View, Search, Delete, and Edit all print `No records found.` when `read_all()` returns no records.

---

## 8. Project File Structure

```
bmi-calculator/
├── headers/
│   ├── app.h              Application controller
│   ├── bmi_service.h      BMI logic and classification
│   ├── file_manager.h     PSV persistence
│   ├── input_utility.h    Validated numeric input template
│   ├── ui.h               Console UI
│   └── user.h             User record model
├── src/
│   ├── main.cpp           Program entry point
│   ├── app.cpp
│   ├── bmi_service.cpp
│   ├── file_manager.cpp
│   ├── ui.cpp
│   └── user.cpp
├── build/                 Compiled executable (generated)
├── database/              PSV data (generated at runtime)
├── .vscode/               Build and debug configuration
├── BMI_CLASSIFICATION.md  WHO categories reference
├── CLASS_REFERENCE.md     Classes and functions reference
├── DOCUMENTATION.md       This file
├── UI_FLOW.md             Console UI and menu flows
└── README.md              Quick start guide
```

---

## 9. Build and Run Instructions

### 9.1 Prerequisites

- C++17-compatible compiler (e.g. g++ from MSYS2/MinGW on Windows)
- Terminal or VS Code with C++ extension

### 9.2 Build (command line)

From the project root:

```bash
g++ -std=c++17 -I headers \
  src/main.cpp src/app.cpp src/file_manager.cpp \
  src/ui.cpp src/user.cpp src/bmi_service.cpp \
  -o build/bmi-calculator.exe
```

### 9.3 Build (VS Code)

1. Open the project folder in VS Code.
2. Run **Terminal → Run Build Task** (default: `Build bmi-calculator`).
3. Output: `build/bmi-calculator.exe`.

### 9.4 Run

```bash
./build/bmi-calculator.exe
```

On first run, the program creates `database/records.psv` if it does not exist.

---

## 10. Sample Test Scenario

Use this scenario to verify end-to-end behavior:

| Step | Action | Expected outcome |
|------|--------|------------------|
| 1 | Quick BMI: 164 cm, 54 kg | BMI ≈ 20.08, Normal weight |
| 2 | Save record: name, Female, age 21, 156 cm, 53 kg | Record saved with new ID |
| 3 | View all records | Saved entry appears in list |
| 4 | Search `man` | Partial match returns matching records |
| 5 | Delete record with confirmation `y` | Record removed from list and PSV file |
| 6 | Edit record: change weight, confirm `y` | BMI recalculated; `Record updated!`; PSV and backup updated |

---

## 11. Limitations

- **Console only** — No graphical user interface.
- **Local storage** — Data is not encrypted; suitable for single-user/local use only.
- **Medical disclaimer** — Results are informational; not a substitute for professional medical advice.

---

## 12. Possible Future Enhancements
- Export reports (PDF/text summary).
- Filter or export records by category.
- Simple login or record ownership per user.
- Unit tests for `BMIService` and `User::from_psv()`.

---

## 13. References

- World Health Organization (WHO) — BMI classification for adults ([BMI_CLASSIFICATION.md](BMI_CLASSIFICATION.md) in this repository).
- C++ Standard Library — `std::filesystem`, streams, and containers (C++17).

---

## 14. Related Documentation

| Document | Contents |
|----------|----------|
| [README.md](README.md) | Quick overview and setup |
| [CLASS_REFERENCE.md](CLASS_REFERENCE.md) | Classes, functions, pointers, struct, file I/O |
| [UI_FLOW.md](UI_FLOW.md) | Console screens, prompts, validation per feature |
| [BMI_CLASSIFICATION.md](BMI_CLASSIFICATION.md) | Category thresholds, advice, and risk text |
| [DOCUMENTATION.md](DOCUMENTATION.md) | Full project documentation (this file) |