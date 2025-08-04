import subprocess
import time
import glob

# Define paths for profiles and AntiMicroX
antimicrox_path = r"C:\Program Files\AntiMicroX\bin\antimicrox.exe"
mouse_profile_path = r"mouse.gamecontroller.amgp"
keyboard1_profile_path = r"keyboard1.gamecontroller.amgp"
keyboard2_profile_path = r"keyboard2.gamecontroller.amgp"

#Load profile based on input
def load_profile(input_device):
    if input_device.lower() == "mouse":
        mouseCommand = [antimicrox_path, "--hidden", "--profile", mouse_profile_path]
        try:        
            subprocess.run(mouseCommand, check=True)
        except Exception as e:
            print(f"Error executing command: {e}")
    elif input_device.lower() == "keyboard":
        # Load keyboard1 profile onto joystick 1 and keyboard2 profile onto joystick 2
        command = [antimicrox_path, "--unload", "1", "--hidden"]
        command1 = [antimicrox_path, "--profile-controller", "1", "--profile", keyboard1_profile_path]
        command2 = [antimicrox_path, "--unload", "2"]
        command3 = [antimicrox_path, "--profile-controller", "2", "--profile", keyboard2_profile_path]
        try:        
            subprocess.Popen(command)
            time.sleep(3)
            subprocess.Popen(command1)
            time.sleep(3)
            subprocess.Popen(command2)            
            # Terminate the first instance of AntiMicroX in order to load correctly
            kill_antimicrox()
            time.sleep(3)
            subprocess.Popen(command3)
        except Exception as e:
            print(f"Error executing command: {e}")
    else:
        print("Invalid input device. Please choose 'Mouse' or 'Keyboard'.")
        return

# Command to kill AntiMicroX process
def kill_antimicrox():
    subprocess.run(["taskkill", "/f", "/im", "antimicrox.exe"])

# Commands to find the line in path with Control:
def find_control_line(file_path):
    try:
        with open(file_path, 'r') as file:
            for line in file:
                if line.startswith("Controller:"):
                    return line.strip().split(":")[1].strip()
        print("Controller line not found in the file.")
        return None
    except FileNotFoundError:
        print(f"Error: File '{file_path}' not found.")
        return None

# Path to the file containing input device
text_files = glob.glob("*.txt")
# Find the control line in a text file

if not text_files:
    print("No text files found in the directory.")
else:
    for file_path in text_files:
        input_device = find_control_line(file_path)
        if input_device:
            load_profile(input_device)