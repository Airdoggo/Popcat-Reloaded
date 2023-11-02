import argparse
import os
import os.path
import subprocess
import time

files = [
    "large/big_base_depth-6.perft",
    "large/big_kiwipete_depth-5.perft",
    "large/big_pos3_depth-7.perft",
    "large/big_pos6_depth-5.perft"
]

n_repeat = 10

def run_benchmark(dir_path, engine_path):
    for f in files:
        file_path = os.path.join(dir_path, f)
        total_time = 0
        total_nodes = 0

        for i in range(n_repeat):
            start = time.time()

            try:
                engine_perft = int(subprocess.run([engine_path, '--perft', file_path],
                                   stdout=subprocess.PIPE,
                                   encoding="utf-8").stdout)
            except Exception as e:
                raise Exception("Could not read the result of your engine. Make sure it is a valid unsigned integer printed on stdout.")

            end = time.time()

            total_time += end - start
            total_nodes += engine_perft # should just be the same result x10 but it doesn't really matter anyway
        
        print(f"{file_path}: {(int)(total_nodes / total_time / 1000)} kNodes/s", flush=True)


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Benchmark the engine.")
    parser.add_argument("directory", type=str, help="A directory containing the .perft files you want to benchmark with.")
    parser.add_argument("binary", type=str, help="The path to the chess engine binary.")
    args = parser.parse_args()

    res = run_benchmark(args.directory, args.binary)
