import os
import subprocess
import argparse

def run_brogue_tests(directory):
    # Get the absolute path of the directory
    directory = os.path.abspath(directory)

    # Get a list of files with the extension '.broguerec'
    files = [file for file in os.listdir(directory) if file.endswith('.broguerec')]

    # Run the brogue command for each file
    for file in files:
        file_path = os.path.join(directory, file)
        command = f'./brogue -vn {file_path}'
        print(f"{file}: Running {command}")
        result = subprocess.run(command, shell=True, capture_output=True, text=True)

        # Print the command output
        print(result.stdout)
        print('-' * 50)

def main():
    # Create the argument parser
    parser = argparse.ArgumentParser(description='Brogue Test Runner')
    parser.add_argument('directory', help='Directory containing test files')

    # Parse the command line arguments
    args = parser.parse_args()

    # Call the function to run the brogue tests
    run_brogue_tests(args.directory)

if __name__ == '__main__':
    main()