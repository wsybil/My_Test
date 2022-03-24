#!/bin/bash

end=$(date +%s%N)
diff=$((($end - $start)/1000000))
times=$(date +%H:%M)
echo "Start time: $time"
echo "End time: $times"
echo "Script working $diff ms"
echo "🔥🔥🔥🔥🔥🔥🔥🔥🔥🔥🔥🔥🔥🔥🔥🔥🔥🔥🔥🔥🔥🔥🔥🔥🔥🔥🔥🔥🔥🔥🔥🔥🔥🔥🔥🔥🔥🔥🔥🔥🔥🔥🔥🔥🔥">> filelog
echo "Start time: $time" >> filelog
echo "End time: $times" >> filelog
echo "Script working $diff ms" >> filelog
