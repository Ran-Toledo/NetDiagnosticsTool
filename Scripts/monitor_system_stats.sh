#!/bin/bash

# Default configurations
OUTPUT_DIR="./../Data"            # Default output directory
INTERVAL=1                        # Time interval between samples in seconds
DURATION=60                       # Duration for monitoring in seconds

# Handle script arguments
if [ $# -ge 1 ]; then
    OUTPUT_DIR="$1"
fi

# Ensure output directory exists
mkdir -p "$OUTPUT_DIR"

# Set output file path
OUTPUT_FILE="$OUTPUT_DIR/system_stats.csv"

# Add CSV header
echo "Timestamp,CPU_Usage(%),Memory_Availability(MB),Cache_Pages/sec,Disk_Read(KB/s),Disk_Write(KB/s)" > "$OUTPUT_FILE"

# Start monitoring
START_TIME=$(date +%s)
while [ $(( $(date +%s) - START_TIME )) -lt $DURATION ]; do
    TIMESTAMP=$(date +"%Y-%m-%d %H:%M:%S")
    
    # CPU usage (%)
    CPU_USAGE=$(top -bn1 | grep "Cpu(s)" | sed "s/.*, *\([0-9.]*\)%* id.*/\1/" | awk '{print 100 - $1}')
    
    # Memory availability (MB)
    MEM_AVAILABLE=$(free -m | grep Mem | awk '{print $7}')
    
    # Cache pages/sec (pages read/written to/from cache)
    CACHE_PAGES=$(vmstat 1 2 | tail -1 | awk '{print $6}')
    
    # Disk read/write (KB/s)
    DISK_READ=$(iostat -d | grep sda | awk '{print $3}')
    DISK_WRITE=$(iostat -d | grep sda | awk '{print $4}')
    
    # Write data to CSV
    echo "$TIMESTAMP,$CPU_USAGE,$MEM_AVAILABLE,$CACHE_PAGES,$DISK_READ,$DISK_WRITE" >> "$OUTPUT_FILE"

    # Wait for the next interval
    sleep "$INTERVAL"
done

echo "System monitoring completed. Data saved to $OUTPUT_FILE."
