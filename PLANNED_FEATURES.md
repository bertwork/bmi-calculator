# Planned Features

This file tracks all planned, in-progress, and completed improvements to the BMI Calculator System. Before implementing any feature, add a plan entry here first. Update `todo.md` when done.

---

## How to use this file

1. Add a new entry under **Planned** before writing any code.
2. Move it to **In Progress** when you start.
3. Move it to **Completed** when merged and all related docs are updated.

---

## Completed

---

### ANSI Color Scheme

**Goal:** Add color to console output to improve readability and communicate BMI severity visually.

**Files changed:**

| File | Change |
|------|--------|
| `headers/colors.h` | New file — defines all ANSI color macros |
| `headers/user.h` | Added `get_text_color()` derived from category |
| `src/user.cpp` | Implemented `get_text_color()` |
| `src/ui.cpp` | Applied colors to error messages, prompts, headers, BMI result, and record list |
| `src/bmi_service.cpp` | Removed `text_color` from `BMIResult` (now derived in `User`) |

**New functions:**

- `User::get_text_color() const` — derives ANSI color string from `category`; no setter needed

**Color scheme:**

| Color | Used for |
|-------|----------|
| `LBLUE` | Section headers |
| `CYAN` | Menu options, BMI value, record list index |
| `YELLOW` | Input prompts (`Press Enter to continue...`) |
| `GREEN` | Normal weight category |
| `LYELLOW` | Underweight / Overweight categories |
| `RED` | Obese Class I, invalid input errors |
| `BOLD + RED` | Obese Class II |
| `BOLD + LRED` | Obese Class III |
| `RESET` | After every colored segment |

**Notes:**
- ANSI codes work on Linux, macOS, and Windows 10+ terminals.
- `text_color` is never stored in CSV — always derived from `category` at display time.

**Docs to update:** `CLASS_REFERENCE.md` ✓

---

### Search Match Count and Result Indicator

**Goal:** Show how many records matched a search query, and label each result card with its position (e.g. `BMI RESULT (1 of 3)`).

**Files changed:**

| File | Change |
|------|--------|
| `src/app.cpp` | `searchRecord()` — two-pass loop: count first, then display |
| `headers/ui.h` | `displayBMIResult` updated with optional `current` and `total` params |
| `src/ui.cpp` | `displayBMIResult` — header shows `BMI RESULT (N of Total)` when `total > 0` |

**New behavior:**

- Count line printed before results: `N record(s) found for "query".`
- Each BMI card header shows `BMI RESULT (N of Total)` in search context.
- Calls from `quickCalculate` and `saveRecord` are unaffected (default params show `BMI RESULT`).

**Docs to update:** `UI_FLOW.md` ✓

---

## In Progress

---

### BMI History Summary

**Goal:** Show a summary block at the bottom of view all records with key statistics across all saved records.

**Files changed:**

| File | Change |
|------|--------|
| `headers/ui.h` | Add `displayBMISummary(const std::vector<const User *> &records) const` |
| `src/ui.cpp` | Implement `displayBMISummary` |
| `src/app.cpp` | `viewRecords()` calls `displayBMISummary(records)` after the list |

**New functions:**

- `UI::displayBMISummary(records)` — computes and displays summary statistics with colors

**Sample output:**
```
------------------------------------------------------------
  SUMMARY
------------------------------------------------------------
  Total Records : 6
  Average BMI   : 22.41
  Lowest BMI    : 11.81 (mandy)     - Underweight
  Highest BMI   : 31.60 (bert)      - Obese Class I
  Most Common   : Normal weight (3 records)
------------------------------------------------------------
```

**Color rules:**
- `Average BMI` — `CYAN`
- `Lowest BMI` value and category — colored by that user's `get_text_color()`
- `Highest BMI` value and category — colored by that user's `get_text_color()`
- `Most Common` category — colored by that category's color

**Notes:**
- Only shown when at least one record exists.
- Appears after the record list, before `pauseScreen`.

**Docs to update:** `UI_FLOW.md`, `CLASS_REFERENCE.md`

---

### Automatic Backup

**Goal:** Silently create a timestamped backup of the database after every save, keeping only the last 3 backups.

**Files changed:**

| File | Change |
|------|--------|
| `headers/file_manager.h` | Add `backup()` private method |
| `src/file_manager.cpp` | Implement `backup()` — create `backup/` folder, write timestamped `.psv`, delete oldest if more than 3 exist; call from `create()` |

**New functions:**

- `FileManager::backup()` — private; called automatically at the end of `create()`

**Behavior:**
- Backup folder: `backup/`
- Filename format: `records_YYYY-MM-DD_HH-MM-SS.psv`
- Keeps only the last 3 backups — when a 4th is created, the oldest is deleted
- Runs silently; no output to the user

**Notes:**
- Uses `<filesystem>` for folder creation and file listing.
- Uses `<chrono>` and `<ctime>` for timestamp generation.
- Backup is a full copy of the current `records.psv`.

**Docs to update:** `DOCUMENTATION.md`, `CLASS_REFERENCE.md`

---

## Planned

---

### Sort Records by BMI, Name, or Age

**Goal:** When viewing all records, allow the user to choose a sort order before the list is displayed.

**Files changed (planned):**

| File | Change |
|------|--------|
| `headers/ui.h` | Add `promptSortOption()` |
| `src/ui.cpp` | Implement sort prompt submenu |
| `src/app.cpp` | `viewRecords()` — sort the records vector before passing to `displayRecordList` |

**Notes:**
- Sort is display-only; PSV order is unchanged.
- Default sort is insertion order (current behavior).

---

### Edit/Update Record

**Goal:** Allow the user to select a saved record and update one or more fields. BMI and category are recomputed after the edit.

**Files changed (planned):**

| File | Change |
|------|--------|
| `headers/file_manager.h` | Add `update(const User &user)` |
| `src/file_manager.cpp` | Implement `update` — find by ID, replace in memory, rewrite PSV |
| `src/app.cpp` | Add `editRecord()` feature method |
| `headers/ui.h` | Add edit prompts |
| `src/ui.cpp` | Implement edit field selection submenu |
| `headers/app.h` | Add `editRecord()` declaration |

**Notes:**
- BMI and classification are recalculated via `BMIService::applyToUser()` after edits.
- Adds the missing **Update** to the current Create-Read-Delete pattern.

---

### Filter Records by Category

**Goal:** When viewing records, allow the user to filter by a specific BMI category.

**Files changed (planned):**

| File | Change |
|------|--------|
| `headers/ui.h` | Add `promptCategoryFilter()` |
| `src/ui.cpp` | Implement category selection submenu |
| `src/app.cpp` | `viewRecords()` — apply filter before passing to display |

**Notes:**
- Filter is display-only; no records are deleted.
- Option to show all (no filter) always available.

---

### Duplicate Name Warning

**Goal:** When saving a record, warn the user if a record with the same name already exists. User can still proceed or cancel.

**Files changed (planned):**

| File | Change |
|------|--------|
| `src/file_manager.cpp` | Add `existsByName(const std::string &name) const` |
| `headers/file_manager.h` | Declare `existsByName` |
| `src/app.cpp` | `saveRecord()` — check before saving, prompt user to confirm or cancel |

**Notes:**
- Name comparison is case-insensitive.
- User is always given the choice to proceed — useful for tracking the same person over time.