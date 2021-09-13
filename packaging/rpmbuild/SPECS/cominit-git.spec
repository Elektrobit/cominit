Summary: EB BaseOS Compact Init
Name: cominit
Group: System/Base
Version: 0.5.7
Release: 1
Source0: cominit-%{version}.tar.gz
License: Closed
BuildRequires: cmake

%description
The EB BaseOS Compact Init Daemon for rootfs setup from initramfs.

%prep
%setup

%build
cmake . -DUNIT_TESTS=Off
make

%install
install -d %{buildroot}/sbin
install -m 0755 src/cominit %{buildroot}/sbin/rdinit

%files
%doc README.md
/sbin/rdinit

