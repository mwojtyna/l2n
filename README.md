# l2n

## QEMU setup

1. Download Debian ISO from https://cdimage.debian.org/debian-cd/current/amd64/iso-cd/
2. Create base disk: `qemu-img create -f qcow2 base.qcow2 10G`.
3. Run the VM and install Debian, during software selection check ONLY `SSH server` and `standard system utilities`

```bash
   qemu-system-x86_64 \
     -enable-kvm \
     -m 2G \
     -smp 2 \
     -drive file=base.qcow2,media=disk,if=virtio \
     -cdrom debian-12.5.0-amd64-netinst.iso \
     -boot d
```

4. After installation turn off the VM and run it normally: `qemu-system-x86_64 -enable-kvm -m 2G -smp 2 -drive file=base.qcow2,media=disk,if=virtio`.
5. Install build tools:

```bash
su -  # Switch to root
apt update && apt install -y build-essential git
shutdown -h now
```

6. Create forked VMs disks:

```bash
qemu-img create -f qcow2 -b base.qcow2 -F qcow2 vm1.qcow2
qemu-img create -f qcow2 -b base.qcow2 -F qcow2 vm2.qcow2
...
```

7. Create virtual L2 switch `sudo ip link add name br0 type bridge`.
8. Write `allow br0` to file `/etc/qemu/bridge.conf`.
9. Run `./qemu-start.sh`, multiple VMs should open.
