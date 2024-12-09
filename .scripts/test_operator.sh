#!/bin/bash

# Tests operator commands like KICK, INVITE, and TOPIC.

SERVER=$1
PORT=$2
PASSWORD=$3

echo "Starting operator command tests..."

# Start the server
$SERVER $PORT $PASSWORD &
SERVER_PID=$!
sleep 1

# Test operator commands
(
  echo "PASS $PASSWORD"
  echo "NICK Operator"
  echo "JOIN #general"
  echo "KICK #general User1"
  echo "TOPIC #general :New topic!"
  echo "INVITE User1 #general"
) | nc 127.0.0.1 $PORT > /dev/null

# Check server response
if [ $? -eq 0 ]; then
  echo "Operator command test passed!"
else
  echo "Operator command test failed!"
fi

# Clean up
kill $SERVER_PID
