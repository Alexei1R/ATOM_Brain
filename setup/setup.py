
import subprocess
import os
import shutil
import platform




subprocess.run("clear")
BinFolder = "build"
if os.path.exists(BinFolder) and os.path.isdir(BinFolder):
    shutil.rmtree(BinFolder)
    print(f"The folder '{BinFolder}' deleted.")
else:
    print(f"The folder '{BinFolder}' not exist.")




# Get the current directory
current_directory = os.getcwd()
print(f"Current directory: {current_directory}")
# Move up to the main folder
os.chdir('..')

# Create the build folder
build_folder = os.path.join(current_directory, 'build')
if not os.path.exists(build_folder):
    os.makedirs(build_folder)
else:
    print(f"The '{build_folder}' folder already exists.")

# Change to the build folder
os.chdir(build_folder)
print(f"Current directory: {os.getcwd()}")

# Run CMake
print("Running CMake...")
subprocess.run(['cmake', '..'])

# Build the project
print("Building the project...")
subprocess.run(['make'])

print("Run Project >> ")
path_to_executable = os.path.join(build_folder, 'app')
for filename in os.listdir(path_to_executable):
    filepath = os.path.join(path_to_executable, filename)
    if os.path.isfile(filepath) and os.access(filepath, os.X_OK):
        os.system(filepath)