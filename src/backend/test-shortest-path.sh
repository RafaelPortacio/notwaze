#!/bin/sh

INPUTS_DIR="$1"
METHOD="$2"

trim() {
    sed 's/ *\(.\+\) */\1/'
}

for input_graph in "$INPUTS_DIR"/*.json
do
    sed -i 's/ //g' "$input_graph"
    while read -r correct_path
    do
        correct_path=$(echo "$correct_path" | trim)
        starting_point="${correct_path%% *}"
        ending_point="${correct_path## *}"
        cpp_shortest_path=$(get-shortest-path "$METHOD" "$input_graph" "$starting_point" "$ending_point" | trim)
        if [ "$correct_path" = "$cpp_shortest_path" ]
        then
            echo "Succeeded"
        else
            echo 'Failed!'
            echo 'Inputs:'
            echo "  method = $METHOD"
            echo "  graph = $input_graph"
            echo "  starting_point = $starting_point"
            echo "  ending_point = $ending_point"
            echo 'Outputs:'
            echo "  correct: $correct_path"
            echo "  ours:    $cpp_shortest_path"
        fi
    done < "${input_graph%.json}.txt"
done
