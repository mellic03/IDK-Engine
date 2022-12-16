#!/bin/bash
find './src/' \( -name "*.cpp" -o -name "*.h" \) -not -path './src/include/*' | xargs wc -l