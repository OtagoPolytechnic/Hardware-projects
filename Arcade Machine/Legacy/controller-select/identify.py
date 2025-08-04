import subprocess
import re
antimicrox_path = r"C:\Program Files\AntiMicroX\bin\antimicrox.exe"

def identify_controllers():
    try:
        # Run the AntiMicroX command with the --list option and capture its output
        output = subprocess.check_output([antimicrox_path, "--list"], universal_newlines=True)
        
        # Use regular expressions to extract controller information from the output
        controller_info = re.findall(r".+", output)
        
        # If controller information is found, return it
        if controller_info:
            return controller_info
        else:
            return None
    except subprocess.CalledProcessError:
        print("Error: Failed to execute AntiMicroX command.")
        return None

controllers = identify_controllers()
if controllers:
    print("Detected controllers:")
    for controller in controllers:
        print(controller)
else:
    print("No controllers detected.")
