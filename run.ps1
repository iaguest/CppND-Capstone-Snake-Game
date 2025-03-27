# run.ps1

# Navigate to the build directory
Push-Location -Path ".\build"

# Run the game
.\SnakeGame.exe

# Return to the root folder after execution
Pop-Location
