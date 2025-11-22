#!/bin/bash
# -------------------------------------------------
# Script pour ouvrir docs/html/index.html dans le navigateur par défaut
# -------------------------------------------------

INDEX_FILE="docs/html/index.html"

if [ -f "$INDEX_FILE" ]; then
    echo "Opening documentation in default browser..."
    if command -v xdg-open &> /dev/null; then
        xdg-open "$INDEX_FILE"
    elif command -v gnome-open &> /dev/null; then
        gnome-open "$INDEX_FILE"
    elif command -v open &> /dev/null; then
        open "$INDEX_FILE"  # macOS
    else
        echo "Impossible d'ouvrir le navigateur automatiquement. Ouvrez $INDEX_FILE manuellement."
    fi
else
    echo "Fichier $INDEX_FILE non trouvé."
fi
