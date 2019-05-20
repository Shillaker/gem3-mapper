#!/bin/bash

set -e

URL=ftp://ftp-trace.ncbi.nih.gov/1000genomes/ftp/technical/reference/human_g1k_v37.fasta.gz
FILENAME=human_g1k_v37.fasta

mkdir -p data
pushd data

wget ${URL}
gunzip ${FILENAME}.gz

popd

echo "Genome downloaded to data/${FILENAME}"
