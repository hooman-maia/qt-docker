#!/bin/bash

# Navigate to the source code directory
cd /app/src

# Function to build and run the application
build_and_run() {
  echo "Running qmake..."
  qmake api-client.pro

  echo "Running make..."
  make

  echo "Running application..."
  ./api-client &
  APP_PID=$!
}

# Initial build and run
build_and_run

# Watch for changes and rebuild
while true; do
  inotifywait -e modify,create,delete,move -r . --exclude '.*\.o$|.*moc_.*\.cpp$|Makefile'
  echo "Detected change, rebuilding..."
  
  # Kill the previous instance of the application
  if [ ! -z "$APP_PID" ]; then
    kill $APP_PID
  fi
  
  # Rebuild and run
  build_and_run
done