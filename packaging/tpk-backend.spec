#Available values for profile: mobile, wearable, tv, ivi, common
%{!?profile:%define profile tv}

Name:           tpk-backend
Summary:        Application installer backend for TPK
Version:        0.1
Release:        1
Group:          Application Framework/Package Management
License:        Apache-2.0
Source0:        %{name}-%{version}.tar.gz
Source1000:     tpk-backend.manifest
Source1001:     tpk-backend-tests.manifest

BuildRequires:  boost-devel
BuildRequires:  cmake
BuildRequires:  gtest-devel
BuildRequires:  pkgconfig(app-installers)
BuildRequires:  pkgconfig(manifest-parser)
BuildRequires:  pkgconfig(pkgmgr)
BuildRequires:  pkgconfig(tpk-manifest-handlers)

%description
Backend for tizen package files

%package devel
Summary:    Tpk-backend development files
Group:      Application Framework/Package Management
Requires:   %{name} = %{version}

%description devel
This package contains header files of tpk-installer library

%package tests
Summary: Unit tests for tpk-backend
Requires: %{name} = %{version}

%description tests
Unit tests for al modules of app-installers

%prep
%setup -q

cp %{SOURCE1000} .
cp %{SOURCE1001} .

%build
%cmake . -DCMAKE_BUILD_TYPE=%{?build_type:%build_type}
make %{?_smp_mflags}

%install
%make_install

mkdir -p %{buildroot}/etc/package-manager/backend
ln -s %{_bindir}/tpk-backend %{buildroot}%{_sysconfdir}/package-manager/backend/tpk
ln -s %{_bindir}/tpk-backend %{buildroot}%{_sysconfdir}/package-manager/backend/rpm

%files
%{_sysconfdir}/package-manager/backend/tpk
%{_sysconfdir}/package-manager/backend/rpm
%license LICENSE
%manifest tpk-backend.manifest
%{_bindir}/tpk-backend
%{_libdir}/libtpk-installer.so*

%files devel
%{_includedir}/app-installers/tpk/*.h
%{_includedir}/app-installers/tpk/*/*.h
%{_libdir}/pkgconfig/tpk-installer.pc

%files tests
%manifest tpk-backend-tests.manifest
%{_bindir}/tpk-backend-ut/*
%{_datadir}/tpk-backend-ut/*

%changelog
* Thu Dec 18 2015 Pawel Sikorski <p.sikorski@samsung.com> 0.1-1
- initial files creation
