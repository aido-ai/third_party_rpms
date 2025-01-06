Name: cppunit
Version: 1.12.1
Release: %(echo $RELEASE)%{?dist}
Summary:   Libraries and headers for cppunit development
URL: http://cppunit.sourceforge.net/
%define _prefix /usr/local
Group: aido.ai
License: LGPLv2+
Prefix: %{_prefix}

%description
This package contains the libraries and headers necessary for developing
programs that use cppunit.

%build
cd $OLDPWD/..
mkdir -p BUILD
cd BUILD
../configure --prefix=%{buildroot}/%{_prefix}
make clean
make %{_smp_mflags}

%install
mkdir -p %{buildroot}/%{_prefix}/include
mkdir -p %{buildroot}/%{_prefix}/lib64
cd $OLDPWD/../BUILD && make INSTALL_ROOT=%{buildroot} install
mv %{buildroot}/%{_prefix}/lib/* %{buildroot}/%{_prefix}/lib64

%clean
rm -rf %{buildroot}

%files
%{_prefix}/include
%{_prefix}/lib64
%{_prefix}/bin
%{_prefix}/share
