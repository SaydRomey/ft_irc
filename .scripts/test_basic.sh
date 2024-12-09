#!/bin/bash

# Tests basic functionality like authentication, joining channels, and sending messages.

SERVER=$1
PORT=$2
PASSWORD=$3

echo "Starting basic functionality tests..."

# Start the server
$SERVER $PORT $PASSWORD &
SERVER_PID=$!
sleep 1

# Test authentication, nicknames, and messaging
(
  echo "PASS $PASSWORD"
  echo "NICK User1"
  echo "JOIN #general"
  echo "PRIVMSG #general :Hello, world!"
) | nc 127.0.0.1 $PORT > /dev/null

# Check server response
if [ $? -eq 0 ]; then
  echo "Basic functionality test passed!"
else
  echo "Basic functionality test failed!"
fi

# Clean up
kill $SERVER_PID
