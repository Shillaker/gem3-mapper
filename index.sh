#!/bin/bash

set -e

FILENAME=human_g1k_v37

./bin/gem-indexer -i data/${FILENAME}.fasta -o data/${FILENAME}.index

