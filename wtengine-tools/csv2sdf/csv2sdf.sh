#!/bin/bash
TOOLS_LOCATION="$(<$HOME/.wtengine/tools_location)"
python "$TOOLS_LOCATION"/csv2sdf/csv2sdf.py $1