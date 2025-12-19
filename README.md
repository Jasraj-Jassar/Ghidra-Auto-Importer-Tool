# Ghidra Auto-Importer

Automates importing a binary into a fresh Ghidra project and opens it in the GUI. Each run drops a new project under `$HOME/GhidraProjects/<binary>_<timestamp>` so you keep clean, timestamped workspaces.

## Requirements
- Ghidra installed.
- `analyzeHeadless` and `ghidra` available in `PATH` (typical install puts `analyzeHeadless` at `/opt/ghidra/support/analyzeHeadless`; add it with `export PATH="/opt/ghidra/support:$PATH"`).
- A writable `$HOME` (falls back to `./GhidraProjects` if unset).

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

## Usage
```bash
ghidra_auto <binary>
```

- `-h` or `--help` prints a short guide.
- The tool creates a timestamped project, imports the binary via `analyzeHeadless`, then launches `ghidra` opening the new `.gpr`.

## Notes
- Ensure `analyzeHeadless` resolves correctly:
  ```bash
  which analyzeHeadless
  # /opt/ghidra/support/analyzeHeadless  (example)
  ```
- Projects land in `$HOME/GhidraProjects`; adjust that in the source if you want a different root.
