#!/bin/bash
set -e
FILE_WX=test/input/multiple/multiple.csv
DIR_OUT=test/output/multiple
scripts/build.sh
rm -rf "${DIR_OUT}"
mkdir -p "${DIR_OUT}"
./firestarr \
  "${DIR_OUT}" \
  2024-06-03 59 -123 01:00 \
  --ffmc 89.9 \
  --dmc 59.5 \
  --dc 450.9 \
  --apcp_prev 0 \
  --no-probability \
  -v \
  -i \
  -s \
  --deterministic \
  --raster-root test/input/stations \
  --wx ${FILE_WX} \
  --output_date_offsets [1] \
  --tz -7 \
  $@
