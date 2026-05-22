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
| **Pointers** (`*`) | `User *`, `vector<User *>`, `new` / `delete`, `nullptr` checks |
| **References** (`&`) | `applyToUser(User &)`, `getInput(T &out)`, output parameters in `UI` |
| **File handling** | `ifstream`, `ofstream`, `records.csv`, Create / Read / Delete |
| **Templates** | `getInput()` — one validator for `int` and `double` |

### Table of contents

| § | Topic |
|---|--------|
| [1](#1-classes-vs-struct-in-this-project) | Classes vs struct (`BMIResult`) |
| [2](#2-pointers--how-this-project-uses-them) | Pointers and references |
| [3](#3-file-handling--how-this-project-uses-files) | File I/O and CSV |
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

### Classes used for OOP

| Class | Role |
|-------|------|
| `User` | Data model with private fields and getters/setters |
| `BMIService` | Business logic (static methods) |
| `FileManager` | File I/O and in-memory record list |
| `UI` | Console interface |
| `App` | Main controller; owns `FileManager` and `UI` |

---

## 2. Pointers — How This Project Uses Them

A **pointer** stores the **address** of a variable or object (`User *ptr`). The project uses pointers to:

1. Store many records dynamically on the **heap**
2. Pass records to functions without copying large objects
3. Share the same record between `FileManager` and `App`/`UI`
4. Check for invalid memory with **`nullptr`**

### Pointer examples in the codebase

| Code | Type | Purpose |
|------|------|---------|
| `std::vector<User *> records` | Container of pointers | `FileManager` holds all saved records |
| `new User(...)` | Heap allocation | Create record when loading CSV or saving |
| `delete user` | Free memory | Destructor and `delete_by_id()` avoid leaks |
| `User *user` in `create()` | Pointer parameter | Pass record to save without copy |
| `const User *record` | Const pointer | Read-only access in search/view loops |
| `User *target` | Pointer | Selected record for deletion |
| `record->get_name()` | Arrow operator | Access member through pointer |
| `*record` | Dereference | Pass object to `displayBMIResult(const User &)` |
| `if (user == nullptr)` | Safety check | Skip invalid pointers |

### Text flow: pointer lifecycle of one saved record

```
App::saveRecord()
    User user;                    // stack object (temporary)
    file_manager.create(&user);   // passes ADDRESS of user

FileManager::create(User *user)
    user->set_id(...);            // arrow: modify through pointer
    records.push_back(new User(*user));  // NEW copy on heap
    write_to_file();

Later: read_all() returns vector<User *>
    App loops: for (const User *record : records)
        ui.displayBMIResult(*record);   // dereference for display

On delete:
    delete records[i];            // free heap memory
    records.erase(...);
```

### Reference vs pointer (also used)

| Syntax | Example | Meaning |
|--------|---------|---------|
| `User &user` | `applyToUser(User &user)` | Alias; must refer to valid object; not null |
| `double &heightCm` | `collectHeightWeight(double &heightCm, ...)` | Output parameter; function updates caller’s variable |
| `const User &user` | `displayBMIResult(const User &user)` | Read-only reference; no copy |

**Course note:** References (`&`) are often used when the object always exists. Pointers (`*`) are used when ownership is shared, objects are on the heap, or `nullptr` is possible.

---

## 3. File Handling — How This Project Uses Files

All persistent data is stored in **`database/records.csv`**. The `FileManager` class handles every file operation.

### Libraries and types

| Include / type | Purpose |
|----------------|---------|
| `<fstream>` | `std::ifstream` (read), `std::ofstream` (write) |
| `<filesystem>` | `std::filesystem::exists`, `create_directories` |
| `std::ifstream file(db_file_path)` | Open CSV for reading |
| `std::ofstream file(db_file_path)` | Open CSV for writing (overwrites file) |
| `std::getline(file, line)` | Read one line at a time |
| `file << data` | Write text to file |
| `file.is_open()` | Check if open succeeded |

### File operations map

| Function | File mode | What it does |
|----------|-----------|--------------|
| `init_database()` | Create folder/file | Creates `database/` and empty CSV with header if missing |
| `read_from_file()` | Read (`ifstream`) | Loads all records into `vector<User *>` on startup |
| `write_to_file()` | Write (`ofstream`) | Rewrites entire CSV from memory |
| `create()` | Calls `write_to_file()` | After adding record |
| `delete_by_id()` | Calls `write_to_file()` | After removing record |

### Text flow: reading a record from CSV

```
1. Open records.csv with ifstream
2. Read first line → skip (header)
3. For each data line:
       User::from_csv(line)  → builds User on stack
       new User(...)         → copy to heap
       records.push_back(pointer)
4. Close file
```

### Text flow: saving after create/delete

```
1. Open records.csv with ofstream (truncates file)
2. Write header row
3. For each User* in records:
       file << user->to_csv() << "\n"
4. Close file
```

**Course note:** The program keeps data **in memory** (`vector<User *>`) while running and **syncs to disk** on every create or delete.

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

1. **Construction** — `App` constructor runs `FileManager("database")` (loads CSV) and `init_database()` (creates folder/file if needed).
2. **Execution** — `run()` owns the entire interactive session until the user selects Exit.
3. **Termination** — Returns `0` to the OS; `App` destructor destroys `file_manager` (frees all `User*` records) and `ui`.

`main` includes only `app.h` — the smallest possible entry point with no direct dependency on `User`, `BMIService`, or `UI` headers.

---

## 5. Class `App` — Application Controller

**Files:** `headers/app.h`, `src/app.cpp`

**Purpose:** `App` is the **central controller** of the program. It does not format the screen or parse CSV files itself. Instead, it owns `FileManager` and `UI`, runs the infinite menu loop in `run()`, and routes each menu choice to the correct workflow method.

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
- **Steps:** Initializes `file_manager` with the folder path (default `"database"` if omitted in header default). Calls `file_manager.init_database()` so the folder and CSV header exist before the first menu display.
- **Does not:** Start the menu loop (that is `run()`’s job).

#### `void run()`

- **When called:** Immediately after construction in `main()`.
- **Steps:**
  1. Declares `menu_choice` and the exit value (6).
  2. Loop: `displayMenu(getRecordCount())` → `menuChoice(menu_choice)` → `handleMenuChoice(static_cast<MenuOption>(menu_choice))`.
  3. Repeats until `menu_choice == EXIT`.
- **Effect:** This is the **main program loop**; all features are reached through this cycle.

### Private methods — detailed

#### `void handleMenuChoice(UI::MenuOption choice)`

- **Input:** Enum value from the user’s menu selection (1–6).
- **Behavior:** `switch` dispatches to `quickCalculate`, `saveRecord`, `viewRecords`, `searchRecord`, or `deleteRecord`. Each branch (except Exit) ends with `ui.pauseScreen()`. Exit prints a goodbye header via `displayHeader("Goodbye!")` and returns without pausing.

#### `void quickCalculate()`

1. `ui.collectHeightWeight` fills `heightCm` and `weightKg` (metric internally).
2. Stack `User user` is created; name, gender, and age set to anonymous placeholders.
3. Height and weight copied into `user`.
4. `BMIService::applyToUser(user)` computes BMI and classification.
5. `ui.displayBMIResult(user)` shows the card.
6. **No call to `file_manager.create`** — data is discarded when the function ends.

#### `void saveRecord()`

1. If `getRecordCount() >= UI::MAX_RECORDS`, prints limit message and returns.
2. Prompts name (`promptLine`), gender (`promptGender`), age (`promptAge`), height/weight (`collectHeightWeight`).
3. `BMIService::applyToUser(user)` fills BMI fields.
4. Shows result, then `file_manager.create(&user)` — passes **address** of stack `user`; `FileManager` copies to heap and assigns ID.

#### `void viewRecords()`

1. `read_all()` gets `vector<User *>`.
2. If empty → message. Else → `displayRecordList(records)`.

#### `void searchRecord()`

1. Loads all records; exits early if none.
2. Reads search query via `promptLine`.
3. For each `const User *record`, skips `nullptr`, calls `nameMatches(record->get_name(), query)`.
4. On match, prints `displayBMIResult(*record)` (dereference pointer to reference).
5. If no match after loop, prints not-found message.

#### `void deleteRecord()`

1. Lists all records with header and `displayRecordList`.
2. `getInput` asks for list number from 1 to `records.size()`.
3. Resolves `User *target` using `selection - LIST_DISPLAY_OFFSET` as vector index.
4. `ui.confirm` with personalized prompt including target’s name.
5. On yes, `delete_by_id(target->get_id())` — uses **database ID**, not list index.

### `App` workflow summary table

| Method | `UI` calls | `BMIService` | `FileManager` | Persists? |
|--------|------------|--------------|---------------|-----------|
| `quickCalculate` | `collectHeightWeight`, `displayBMIResult` | `applyToUser` | — | No |
| `saveRecord` | profile + height/weight + result | `applyToUser` | `create` | Yes |
| `viewRecords` | `displayRecordList` | — | `read_all` | — |
| `searchRecord` | `promptLine`, `nameMatches`, `displayBMIResult` | — | `read_all` | — |
| `deleteRecord` | header, list, `confirm` | — | `read_all`, `delete_by_id` | Yes (remove) |

Console flow for each row: **[UI_FLOW.md](UI_FLOW.md)** §5.

---

## 6. Class `User` — Record Data Model

**Files:** `headers/user.h`, `src/user.cpp`

**Purpose:** `User` is the **entity** that travels through the system: created in `App`, enriched by `BMIService`, displayed by `UI`, and stored by `FileManager`. It demonstrates **encapsulation** — ten private fields with a controlled public interface.

### Private data members

| Member | Type | Description |
|--------|------|-------------|
| `id` | `int` | Unique record ID |
| `name` | `string` | Person’s name |
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

#### `User(int id, const std::string &name, ...)`

Member initializer list sets all ten fields in one step. Used by:

- `User::from_csv` when loading from disk
- `FileManager::create` when doing `new User(*user)` (heap copy)

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

All getters are `const` — safe to call on read-only references like `displayBMIResult(const User &)`.

### Setters — detailed

| Method | Effect |
|--------|--------|
| `set_id(int)` | Written by `FileManager::create` after `get_next_id()` |
| `set_name`, `set_gender`, `set_age` | Filled by `App` / `UI` during save or quick BMI |
| `set_height`, `set_weight` | Metric values after UI conversion |
| `set_bmi`, `set_category`, `set_advice`, `set_risk` | Filled by `BMIService::applyToUser` |

### File-related methods — detailed

#### `std::string to_csv() const`

- Uses `std::ostringstream` to join fields with commas in fixed order.
- No trailing comma; one line = one record.
- **Assumption:** Name and text fields do not contain commas (documented limitation).

#### `static User from_csv(const std::string &csvLine)`

- Uses `std::istringstream` and `getline(..., ',')` to split tokens.
- Parses `id` and `age` with `stoi`; `height`, `weight`, `bmi` with `stod`.
- Reads `category`, `advice`, `risk` as remaining string tokens.
- Returns a **value** `User` on the stack; caller (`FileManager`) copies to heap with `new`.

**Pointer link:** `FileManager` stores `User*` on heap; CSV methods serialize/deserialize that data.

---

## 7. Class `BMIService` — BMI Business Logic

**Files:** `headers/bmi_service.h`, `src/bmi_service.cpp`

**Purpose:** Isolates **all health math and rules** from UI and file code. **No `BMIService` instances** exist — only `static` methods, so the class cannot hold per-user state.

### Constants

| Constant | Value | Used for |
|----------|-------|----------|
| `POUND_TO_KILOGRAM` | 0.453592 | Pounds → kg |
| `FEET_TO_METER` | 0.3048 | Feet → meters (then × 100 for cm) |
| `CM_TO_METERS` | 100.0 | cm → m for BMI formula |

### Methods — detailed

#### `static double calculateBMI(double weightKg, double heightMeters)`

- **Formula:** `weight / (height × height)`.
- **Guard:** If `height <= 0` or `weight <= 0`, returns `0.0` instead of dividing by zero.
- **Called by:** `applyToUser` after converting cm to meters.

#### `static BMIResult classifyBMI(double bmi)`

- **Returns:** Local `BMIResult` struct with three strings filled by a chain of `if / else if`:
  - `< 18.5` → Underweight
  - `< 25` → Normal weight
  - `< 30` → Overweight
  - `< 35` → Obese Class I
  - `< 40` → Obese Class II
  - else → Obese Class III
- Each branch sets **different** `advice` and `risk` strings (see [BMI_CLASSIFICATION.md](BMI_CLASSIFICATION.md)).
- **Does not** modify any `User` — only returns data.

#### `static double convertMass(double mass, bool isPoundToKg = true)`

- **Default `true`:** `mass * POUND_TO_KILOGRAM` (pounds → kg) — used by `UI::collectWeight`.
- **`false`:** `mass / POUND_TO_KILOGRAM` (kg → pounds) — available for reverse conversion if needed.

#### `static void applyToUser(User &user)`

Orchestrates the full calculation pipeline:

1. `heightMeters = user.get_height() / CM_TO_METERS`
2. `bmi = calculateBMI(user.get_weight(), heightMeters)`
3. `result = classifyBMI(bmi)`
4. `user.set_bmi`, `set_category`, `set_advice`, `set_risk`

**Single call site pattern:** Both `quickCalculate` and `saveRecord` in `App` use only this method for BMI logic — avoids duplicating formulas in `App` or `UI`.

**Struct highlight:** `classifyBMI()` builds a `BMIResult` struct, then `applyToUser()` copies strings into `User`.

---

## 8. Class `FileManager` — File Handling & Record Storage

**Files:** `headers/file_manager.h`, `src/file_manager.cpp`

**Purpose:** Acts as a **local data store**: load CSV at startup, keep records in RAM as `User*` pointers, sync disk on every create or delete. Public API maps to **Create**, **Read**, and **Delete** (no Update).

| Pattern | Public API | Private helper |
|---------|------------|----------------|
| Create | `create()` | `get_next_id()`, `write_to_file()` |
| Read | `read_all()` | `read_from_file()` (startup) |
| Delete | `delete_by_id()` | `write_to_file()` |

### Private members

| Member | Type | Purpose |
|--------|------|---------|
| `db_folder` | `string` | Folder path (e.g. `database`) |
| `db_file_path` | `string` | Full path to `records.csv` |
| **`records`** | **`vector<User *>`** | **Pointer container** — all loaded records |

### Public methods — detailed

#### `FileManager(const std::string &folder)`

- Sets `db_folder` and `db_file_path = folder + "/records.csv"`.
- Calls `read_from_file()` immediately so previous session data is available.

#### `~FileManager()`

- For each pointer in `records`: `delete user`, then `records.clear()`.
- Runs when `App` is destroyed at program exit.

#### `void init_database()`

- `fs::exists` / `create_directories` for folder.
- If CSV missing: `ofstream` writes header row only.
- If CSV exists: prints ready message (does not reload — load happened in constructor).

#### `int getRecordCount() const`

- Returns `static_cast<int>(records.size())` for menu and save limit.

#### `void create(User *user)`

1. Rejects `nullptr` with error message.
2. `user->set_id(get_next_id())` — updates caller’s stack object ID too.
3. `records.push_back(new User(*user))` — **heap copy** so data survives after `App`’s stack `User` is destroyed.
4. `write_to_file()` — full file rewrite.
5. Prints `Record saved! (ID: n)`.

#### `std::vector<User *> read_all() const`

- Returns the `records` vector by value (copies pointer values, not `User` objects).
- **Ownership:** `FileManager` still owns and will `delete` pointers later.

#### `bool delete_by_id(int id)`

1. Linear search for matching `get_id()`.
2. `delete records[i]`, `erase` from vector.
3. `write_to_file()`.
4. Returns `true` on success; `false` + stderr message if ID not found.

### Private methods — detailed

#### `void read_from_file()`

- No-op if file path does not exist yet (first run).
- Opens `ifstream`; on failure prints error and returns.
- Skips first line (header); for each data line: `User::from_csv` → `new User(...)` → `push_back`.
- Does **not** clear `records` first — constructor runs once at startup on empty vector.

#### `void write_to_file()`

- Opens `ofstream` (truncates existing file).
- Writes header, then each `user->to_csv()` on its own line.
- Entire database rewritten each time — simple consistency model.

#### `int get_next_id()`

- Scans all non-null records for maximum `id`, returns `maxId + 1`.
- Ensures monotonic IDs even after deletions (gaps in IDs are allowed).

---

## 9. Class `UI` — Console Interface

**Files:** `headers/ui.h`, `src/ui.cpp`

**Purpose:** **Presentation layer** — all console output and user prompts. `UI` does not save files or compute BMI; it collects and displays data and delegates unit conversion to `BMIService` and numeric checks to `getInput()`.

> **Screen-by-screen flows** (main menu, each option, height/weight submenus): see **[UI_FLOW.md](UI_FLOW.md)**.

### Public constants

| Constant | Value | Meaning |
|----------|-------|---------|
| `MAX_RECORDS` | 500 | Checked in `App::saveRecord` before save |
| `LIST_DISPLAY_OFFSET` | 1 | List labels start at `[1]`, not `[0]` |

### Enums

| Enum | Values | Purpose |
|------|--------|---------|
| `MenuOption` | 1–6 | `QUICK_BMI`, `SAVE_RECORD`, `VIEW_RECORDS`, `SEARCH`, `DELETE`, `EXIT` — `App::handleMenuChoice` casts `int` to this |
| `GenderChoice` (private) | 1–3 | Internal; mapped to display strings in `promptGender` |
| `HeightUnit` (private) | 1–2 | Centimeters or feet input path |
| `WeightUnit` (private) | 1–2 | Kilograms or pounds input path |

### Private layout and validation constants

| Constant | Value | Used in |
|----------|-------|---------|
| `LINE_WIDTH` | 60 | `displayHeader`, `printLine` |
| `DECIMAL_PRECISION` | 2 | BMI and list formatting |
| `MENU_OPTION_MIN` | 1 | Menu lower bound |
| `MIN_USER_AGE` / `MAX_USER_AGE` | 2 – 120 | `promptAge` |
| `GENDER_OPTION_MIN` / `MAX` | 1 – 3 | `promptGender` |
| `UNIT_OPTION_MIN` / `MAX` | 1 – 2 | Height/weight unit menus |
| `MIN_HEIGHT_CM` / `MAX_HEIGHT_CM` | 50 – 272 | Direct cm entry |
| `MIN_HEIGHT_FEET` / `MAX_HEIGHT_FEET` | 1.6 – 9 | Feet entry → converted to cm |
| `MIN_WEIGHT_KG` / `MAX_WEIGHT_KG` | 2 – 500 | Direct kg entry |
| `MIN_WEIGHT_LB` / `MAX_WEIGHT_LB` | 4 – 1102 | Pounds → kg |

### Public methods — full reference

#### Display and navigation

| Method | Signature | Detailed explanation |
|--------|-----------|----------------------|
| `displayHeader` | `void (const string &header) const` | Prints `printLine('=')`, centers `header` in 60 characters (padding 0 if title too long), prints closing `=`. Used for main title, BMI result, delete list, goodbye. |
| `displayMenu` | `void (int currentRecordCount) const` | Calls `displayHeader("BMI CALCULATOR SYSTEM")`, prints `Total Records: N / 500`, then each string in private `menu` vector as `[1]`…`[6]`. Does not read input. |
| `printLine` | `void (char ch = '=') const` | One line of `ch` repeated `LINE_WIDTH` times. Visual separator between sections. |
| `pauseScreen` | `void () const` | Prints `-` line and `Press Enter to continue...`; `cin.clear()` + `ignore` to newline. Called by `App` after every feature except Exit. |
| `menuChoice` | `void (int &choice) const` | Builds prompt `Select an option (1-N):`, calls `getInput` with `MENU_OPTION_MIN` and `menu.size()`. Writes valid choice into reference `choice`. |

#### Results and lists

| Method | Signature | Detailed explanation |
|--------|-----------|----------------------|
| `displayBMIResult` | `void (const User &user) const` | Full **BMI RESULT** card: profile (name, gender, age, height cm, weight kg), dashed line, then BMI, category, advice, risk with `fixed` + 2 decimals. Takes **reference** — caller passes stack `User` or `*record` from pointer. |
| `displayRecordList` | `void (const vector<User *> &records) const` | Opens with `-` line; for each index `i`, if `records[i] != nullptr`, calls `displayRecordLine(i + LIST_DISPLAY_OFFSET, *records[i])` and another `-` line. Skips null pointers safely. |

#### Text and profile input

| Method | Signature | Detailed explanation |
|--------|-----------|----------------------|
| `promptLine` | `string (const string &prompt) const` | Prints `prompt`, `getline` into string. Loops until non-empty; prints `Input cannot be empty...` on failure. Used for name and search query. |
| `promptGender` | `string () const` | Shows gender submenu with `-` borders; `getInput` 1–3; `switch` returns `"Male"`, `"Female"`, or `"Prefer not to say"`. |
| `promptAge` | `int () const` | `getInput` for age between `MIN_USER_AGE` and `MAX_USER_AGE`; returns validated integer. |
| `collectHeightWeight` | `void (double &heightCm, double &weightKg) const` | Orchestrator: `collectHeight` then `collectWeight`. Both outputs always **metric** for `BMIService`. |
| `confirm` | `bool (const string &prompt) const` | `getline` loop; first character lowercased: `y` → `true`, `n` → `false`; empty or other → re-prompt. Used for delete confirmation. |
| `nameMatches` | `bool (const string &name, const string &query) const` | If `query` empty → `true` (match all). Else copies both strings, lowercases with `transform`, returns whether `lowerName.find(lowerQuery) != npos`. |

### Private methods — full reference

| Method | Signature | Detailed explanation |
|--------|-----------|----------------------|
| `displayRecordLine` | `void (int listIndex, const User &user) const` | Single compact row: `[listIndex] ID: id \| name \| gender \| Age: n \| BMI: x.xx \| category`. List index is **display only**; delete uses separate `getInput` then maps to vector index. |
| `collectHeight` | `void (double &heightCm) const` | Unit submenu → if cm: `getInput` into `heightCm`; if feet: `getInput` into local `feet`, assign `heightCm = feet * FEET_TO_METER * CM_TO_METERS`. |
| `collectWeight` | `void (double &weightKg) const` | Unit submenu → if kg: direct `getInput`; if pounds: `getInput` then `weightKg = convertMass(pounds, true)`. Ends with `printLine('-')`. |

### `UI` method map by feature

| Menu | `UI` methods called (in order) |
|------|--------------------------------|
| Main loop | `displayMenu`, `menuChoice`, `pauseScreen` (or `displayHeader` on exit) |
| [1] Quick BMI | `collectHeightWeight`, `displayBMIResult` |
| [2] Save | `promptLine`, `printLine`, `promptGender`, `promptAge`, `printLine`, `collectHeightWeight`, `displayBMIResult` |
| [3] View | `displayRecordList` |
| [4] Search | `promptLine`, `displayBMIResult` (per match), `nameMatches` (called from `App`) |
| [5] Delete | `displayHeader`, `displayRecordList`, `printLine`, `confirm` |

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

1. **Prompt** — prints `prompt` without newline requirement (caller’s prompt usually includes formatting).
2. **Read as `double`** — single `cin >> raw` attempt; allows one parsing path for int and double.
3. **Integer check** — if `T` is integral, rejects when `floor(raw) != raw` (no `3.5` for menu choice).
4. **Range check** — compares `raw` to `min`/`max` as `double` before `static_cast<T>`.
5. **Success** — assigns `out`, ignores remainder of line, `return`.
6. **Failure** — `cin.clear()`, ignore line, print range error, loop forever until valid.

### Why a template?

One function handles menu integers, age integers, and height/weight doubles without duplicating validation loops.

### Call sites

| Caller | Typical `T` | Example range |
|--------|---------------|---------------|
| `UI::menuChoice` | `int` | 1 – 6 |
| `UI::promptAge` | `int` | 2 – 120 |
| `UI::collectHeight` | `double` | 50 – 272 cm or 1.6 – 9 ft |
| `UI::collectWeight` | `double` | kg or lb bounds |
| `App::deleteRecord` | `int` | 1 – record count |

---

## 11. Quick Reference — All Source Files

| File | Contains |
|------|----------|
| `main.cpp` | `main()` |
| `app.cpp` | `App` methods |
| `user.cpp` | `User` methods, CSV parse/format |
| `bmi_service.cpp` | BMI math and classification |
| `file_manager.cpp` | **File I/O**, pointers, `new`/`delete` |
| `ui.cpp` | Console UI |

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

**Data flow summary**

```
Input (keyboard) → UI / getInput → App → BMIService → User fields
                              └──────► FileManager → records.csv
```

---

## 13. Complete Function Index

Alphabetical within each component. **Static** = no instance required.

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
| `searchRecord()` | private | Filter by name + result cards |
| `deleteRecord()` | private | List + confirm + `delete_by_id` |

### `User`

| Function | Access | Summary |
|----------|--------|---------|
| `User()` | public | Default constructor |
| `User(id, name, …)` | public | Full constructor |
| `get_id` … `get_risk` | public | Ten getters (`const`) |
| `set_id` … `set_risk` | public | Ten setters |
| `to_csv()` | public | One CSV line |
| `from_csv(line)` | public static | Parse line → `User` |

### `BMIService` (all static)

| Function | Summary |
|----------|---------|
| `calculateBMI(weightKg, heightMeters)` | BMI formula; guard zero |
| `classifyBMI(bmi)` | Returns `BMIResult` |
| `convertMass(mass, isPoundToKg)` | lb ↔ kg |
| `applyToUser(user)` | Full pipeline into `User` |

### `FileManager`

| Function | Access | Summary |
|----------|--------|---------|
| `FileManager(folder)` | public | Load CSV on construct |
| `~FileManager()` | public | `delete` all `User*` |
| `init_database()` | public | Create folder/file + header |
| `getRecordCount()` | public | `records.size()` |
| `create(user*)` | public | ID + heap copy + write |
| `read_all()` | public | Copy of pointer vector |
| `delete_by_id(id)` | public | Remove + rewrite CSV |
| `read_from_file()` | private | `ifstream` load |
| `write_to_file()` | private | `ofstream` full rewrite |
| `get_next_id()` | private | max ID + 1 |

### `UI`

| Function | Access | Summary |
|----------|--------|---------|
| `displayHeader` | public | Centered titled border |
| `displayMenu` | public | Main menu + count |
| `printLine` | public | Border line |
| `pauseScreen` | public | Wait for Enter |
| `menuChoice` | public | Validated 1–6 |
| `displayBMIResult` | public | Full result card |
| `displayRecordList` | public | All records compact |
| `promptLine` | public | Non-empty string |
| `promptGender` | public | Gender submenu → string |
| `promptAge` | public | Age 2–120 |
| `collectHeightWeight` | public | Height + weight metric |
| `confirm` | public | y/n |
| `nameMatches` | public | Case-insensitive substring |
| `displayRecordLine` | private | One list row |
| `collectHeight` | private | Unit + value → cm |
| `collectWeight` | private | Unit + value → kg |

### `getInput` (template)

| Function | Summary |
|----------|---------|
| `getInput(prompt, out, min, max)` | Loop until valid numeric in range |

---

## 14. Concept Summary

**Classes:** `User` holds data, `BMIService` calculates, `FileManager` handles files, `UI` handles the screen, and `App` runs the menu.

**Struct:** `BMIResult` groups three strings from `classifyBMI()` before they are copied into `User`.

**Pointers:** Saved records are `User*` in `FileManager::records`. `new` / `delete` manage heap memory; `read_all()` shares pointers with `App` for view/search/delete.

**File handling:** Load at startup (`read_from_file`), rewrite on create/delete (`write_to_file`). `to_csv` / `from_csv` bridge objects and disk text.

**UI:** All console flows are documented in **[UI_FLOW.md](UI_FLOW.md)** with screen layouts and step-by-step branches.
