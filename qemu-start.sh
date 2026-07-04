#!/bin/bash

qemu-system-x86_64 \
  -enable-kvm -m 2G -smp 2 \
  -drive file=qemu/vm1.qcow2,media=disk,if=virtio \
  -netdev bridge,id=net1,br=br0 \
  -device virtio-net-pci,netdev=net1,mac=00:00:00:00:00:01 &
qemu-system-x86_64 \
  -enable-kvm -m 2G -smp 2 \
  -drive file=qemu/vm2.qcow2,media=disk,if=virtio \
  -netdev bridge,id=net2,br=br0 \
  -device virtio-net-pci,netdev=net2,mac=00:00:00:00:00:02 &

sleep 2s

sudo ip link set tap0 master br0
sudo ip link set tap1 master br0

