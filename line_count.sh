#!/usr/bin/env bash

# Target files and directories
TARGETS=( "main.cpp" "src/" "include/" )

# Filter targets to only existing ones to avoid find errors
EXISTING_TARGETS=()
for target in "${TARGETS[@]}"; do
    [[ -e "$target" ]] && EXISTING_TARGETS+=("$target")
done

# If no targets exist, exit early
if [ ${#EXISTING_TARGETS[@]} -eq 0 ]; then
    echo 0
    exit 0
fi

# Calculate the total lines and extract just the number
total_lines=$(find "${EXISTING_TARGETS[@]}" -type f 2>/dev/null | xargs wc -l 2>/dev/null | tail -n 1 | awk '{print $1}')

# Fallback to 0 if total_lines is empty (e.g., if files exist but are completely empty)
echo "${total_lines:-0}"
