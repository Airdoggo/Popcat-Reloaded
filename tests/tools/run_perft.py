import argparse
import os
import os.path
import subprocess

os.system('color')

good = 0
fail = 0
errors = []

no_color = '\033[0m'
good_color = '\033[90m'
fail_color = '\033[91m'

def print_test_result(test_name, ref_result, engine_result):
    name = test_name.name.split('/')[-1]

    if ref_result == int(engine_result):
        print(f"{good_color}[ PASSED ]{no_color} ", end="")
        print(f"{name}: {ref_result}")

        global good
        good += 1

    else:
        err = f"{fail_color}[ FAILED ]{no_color} {name}: Got {engine_result} but {ref_result} was expected"
        print(err)
        errors.append(err)

        global fail
        fail += 1


def perft_test_file(perft_file_path, engine_path):
    try:
        engine_perft = int(
            subprocess.run([engine_path, '--perft', perft_file_path],
                           stdout=subprocess.PIPE,
                           encoding="utf-8").stdout)
    except Exception as e:
        raise Exception("Could not read the result of your engine. Make sure it is a valid unsigned integer printed on stdout.")

    with open(perft_file_path) as p:
        perft_line = str.strip(p.readline())
        fen, depth = ' '.join(perft_line.split()[:-1]), perft_line.split()[-1]

        file = f"{perft_file_path[:-6]}.res"

        if os.path.isfile(file):
            with open(file, "r") as f:
                ref_perft = int(f.readline())
                print_test_result(p, ref_perft, engine_perft)


def run_perft_directory(perft_dir_path, engine_path, large):
    for dir in sorted([d for d in os.listdir(perft_dir_path)]):
        if not large and dir == "large":
            continue

        for perft_file in sorted([f for f in os.listdir(perft_dir_path + '/' +
                                                        dir)]):
            if perft_file.split('.')[-1] != "perft":
                continue

            perft_test_file(os.path.join(perft_dir_path, dir + '/'
                                         + perft_file),
                            engine_path)

    print("\nFails recap:")
    for err in errors:
        print(err)
    print()
    print(f"{fail_color}Failed: {fail} / {fail + good}", f"{good_color}Good: {good} /"
                                                f" {fail + good}{no_color}",sep='\n')

    return fail == 0


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Test your engine with perft.")
    parser.add_argument("perft_directory", type=str, help="A directory containing the .perft files you want to test.")
    parser.add_argument("binary", type=str, help="The path to the chess engine binary.")
    parser.add_argument("large", type=str, help="True if you want to run large perfts, False otherwise")
    parser.add_argument("color", default="True", type=str, help="Indicate if the output should be colored or not.")
    args = parser.parse_args()

    if (args.color.lower() == "false"):
        no_color = ""
        good_color = ""
        fail_color = ""

    large = args.large.lower() == "true"
    res = run_perft_directory(args.perft_directory, args.binary, large)
    if not res:
        exit(1)
