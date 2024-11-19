# #!/bin/bash
#
# # Find all processes named 'minishell'
# processes=$(pgrep -x minishell)
#
# # while process in $processes
# #
# # 	kill $processes
# # fi
#
#
# # Check if the processes were successfully terminated
# if [ $? -eq 0 ]; then
#     echo "'minishell' processes terminated successfully."
# else
#     echo "Failed to terminate some or all 'minishell' processes. Check permissions."
# fi

#!/bin/bash

# Find all processes named 'minishell'
processes=$(pgrep -x minishell)

if [ -z "$processes" ]; then
    echo "No 'minishell' processes found."
    exit 0
fi

echo "The following 'minishell' processes will be terminated:"
echo "$processes"

# Loop through each process ID
while read -r pid; do
    echo "Terminating process ID: $pid"
    kill "$pid"
    if [ $? -eq 0 ]; then
        echo "Process $pid terminated successfully."
    else
        echo "Failed to terminate process $pid. Check permissions."
    fi
done

echo "All termination attempts completed."
