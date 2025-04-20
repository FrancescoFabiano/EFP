#!/usr/bin/env bash
set -euo pipefail

if [ "$#" -ne 2 ]; then
  echo "Usage: $0 <dot_file> <number>"
  exit 1
fi

DOT_FILE="$1"
DEPTH="$2"

# ── Find the absolute path of the current script ────────────────────────────────
SCRIPT_PATH="$(realpath "${BASH_SOURCE[0]}")"
SCRIPT_DIR="$(dirname "$SCRIPT_PATH")"

# ── Walk up until we find the 'epistemic_rl' directory ──────────────────────────
EPIS_RL_DIR="$SCRIPT_DIR"
while [[ "$(basename "$EPIS_RL_DIR")" != "epistemic_rl" && "$EPIS_RL_DIR" != "/" ]]; do
  EPIS_RL_DIR="$(dirname "$EPIS_RL_DIR")"
done

if [[ "$(basename "$EPIS_RL_DIR")" != "epistemic_rl" ]]; then
  echo "❌ Could not locate the 'epistemic_rl' repo root."
  exit 1
fi

# ── Activate the virtual environment in that directory ──────────────────────────
VENV_PATH="$EPIS_RL_DIR/.venv/bin/activate"
if [[ ! -f "$VENV_PATH" ]]; then
  echo "❌ No .venv found at: $VENV_PATH"
  exit 1
fi

source "$VENV_PATH"

# ── Run the Python script ───────────────────────────────────────────────────────
python3 "$EPIS_RL_DIR/lib/RL/main.py" "$DOT_FILE" "$DEPTH"
