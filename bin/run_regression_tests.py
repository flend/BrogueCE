import os
import subprocess
import argparse
from concurrent.futures import ThreadPoolExecutor

def run_brogue_tests(directory, num_processes, extra_args):
    # Get the absolute path of the directory
    directory = os.path.abspath(directory)

    # Get a list of files with the extension '.broguerec'
    files = [file for file in os.listdir(directory) if file.endswith('.broguerec')]

    # Create a ThreadPoolExecutor with the specified number of processes
    with ThreadPoolExecutor(max_workers=num_processes) as executor:
        # Submit the brogue command for each file
        for file in files:
            file_path = os.path.join(directory, file)
            if extra_args:
                command = f'./brogue {extra_args} -vn {file_path}'
            else:
                command = f'./brogue -vn {file_path}'

            # Use executor.submit to run the command in a separate thread
            executor.submit(run_command, command)

def run_command(command):
    # Run the command using subprocess.run
    print(f"Running {command}")
    result = subprocess.run(command, shell=True, capture_output=True, text=True)

    # Print the command output
    print(f"Results of {command}: {result.stdout}")
    print('-' * 50)

def main():
    # Create the argument parser
    parser = argparse.ArgumentParser(description='Brogue Test Runner')
    parser.add_argument('directory', help='Directory containing test files')
    parser.add_argument('--num_processes', type=int, default=1, help='Number of tests to run simultaneously')
    parser.add_argument('--extra_args', help='Extra command-line arguments to be passed to brogue (e.g. to select variant)')
    
    # Parse the command line arguments
    args = parser.parse_args()

    # Call the function to run the brogue tests
    run_brogue_tests(args.directory, args.num_processes, args.extra_args)

if __name__ == '__main__':
    main()