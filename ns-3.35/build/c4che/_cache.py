APPNAME = 'ns'
AR = ['/usr/bin/ar']
ARFLAGS = ['rcs']
BINDIR = '/usr/local/bin'
BUILD_PROFILE = 'debug'
BUILD_SUFFIX = '-debug'
CC = ['/usr/bin/gcc']
CCDEFINES = ['_DEBUG']
CCFLAGS = ['-O0', '-ggdb', '-g3', '-Wall', '-Werror', '-O0', '-ggdb', '-g3', '-Wall', '-Werror', '-std=c++17', '-fstrict-aliasing', '-Wstrict-aliasing']
CCFLAGS_PTHREAD = '-pthread'
CCFLAGS_PYEXT = ['-fvisibility=hidden']
CCLNK_SRC_F = []
CCLNK_TGT_F = ['-o']
CC_NAME = 'gcc'
CC_SRC_F = []
CC_TGT_F = ['-c', '-o']
CC_VERSION = ('9', '3', '0')
CFLAGS_MACBUNDLE = ['-fPIC']
CFLAGS_PYEMBED = ['-g', '-fdebug-prefix-map=/build/python3.8-4OrTnN/python3.8-3.8.10=.', '-fstack-protector', '-g', '-fwrapv', '-O3']
CFLAGS_PYEXT = ['-g', '-fdebug-prefix-map=/build/python3.8-4OrTnN/python3.8-3.8.10=.', '-fstack-protector', '-g', '-fwrapv', '-O3']
CFLAGS_cshlib = ['-fPIC']
COMPILER_CC = 'gcc'
COMPILER_CXX = 'g++'
CPPPATH_ST = '-I%s'
CXX = ['/usr/bin/g++']
CXXDEFINES = ['_DEBUG']
CXXFLAGS = ['-O0', '-ggdb', '-g3', '-Wall', '-Werror', '-std=c++17', '-fstrict-aliasing', '-Wstrict-aliasing']
CXXFLAGS_MACBUNDLE = ['-fPIC']
CXXFLAGS_PTHREAD = '-pthread'
CXXFLAGS_PYEMBED = ['-g', '-fdebug-prefix-map=/build/python3.8-4OrTnN/python3.8-3.8.10=.', '-fstack-protector', '-g', '-fwrapv', '-O3']
CXXFLAGS_PYEXT = ['-g', '-fdebug-prefix-map=/build/python3.8-4OrTnN/python3.8-3.8.10=.', '-fstack-protector', '-g', '-fwrapv', '-O3', '-fvisibility=hidden', '-Wno-array-bounds']
CXXFLAGS_cxxshlib = ['-fPIC']
CXXLNK_SRC_F = []
CXXLNK_TGT_F = ['-o']
CXX_NAME = 'gcc'
CXX_SRC_F = []
CXX_TGT_F = ['-c', '-o']
DATADIR = '/usr/local/share'
DATAROOTDIR = '/usr/local/share'
DEFINES = ['NS3_BUILD_PROFILE_DEBUG', 'NS3_ASSERT_ENABLE', 'NS3_LOG_ENABLE', 'HAVE_SYS_IOCTL_H=1', 'HAVE_IF_NETS_H=1', 'HAVE_NET_ETHERNET_H=1', 'HAVE_PACKET_H=1', 'HAVE_SEMAPHORE_H=1', 'HAVE_IF_TUN_H=1']
DEFINES_PYEMBED = ['NDEBUG']
DEFINES_PYEXT = ['NDEBUG']
DEFINES_ST = '-D%s'
DEFINE_COMMENTS = {'PYTHONDIR': '', 'PYTHONARCHDIR': '', 'HAVE_PYEMBED': '', 'HAVE_PYEXT': '', 'HAVE_PYTHON_H': '', 'HAVE_UINT128_T': '', 'HAVE___UINT128_T': '', 'INT64X64_USE_128': '', 'HAVE_STDINT_H': '', 'HAVE_INTTYPES_H': '', 'HAVE_SYS_INT_TYPES_H': '', 'HAVE_SYS_TYPES_H': '', 'HAVE_SYS_STAT_H': '', 'HAVE_DIRENT_H': '', 'HAVE_SIGNAL_H': '', 'HAVE_PTHREAD_H': '', 'HAVE_RT': '', 'HAVE_SYS_IOCTL_H': '', 'HAVE_IF_NETS_H': '', 'HAVE_NET_ETHERNET_H': '', 'HAVE_IF_TUN_H': '', 'HAVE_PACKET_H': '', 'HAVE_NETMAP_USER_H': '', 'HAVE_SEMAPHORE_H': ''}
DEST_BINFMT = 'elf'
DEST_CPU = 'x86_64'
DEST_OS = 'linux'
DOCDIR = '/usr/local/share/doc/ns'
DVIDIR = '/usr/local/share/doc/ns'
ENABLE_BRITE = False
ENABLE_BUILD_VERSION = False
ENABLE_DPDKNETDEV = False
ENABLE_EMU = True
ENABLE_EXAMPLES = False
ENABLE_FDNETDEV = True
ENABLE_GSL = None
ENABLE_GTK = None
ENABLE_LIBXML2 = None
ENABLE_NETMAP_EMU = None
ENABLE_NSC = False
ENABLE_PYTHON_BINDINGS = True
ENABLE_PYVIZ = False
ENABLE_REAL_TIME = True
ENABLE_STATIC_NS3 = False
ENABLE_SUDO = False
ENABLE_TAP = True
ENABLE_TESTS = False
ENABLE_THREADING = True
EXAMPLE_DIRECTORIES = ['energy', 'matrix-topology', 'udp', 'tutorial', 'naming', 'socket', 'channel-models', 'routing', 'wireless', 'error-model', 'tcp', 'stats', 'traffic-control', 'ipv6', 'realtime', 'udp-client-server']
EXEC_PREFIX = '/usr/local'
GCC_RTTI_ABI_COMPLETE = 'True'
HAVE_DIRENT_H = 1
HAVE_INTTYPES_H = 1
HAVE_LINUX_IF_TUN_H = 1
HAVE_NETPACKET_PACKET_H = 1
HAVE_NET_ETHERNET_H = 1
HAVE_NET_IF_H = 1
HAVE_PTHREAD_H = 1
HAVE_PYEMBED = 1
HAVE_PYEXT = 1
HAVE_PYTHON_H = 1
HAVE_RT = 1
HAVE_SEMAPHORE_H = 1
HAVE_SIGNAL_H = 1
HAVE_STDINT_H = 1
HAVE_SYS_IOCTL_H = 1
HAVE_SYS_STAT_H = 1
HAVE_SYS_TYPES_H = 1
HAVE___UINT128_T = 1
HTMLDIR = '/usr/local/share/doc/ns'
INCLUDEDIR = '/usr/local/include'
INCLUDES_PYEMBED = ['/usr/include/python3.8']
INCLUDES_PYEXT = ['/usr/include/python3.8']
INFODIR = '/usr/local/share/info'
INT64X64_USE_128 = 1
LIBDIR = '/usr/local/lib'
LIBEXECDIR = '/usr/local/libexec'
LIBPATH_PYEMBED = ['/usr/lib/python3.8/config-3.8-x86_64-linux-gnu', '/usr/lib']
LIBPATH_PYEXT = ['/usr/lib/python3.8/config-3.8-x86_64-linux-gnu', '/usr/lib']
LIBPATH_ST = '-L%s'
LIB_BOOST = []
LIB_PYEMBED = ['python3.8', 'crypt', 'pthread', 'dl', 'util', 'm', 'm', 'python3.8', 'crypt', 'pthread', 'dl', 'util', 'm', 'm']
LIB_PYEXT = ['crypt', 'pthread', 'dl', 'util', 'm', 'm', 'crypt', 'pthread', 'dl', 'util', 'm', 'm']
LIB_RT = ['rt']
LIB_ST = '-l%s'
LINKFLAGS_MACBUNDLE = ['-bundle', '-undefined', 'dynamic_lookup']
LINKFLAGS_PTHREAD = '-pthread'
LINKFLAGS_PYEMBED = []
LINKFLAGS_PYEXT = []
LINKFLAGS_cshlib = ['-shared']
LINKFLAGS_cstlib = ['-Wl,-Bstatic']
LINKFLAGS_cxxshlib = ['-shared']
LINKFLAGS_cxxstlib = ['-Wl,-Bstatic']
LINK_CC = ['/usr/bin/gcc']
LINK_CXX = ['/usr/bin/g++']
LOCALEDIR = '/usr/local/share/locale'
LOCALSTATEDIR = '/usr/local/var'
MANDIR = '/usr/local/share/man'
MODULES_NOT_BUILT = ['brite', 'click', 'dpdk-net-device', 'mpi', 'openflow', 'visualizer']
NS3_CONTRIBUTED_MODULES = []
NS3_ENABLED_CONTRIBUTED_MODULES = []
NS3_ENABLED_MODULES = ['ns3-antenna', 'ns3-aodv', 'ns3-applications', 'ns3-bridge', 'ns3-buildings', 'ns3-config-store', 'ns3-core', 'ns3-csma', 'ns3-csma-layout', 'ns3-dsdv', 'ns3-dsr', 'ns3-energy', 'ns3-fd-net-device', 'ns3-flow-monitor', 'ns3-internet', 'ns3-internet-apps', 'ns3-lr-wpan', 'ns3-lte', 'ns3-mesh', 'ns3-mobility', 'ns3-netanim', 'ns3-network', 'ns3-nix-vector-routing', 'ns3-olsr', 'ns3-point-to-point', 'ns3-point-to-point-layout', 'ns3-propagation', 'ns3-sixlowpan', 'ns3-spectrum', 'ns3-stats', 'ns3-tap-bridge', 'ns3-test', 'ns3-topology-read', 'ns3-traffic-control', 'ns3-uan', 'ns3-virtual-net-device', 'ns3-wave', 'ns3-wifi', 'ns3-wimax']
NS3_EXECUTABLE_PATH = ['/home/naeem/CSE322/NS3/ns-allinone-3.35/ns-3.35/build/src/fd-net-device', '/home/naeem/CSE322/NS3/ns-allinone-3.35/ns-3.35/build/src/tap-bridge']
NS3_MODULES = ['ns3-antenna', 'ns3-aodv', 'ns3-applications', 'ns3-bridge', 'ns3-buildings', 'ns3-config-store', 'ns3-core', 'ns3-csma', 'ns3-csma-layout', 'ns3-dsdv', 'ns3-dsr', 'ns3-energy', 'ns3-fd-net-device', 'ns3-flow-monitor', 'ns3-internet', 'ns3-internet-apps', 'ns3-lr-wpan', 'ns3-lte', 'ns3-mesh', 'ns3-mobility', 'ns3-netanim', 'ns3-network', 'ns3-nix-vector-routing', 'ns3-olsr', 'ns3-point-to-point', 'ns3-point-to-point-layout', 'ns3-propagation', 'ns3-sixlowpan', 'ns3-spectrum', 'ns3-stats', 'ns3-tap-bridge', 'ns3-test', 'ns3-topology-read', 'ns3-traffic-control', 'ns3-uan', 'ns3-virtual-net-device', 'ns3-wave', 'ns3-wifi', 'ns3-wimax']
NS3_MODULE_PATH = ['/usr/lib/gcc/x86_64-linux-gnu/9', '/home/naeem/CSE322/NS3/ns-allinone-3.35/ns-3.35/build/lib', '/home/naeem/CSE322/NS3/ns-allinone-3.35/ns-3.35/build']
NS3_OPTIONAL_FEATURES = [('python', 'Python Bindings', True, None), ('castxml', 'Python API Scanning Support', False, "Missing 'pygccxml' Python module"), ('brite', 'BRITE Integration', False, 'BRITE not enabled (see option --with-brite)'), ('nsclick', 'NS-3 Click Integration', False, 'nsclick not enabled (see option --with-nsclick)'), ('GtkConfigStore', 'GtkConfigStore', [], "library 'gtk+-3 >= 3.22' not found"), ('XmlIo', 'XmlIo', [], "library 'libxml-2.0 >= 2.7' not found"), ('Threading', 'Threading Primitives', True, '<pthread.h> include not detected'), ('RealTime', 'Real Time Simulator', True, 'threading not enabled'), ('FdNetDevice', 'File descriptor NetDevice', True, 'FdNetDevice module enabled'), ('DpdkNetDevice', 'DPDK NetDevice', False, 'libdpdk not found, $RTE_SDK and/or $RTE_TARGET environment variable not set or incorrect'), ('TapFdNetDevice', 'Tap FdNetDevice', True, 'Tap support enabled'), ('EmuFdNetDevice', 'Emulation FdNetDevice', True, 'Emulation support enabled'), ('NetmapFdNetDevice', 'Netmap emulation FdNetDevice', False, 'needs net/netmap_user.h'), ('PlanetLabFdNetDevice', 'PlanetLab FdNetDevice', False, 'PlanetLab operating system not detected (see option --force-planetlab)'), ('nsc', 'Network Simulation Cradle', False, 'NSC not found (see option --with-nsc)'), ('mpi', 'MPI Support', False, 'option --enable-mpi not selected'), ('openflow', 'NS-3 OpenFlow Integration', False, 'OpenFlow not enabled (see option --with-openflow)'), ('SQLiteStats', 'SQLite stats support', [], "library 'sqlite3' and/or semaphore.h not found"), ('TapBridge', 'Tap Bridge', True, '<linux/if_tun.h> include not detected'), ('PyViz', 'PyViz visualizer', False, 'Missing python modules: gi.repository.GooCanvas'), ('ENABLE_SUDO', 'Use sudo to set suid bit', False, 'option --enable-sudo not selected'), ('ENABLE_TESTS', 'Tests', False, 'defaults to disabled'), ('ENABLE_EXAMPLES', 'Examples', False, 'defaults to disabled'), ('GSL', 'GNU Scientific Library (GSL)', [], 'GSL not found'), ('libgcrypt', 'Gcrypt library', [], 'libgcrypt not found: you can use libgcrypt-config to find its location.'), ('DES Metrics', 'DES Metrics event collection', [], 'defaults to disabled')]
OLDINCLUDEDIR = '/usr/include'
PACKAGE = 'ns'
PDFDIR = '/usr/local/share/doc/ns'
PKGCONFIG = ['/usr/bin/pkg-config']
PLATFORM = 'linux'
PREFIX = '/usr/local'
PRINT_BUILT_MODULES_AT_END = False
PSDIR = '/usr/local/share/doc/ns'
PYC = 1
PYFLAGS = ''
PYFLAGS_OPT = '-O'
PYO = 1
PYTAG = 'cpython-38'
PYTHON = ['/usr/bin/python3']
PYTHONARCHDIR = '/usr/local/lib/python3/dist-packages'
PYTHONDIR = '/usr/local/lib/python3/dist-packages'
PYTHON_BINDINGS_APIDEFS = 'gcc-LP64'
PYTHON_CONFIG = ['/usr/bin/python3-config']
PYTHON_VERSION = '3.8'
REQUIRED_BOOST_LIBS = []
RPATH_ST = '-Wl,-rpath,%s'
SBINDIR = '/usr/local/sbin'
SEMAPHORE_ENABLED = True
SHAREDSTATEDIR = '/usr/local/com'
SHLIB_MARKER = '-Wl,-Bdynamic'
SONAME_ST = '-Wl,-h,%s'
SQLITE_STATS = None
STLIBPATH_ST = '-L%s'
STLIB_MARKER = '-Wl,-Bstatic'
STLIB_ST = '-l%s'
SUDO = ['/usr/bin/sudo']
SYSCONFDIR = '/usr/local/etc'
VALGRIND_FOUND = False
VERSION = '3.35'
WITH_PYBINDGEN = '/home/naeem/CSE322/NS3/ns-allinone-3.35/pybindgen-0.22.0'
WL_SONAME_SUPPORTED = True
cfg_files = ['/home/naeem/CSE322/NS3/ns-allinone-3.35/ns-3.35/build/ns3/config-store-config.h', '/home/naeem/CSE322/NS3/ns-allinone-3.35/ns-3.35/build/ns3/core-config.h']
cprogram_PATTERN = '%s'
cshlib_PATTERN = 'lib%s.so'
cstlib_PATTERN = 'lib%s.a'
cxxprogram_PATTERN = '%s'
cxxshlib_PATTERN = 'lib%s.so'
cxxstlib_PATTERN = 'lib%s.a'
define_key = ['HAVE_SYS_IOCTL_H', 'HAVE_IF_NETS_H', 'HAVE_NET_ETHERNET_H', 'HAVE_IF_TUN_H', 'HAVE_PACKET_H', 'HAVE_NETMAP_USER_H', 'HAVE_SEMAPHORE_H']
macbundle_PATTERN = '%s.bundle'
pyext_PATTERN = '%s.cpython-38-x86_64-linux-gnu.so'
