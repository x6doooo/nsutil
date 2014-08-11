{
    "variables": {
        "conditions": [
            ["OS=='mac'", {
                "target_os%": "osx"
            }],
            ["OS=='linux'", {
                "target_os%": "linux"
            }]
        ]
    },
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
                        './src/nsutil/nsutil_osx_funcs_async.cpp',
                        './src/nsutil/nsutil_osx.cpp'
                    ],
                    "cflags_cc": ["-std=c++11"]
                },
                {
                    "target_name": "nsutil_posix_osx",
                    "sources": [
                        './src/sutil/sutil_posix.cpp',
                        './src/nsutil/nsutil_posix_funcs.cpp',
                        './src/nsutil/nsutil_posix.cpp'
                    ],
                    "cflags_cc": ["-std=c++11"]
                }
            ],
            'xcode_settings': {
                'OTHER_CPLUSPLUSFLAGS' : ['-std=c++11'],
                'OTHER_LDFLAGS': [
                    '-framework CoreFoundation -framework IOKit'
                ]
            }
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
                },
                {
                    "target_name": "nsutil_posix_linux",
                    "sources": [
                        './src/sutil/sutil_posix.cpp',
                        './src/nsutil/nsutil_posix_funcs.cpp',
                        './src/nsutil/nsutil_posix.cpp'
                    ],
                    "cflags_cc": ["-std=c++11"]
                }
            ]
        }]
    ],
    "targets": [
        {
            "target_name": "action_after_build",
            "type": "none",
            "dependencies": [ "nsutil_<(target_os)", "nsutil_posix_<(target_os)" ],
            "copies": [
                {
                    "files": [ 
                        "<(module_root_dir)/build/Release/nsutil_<(target_os).node", 
                        "<(module_root_dir)/build/Release/nsutil_posix_<(target_os).node" 
                    ],
                    "destination": "<(module_root_dir)/build/Release/<(target_arch)/"
                }
            ]
        }
    ]
}


