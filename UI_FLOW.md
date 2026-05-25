# UI Flow — Console Screens & Interaction

**BMI Calculator System** — Sample console output for each feature, aligned with the current source code. For class/method reference see **[CLASS_REFERENCE.md](CLASS_REFERENCE.md)** §9; for architecture see **[DOCUMENTATION.md](DOCUMENTATION.md)** §3 and §7.

**Legend:** Sample values (e.g. `164`, `mandy`) are examples. Prompt text matches `ui.cpp` / `app.cpp` unless noted.

---

## Global loop

After startup, the program repeats until the user picks **[7] Exit**:

```
[Main menu] → user picks 1–7 → App runs feature → Press Enter (except Exit) → [Main menu]
```

| Step | Code |
|------|------|
| Show menu | `App::run()` → `UI::displayMenu(getRecordCount())` |
| Read choice | `UI::menuChoice()` → `getInput(..., 1, 7)` |
| Run feature | `App::handleMenuChoice()` |
| Pause | `UI::pauseScreen()` (skipped on Exit) |

---

## START — Main menu

### Sample screen

```
============================================================
                  BMI CALCULATOR SYSTEM
============================================================
Total Records: 0 / 500

MENU OPTIONS:
[1] Quick BMI Calculation
[2] Save BMI Record
[3] View All Records
[4] Search Record
[5] Delete Record
[6] Edit Record
[7] Exit

Select an option (1-7):
```

### Details

| Item | Value |
|------|--------|
| `App` method | `run()` |
| `UI` methods | `displayHeader`, `displayMenu`, `menuChoice` |
| Record line | `Total Records: N / 500` (`UI::MAX_RECORDS`) |
| Valid input | Integer **1–7** only (decimals rejected) |
| Invalid input | `Invalid input. Enter a value between 1 and 7.` then re-prompt |

| Choice | `App` handler |
|--------|----------------|
| 1 | `quickCalculate()` |
| 2 | `saveRecord()` |
| 3 | `viewRecords()` |
| 4 | `searchRecord()` |
| 5 | `deleteRecord()` |
| 6 | `editRecord()` |
| 7 | Exit → `displayHeader("Goodbye!")` — no pause |

---

## [1] Quick BMI Calculation

### Sample screen (metric units)

```
------------------------------------------------------------
Height unit:
[1] Centimeters
[2] Feet
------------------------------------------------------------
Enter unit option (1-2): 1
------------------------------------------------------------
Enter height in cm (50-272): 164
------------------------------------------------------------
Weight unit:
[1] Kilograms
[2] Pounds
------------------------------------------------------------
Enter unit option (1-2): 1
------------------------------------------------------------
Enter weight in kg (2-500): 54
------------------------------------------------------------

============================================================
                         BMI RESULT
============================================================
  Name     : Anonymous
  Gender   : -
  Age      : 0
  Height   : 164.00 cm
  Weight   : 54.00 kg
------------------------------------------------------------
  BMI      : 20.08
  Category : Normal weight
  Advice   : Maintain current habits with balanced diet and exercise.
  Risk     : Low risk - keep it up!
------------------------------------------------------------
Press Enter to continue...
```

### Details

| Item | Explanation |
|------|-------------|
| `App` | `quickCalculate()` |
| `UI` | `collectHeightWeight` → `displayBMIResult` → (later) `pauseScreen` |
| Profile | Name `Anonymous`, gender `-`, age `0` — not asked on screen |
| Saved? | **No** — no `FileManager::create()` |
| BMI | `BMIService::applyToUser(user)` after height/weight stored in cm/kg |

### Alternate path — feet and pounds

If the user picks **[2] Feet** for height:

```
Enter height in feet (1.6-9): 5.4
```
→ Stored as cm: `feet × 0.3048 × 100`

If the user picks **[2] Pounds** for weight:

```
Enter weight in pounds (4-1102): 119
```
→ Stored as kg: `BMIService::convertMass(pounds, true)`

Result card still shows **cm** and **kg**.

---

## [2] Save BMI Record

### Sample screen

```
Enter name: mandy
------------------------------------------------------------
Select gender:
[1] Male
[2] Female
[3] Prefer not to say
------------------------------------------------------------
Enter gender option (1-3): 2
------------------------------------------------------------
Enter age (2-120): 21
------------------------------------------------------------
Height unit:
[1] Centimeters
[2] Feet
------------------------------------------------------------
Enter unit option (1-2): 1
------------------------------------------------------------
Enter height in cm (50-272): 156
------------------------------------------------------------
Weight unit:
[1] Kilograms
[2] Pounds
------------------------------------------------------------
Enter unit option (1-2): 1
------------------------------------------------------------
Enter weight in kg (2-500): 53
------------------------------------------------------------

============================================================
                         BMI RESULT
============================================================
  Name     : mandy
  Gender   : Female
  Age      : 21
  Height   : 156.00 cm
  Weight   : 53.00 kg
------------------------------------------------------------
  BMI      : 21.78
  Category : Normal weight
  Advice   : Maintain current habits with balanced diet and exercise.
  Risk     : Low risk - keep it up!
Record saved! (ID: 2)
------------------------------------------------------------
Press Enter to continue...
```

### Details

| Step | `UI` / `App` | Validation |
|------|--------------|------------|
| Name | `promptLine("Enter name: ")` | Cannot be empty |
| Gender | `promptGender()` | Options 1–3 → `Male` / `Female` / `Prefer not to say` |
| Age | `promptAge()` | Integer 2–120 |
| Height/weight | `collectHeightWeight` | Same sub-flow as Quick BMI |
| Result | `displayBMIResult` | Shown **before** save |
| Persist | `App` → `file_manager.create(&user)` | Assigns ID, writes `database/records.psv` |

| Item | Explanation |
|------|-------------|
| Gender option 2 | Maps to string `"Female"` (not the number 2 on the card) |
| Save message | Printed by `FileManager::create`: `Record saved! (ID: n)` |
| Saved? | **Yes** — heap copy + PSV file rewrite |

### Edge case — record limit (500)

If `getRecordCount() >= 500` before prompts:

```
Maximum record limit reached (500). Cannot save more records.
```
Then `pauseScreen` still runs; no profile prompts.

### Edge case — empty name

```
Enter name: 
  Input cannot be empty. Please try again.
Enter name: mandy
```

---

## [3] View All Records

### Sample screen (with data)

```
============================================================
                        ALL RECORDS
============================================================
------------------------------------------------------------
  [1] ID: 1 | John Reyes | Male | Age: 22 | BMI: 16.98 | Underweight
------------------------------------------------------------
  [2] ID: 2 | Maria Santos | Female | Age: 28 | BMI: 22.10 | Normal weight
------------------------------------------------------------
  ... (remaining records) ...

------------------------------------------------------------
  SUMMARY
------------------------------------------------------------
  Total Records : 6
  Average BMI   : 29.74
  Lowest BMI    : 16.98 (John Reyes)     - Underweight
  Highest BMI   : 42.46 (Catherine Lopez)      - Obese Class III
  Most Common   : Normal weight (1 record)
------------------------------------------------------------
Press Enter to continue...
```

**Note:** View prints an `ALL RECORDS` header, the record list, then a **SUMMARY** block before pause. Delete and Edit also use `displayHeader("ALL RECORDS")` but do not show the summary.

### Details

| Item | Explanation |
|------|-------------|
| `App` | `viewRecords()` → `read_all()` |
| `UI` | `displayHeader("ALL RECORDS")`, `displayRecordList(records)`, `displayBMISummary(records)` |
| `[n]` | List index (1-based) for display |
| `ID:` | Database ID — used internally on delete, not the same as list position after other deletes |
| Summary | Shown only when records exist; includes total count, average BMI (`CYAN`), lowest/highest BMI with name and category (category colors), and most common category |

### Edge case — no records

```
No records found.
------------------------------------------------------------
Press Enter to continue...
```

---

## [4] Search Record

### Sample screen (matches found)
 
```
Enter name to search: a
 
2 record(s) found for "a".
 
============================================================
                    BMI RESULT (1 of 2)
============================================================
  Name     : mandy
  Gender   : Female
  Age      : 21
  Height   : 156.00 cm
  Weight   : 52.00 kg
------------------------------------------------------------
  BMI      : 21.37
  Category : Normal weight
  Advice   : Maintain current habits with balanced diet and exercise.
  Risk     : Low risk - keep it up!
 
============================================================
                    BMI RESULT (2 of 2)
============================================================
  Name     : allan
  Gender   : Male
  Age      : 23
  Height   : 165.00 cm
  Weight   : 70.00 kg
------------------------------------------------------------
  BMI      : 25.71
  Category : Overweight
  Advice   : Reduce refined sugars and increase physical activity.
  Risk     : Elevated risk of diabetes and heart disease.
 
------------------------------------------------------------
Press Enter to continue...
```
 
### Details
 
| Item | Explanation |
|------|-------------|
| `App` | `searchRecord()` — two passes: count matches first, then display |
| `UI` | `promptLine`, `displayBMIResult(user, current, total)` per match |
| Match rule | Case-insensitive **substring** (`"a"` matches `"mandy"`, `"allan"`) |
| Count line | Printed before results: `N record(s) found for "query".` |
| Result header | Each card shows `BMI RESULT (N of Total)` instead of `BMI RESULT` |
| Multiple hits | One full BMI card per match, blank line between |
| Search query | Cannot be empty (`promptLine`) |
 
### `displayBMIResult` signature
 
The function accepts optional `current` and `total` parameters (defaulting to `0`):
 
```cpp
void displayBMIResult(const User &user, int current = 0, int total = 0) const;
```
 
- When called from `quickCalculate` or `saveRecord`: header shows `BMI RESULT`
- When called from `searchRecord`: header shows `BMI RESULT (N of Total)`
### Edge case — no match
 
```
Enter name to search: xyz
No records matched "xyz".
------------------------------------------------------------
Press Enter to continue...
```

### Edge case — no records in database

```
No records found.
```
(Same as View — before search prompt.)

---

## [5] Delete Record

### Sample screen

```
============================================================
                        ALL RECORDS
============================================================
------------------------------------------------------------
  [1] ID: 1 | mandy | Female | Age: 21 | BMI: 21.37 | Normal weight
------------------------------------------------------------
  [2] ID: 2 | mandy | Female | Age: 21 | BMI: 21.78 | Normal weight
------------------------------------------------------------
Enter record number to delete (1-2): 1
------------------------------------------------------------
Delete record for "mandy"? (y/n): y
Record for "mandy" deleted.
------------------------------------------------------------
Press Enter to continue...
```

### Details

| Step | Handler | Notes |
|------|---------|-------|
| List header | `UI::displayHeader("ALL RECORDS")` | Delete and Edit flows |
| Pick row | `getInput` in `App` | Range **1** to `records.size()` (list number, not ID) |
| Confirm | `UI::confirm` | First character: `y` / `n` (case-insensitive) |
| Remove | `file_manager.delete_by_id(target->get_id())` | Uses **database ID**, not list label |
| File | `write_to_file()` | PSV file rewritten without that row |

### Edge case — user cancels

```
Delete record for "mandy"? (y/n): n
Deletion cancelled.
```

### Edge case — invalid confirm

```
Delete record for "mandy"? (y/n): maybe
  Please enter y or n.
Delete record for "mandy"? (y/n):
```

### Edge case — no records

```
No records found.
```

### List number vs ID

| Display | Meaning |
|---------|---------|
| `[1]` | First row in list (selection **1**) |
| `ID: 1` | Primary key in the PSV file — passed to `delete_by_id(1)` when user picks `[1]` |

If record 1 is deleted, the next save may get `ID: 3` while the list still shows `[1]`, `[2]` for remaining rows.

---

## [6] Edit Record

### Sample screen

```
============================================================
                        ALL RECORDS
============================================================
------------------------------------------------------------
  [1] ID: 1 | mandy | Female | Age: 21 | BMI: 21.37 | Normal weight
------------------------------------------------------------
Enter record number to edit (1-1): 1
------------------------------------------------------------

============================================================
                         BMI RESULT
============================================================
  Name     : mandy
  ...
------------------------------------------------------------
Select field to edit:
[1] Name
[2] Gender
[3] Age
[4] Height
[5] Weight
[6] All fields
[7] Cancel
------------------------------------------------------------
Enter field option (1-7): 5
------------------------------------------------------------
Weight unit:
...
------------------------------------------------------------

============================================================
                         BMI RESULT
============================================================
  ...
------------------------------------------------------------
Save changes to "mandy"? (y/n): y

Record updated! (ID: 1)
------------------------------------------------------------
Press Enter to continue...
```

### Details

| Step | Handler | Notes |
|------|---------|-------|
| List | `read_all()` → `displayRecordList` | Same list selection pattern as Delete |
| Pick row | `getInput` in `App` | Range **1** to `records.size()` (list number, not ID) |
| Current card | `displayBMIResult(edited)` | Shown before field submenu |
| Field choice | `UI::promptEditField()` | Options 1–7; **7** = Cancel (no file change) |
| Recompute | `BMIService::applyToUser(edited)` | After any field change (including height/weight only) |
| Preview | `displayBMIResult(edited)` | Updated BMI card before save confirm |
| Confirm | `UI::confirm` | `Save changes to "name"? (y/n):` |
| Persist | `file_manager.update(edited)` | Same ID kept; `write_to_file()` + `backup()` |

### Edge case — no records

```
No records found.
```

(Same message as View, Search, and Delete.)

### Edge case — cancel at field menu

```
Enter field option (1-7): 7
Edit cancelled.
```

### Edge case — cancel at save confirm

```
Save changes to "mandy"? (y/n): n
Update cancelled.
```

(Disk unchanged — in-memory preview was not written.)

---

## [7] Exit

### Sample screen

```
============================================================
                          Goodbye!
============================================================
```

### Details

| Item | Explanation |
|------|-------------|
| `App` | `handleMenuChoice(EXIT)` → `displayHeader("Goodbye!")` |
| Pause | **Not** called — program returns from `run()` and exits `main` |

---

## Input validation quick reference

| Screen | Prompt pattern | Valid range |
|--------|----------------|-------------|
| Main menu | `Select an option (1-7):` | 1 – 7 |
| Gender | `Enter gender option (1-3):` | 1 – 3 |
| Age | `Enter age (2-120):` | 2 – 120 |
| Height unit | `Enter unit option (1-2):` | 1 – 2 |
| Height cm | `Enter height in cm (50-272):` | 50 – 272 |
| Height feet | `Enter height in feet (1.6-9):` | 1.6 – 9 |
| Weight unit | `Enter unit option (1-2):` | 1 – 2 |
| Weight kg | `Enter weight in kg (2-500):` | 2 – 500 |
| Weight lb | `Enter weight in pounds (4-1102):` | 4 – 1102 |
| Name / search | `Enter name:` / `Enter name to search:` | Non-empty line |
| Delete # | `Enter record number to delete (1-N):` | 1 – N (count of records) |
| Delete confirm | `...? (y/n):` | `y` or `n` (first character) |
| Edit # | `Enter record number to edit (1-N):` | 1 – N (count of records) |
| Edit field | `Enter field option (1-7):` | 1 – 7 (**7** = Cancel) |
| Edit save confirm | `Save changes to "..."? (y/n):` | `y` or `n` (first character) |

Invalid numbers loop with: `Invalid input. Enter a value between min and max.`

---

## App vs UI — who prints what

| Output | Printed by |
|--------|------------|
| Menus, borders, BMI card, lists, BMI summary | `UI` |
| `Record saved! (ID: n)` | `FileManager` |
| `Record updated! (ID: n)` | `FileManager` |
| Automatic PSV backup on save/update | `FileManager` (silent; `database/backup/`) |
| `No records found.`, `No records matched...` | `App` |
| `Maximum record limit reached...` | `App` |
| `Deletion cancelled.`, `Record for "..." deleted.` | `App` |
| `Edit cancelled.`, `Update cancelled.` | `App` |
| `Database ready at ...` (first run) | `FileManager::init_database` |

---

## Related documentation

| Document | Contents |
|----------|----------|
| [CLASS_REFERENCE.md](CLASS_REFERENCE.md) | `UI` methods, `getInput`, full API |
| [DOCUMENTATION.md](DOCUMENTATION.md) | Architecture, menu routing table |
| [BMI_CLASSIFICATION.md](BMI_CLASSIFICATION.md) | Category, advice, risk text on result card |
