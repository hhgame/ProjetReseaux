@echo off
REM -------------------------------------------------
REM Script pour ouvrir docs/html/index.html dans le navigateur par défaut
REM -------------------------------------------------

IF EXIST docs\html\index.html (
    echo Opening documentation in default browser...
    start "" docs\html\index.html
) ELSE (
    echo Fichier docs\html\index.html non trouvé.
)

pause
