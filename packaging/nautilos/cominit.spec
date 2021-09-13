Summary: EB BaseOS Compact Init
Name: cominit
Group: System/Base
Version: 0.5.7
Release: 1
Source0: %{name}.tar.gz
License: Closed
%if "%{_vendor}" == "debbuild"
# Needed to set Maintainer in output debs
Packager: Rainer Müller <rainer.mueller@emlix.com>
%endif
BuildRequires: cmake
BuildRequires: mbedtls-musl-devel
BuildRequires: gcc
BuildRequires: make
BuildRequires: musl-dev
BuildRequires: musl-tools
BuildRequires: linux-libc-dev
BuildRequires: doxygen
BuildRequires: libcmocka-musl-dev

%description
The EB BaseOS Compact Init Daemon for rootfs setup from initramfs.

%package tests-reports
Summary: The unit tests report package
Group: System/Base

%description tests-reports
tests reports and coverage reports for unit tests run on build.

%prep
%setup -n %{name}

%build
mkdir -p %{_builddir}/include
ln -s /usr/include/linux %{_builddir}/include/linux
ln -s /usr/include/asm-generic %{_builddir}/include/asm-generic
ln -s /usr/include/%{_host_cpu}-linux-gnu/asm %{_builddir}/include/asm
export CFLAGS="-isystem /usr/include/%{_host_cpu}-linux-musl -isystem %{_builddir}/include -include /usr/include/%{_host_cpu}-linux-musl/sys/reg.h"
cmake . \
  -DCMAKE_C_COMPILER=/usr/bin/musl-gcc \
  -DCMAKE_SYSTEM_INCLUDE_PATH="/usr/include/%{_host_cpu}-linux-musl" \
  -DCMAKE_SYSTEM_LIBRARY_PATH="/usr/lib/%{_host_cpu}-linux-musl" \
  -Dcmocka_DIR="/usr/lib/%{_host_cpu}-linux-musl/cmake/cmocka" \
  -DUNIT_TESTS=On \
  -DFAKE_HSM=On
make VERBOSE=1
touch cominit-test-report.xml
ctest --output-on-failure --output-junit cominit-test-report.xml

%install
install -d %{buildroot}/sbin
install -m 0755 src/cominit %{buildroot}/sbin/rdinit
# tests reports
mkdir -p %{buildroot}/opt/testing
install -m 0644 cominit-test-report.xml %{buildroot}/opt/testing

%files
%doc README.md
/sbin/rdinit

%files tests-reports
%defattr (-, root, root)
%dir /opt/testing
/opt/testing/*
