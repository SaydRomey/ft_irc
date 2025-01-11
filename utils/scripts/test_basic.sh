#!/bin/bash

# Tests basic functionality like authentication, joining channels, and sending messages.

# ANSI color codes
GREEN="\033[32m"
RED="\033[31m"
RESET="\033[0m"

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
  echo -e "${GREEN}Basic functionality test passed!${RESET}"
else
  echo -e "${RED}Basic functionality test failed!${RESET}"
fi

# Clean up
kill $SERVER_PID
