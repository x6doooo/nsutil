{
    "make_global_settings": [
        ["CXX", "/usr/bin/clang++"],
        ["LINK", "/usr/bin/clang++"],
    ],
    "conditions": [
        [ "OS=='mac'", {
            "targets": [
                {
                    "target_name": "nsutil_osx",
                    'sources': [
                        './src/sutil/sutil_common.cpp',
                        './src/sutil/osx/process_info.cpp',
                        './src/sutil/sutil_osx.cpp',
                        './src/sutil/sutil_posix.cpp',
                        './src/nsutil/nsutil_osx_funcs.cpp',
                        './src/nsutil/nsutil_osx.cpp'
                    ],
                    "cflags_cc": ["-std=c++11"]
                }
            ],
            'xcode_settings': {
                'OTHER_CPLUSPLUSFLAGS' : ['-std=c++11'],
                'OTHER_LDFLAGS': [
                    '-framework CoreFoundation -framework IOKit'
                ]
            },
        }],
    ]
}


