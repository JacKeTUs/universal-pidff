Name:           universal-pidff
Version:        0.0.10
Release:        1%{?dist}
Summary:        Universal FFB Driver for Moza/Cammus/VRS and more
License:        GPLv2
URL:            https://github.com/JacKeTUs/universal-pidff
Source0:        https://github.com/JacKeTUs/universal-pidff/archive/refs/tags/%{version}.tar.gz

Requires:       dkms

%description
Universal Force Feedback (FFB) Driver for Moza, Cammus, VRS, and other devices.

%global debug_package %{nil}

%prep
%setup -n %{name}-%{version}

# replace placeholders in source files
# find . -type f \( -name 'dkms.conf' -o -name '*.c' \) -exec sed -i "s/#VERSION#/%{version}/g" {} +

%build
# empty build step for dkms

%install
mkdir -p %{buildroot}/usr/src/%{name}-%{version}
cp -r . %{buildroot}/usr/src/%{name}-%{version}

%files
%attr(0755,root,root) /usr/src/%{name}-%{version}/

%post
occurrences=/usr/sbin/dkms status | grep "%{name}" | grep "%{version}" | wc -l
if [ ! occurrences > 0 ];
then
    /usr/sbin/dkms add -m %{name} -v %{version}
fi
/usr/sbin/dkms build -m %{name} -v %{version}
/usr/sbin/dkms install -m %{name} -v %{version}
exit 0

%preun
/usr/sbin/dkms remove -m %{name} -v %{version} --all
exit 0

%changelog
* Tue Dec 17 2024 J.P. Zivalich <j.p.zivalich@gmail.com> - ${version}-1
- Initial RPM release
