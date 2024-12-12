#!/bin/bash

# Tests bot responses to various commands.

SERVER=$1
PORT=$2
PASSWORD=$3

echo "Starting bot interaction tests..."

# Start the server
$SERVER $PORT $PASSWORD &
SERVER_PID=$!
sleep 1

# Test bot responses
(
  echo "PASS $PASSWORD"
  echo "NICK BotUser"
  echo "BOT HELLO"
  echo "BOT TIME"
) | nc 127.0.0.1 $PORT > bot_response.txt

# Check responses
if grep -q "Hello! I'm HelperBot" bot_response.txt && grep -q "The current time is" bot_response.txt; then
  echo "Bot interaction test passed!"
else
  echo "Bot interaction test failed!"
fi

# Clean up
kill $SERVER_PID
rm -f bot_response.txt
