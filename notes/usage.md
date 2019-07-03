# Notes

## Usage

### Indexing (Native)

You can download a genome with the `download_genome.py` script:

```
python3 download_genome.py <genome_or_chromosome_name>
```

You can then index it with

```
./bin/gem-indexer -i data/<your_genome> -o data/<your_index>.gem
```

### Mapping

You can download reads to index using the `download_reads.py` script:

```
python3 download_reads.py
```

Picking a file, you can map it with:

```
./bin/gem-mapper -I data/<your_index>.gem -i data/<your_reads> -o data/<your_output>.sam
```

You can change threads with `-t`. Adding `-t 1` can be useful for debugging.

### WASM

TBC


## Data

Lots of animal genomes at [this FTP server](ftp://ftp-trace.ncbi.nih.gov/genomes/).

See the readme for the file layout. Can add more in the `download_genome.py` script.

## Building

### Native

To run locally for testing things out:

```
# Native build
./configure --enable-cuda=no
make

# Debug build
make debug
```

### WASM

To build to WASM for running on Faasm:

```
# WASM build
./wasm_build.sh
```

## CLion

- Add a new native toolchain (Settings -> Build, Execution, Deployment -> Toolchains)
- Add a new custom build target (along with a new build tool for `make` under the "build" field)
- Create a new run configuration for this target
- Have it run `bin/gem-indexer` with the relevant input/ output files

# Internals

Mapping is handled through `mapper.c` which calls `mapper_run`. For each thread it creates a
`mapping_stats_t` and a `mapper_search_t`.

Threads are either a `mapper_pe_thread` or a `mapper_se_thread`, these are just different
types of mapping and also live in `mapper.c`. `mapper_se_thread` is default.

The mapper parameters tell each thread which files it's dealing with, which thread number
it is etc.

Concerns/ challenges:

- Threads share several pointers around parameters and I/O
- Worth trying to find out which ones are needed and which aren't
