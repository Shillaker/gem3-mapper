import sys
from os.path import join, exists
from os import mkdir, remove
from subprocess import check_output
# Genome data can be found at ftp://ftp-trace.ncbi.nih.gov/genomes
# The top-level README describes the layout (ftp://ftp.ncbi.nih.gov/genomes/README.txt)
# In general you are looking for a file with type `.fasta` or `.fa` or `.fna`

GENOMES = {
    "human": {
        "url": "ftp://ftp-trace.ncbi.nih.gov/1000genomes/ftp/technical/reference/human_g1k_v37.fasta.gz"
    },
    "puffer": {
        "url": "ftp://ftp-trace.ncbi.nih.gov/genomes/genbank/vertebrate_other/Tetraodon_nigroviridis/representative/GCA_000180735.1_ASM18073v1/GCA_000180735.1_ASM18073v1_genomic.fna.gz",
    }
}


def main():
    if len(sys.argv) != 2:
        print("Usage: download_genome.py <species>")
        exit(1)

    species = sys.argv[1]
    if species not in GENOMES.keys():
        print("Species not regonised (options = {})".format(GENOMES.keys()))
        exit(1)

    proj_root = sys.path[0]
    data_dir = join(proj_root, "data")

    if not exists(data_dir):
        mkdir(data_dir)

    genome = GENOMES[species]
    url = genome["url"]
    zip_filename = url.split("/")[-1]

    download_file = join(data_dir, zip_filename)
    print("Downloading {} genome:\nURL: {}\ndest: {}\n".format(species, url, download_file))
    check_output("wget {} -O {}".format(url, download_file), shell=True)

    print("Extracting {}".format(download_file))
    check_output("gunzip -f {}".format(download_file), shell=True, cwd=data_dir)

    filename = zip_filename.replace(".gz", "")
    unzipped_file = join(data_dir, filename)
    if not exists(unzipped_file):
        print("Didn't find unzipped file at {} as expected.".format(unzipped_file))
        exit(1)

    print("Unzipped file at {}".format(unzipped_file))


if __name__ == "__main__":
    main()

