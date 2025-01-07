import sqlite3
import matplotlib.pyplot as plt
import datetime
import os

from datetime import datetime

def parse_timestamp(timestamp_str):
    try:
        return datetime.strptime(timestamp_str, "%Y-%m-%d %H:%M:%S.%f")
    except ValueError:
        return datetime.strptime(timestamp_str, "%Y-%m-%d %H:%M:%S")


def plot_bandwith_usage(db_file, table_name):
    conn = None  # Define conn outside the try block
    try:
        # Connect to the SQLite database
        conn = sqlite3.connect(db_file)
        cursor = conn.cursor()

        # Execute the query to select all data from the specified table
        query = f"SELECT * FROM {table_name}"
        cursor.execute(query)

        # Fetch all rows from the query result
        rows = cursor.fetchall()

        # Get the column names
        column_names = [description[0] for description in cursor.description]

        # Ensure the table has at least 6 columns
        if len(column_names) < 6:
            print("Error: Table does not have a 6th column.")
            return

        # Separate the data
        timestamps = []
        bandwidth_data = []

        for row in rows:
            # Convert the first column to a timestamp using the provided format
            timestamp_str = row[0].strip('"')
            timestamp = parse_timestamp(timestamp_str)
            timestamps.append(timestamp)

            # Convert the 6th column to float
            value_str = row[5].strip('"')  # The 6th column (index 5)
            bandwidth_data.append(float(value_str))

        # Create the plot
        plt.figure(figsize=(10, 6))
        plt.plot(timestamps, bandwidth_data, label=column_names[5])  # Use the 6th column's name

        plt.xlabel('Timestamp')
        plt.ylabel('Bytes Total/Sec')
        plt.title('Bandwidth Usage')
        plt.legend()
        plt.grid(True)
        plt.xticks(rotation=45)
        plt.tight_layout()

        # Save the plot as an image
        file_path = os.path.join(current_directory, 'bandwidth_usage.png')
        plt.savefig(file_path)
        print(f"Bandwidth usage plot saved to {file_path}")

    except sqlite3.Error as e:
        print(f"An error occurred: {e}")

    finally:
        # Close the connection to the database
        if conn:
            conn.close()


def plot_ping_latency(db_file, table_name):
    conn = None  # Define conn outside the try block
    try:
        # Connect to the SQLite database
        conn = sqlite3.connect(db_file)
        cursor = conn.cursor()

        # Execute the query to select all data from the specified table
        query = f"SELECT * FROM {table_name}"
        cursor.execute(query)

        # Fetch all rows from the query result
        rows = cursor.fetchall()

        # Separate the data
        timestamps = []
        latencies = []

        for row in rows:
            # Convert the first column to a timestamp
            timestamp = parse_timestamp(row[0])
            timestamps.append(timestamp)

            # Append the latency value
            latencies.append(float(row[1]))

        # Create the plot
        plt.figure(figsize=(10, 6))
        plt.plot(timestamps, latencies, label='Latency (ms)')

        plt.xlabel('Timestamp')
        plt.ylabel('Latency (ms)')
        plt.title('Ping Latency Over Time')
        plt.legend()
        plt.grid(True)
        plt.xticks(rotation=45)
        plt.tight_layout()

        file_path = os.path.join(current_directory, 'ping_latency.png')

        # Save the plot as an image
        plt.savefig(file_path)

    except sqlite3.Error as e:
        print(f"An error occurred: {e}")

    finally:
        # Close the connection to the database
        if conn:
            conn.close()


# Get the current script directory
current_directory = os.path.dirname(__file__)

# Path to the SQLite database file (relative path)
db_file = os.path.join(current_directory, "..", "Data", "NetDiagnostics.db")

# Name of the tables to query
network_stats_table = "NetworkStats"
system_stats_table = "SystemStats"

# Plot the network statistics and ping latency
plot_bandwith_usage(db_file, network_stats_table)
plot_ping_latency(db_file, network_stats_table)

