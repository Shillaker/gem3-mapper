# Notes

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

## Running

### Native

Once you've downloaded the relevant genome file, you can do:

```
./bin/gem-indexer -i data/<your_genome> -o data/<your_genome_index_name>
```

### WASM

TBC

## CLion

- Add a new native toolchain (Settings -> Build, Execution, Deployment -> Toolchains)
- Add a new custom build target (along with a new build tool for `make` under the "build" field)
- Create a new run configuration for this target
- Have it run `bin/gem-indexer` with the relevant input/ output files

# Internals

Indexing eventually calls into `sa_builder_store_suffixes` which spawns a number of threads via
`pthread_create` then calls `pthread_join`.
