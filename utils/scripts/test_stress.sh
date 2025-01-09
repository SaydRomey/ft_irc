#!/bin/bash

# Simulates multiple clients connecting simultaneously.

SERVER=$1
PORT=$2
PASSWORD=$3

echo "Starting stress tests..."

# Start the server
$SERVER $PORT $PASSWORD &
SERVER_PID=$!
sleep 1

# Launch multiple clients
for i in {1..50}; do
  (
    echo "PASS $PASSWORD"
    echo "NICK User$i"
    echo "JOIN #general"
    echo "PRIVMSG #general :Hello from User$i"
  ) | nc 127.0.0.1 $PORT > /dev/null &
done

# Wait for clients to connect
sleep 5

# Check server stability
if kill -0 $SERVER_PID 2>/dev/null; then
  echo "Stress test passed!"
else
  echo "Stress test failed!"
fi

# Clean up
kill $SERVER_PID
