#!/bin/bash

# Check if the SQLite database file exists
if [ -f "NetDiagnostics.db" ]; then
    # Delete the existing database file
    rm "NetDiagnostics.db"
    echo "Database 'NetDiagnostics.db' has been deleted."
else
    echo "No existing database found to delete."
fi

# Create a new, empty SQLite database
sqlite3 "NetDiagnostics.db" "VACUUM;"

echo "Database 'NetDiagnostics.db' has been reset."

read -p "Press [Enter] to continue..."
