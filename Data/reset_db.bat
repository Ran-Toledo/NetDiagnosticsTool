@echo off
REM Check if the SQLite database file exists
IF EXIST NetDiagnostics.db (
    REM Delete the existing database file
    del NetDiagnostics.db
    echo Database "NetDiagnostics.db" has been deleted.
) ELSE (
    echo No existing database found to delete.
)

REM Create a new, empty SQLite database (this will automatically create the file)
sqlite3 NetDiagnostics.db "VACUUM;"

echo Database "NetDiagnostics.db" has been reset.

pause
