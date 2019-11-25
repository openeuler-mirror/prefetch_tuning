%define kmod_name prefetch_tunning

Name          : prefetch_tunning
Summary       : CPU prefetch tunning module for Kunpeng 920
Version       : 1.0
Release       : 1
License       : GPLv2
Source0       : %{kmod_name}-%{version}.tar.gz
buildroot     : %{_tmppath}/%{kmod_name}-%{version}-build

BuildRequires:  make
BuildRequires:  gcc
BuildRequires:  kernel-devel

%description
The driver of CPU prefetch tunning module for Kunpeng 920.
Users can user this module to control CPU prefetch settings.

%global debug_package %{nil}

%prep
%setup -n   %{kmod_name}-%{version}

%build
%make_build

%install
mkdir -p %{buildroot}/lib/modules/EulerOS/%{kmod_name}
install -m 640 %{kmod_name}.ko   %{buildroot}/lib/modules/EulerOS/%{kmod_name}/%{kmod_name}.ko

%clean
rm -rf %{buildroot}
rm -rf %_builddir/%{kmod_name}-%{version}

%files
%license License/LICENSE
%defattr(-,root,root)
%dir /lib/modules/EulerOS/%{kmod_name}
%attr(0640,root,root)  /lib/modules/EulerOS/%{kmod_name}/%{kmod_name}.ko

%pre

%post
depmod -a `uname -r`

%preun

%changelog
* Mon Nov 4 2019 openEuler Buildteam <buildteam@openeuler.org> - 1.0-1
- Package init
