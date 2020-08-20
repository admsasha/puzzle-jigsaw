Name:           puzzle-jigsaw
Version:        1.0.1
Release:        1
Summary:        Tiling puzzle that requires the assembly mosaiced pieces
Group:          Games/Puzzles
License:        GPLv3+
Url:            https://bitbucket.org/admsasha/puzzle-jigsaw
Source0:        https://bitbucket.org/admsasha/puzzle-jigsaw/downloads/%{name}-%{version}.tar.gz

BuildRequires:  qt5-tools
BuildRequires:  pkgconfig(Qt5Core)
BuildRequires:  pkgconfig(Qt5Gui)
BuildRequires:  pkgconfig(Qt5Widgets)
BuildRequires:  pkgconfig(Qt5Multimedia)

%description
puzzle-jigsaw  is a tiling puzzle that requires the assembly 
of often oddly shaped interlocking and mosaiced pieces.

%prep
%setup -q

%build
%qmake_qt5
%make

%install
%make_install INSTALL_ROOT=%{buildroot}

%files
%doc README* CONTRIBUTORS
%{_bindir}/%{name}
%{_datadir}/%{name}
%{_datadir}/applications/%{name}.desktop
%{_iconsdir}/hicolor/*/apps/%{name}.png
