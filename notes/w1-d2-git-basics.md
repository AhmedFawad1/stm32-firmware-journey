# W1 D2: Git basics

**Date:** 2026-10-27
**Status:** Done
**Result:** Repo `stm32-firmware-journey` created and pushed to GitHub with 3 commits.

---

## What I learned

### Git vs GitHub
- **Git** = version control system on my laptop. Saves snapshots (commits) of my project. The full history lives locally in the hidden `.git/` folder (never edit or delete it).
- **GitHub** = website that stores a copy of my repo online (backup + portfolio for recruiters).
- Git works with no internet and no GitHub at all.

### The three areas
```
Working folder  --git add-->  Staging area  --git commit-->  Repository  --git push-->  GitHub
```
- `git add` stages the file's **contents at that moment**, not "the file". If I edit after `git add`, I must add again.
- The staging area lets me split unrelated changes into separate commits (one logical change per commit).
- After `git commit` the change is in my **local** repo only. GitHub sees it only after `git push`.

### .gitignore
- Rule: **commit what I write, ignore what the tools generate.**
- Ignored: `build/`, `*.o`, `*.elf`, `*.bin`, `*.hex`, `*.map`.
- Tested it: a dummy `test.o` did not appear in `git status` ("working tree clean").
- Create it in VS Code, not Notepad (Windows may save it as `.gitignore.txt`).
- Git does not track empty folders, only files.

### Good commit messages
- Imperative mood: "Add", "Fix", "Remove" (test: "If applied, this commit will ...").
- Short (~50 chars), capital letter, no full stop.
- Say what and why, not how.
- Example: `i changed the delay because the led was blinking too fast` → `Slow down LED blink rate`

### Connecting to GitHub
- `git remote add origin <url>` links the local repo to the GitHub repo. `origin` is a nickname for the URL.
- `git branch -M main` renames the branch `master` → `main` (GitHub's default).
- `git push -u origin main` uploads commits. `-u` remembers the link, so later just `git push`.
- Create the GitHub repo **empty** (no README/.gitignore/license) to avoid a conflict on the first push.

## Commands used today
| Command | What it does |
| --- | --- |
| `git --version` | Check Git is installed |
| `git config --global user.name "Name"` | Set author name (key and value separated by a space) |
| `git config --global user.email "email"` | Set author email (same as GitHub account) |
| `git config --global --list` | Show settings |
| `git init <folder>` | Turn a folder into a repo |
| `git status` | Show which area each change is in |
| `git add <file>` | Stage a change |
| `git commit -m "msg"` | Save staged changes as a snapshot |
| `git log --oneline` | Short history (hash + message) |
| `git push` | Send commits to GitHub |

## Daily routine (from now on)
```
edit → git status → git add → git status → git commit -m "..." → git push
```

## What broke and how I fixed it
1. **Only `user.email` showed in `--list`.** I had forgotten `user.name`. Set it separately.
2. **`error: invalid key: user.name=Ahmed Fawad`.** I used `=`. The list *shows* `key=value`, but the set command takes key and value as two separate words. Fix: replace `=` with a space.
3. **README showed as both "to be committed" and "not staged".** I edited it after `git add`. Committing then would have saved the empty version. Fix: `git add README.md` again.
4. **Commit message style.** `add gitignore` works but is lowercase and vague. Better: `Ignore build outputs`.

## Setup notes
- Repo location: `F:\Embedded\stm32-firmware-journey` (no spaces in path; spaces break Make/CMake later).
- Git version: 2.51.1.windows.1

## Commit log
```
b8694f7 Add glossary.md containing terms that i learn
b7eac01 add gitignore
5a23a45 Add README.md with introductory text
```

## Next: W1 D3
Build systems: why Make/CMake exist; build a "hello world" C program on the PC with CMake.