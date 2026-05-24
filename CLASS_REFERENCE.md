# Classes and Functions Reference

**BMI Calculator System — Final Project, Computer Programming 2**

This document is the **complete API reference** for the project: every **class**, **struct**, and **function** with detailed explanations of purpose, parameters, behavior, and relationships. Pair it with:

| Document | Use when you need |
|----------|-------------------|
| [DOCUMENTATION.md](DOCUMENTATION.md) | System architecture, layers, startup, persistence |
| [UI_FLOW.md](UI_FLOW.md) | Console screens, prompts, menu flows, validation per screen |
| [BMI_CLASSIFICATION.md](BMI_CLASSIFICATION.md) | WHO BMI thresholds, advice, and risk text |

### Course concepts in this codebase

| Concept | Where it appears |
|---------|------------------|
| **Classes** (`class`) | `User`, `App`, `UI`, `FileManager`, `BMIService` — encapsulation and roles |
| **Struct** (`struct`) | `BMIResult` — grouped return data from classification |
| **Smart pointers** (`unique_ptr`) | `vector<unique_ptr<User>>` in `FileManager` — automatic memory management |
| **References** (`&`) | `applyToUser(User &)`, `getInput(T &out)`, output parameters in `UI` |
| **File handling** | `ifstream`, `ofstream`, `records.psv`, Create / Read / Delete |
| **Templates** | `getInput()` — one validator for `int` and `double` |

### Table of contents

| § | Topic |
|---|--------|
| [1](#1-classes-vs-struct-in-this-project) | Classes vs struct (`BMIResult`) |
| [2](#2-smart-pointers--how-this-project-uses-them) | Smart pointers and references |
| [3](#3-file-handling--how-this-project-uses-files) | File I/O and PSV |
| [4](#4-main--program-entry) | `main()` |
| [5](#5-class-app--application-controller) | `App` |
| [6](#6-class-user--record-data-model) | `User` |
| [7](#7-class-bmiservice--bmi-business-logic) | `BMIService` |
| [8](#8-class-filemanager--file-handling--record-storage) | `FileManager` |
| [9](#9-class-ui--console-interface) | `UI` (see also [UI_FLOW.md](UI_FLOW.md)) |
| [10](#10-function-getinput--input-validation-template) | `getInput()` |
| [11](#11-quick-reference--all-source-files) | Source file map |
| [12](#12-class-interaction-matrix) | Who calls whom |
| [13](#13-complete-function-index) | All functions A–Z by component |
| [14](#14-concept-summary) | Short recap |

### Master component index

| Component | Type | Header | Implementation | § |
|-----------|------|--------|----------------|---|
| `main` | function | — | `main.cpp` | [4](#4-main--program-entry) |
| `App` | class | `app.h` | `app.cpp` | [5](#5-class-app--application-controller) |
| `User` | class | `user.h` | `user.cpp` | [6](#6-class-user--record-data-model) |
| `BMIResult` | struct | `bmi_service.h` | — | [1](#1-classes-vs-struct-in-this-project) |
| `BMIService` | class (static) | `bmi_service.h` | `bmi_service.cpp` | [7](#7-class-bmiservice--bmi-business-logic) |
| `FileManager` | class | `file_manager.h` | `file_manager.cpp` | [8](#8-class-filemanager--file-handling--record-storage) |
| `UI` | class | `ui.h` | `ui.cpp` | [9](#9-class-ui--console-interface) |
| `getInput` | template | `input_utility.h` | — | [10](#10-function-getinput--input-validation-template) |

---

## 1. Classes vs Struct in This Project

In C++, `class` and `struct` are almost the same. The main difference is default access:

| Keyword | Default member access | Typical use in this project |
|---------|----------------------|-----------------------------|
| `class` | `private` | Types with behavior + hidden data (`User`, `FileManager`) |
| `struct` | `public` | Simple data containers (`BMIResult`) |

### `BMIResult` (struct)

A lightweight container for three related strings returned after BMI classification. No methods—only public fields.

```cpp
struct BMIResult {
  std::string category;
  std::string advice;
  std::string risk;
};
```

| Field | Set by | Consumed by |
|-------|--------|-------------|
| `category` | `classifyBMI()` threshold logic | `applyToUser()` → `User::set_category` |
| `advice` | Same function, per category | `User::set_advice` |
| `risk` | Same function, per category | `User::set_risk` |

**Why struct here?** `classifyBMI()` must return three strings at once. A struct avoids three separate return values or out-parameters. `User` is not used as the return type because classification is a **pure calculation** — it does not need name, age, or height.

**Note on display color:** `BMIResult` contains only `category`, `advice`, and `risk`. Display color is derived separately via `User::get_text_color()` at display time and is never stored in the struct or PSV file.

### Classes used for OOP

| Class | Role |
|-------|------|
| `User` | Data model with private fields and getters/setters |
| `BMIService` | Business logic (static methods) |
| `FileManager` | File I/O and in-memory record list |
| `UI` | Console interface |
| `App` | Main controller; owns `FileManager` and `UI` |

---

## 2. Smart Pointers — How This Project Uses Them

A **smart pointer** wraps a raw pointer and automatically manages the lifetime of the object it points to. The project uses `std::unique_ptr<User>` so that heap-allocated records are freed automatically when `FileManager` is destroyed — no manual `delete` needed.

### Smart pointer examples in the codebase

| Code | Type | Purpose |
|------|------|---------|
| `std::vector<std::unique_ptr<User>> records` | Container of smart pointers | `FileManager` holds all saved records |
| `std::make_unique<User>(...)` | Heap allocation | Create record when loading PSV or saving |
| `records.push_back(std::move(stored))` | Transfer ownership | Move unique_ptr into the vector |
| `const User *record` | Raw observer pointer | Read-only access in search/view loops (non-owning) |
| `u.get()` | Raw pointer from unique_ptr | `read_all()` exposes non-owning pointers to callers |
| `record->get_name()` | Arrow operator | Access member through pointer |
| `*record` | Dereference | Pass object to `displayBMIResult(const User &)` |

### Text flow: pointer lifecycle of one saved record

```
App::saveRecord()
    User user;                      // stack object (temporary)
    file_manager.create(user);      // passes user by const reference

FileManager::create(const User &user)
    auto stored = make_unique<User>(user);  // heap copy via unique_ptr
    stored->set_id(...);                    // assign ID to the stored copy
    records.push_back(std::move(stored));   // move into vector
    write_to_file();

Later: read_all() returns vector<const User *>
    App loops: for (const User *record : records)
        ui.displayBMIResult(*record);       // dereference for display

On destruction:
    FileManager destroyed → unique_ptrs free heap memory automatically
```

### Reference vs pointer (also used)

| Syntax | Example | Meaning |
|--------|---------|---------|
| `User &user` | `applyToUser(User &user)` | Alias; must refer to valid object; not null |
| `double &heightCm` | `collectHeightWeight(double &heightCm, ...)` | Output parameter; function updates caller's variable |
| `const User &user` | `displayBMIResult(const User &user)` | Read-only reference; no copy |

**Course note:** References (`&`) are used when the object always exists. `unique_ptr` is used for heap-owned objects where lifetime must be managed. Raw observer pointers (`const User *`) are used when reading data without owning it.

---

## 3. File Handling — How This Project Uses Files

All persistent data is stored in **`database/records.psv`**. The `FileManager` class handles every file operation.

### Libraries and types

| Include / type | Purpose |
|----------------|---------|
| `<fstream>` | `std::ifstream` (read), `std::ofstream` (write) |
| `<filesystem>` | `std::filesystem::exists`, `create_directories` |
| `std::ifstream file(db_file_path)` | Open PSV file for reading |
| `std::ofstream file(db_file_path)` | Open PSV file for writing (overwrites file) |
| `std::getline(file, line)` | Read one line at a time |
| `file << data` | Write text to file |
| `file.is_open()` | Check if open succeeded |

### File operations map

| Function | File mode | What it does |
|----------|-----------|--------------|
| `init_database()` | Create folder/file | Creates `database/` and empty PSV file with header if missing |
| `read_from_file()` | Read (`ifstream`) | Loads all records into `vector<unique_ptr<User>>` on startup |
| `write_to_file()` | Write (`ofstream`) | Rewrites entire PSV file from memory |
| `create()` | Calls `write_to_file()` | After adding record |
| `delete_by_id()` | Calls `write_to_file()` | After removing record |

### Text flow: reading a record from PSV

```
1. Open records.psv with ifstream
2. Read first line → skip (header)
3. For each data line:
       User::from_psv(line)           → builds User on stack
       make_unique<User>(from_psv)    → copy to heap via unique_ptr
       records.push_back(move(...))   → transfer ownership into vector
4. Close file (ifstream destructor)
```

### Text flow: saving after create/delete

```
1. Open records.psv with ofstream (truncates file)
2. Write header row
3. For each unique_ptr in records:
       file << user->to_psv() << "\n"
4. Close file
```

**Course note:** The program keeps data **in memory** (`vector<unique_ptr<User>>`) while running and **syncs to disk** on every create or delete.

---

## 4. `main` — Program Entry

**File:** `src/main.cpp`

| Function | Returns | What it does |
|----------|---------|--------------|
| `main()` | `int` | Creates `App` with folder `"database"`, calls `run()`, returns `0` |

```cpp
int main() {
  App app("database");
  app.run();
  return 0;
}
```

### Detailed behavior

1. **Construction** — `App` constructor runs `FileManager("database")` (loads PSV file) and `init_database()` (creates folder/file if needed).
2. **Execution** — `run()` owns the entire interactive session until the user selects Exit.
3. **Termination** — Returns `0` to the OS; `App` destructor destroys `file_manager` (unique_ptrs free all `User` records automatically) and `ui`.

`main` includes only `app.h` — the smallest possible entry point with no direct dependency on `User`, `BMIService`, or `UI` headers.

---

## 5. Class `App` — Application Controller

**Files:** `headers/app.h`, `src/app.cpp`

**Purpose:** `App` is the **central controller** of the program. It does not format the screen or parse PSV files itself. Instead, it owns `FileManager` and `UI`, runs the infinite menu loop in `run()`, and routes each menu choice to the correct workflow method.

**Composition (member objects, not pointers):**

```cpp
FileManager file_manager;  // owns database logic
UI ui;                     // owns console interface
```

`BMIService` is never stored as a member — only static methods are called when BMI is needed.

### Constants (private)

| Name | Value | Purpose |
|------|-------|---------|
| `QUICK_CALC_AGE` | `0` | Age shown for quick (anonymous) BMI |
| `ANONYMOUS_NAME` | `"Anonymous"` | Name for quick BMI |
| `UNKNOWN_GENDER` | `"-"` | Gender placeholder for quick BMI |

### Public methods — detailed

#### `App(const std::string &db_folder)`

- **When called:** Once from `main()`, before `run()`.
- **Steps:** Initializes `file_manager` with the folder path. Calls `file_manager.init_database()` so the folder and PSV header exist before the first menu display.
- **Does not:** Start the menu loop (that is `run()`'s job).

#### `void run()`

- **When called:** Immediately after construction in `main()`.
- **Steps:**
  1. Declares `menu_choice` and the exit value.
  2. Loop: `displayMenu(getRecordCount())` → `menuChoice(menu_choice)` → `handleMenuChoice(static_cast<MenuOption>(menu_choice))`.
  3. Repeats until `menu_choice == EXIT`.
- **Effect:** This is the **main program loop**; all features are reached through this cycle.

### Private methods — detailed

#### `void handleMenuChoice(UI::MenuOption choice)`

- **Input:** Enum value from the user's menu selection (1–6).
- **Behavior:** `switch` dispatches to `quickCalculate`, `saveRecord`, `viewRecords`, `searchRecord`, or `deleteRecord`. Each branch (except Exit) ends with `ui.pauseScreen()`. Exit prints a goodbye header and returns without pausing.

#### `void quickCalculate()`

1. `ui.collectHeightWeight` fills `heightCm` and `weightKg` (metric internally).
2. Stack `User user` is created; name, gender, and age set to anonymous placeholders.
3. `BMIService::applyToUser(user)` computes BMI and classification.
4. `ui.displayBMIResult(user)` shows the card.
5. **No call to `file_manager.create`** — data is discarded when the function ends.

#### `void saveRecord()`

1. If `getRecordCount() >= UI::MAX_RECORDS`, prints limit message and returns.
2. Prompts name, gender, age, height/weight.
3. `BMIService::applyToUser(user)` fills BMI fields.
4. Shows result, then `file_manager.create(user)` — passes `user` by const reference; `FileManager` copies to heap via `unique_ptr` and assigns ID.

#### `void viewRecords()`

1. `read_all()` gets `vector<const User *>`.
2. If empty → message. Else → `displayRecordList(records)`.

#### `void searchRecord()`

1. Loads all records; exits early if none.
2. Reads search query via `promptLine`.
3. Two-pass loop: first counts total matches, then displays each with `displayBMIResult(*record, current, totalMatches)`.
4. Prints match count before results: `N record(s) found for "query".`
5. If no match, prints not-found message.

#### `void deleteRecord()`

1. Lists all records with header and `displayRecordList`.
2. `getInput` asks for list number.
3. Resolves `const User *target` using `selection - LIST_DISPLAY_OFFSET` as vector index.
4. `ui.confirm` with personalized prompt including target's name.
5. On yes, `delete_by_id(target->get_id())` — uses **database ID**, not list index.

### `App` workflow summary table

| Method | `UI` calls | `BMIService` | `FileManager` | Persists? |
|--------|------------|--------------|---------------|-----------|
| `quickCalculate` | `collectHeightWeight`, `displayBMIResult` | `applyToUser` | — | No |
| `saveRecord` | profile + height/weight + result | `applyToUser` | `create` | Yes |
| `viewRecords` | `displayRecordList` | — | `read_all` | — |
| `searchRecord` | `promptLine`, `nameMatches`, `displayBMIResult` | — | `read_all` | — |
| `deleteRecord` | header, list, `confirm` | — | `read_all`, `delete_by_id` | Yes (remove) |

---

## 6. Class `User` — Record Data Model

**Files:** `headers/user.h`, `src/user.cpp`

**Purpose:** `User` is the **entity** that travels through the system: created in `App`, enriched by `BMIService`, displayed by `UI`, and stored by `FileManager`. It demonstrates **encapsulation** — ten private fields with a controlled public interface.

**Note on includes:** `user.h` includes `colors.h` — required for `get_text_color()` to reference the ANSI macros (`YELLOW`, `GREEN`, `LYELLOW`, `RED`, `BOLD`, `LRED`, `RESET`).

### Private data members

| Member | Type | Description |
|--------|------|-------------|
| `id` | `int` | Unique record ID |
| `name` | `string` | Person's name |
| `gender` | `string` | Male / Female / Prefer not to say |
| `age` | `int` | Age in years |
| `height` | `double` | Height in **cm** |
| `weight` | `double` | Weight in **kg** |
| `bmi` | `double` | Computed BMI |
| `category` | `string` | WHO category |
| `advice` | `string` | Health advice text |
| `risk` | `string` | Risk description |

### Constructors — detailed

#### `User()`

Initializes: `id = 0`, empty strings for text fields, numeric fields `0` / `0.0`. Used when `App` builds a fresh record before user input.

#### `User(int id_, const std::string &name_, ...)`

Member initializer list sets all ten fields in one step. Parameters use a trailing underscore to avoid shadowing the member variables. Used by:

- `User::from_psv` when loading from disk
- `FileManager::create` when doing `make_unique<User>(user)` (heap copy)

### Getters — detailed

| Method | Returns | Notes |
|--------|---------|-------|
| `get_id()` | `int` | `0` until `FileManager::create` assigns an ID |
| `get_name()` | `string` | Copy of name string |
| `get_gender()` | `string` | Copy of gender |
| `get_age()` | `int` | Years |
| `get_height()` | `double` | Always centimeters |
| `get_weight()` | `double` | Always kilograms |
| `get_bmi()` | `double` | Set by `applyToUser` |
| `get_category()` | `string` | WHO label |
| `get_advice()` | `string` | Recommendation text |
| `get_risk()` | `string` | Risk text |
| `get_text_color()` | `string` | Derives ANSI color string from `category`; computed on every call, not stored |

All getters are `const` — safe to call on read-only references like `displayBMIResult(const User &)`.

### Setters — detailed

Setters are grouped by which layer calls them:

| Method | Called by |
|--------|-----------|
| `set_name`, `set_gender`, `set_age`, `set_height`, `set_weight` | `App` — filled during save or quick BMI |
| `set_bmi`, `set_category`, `set_advice`, `set_risk` | `BMIService::applyToUser` |
| `set_id` | `FileManager::create` internally |

**Note:** `text_color` has no setter — color is always derived from `category` via `get_text_color()`, so it stays in sync automatically and is never written to the PSV file.

### File-related methods — detailed

#### `std::string to_psv() const`

- Uses `std::ostringstream` to join fields with `|` as delimiter.
- No trailing delimiter; one line = one record.
- Using `|` avoids corruption when fields like `advice` or `risk` contain commas.
- `text_color` is not included — it is always re-derived from `category` at display time.

#### `static User from_psv(const std::string &psvLine)`

- Uses `std::istringstream` and `getline(..., '|')` to split tokens.
- Parses `id` and `age` with `stoi`; `height`, `weight`, `bmi` with `stod`.
- All numeric conversions are wrapped in `try/catch` — malformed lines log a warning and return a default `User` instead of crashing.
- Returns a **value** `User` on the stack; caller (`FileManager`) copies to heap with `make_unique`.

---

## 7. Class `BMIService` — BMI Business Logic

**Files:** `headers/bmi_service.h`, `src/bmi_service.cpp`

**Purpose:** Isolates **all health math and rules** from UI and file code. **No `BMIService` instances** exist — only `static` methods, so the class cannot hold per-user state.

### Public constants

| Constant | Value | Used for |
|----------|-------|----------|
| `POUND_TO_KILOGRAM` | 0.453592 | Pounds → kg |
| `FEET_TO_METER` | 0.3048 | Feet → meters |
| `CM_TO_METERS` | 100.0 | cm → m for BMI formula |

### Private constants (BMI thresholds)

| Constant | Value | Boundary |
|----------|-------|----------|
| `UNDERWEIGHT_THRESHOLD` | 18.5 | Below = Underweight |
| `NORMAL_WEIGHT_THRESHOLD` | 25.0 | Below = Normal weight |
| `OVERWEIGHT_THRESHOLD` | 30.0 | Below = Overweight |
| `OBESE_CLASS_I_THRESHOLD` | 35.0 | Below = Obese Class I |
| `OBESE_CLASS_II_THRESHOLD` | 40.0 | Below = Obese Class II, else Class III |

### Methods — detailed

#### `static double calculateBMI(double weightKg, double heightMeters)`

- **Formula:** `weight / (height × height)`.
- **Guard:** If `height <= 0` or `weight <= 0`, returns `0.0` instead of dividing by zero.
- **Called by:** `applyToUser` after converting cm to meters.

#### `static BMIResult classifyBMI(double bmi)`

- **Returns:** Local `BMIResult` struct with three strings filled by a chain of `if / else if` using the named threshold constants.
- Each branch sets **different** `advice` and `risk` strings (see [BMI_CLASSIFICATION.md](BMI_CLASSIFICATION.md)).
- **Does not** modify any `User` — only returns data.

#### `static double convertMass(double mass, bool isPoundToKg = true)`

- **Default `true`:** `mass * POUND_TO_KILOGRAM` (pounds → kg).
- **`false`:** `mass / POUND_TO_KILOGRAM` (kg → pounds).

#### `static double convertHeightToCm(double feet)`

- Converts a height value in feet to centimeters using `feet * FEET_TO_METER * CM_TO_METERS`.
- Called by `UI::collectHeight` when the user selects feet as the input unit.

#### `static void applyToUser(User &user)`

Orchestrates the full calculation pipeline:

1. `heightMeters = user.get_height() / CM_TO_METERS`
2. `bmi = calculateBMI(user.get_weight(), heightMeters)`
3. `result = classifyBMI(bmi)`
4. `user.set_bmi`, `set_category`, `set_advice`, `set_risk`

---

## 8. Class `FileManager` — File Handling & Record Storage

**Files:** `headers/file_manager.h`, `src/file_manager.cpp`

**Purpose:** Acts as a **local data store**: load PSV file at startup, keep records in RAM as `unique_ptr<User>`, sync disk on every create or delete. Public API maps to **Create**, **Read**, and **Delete** (no Update).

| Pattern | Public API | Private helper |
|---------|------------|----------------|
| Create | `create()` | `get_next_id()`, `write_to_file()` |
| Read | `read_all()` | `read_from_file()` (startup) |
| Delete | `delete_by_id()` | `write_to_file()` |

### Private members

| Member | Type | Purpose |
|--------|------|---------|
| `db_folder` | `string` | Folder path (e.g. `database`) |
| `db_file_path` | `string` | Full path to `records.psv` |
| **`records`** | **`vector<unique_ptr<User>>`** | **Smart pointer container** — owns all records |

### Public methods — detailed

#### `FileManager(const std::string &folder)`

- Sets `db_folder` and `db_file_path = folder + "/records.psv"`.
- Calls `read_from_file()` immediately so previous session data is available.
- No destructor needed — `unique_ptr` automatically frees all `User` objects when `FileManager` is destroyed.

#### `void init_database()`

- `fs::exists` / `create_directories` for folder.
- If PSV file missing: `ofstream` writes header row only.
- If PSV file exists: prints ready message.

#### `int getRecordCount() const`

- Returns `static_cast<int>(records.size())` for menu and save limit.

#### `void create(const User &user)`

1. Makes a heap copy via `make_unique<User>(user)`.
2. Assigns the next ID to the stored copy via `stored->set_id(get_next_id())`.
3. Moves the `unique_ptr` into `records`.
4. Calls `write_to_file()` — full file rewrite.
5. Prints `Record saved! (ID: n)`.

#### `std::vector<const User *> read_all() const`

- Returns a vector of raw **non-owning** observer pointers extracted from the `unique_ptr` records via `.get()`.
- **Ownership:** `FileManager` still owns the `User` objects via `unique_ptr`; callers may only read through these pointers.

#### `bool delete_by_id(int id)`

1. Linear search for matching `get_id()`.
2. Erases the `unique_ptr` from the vector (destructor frees the `User` automatically).
3. `write_to_file()`.
4. Returns `true` on success; `false` + stderr message if ID not found.

### Private methods — detailed

#### `void read_from_file()`

- No-op if file path does not exist yet (first run).
- Opens `ifstream`; on failure prints error and returns.
- Skips first line (header); for each data line: `User::from_psv` → `make_unique<User>(...)` → `push_back`.

#### `void write_to_file()`

- Opens `ofstream` (truncates existing file).
- Writes header, then each `user->to_psv()` on its own line using `|` as delimiter.
- Entire database rewritten each time — simple consistency model.

#### `int get_next_id() const`

- Scans all records for maximum `id`, returns `maxId + 1`.
- Ensures monotonic IDs even after deletions.

---

## 9. Class `UI` — Console Interface

**Files:** `headers/ui.h`, `src/ui.cpp`

**Purpose:** **Presentation layer** — all console output and user prompts. `UI` does not save files or compute BMI; it collects and displays data and delegates unit conversion to `BMIService` and numeric checks to `getInput()`.

> **Screen-by-screen flows**: see **[UI_FLOW.md](UI_FLOW.md)**.

### Public constants

| Constant | Value | Meaning |
|----------|-------|---------|
| `MAX_RECORDS` | 500 | Checked in `App::saveRecord` before save |
| `LIST_DISPLAY_OFFSET` | 1 | List labels start at `[1]`, not `[0]` |

### Enums

| Enum | Values | Purpose |
|------|--------|---------|
| `MenuOption` | 1–6 | `QUICK_BMI`, `SAVE_RECORD`, `VIEW_RECORDS`, `SEARCH`, `DELETE`, `EXIT` |
| `GenderChoice` (private) | 1–3 | Internal; mapped to display strings in `promptGender` |
| `HeightUnit` (private) | 1–2 | Centimeters or feet input path |
| `WeightUnit` (private) | 1–2 | Kilograms or pounds input path |

### Public methods — full reference

#### Display and navigation

| Method | Signature | Detailed explanation |
|--------|-----------|----------------------|
| `displayHeader` | `void (const string &header) const` | Prints `printLine('=')`, centers `header` in 60 characters, prints closing `=`. Applied color: `BOLD + MAGENTA`. |
| `displayMenu` | `void (int currentRecordCount) const` | Calls `displayHeader`, prints record count, then each menu option in `CYAN`. Does not read input. |
| `printLine` | `void (char ch = '=') const` | One line of `ch` repeated `LINE_WIDTH` times. |
| `pauseScreen` | `void () const` | Prints `-` line and `YELLOW` `Press Enter to continue...`; clears and ignores to newline. |
| `menuChoice` | `void (int &choice) const` | Prompts for option 1–N using `getInput`; writes valid choice into `choice`. |

#### Results and lists

| Method | Signature | Detailed explanation |
|--------|-----------|----------------------|
| `displayBMIResult` | `void (const User &user, int current = 0, int total = 0) const` | Full BMI result card. When `total > 0` (search context), header shows `BMI RESULT (N of Total)`; otherwise shows `BMI RESULT`. Category, advice, and risk are colored via `user.get_text_color()`. BMI value is `CYAN`. |
| `displayRecordList` | `void (const vector<const User *> &records) const` | Iterates records; calls `displayRecordLine` for each entry with a 1-based index. |

#### Text and profile input

| Method | Signature | Detailed explanation |
|--------|-----------|----------------------|
| `promptLine` | `string (const string &prompt) const` | Reads a full line; loops until non-empty. Empty input error shown in `RED`. |
| `promptGender` | `string () const` | Shows gender submenu; returns `"Male"`, `"Female"`, or `"Prefer not to say"`. |
| `promptAge` | `int () const` | `getInput` for age between `MIN_USER_AGE` and `MAX_USER_AGE`. |
| `collectHeightWeight` | `void (double &heightCm, double &weightKg) const` | Calls `collectHeight` then `collectWeight`; both outputs always metric. |
| `confirm` | `bool (const string &prompt) const` | Returns `true` on `y`, `false` on `n`; re-prompts otherwise. Invalid input shown in `RED`. |
| `nameMatches` | `bool (const string &name, const string &query) const` | Case-insensitive substring match; empty query matches all. |

### Private methods — full reference

| Method | Signature | Detailed explanation |
|--------|-----------|----------------------|
| `displayRecordLine` | `void (int listIndex, const User &user) const` | Single compact row with list index in `CYAN`, ID, name, gender, age, BMI, and category colored via `user.get_text_color()`. |
| `collectHeight` | `void (double &heightCm) const` | Unit submenu → if cm: `getInput` into `heightCm`; if feet: `getInput` into local `feet`, then `heightCm = BMIService::convertHeightToCm(feet)`. |
| `collectWeight` | `void (double &weightKg) const` | Unit submenu → if kg: direct `getInput`; if pounds: `getInput` then `weightKg = convertMass(pounds, true)`. |

---

## 10. Function `getInput()` — Input Validation Template

**File:** `headers/input_utility.h`

| Item | Detail |
|------|--------|
| Type | **Function template** (not a class) |
| Signature | `void getInput(std::string_view prompt, T &out, T min, T max)` |
| Constraint | `static_assert(std::is_arithmetic_v<T>)` — only `int`, `double`, etc. |
| **`T &out`** | **Reference** — writes valid number back to caller |

### Step-by-step behavior

1. **Prompt** — prints `prompt`.
2. **Read as `double`** — single `cin >> raw` attempt.
3. **Integer check** — if `T` is integral, rejects when `floor(raw) != raw`.
4. **Range check** — compares `raw` to `min`/`max` before `static_cast<T>`.
5. **Success** — assigns `out`, ignores remainder of line, `return`.
6. **Failure** — `cin.clear()`, ignore line, print range error in `RED`, loop forever until valid.

---

## 11. Quick Reference — All Source Files

| File | Contains |
|------|----------|
| `main.cpp` | `main()` |
| `app.cpp` | `App` methods |
| `user.cpp` | `User` methods, PSV parse/format (`to_psv` / `from_psv`), `get_text_color()` |
| `bmi_service.cpp` | BMI math and classification |
| `file_manager.cpp` | **File I/O**, smart pointers, `make_unique` |
| `ui.cpp` | Console UI |
| `headers/colors.h` | ANSI color and style macros |

---

## 12. Class Interaction Matrix

Runtime **calls** between components (✓ = direct use).

| Caller ↓ / Callee → | `App` | `UI` | `FileManager` | `BMIService` | `User` | `getInput` |
|---------------------|:-----:|:----:|:-------------:|:------------:|:------:|:----------:|
| `main` | ✓ | — | — | — | — | — |
| `App` | — | ✓ | ✓ | ✓ | ✓ | ✓ |
| `UI` | — | — | — | ✓ | ✓ | ✓ |
| `FileManager` | — | — | — | — | ✓ | — |
| `BMIService` | — | — | — | — | ✓ | — |

---

## 13. Complete Function Index

### `main`

| Function | Returns | Summary |
|----------|---------|---------|
| `main()` | `int` | `App app("database"); app.run(); return 0;` |

### `App`

| Function | Access | Summary |
|----------|--------|---------|
| `App(db_folder)` | public | Construct `FileManager` + `UI`; `init_database()` |
| `run()` | public | Menu loop until Exit |
| `handleMenuChoice(choice)` | private | Dispatch menu 1–6 |
| `quickCalculate()` | private | Anonymous BMI; no save |
| `saveRecord()` | private | Full profile + `create` |
| `viewRecords()` | private | `read_all` + list |
| `searchRecord()` | private | Two-pass: count matches, then display with position indicator |
| `deleteRecord()` | private | List + confirm + `delete_by_id` |

### `User`

| Function | Access | Summary |
|----------|--------|---------|
| `User()` | public | Default constructor |
| `User(id_, name_, …)` | public | Full constructor (trailing `_` params) |
| `get_id` … `get_risk` | public | Ten getters (`const`) |
| `get_text_color()` | public | Returns ANSI color macro string derived from `category`; no stored field |
| `set_name` … `set_weight` | public | Set by `App` |
| `set_bmi` … `set_risk` | public | Set by `BMIService` |
| `set_id` | public | Set by `FileManager` |
| `to_psv()` | public | One pipe-delimited line; `text_color` excluded |
| `from_psv(line)` | public static | Parse line → `User` (exception-safe) |

### `BMIService` (all static)

| Function | Summary |
|----------|---------|
| `calculateBMI(weightKg, heightMeters)` | BMI formula; guard zero |
| `classifyBMI(bmi)` | Returns `BMIResult` using named threshold constants |
| `convertMass(mass, isPoundToKg)` | lb ↔ kg |
| `convertHeightToCm(feet)` | feet → cm |
| `applyToUser(user)` | Full pipeline into `User` |

### `FileManager`

| Function | Access | Summary |
|----------|--------|---------|
| `FileManager(folder)` | public | Load PSV file on construct; no destructor needed |
| `init_database()` | public | Create folder/file + header |
| `getRecordCount()` | public | `records.size()` |
| `create(const User &)` | public | `make_unique` copy + ID + write |
| `read_all()` | public | Non-owning `const User *` vector |
| `delete_by_id(id)` | public | Remove + rewrite PSV file |
| `read_from_file()` | private | `ifstream` load |
| `write_to_file()` | private | `ofstream` full rewrite |
| `get_next_id()` | private | max ID + 1 |

### `UI`

| Function | Access | Summary |
|----------|--------|---------|
| `displayHeader` | public | Centered titled border in `BOLD + MAGENTA` |
| `displayMenu` | public | Main menu + count; options in `CYAN` |
| `printLine` | public | Border line |
| `pauseScreen` | public | Wait for Enter; prompt in `YELLOW` |
| `menuChoice` | public | Validated 1–6 |
| `displayBMIResult(user, current, total)` | public | Full result card; optional position indicator when `total > 0` |
| `displayRecordList` | public | All records compact |
| `promptLine` | public | Non-empty string; error in `RED` |
| `promptGender` | public | Gender submenu → string |
| `promptAge` | public | Age 2–120 |
| `collectHeightWeight` | public | Height + weight metric |
| `confirm` | public | y/n; invalid input in `RED` |
| `nameMatches` | public | Case-insensitive substring |
| `displayRecordLine` | private | One list row; index in `CYAN`, category colored by severity |
| `collectHeight` | private | Unit + value → cm via `BMIService::convertHeightToCm` |
| `collectWeight` | private | Unit + value → kg |

### `getInput` (template)

| Function | Summary |
|----------|---------|
| `getInput(prompt, out, min, max)` | Loop until valid numeric in range; error in `RED` |

---

## 14. Concept Summary

**Classes:** `User` holds data, `BMIService` calculates, `FileManager` handles files, `UI` handles the screen, and `App` runs the menu.

**Struct:** `BMIResult` groups three strings from `classifyBMI()` before they are copied into `User`. Display color is not part of `BMIResult` — it is derived from `User::get_text_color()` at display time.

**Smart pointers:** Saved records are `unique_ptr<User>` in `FileManager::records`. `make_unique` allocates on the heap; automatic destruction on erase or `FileManager` teardown eliminates manual `delete`. `read_all()` exposes non-owning `const User *` observer pointers for safe read access.

**File handling:** Load at startup (`read_from_file`), rewrite on create/delete (`write_to_file`). `to_psv` / `from_psv` bridge objects and disk text using `|` as delimiter to safely handle commas in data fields. `text_color` is excluded from the PSV file — always re-derived from `category`.

**Colors:** All ANSI codes are defined in `headers/colors.h`. `User::get_text_color()` maps WHO category strings to color macros. `UI` applies colors to headers, prompts, errors, and result cards. No color is ever stored in memory or on disk.

**UI:** All console flows are documented in **[UI_FLOW.md](UI_FLOW.md)** with screen layouts and step-by-step branches.