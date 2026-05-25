# Improvements

A backlog of feature ideas and enhancements to consider for the BMI Calculator System. Items here are not yet planned or scheduled — move to `PLANNED_FEATURES.md` when ready to implement.

---

## Features

- **BMI history summary** — after viewing all records, show total count, average BMI, lowest and highest BMI with name and category, and most common category; colors match BMI severity; shown at bottom of view all records before pause
- **Sort records** — view all records sorted by BMI, name, or age instead of insertion order
- **Edit/Update record** — select a saved record and update any field; BMI and category recomputed after edit
- **Filter by category** — view only records matching a specific BMI category (e.g. Overweight only)
- **Duplicate name warning** — when saving, warn the user if a record with the same name already exists; user can still proceed or cancel
- **Automatic backup** — silently creates a timestamped backup (`records_YYYY-MM-DD_HH-MM-SS.psv`) in a `backup/` folder on every save; keeps only the last 3 backups, oldest is deleted when a 4th is created