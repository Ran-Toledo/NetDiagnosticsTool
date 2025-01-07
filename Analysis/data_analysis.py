import os
import sqlite3
import pandas as pd
import numpy as np
from datetime import datetime
import matplotlib.pyplot as plt

# Get the current script directory
current_directory = os.path.dirname(__file__)
# Path to the SQLite database file (relative path)
db_file = os.path.join(current_directory, "..", "Data", "NetDiagnostics.db")
# Name of the tables to query
network_stats_table = "NetworkStats"
system_stats_table = "SystemStats"

# Function to parse timestamps
def parse_timestamp(timestamp_str):
    try:
        return datetime.strptime(timestamp_str, "%Y-%m-%d %H:%M:%S.%f")
    except ValueError:
        return datetime.strptime(timestamp_str, "%Y-%m-%d %H:%M:%S")

# Connect to the database
conn = sqlite3.connect(db_file)

# Read the data into pandas DataFrames
network_df = pd.read_sql_query(f"SELECT * FROM {network_stats_table}", conn)
system_df = pd.read_sql_query(f"SELECT * FROM {system_stats_table}", conn)

# Close the database connection
conn.close()

# Convert the Timestamp column to datetime objects
network_df['Timestamp'] = network_df['Timestamp'].apply(parse_timestamp)
system_df['Timestamp'] = system_df['Timestamp'].apply(parse_timestamp)

# Convert other columns to numeric types
network_df['Ping_Latency(ms)'] = pd.to_numeric(network_df['Ping_Latency(ms)'], errors='coerce')
network_df['Packet_Loss(%)'] = pd.to_numeric(network_df['Packet_Loss(%)'], errors='coerce')
network_df['TCP_Connections'] = pd.to_numeric(network_df['TCP_Connections'], errors='coerce')
network_df['UDP_Sent'] = pd.to_numeric(network_df['UDP_Sent'], errors='coerce')
network_df['UDP_Received'] = pd.to_numeric(network_df['UDP_Received'], errors='coerce')
network_df['Bandwidth_Utilization(Bytes/sec)'] = pd.to_numeric(network_df['Bandwidth_Utilization(Bytes/sec)'], errors='coerce')
network_df['Errors'] = pd.to_numeric(network_df['Errors'], errors='coerce')
network_df['Retransmissions'] = pd.to_numeric(network_df['Retransmissions'], errors='coerce')

system_df['CPU_Usage(%)'] = pd.to_numeric(system_df['CPU_Usage(%)'], errors='coerce')
system_df['Memory_Availability(MB)'] = pd.to_numeric(system_df['Memory_Availability(MB)'], errors='coerce')
system_df['Cache_Pages/sec'] = pd.to_numeric(system_df['Cache_Pages/sec'], errors='coerce')
system_df['Disk_Read(KB/s)'] = pd.to_numeric(system_df['Disk_Read(KB/s)'], errors='coerce')
system_df['Disk_Write(KB/s)'] = pd.to_numeric(system_df['Disk_Write(KB/s)'], errors='coerce')

# Merge the network stats and system stats DataFrames on the Timestamp column
merged_df = pd.merge(network_df, system_df, on="Timestamp")

# Function to detect sharp spikes in metrics
def detect_spikes(data, column, threshold):
    data[f'{column}_Slope'] = data[column].diff() / data['Timestamp'].diff().dt.total_seconds()
    spikes = data[data[f'{column}_Slope'].abs() > threshold]
    return spikes

# Function to check for prolonged packet loss
def check_packet_loss(data, threshold, duration):
    data['Loss_Flag'] = data['Packet_Loss(%)'] > threshold
    prolonged_loss = data['Loss_Flag'].rolling(duration).sum() == duration
    return data[prolonged_loss]

# Function to check for frequent retransmissions or errors
def check_retransmissions_errors(data, retrans_threshold, error_threshold):
    retransmissions = data[data['Retransmissions'] > retrans_threshold]
    errors = data[data['Errors'] > error_threshold]
    return retransmissions, errors

# Function to check for CPU and memory usage spikes
def check_cpu_memory_spikes(data, cpu_threshold, memory_threshold):
    cpu_spikes = data[data['CPU_Usage(%)'] > cpu_threshold]
    memory_spikes = data[data['Memory_Availability(MB)'] < memory_threshold]
    return cpu_spikes, memory_spikes

# Define thresholds and duration
latency_threshold = 10
udp_threshold = 1000
packet_loss_threshold = 10
prolonged_loss_duration = 5
retrans_threshold = 10
error_threshold = 5
cpu_threshold = 80
memory_threshold = 2000

# Detect sharp spikes in ping latency
latency_spikes = detect_spikes(merged_df, 'Ping_Latency(ms)', latency_threshold)
if not latency_spikes.empty:
    print("Network issues detected due to sharp spikes in ping latency:")
    print(latency_spikes[['Timestamp', 'Ping_Latency(ms)', 'Ping_Latency(ms)_Slope']])
else:
    print("No sharp spikes in ping latency detected.")

# Detect sharp spikes in UDP traffic
udp_sent_spikes = detect_spikes(merged_df, 'UDP_Sent', udp_threshold)
udp_received_spikes = detect_spikes(merged_df, 'UDP_Received', udp_threshold)
if not udp_sent_spikes.empty:
    print("Network issues detected due to sharp spikes in UDP_Sent:")
    print(udp_sent_spikes[['Timestamp', 'UDP_Sent', 'UDP_Sent_Slope']])
else:
    print("No sharp spikes in UDP_Sent detected.")

if not udp_received_spikes.empty:
    print("Network issues detected due to sharp spikes in UDP_Received:")
    print(udp_received_spikes[['Timestamp', 'UDP_Received', 'UDP_Received_Slope']])
else:
    print("No sharp spikes in UDP_Received detected.")

# Check for prolonged packet loss
prolonged_packet_loss = check_packet_loss(merged_df, packet_loss_threshold, prolonged_loss_duration)
if not prolonged_packet_loss.empty:
    print("Network issues detected due to prolonged packet loss:")
    print(prolonged_packet_loss[['Timestamp', 'Packet_Loss(%)']])
else:
    print("No prolonged packet loss detected.")

# Check for frequent retransmissions or errors
retransmissions, errors = check_retransmissions_errors(merged_df, retrans_threshold, error_threshold)
if not retransmissions.empty:
    print("Frequent retransmissions detected:")
    print(retransmissions[['Timestamp', 'Retransmissions']])
else:
    print("No frequent retransmissions detected.")

if not errors.empty:
    print("Frequent errors detected:")
    print(errors[['Timestamp', 'Errors']])
else:
    print("No frequent errors detected.")

# Check for CPU and memory usage spikes
cpu_spikes, memory_spikes = check_cpu_memory_spikes(merged_df, cpu_threshold, memory_threshold)
if not cpu_spikes.empty:
    print("CPU usage spikes detected:")
    print(cpu_spikes[['Timestamp', 'CPU_Usage(%)']])
else:
    print("No CPU usage spikes detected.")

if not memory_spikes.empty:
    print("Memory availability spikes detected:")
    print(memory_spikes[['Timestamp', 'Memory_Availability(MB)']])
else:
    print("No memory availability spikes detected.")

# Check for correlation between network issues and CPU/memory spikes
network_issues = pd.concat([latency_spikes, udp_sent_spikes, udp_received_spikes, prolonged_packet_loss])
correlated_cpu_spikes = pd.merge(network_issues, cpu_spikes, on="Timestamp")
correlated_memory_spikes = pd.merge(network_issues, memory_spikes, on="Timestamp")

if not correlated_cpu_spikes.empty:
    print("Correlation detected between network issues and CPU usage spikes:")
    print(correlated_cpu_spikes[['Timestamp', 'Ping_Latency(ms)', 'UDP_Sent', 'UDP_Received', 'Packet_Loss(%)', 'CPU_Usage(%)']])
else:
    print("No correlation detected between network issues and CPU usage spikes.")

if not correlated_memory_spikes.empty:
    print("Correlation detected between network issues and memory availability spikes:")
    print(correlated_memory_spikes[['Timestamp', 'Ping_Latency(ms)', 'UDP_Sent', 'UDP_Received', 'Packet_Loss(%)', 'Memory_Availability(MB)']])
else:
    print("No correlation detected between network issues and memory availability spikes.")
