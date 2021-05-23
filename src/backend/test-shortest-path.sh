#!/bin/sh -e

GET_SHORTEST_PATH=$(find . -name get-shortest-path)

INPUTS_DIR="$1"
METHOD="$2"

endnl () {
    [ -f "$1" ] && [ -s "$1" ] && [ -z "$(tail -c 1 "$1")" ]
}

trim() {
    sed 's/[ \r]*\([0-9]\+\( [0-9]\+\)*\)[ \r]*/\1/'
}

n=$(echo "$INPUTS_DIR"/*.json | wc -w)
counter=0

for input_graph in "$INPUTS_DIR"/*.json
do
    echo "[ $counter / $n ]"

    paths_file="${input_graph%.json}.txt"
    endnl "$paths_file" || { echo >> "$paths_file"; }

    sed -i 's/ //g' "$input_graph"
    while read -r correct_path
    do
        correct_path=$(echo "$correct_path" | trim)
        starting_point="${correct_path%% *}"
        ending_point="${correct_path##* }"
        cpp_shortest_path=$("$GET_SHORTEST_PATH" "$METHOD" "$input_graph" "$starting_point" "$ending_point" | trim)
        if [ "$correct_path" = "$cpp_shortest_path" ]
        then
            echo "Succeeded"
        else
            echo "$(tput bold)Failed!$(tput sgr0)"
            echo "Inputs:"
            echo "  method = $METHOD"
            echo "  graph = $input_graph"
            echo "  starting_point = $starting_point"
            echo "  ending_point = $ending_point"
            echo "Outputs:"
            echo "  correct: $correct_path"
            echo "  ours:    $cpp_shortest_path"
            exit 1
        fi
    done < "$paths_file"

    counter=$((counter+1))
done
