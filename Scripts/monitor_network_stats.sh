#!/bin/bash

# Default configurations
OUTPUT_DIR="./../Data"            # Default output directory
DEFAULT_IP="8.8.8.8"              # Default target IP for network monitoring
INTERVAL=1                        # Time interval between samples in seconds
DURATION=60                       # Duration for monitoring in seconds

# Handle script arguments
if [ $# -ge 1 ]; then
    OUTPUT_DIR="$1"
fi
if [ $# -ge 2 ]; then
    PING_TARGET="$2"
else
    PING_TARGET="$DEFAULT_IP"
fi

# Ensure output directory exists
mkdir -p "$OUTPUT_DIR"

# Set output file path
OUTPUT_FILE="$OUTPUT_DIR/network_stats.csv"

# Add CSV header
echo "Timestamp,Ping_Latency(ms),Packet_Loss(%),TCP_Connections,UDP_Sent,UDP_Received,Bandwidth_Utilization(Bytes/sec),Errors,Retransmissions" > "$OUTPUT_FILE"

# Start monitoring
START_TIME=$(date +%s)
while [ $(( $(date +%s) - START_TIME )) -lt $DURATION ]; do
    TIMESTAMP=$(date +"%Y-%m-%d %H:%M:%S")
    
    # Ping latency (ms)
    PING_LATENCY=$(ping -c 1 -w 1 "$PING_TARGET" | grep 'time=' | awk -F'time=' '{print $2}' | awk '{print $1}' || echo "N/A")
    
    # Packet loss (%)
    PACKET_LOSS=$(ping -c 10 -w 5 "$PING_TARGET" | grep "loss" | awk -F', ' '{print $3}' | awk '{print $1}' || echo "N/A")
    
    # Established TCP connections
    TCP_CONNECTIONS=$(netstat -an | grep ESTABLISHED | wc -l)
    
    # UDP traffic (datagrams sent and received)
    UDP_SENT=$(cat /proc/net/snmp | grep -w Udp | tail -1 | awk '{print $8}')
    UDP_RECEIVED=$(cat /proc/net/snmp | grep -w Udp | tail -1 | awk '{print $5}')
    
    # Bandwidth utilization (bytes/sec)
    RX_BEFORE=$(cat /proc/net/dev | grep eth0 | awk '{print $2}')
    TX_BEFORE=$(cat /proc/net/dev | grep eth0 | awk '{print $10}')
    sleep 1
    RX_AFTER=$(cat /proc/net/dev | grep eth0 | awk '{print $2}')
    TX_AFTER=$(cat /proc/net/dev | grep eth0 | awk '{print $10}')
    BANDWIDTH=$(( (RX_AFTER - RX_BEFORE + TX_AFTER - TX_BEFORE) / INTERVAL ))
    
    # Errors and retransmissions
    ERRORS=$(cat /proc/net/dev | grep eth0 | awk '{print $4}')
    RETRANSMISSIONS=$(netstat -s | grep "segments retransm" | awk '{print $1}')
    
    # Write data to CSV
    echo "$TIMESTAMP,$PING_LATENCY,$PACKET_LOSS,$TCP_CONNECTIONS,$UDP_SENT,$UDP_RECEIVED,$BANDWIDTH,$ERRORS,$RETRANSMISSIONS" >> "$OUTPUT_FILE"

    # Wait for the next interval
    sleep "$INTERVAL"
done

echo "Network monitoring completed. Data saved to $OUTPUT_FILE."
