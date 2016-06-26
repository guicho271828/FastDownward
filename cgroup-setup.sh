#!/bin/bash

name=${1:-$(whoami)}

echo "Checking cgroup cpu,cpuacct,memory:/$name"

if cgexec -g cpu,cpuacct,memory:/$name true
then
    echo "cgroup already exists, exiting."
    exit
else
    echo "Creating cgroup cpu,cpuacct,memory:/$name"
    SUDO_ASKPASS=/usr/bin/ssh-askpass sudo -A cgcreate -a $name:$name -t $name:$name -g cpu,cpuacct,memory:/$name
fi

echo 1G > /sys/fs/cgroup/memory/$name/memory.limit_in_bytes
echo 1G > /sys/fs/cgroup/memory/$name/memory.memsw.limit_in_bytes
echo 1 > /sys/fs/cgroup/memory/$name/memory.use_hierarchy

