# Ghidra Auto-Importer

Automates importing a binary into a fresh Ghidra project and opens it in the GUI. Each run drops a new project under `$HOME/GhidraProjects/<binary>_<timestamp>` so you keep clean, timestamped workspaces.

## Requirements
- Ghidra installed.
- `analyzeHeadless` and `ghidra` available in `PATH` (skip if you use the one-shot install; typical install puts `analyzeHeadless` at `/opt/ghidra/support/analyzeHeadless`; add it with `export PATH="/opt/ghidra/support:$PATH"`).
- A writable `$HOME` (falls back to `./GhidraProjects` if unset).

## One-shot install (copy/paste)
```bash
set -euo pipefail

# 1) Clone and enter the repo (update REPO_URL if you host it elsewhere)
REPO_URL="https://github.com/your-user/ghidra-auto-importer.git"
git clone "$REPO_URL"
cd "$(basename "${REPO_URL%.git}")"

# 2) Make analyzeHeadless available (adjust path if your Ghidra lives elsewhere)
GHIDRA_SUPPORT="/opt/ghidra/support"
export PATH="$GHIDRA_SUPPORT:$PATH"
grep -q 'export PATH="/opt/ghidra/support:$PATH"' "$HOME/.bashrc" 2>/dev/null || echo 'export PATH="/opt/ghidra/support:$PATH"' >> "$HOME/.bashrc"

# 3) Ensure the default project directory exists (creates it if missing)
[ -d "$HOME/GhidraProjects" ] || mkdir -p "$HOME/GhidraProjects"

# 4) Build the helper
g++ -std=c++17 ghidra_auto.cpp -o ghidra_auto

# 5) Add the binary to your PATH (user-local)
mkdir -p "$HOME/bin"
mv ghidra_auto "$HOME/bin/"
export PATH="$HOME/bin:$PATH"
grep -q 'export PATH="$HOME/bin:$PATH"' "$HOME/.bashrc" 2>/dev/null || echo 'export PATH="$HOME/bin:$PATH"' >> "$HOME/.bashrc"

# Ready to go
ghidra_auto --help
```

## Usage
```bash
ghidra_auto <binary>
```

- `-h` or `--help` prints a short guide.
- The tool creates a timestamped project, imports the binary via `analyzeHeadless`, then launches `ghidra` opening the new `.gpr`.

## Build
```bash
g++ -std=c++17 ghidra_auto.cpp -o ghidra_auto
```

## Install (add to PATH)
- Option A (system-wide):
  ```bash
  sudo mv ghidra_auto /usr/local/bin/
  ```
- Option B (user-only):
  ```bash
  mkdir -p "$HOME/bin"
  mv ghidra_auto "$HOME/bin/"
  echo 'export PATH="$HOME/bin:$PATH"' >> "$HOME/.bashrc"   # or .zshrc
  source "$HOME/.bashrc"
  ```

## Notes
- Add analyzeHeadless to PATH (step-by-step):
  ```bash
  # 1) Confirm the location of analyzeHeadless (default is shown)
  ls /opt/ghidra/support/analyzeHeadless
  
  # 2) Add it to PATH for your shell (bash example; use .zshrc for zsh)
  echo 'export PATH="/opt/ghidra/support:$PATH"' >> "$HOME/.bashrc"
  
  # 3) Reload your shell config so the change applies now
  source "$HOME/.bashrc"
  
  # 4) Verify it resolves correctly
  which analyzeHeadless
  ```
- Projects land in `$HOME/GhidraProjects`; adjust that in the source if you want a different root.

Developed by me.
