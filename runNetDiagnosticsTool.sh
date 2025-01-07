#!/bin/bash

# Path to the NetDiagnosticsTool executable
EXECUTABLE_PATH="Delivery/Linux/linux-debug/NetDiagnosticsTool"

# Check if a TARGET_IP argument is provided
if [ -z "$1" ]; then
    echo "No target IP provided. Running NetDiagnosticsTool on default target."
    $EXECUTABLE_PATH
else
    TARGET_IP=$1
    echo "Running NetDiagnosticsTool with TARGET_IP: $TARGET_IP"
    $EXECUTABLE_PATH $TARGET_IP
fi
