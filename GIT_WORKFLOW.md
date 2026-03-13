# Minimal Git Workflow Toolkit

These utilities formalize the git workflow described in the project brief. They
emphasize transparent commands, safe defaults, and short-lived branches that
sync cleanly across machines.

## Scripts

- `git.start` – create a feature branch from an up-to-date mainline and publish
  it to origin.
- `git.sync` – checkpoint work-in-progress commits, push them, and fast-forward
  the branch.
- `git.finish` – stage pending work (including untracked files), commit with a
  user-supplied message, rebase onto the latest mainline, push with
  `--force-with-lease`, or clean up the branch after merge when called with
  `--cleanup`.
- `git.show_branch_disposition` – report how far the current branch is ahead of
  the base.

All scripts live in `dp/git/git_util` and source `git.workflow_common.inc` for
shared helpers such as `run()` and repository safety checks.

## Shared Conventions

1. **No black boxes** – every git command is echoed before execution.
2. **Safe defaults** – scripts abort on dirty working trees or when the base
   branch cannot be determined.
3. **Short-lived branches** – start new branches with `git.start`, sync often
   with `git.sync`, and prepare PRs via `git.finish`.
4. **Multi-machine friendly** – `git.sync` commits WIP snapshots with a host
   label (e.g., `WIP(sync:hostname)`) and ensures upstream tracking.

## Usage Details

### git.start `<branch>`

1. Assert repository context and a clean working tree.
2. Detect the base branch (`main` preferred, `master` fallback).
3. Check out and fast-forward the base branch.
4. Create the new branch and push it to origin.

Example session:

```
$ git.start feature/parser-refresh
+ git checkout main
+ git pull --ff-only
+ git checkout -b feature/parser-refresh
+ git push -u origin feature/parser-refresh
OK Created branch feature/parser-refresh tracking origin/feature/parser-refresh.
```

### git.sync

1. Refuse to commit directly on the base branch when changes exist.
2. Stage and commit WIP changes if present (`WIP(sync:<hostname>)`).
3. Push the branch (setting upstream if needed).
4. Fetch `origin` and fast-forward to incorporate remote commits.

Use `git.sync` liberally when changing machines or prior to pauses in work.

### git.finish

: Prepares the current feature branch for pull request review.

```
$ git.finish "PR: refresh parser"
```

1. Stage all tracked and untracked changes, committing them with the provided
   message if any modifications exist.
2. Fetch `origin` and fast-forward the base branch locally.
3. Rebase the feature branch onto the refreshed base.
4. Push with `--force-with-lease` (or `-u` for new branches).

`git.finish --cleanup <branch>`
: Remove a feature branch after it has been merged.

1. Fast-forward the base branch.
2. Delete the local branch if it exists.
3. Delete the remote branch if it exists.

### git.show_branch_disposition

Displays the current branch name followed by the number of commits ahead of the
base branch (e.g., `feature/parser-refresh+3`). Useful for status lines or quick
triage.

## Typical Flow

```
# 1. Start work
$ git.start feature/new-report

# 2. Save progress repeatedly
$ git.sync

# 3. Prepare for PR
$ git.finish "Ready for PR"

# 4. After merge
$ git.finish --cleanup feature/new-report

# 5. Inspect divergence
$ git.show_branch_disposition
```

## Implementation Notes

- Scripts are POSIX-friendly bash with `set -euo pipefail`.
- Git commands are wrapped in `run()`/`run_capture()` to echo actions.
- Base branch detection follows the order: local `main`, local `master`,
  `origin/main`, `origin/master`.
- History rewriting uses `--force-with-lease` to prevent accidental loss of
  collaborator commits.
- `git.finish` refuses to operate on the base branch to keep `main` pristine.

With these scripts, the workflow remains transparent and recoverable while
reducing manual ceremony.
