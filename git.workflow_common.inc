# shellcheck shell=bash

# Common helpers shared by git.start/git.sync/git.finish/git.show_branch_disposition.
# All helpers favor safety (strict failure on unexpected states) and transparency.

fail() {
  echo "FAIL $*" >&2
  exit 1
}

ok() {
  echo "OK $*" >&2
}

# run prints the command and executes it immediately.
run() {
  echo "+ $*"
  "$@"
}

# run_capture mirrors run but prints the command to stderr so callers can capture stdout.
run_capture() {
  echo "+ $*" >&2
  "$@"
}

ensure_inside_repo() {
  if ! git rev-parse --is-inside-work-tree >/dev/null 2>&1; then
    fail "Not inside a git repository."
  fi
}

# hostname_short returns a concise host label for sync commit messages.
hostname_short() {
  local name
  if name=$(hostname -s 2>/dev/null); then
    printf '%s\n' "$name"
    return
  fi
  hostname
}

# working_tree_has_changes returns success when tracked or untracked edits exist.
working_tree_has_changes() {
  if [ -n "$(git status --porcelain)" ]; then
    return 0
  fi
  return 1
}

# ensure_working_tree_clean aborts if any edits are pending.
ensure_working_tree_clean() {
  if working_tree_has_changes; then
    fail "Working tree has uncommitted changes."
  fi
}

# current_branch_name refuses detached HEAD states to avoid ambiguous behavior.
current_branch_name() {
  local branch
  branch=$(git rev-parse --abbrev-ref HEAD)
  if [ "$branch" = "HEAD" ]; then
    fail "Detached HEAD is not supported."
  fi
  printf '%s\n' "$branch"
}

# branch_has_upstream reports whether the branch tracks a remote ref.
branch_has_upstream() {
  local branch
  branch="$1"
  if git rev-parse --abbrev-ref "${branch}@{u}" >/dev/null 2>&1; then
    return 0
  fi
  return 1
}

# Globals populated by detect_base_branch
BASE_BRANCH=""
BASE_REF=""
BASE_FROM_REMOTE="false"

# detect_base_branch inspects local refs before falling back to origin to locate mainline.
detect_base_branch() {
  for candidate in main master; do
    if git show-ref --verify --quiet "refs/heads/${candidate}"; then
      BASE_BRANCH="$candidate"
      BASE_REF="$candidate"
      BASE_FROM_REMOTE="false"
      return
    fi
  done

  for candidate in main master; do
    if git show-ref --verify --quiet "refs/remotes/origin/${candidate}"; then
      BASE_BRANCH="$candidate"
      BASE_REF="origin/${candidate}"
      BASE_FROM_REMOTE="true"
      return
    fi
  done

  fail "Unable to determine base branch (looked for main or master)."
}

# checkout_base_branch_for_update refreshes the base branch before rebasing or branching.
checkout_base_branch_for_update() {
  if [ "$BASE_BRANCH" = "" ]; then
    fail "Base branch not detected."
  fi

  if [ "$BASE_FROM_REMOTE" = "true" ]; then
    run git fetch origin "$BASE_BRANCH"
    run git checkout -B "$BASE_BRANCH" "$BASE_REF"
    run git branch --set-upstream-to "origin/${BASE_BRANCH}" "$BASE_BRANCH"
  else
    run git checkout "$BASE_BRANCH"
  fi
  run git pull --ff-only
}

# ensure_not_base_branch avoids mutating the protected base branch.
ensure_not_base_branch() {
  local branch
  branch="$1"
  if [ "$branch" = "$BASE_BRANCH" ]; then
    fail "Operation refused on base branch $BASE_BRANCH."
  fi
}

# ensure_branch_exists_locally guarantees work only targets existing branches.
ensure_branch_exists_locally() {
  local branch
  branch="$1"
  if ! git show-ref --verify --quiet "refs/heads/${branch}"; then
    fail "Local branch ${branch} does not exist."
  fi
}

# remote_branch_exists uses ls-remote to determine whether origin holds the branch.
remote_branch_exists() {
  local branch
  branch="$1"
  if git ls-remote --exit-code --heads origin "$branch" >/dev/null 2>&1; then
    return 0
  fi
  return 1
}
