%define include_busybox 1

Name:     initrd-cominit
Summary:  Initrd for cominit
Version:  0.2
Release:  0
License:  NonFree

BuildRequires:  cominit
BuildRequires:  cpio
BuildRequires:  gzip
BuildRequires:  signing-key-tools
BuildRequires:  storage-key-tools
%if 0%{include_busybox}
BuildRequires:  busybox-static
%endif

%description

%prep
mkdir bin

install -m0755 /sbin/rdinit init

%if 0%{include_busybox}
install -m0755 /bin/busybox bin
install -m0755 /bin/busybox bin/sh
%endif

# install public key for signature verification
mkdir etc
install -m 644 /etc/pki/trust/eb-tools-devel-public.pem etc/rootfs_key_pub.pem
# install keys loaded by cominit into kernel keyring
mkdir -p etc/fake_hsm
install -m 644 /etc/pki/storage/dm-integrity-hmac-secret.bin etc/fake_hsm/
install -m 644 /etc/pki/storage/dm-integrity-jmac-secret.bin etc/fake_hsm/
install -m 644 /etc/pki/storage/dm-integrity-jcrypt-secret.bin etc/fake_hsm/

%build
find . -not -name initrd -print0 | cpio --null --create --verbose --format=newc | gzip --best > initrd

%install
mkdir -p %{buildroot}/boot
install -m0644 initrd %{buildroot}/boot/

%files
/boot/initrd
