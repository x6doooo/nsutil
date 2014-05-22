{
    "make_global_settings": [
        ["CXX", "/usr/bin/clang++"],
        ["LINK", "/usr/bin/clang++"],
    ],
    "targets" : [
    {
        "target_name": "nsutil_posix",
        "sources": [
            './src/sutil/sutil_posix.cpp',
            './src/nsutil/nsutil_posix_funcs.cpp',
            './src/nsutil/nsutil_posix.cpp'
        ],
        "cflags_cc": ["-std=c++11"]
    }    
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
        [ "OS=='linux'", {
            "targets": [
                {
                    "target_name": "nsutil_linux",
                    "sources": [
                        "./src/sutil/sutil_linux.cpp",
                        "./src/nsutil/nsutil_linux_funcs.cpp",
                        "./src/nsutil/nsutil_linux.cpp"
                    ],
                    "cflags_cc": ["-std=c++11"]
                }    
            ]
        }]
    ]
}


