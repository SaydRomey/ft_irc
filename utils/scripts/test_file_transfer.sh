#!/bin/bash

# Tests file upload and download functionality.

SERVER=$1
PORT=$2
PASSWORD=$3

echo "Starting file transfer tests..."

# Create a test file
echo "Sample file content" > testfile.txt

# Start the server
$SERVER $PORT $PASSWORD &
SERVER_PID=$!
sleep 1

# Test file upload and download
(
  echo "PASS $PASSWORD"
  echo "NICK User1"
  echo "UPLOAD testfile.txt"
) | nc 127.0.0.1 $PORT > /dev/null

(
  echo "PASS $PASSWORD"
  echo "NICK User2"
  echo "DOWNLOAD testfile.txt"
) | nc 127.0.0.1 $PORT > received_file.txt

# Verify file content
if cmp -s testfile.txt received_file.txt; then
  echo "File transfer test passed!"
else
  echo "File transfer test failed!"
fi

# Clean up
kill $SERVER_PID
rm -f testfile.txt received_file.txt
