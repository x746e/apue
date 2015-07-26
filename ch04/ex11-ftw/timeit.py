#!/usr/bin/env python
import os
import sys
import json
import time
import subprocess


RECORDS_FILE = 'records.json'


def main():
    command = sys.argv[1]

    start_time = time.time()
    subprocess.check_call(command, shell=True)
    elapsed_time = time.time() - start_time

    write_record(command, elapsed_time)


def write_record(command, elapsed_time):
    if os.path.exists(RECORDS_FILE):
        with open(RECORDS_FILE, 'rt') as f:
            existing_records = json.load(f)
    else:
        existing_records = {}

    if command not in existing_records:
        existing_records[command] = []
    existing_records[command].append(elapsed_time)

    with open(RECORDS_FILE, 'wt') as f:
        json.dump(existing_records, f)


if __name__ == '__main__':
    main()
