import sys
from os.path import join, exists
from os import mkdir, remove
from subprocess import check_output

# Reads are sequences of indexed nucleotides and are the input to the mapper
# Not sure what the best source is so far, but have found some examples

SOURCES = [
    "https://raw.githubusercontent.com/BenLangmead/bowtie2/master/example/reads/reads_1.fq",
]


def main():
    proj_root = sys.path[0]
    data_dir = join(proj_root, "data")

    if not exists(data_dir):
        mkdir(data_dir)

    for url in SOURCES:
        _download_reads(url, data_dir)


def _download_reads(url, download_dir):
    filename = url.split("/")[-1]
    download_file = join(download_dir, filename)
    print("Downloading reads:\nURL: {}\ndest: {}\n".format(url, download_file))
    check_output("wget {} -O {}".format(url, download_file), shell=True)


if __name__ == "__main__":
    main()
