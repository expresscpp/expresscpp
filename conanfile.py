
#!/usr/bin/env python

from conans import ConanFile, CMake
import subprocess
import json

class ExpressCpp(ConanFile):
    name = "expresscpp"
    try:
        with open('package.json') as json_file:
            data = json.load(json_file)
            version =  data['version']
            print(version)
    except:
        version = "0.0.0"
    license = "MIT"
    url = "https://gitlab.com/expresscpp/expresscpp"
    description = "same as expressjs for nodejs but for C++"
    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False], "fPIC": [True, False]}
    default_options = {"shared": False, "fPIC": True}
    generators = ["cmake", "cmake_find_package", "cmake_paths"]
    exports_sources = "CMakeLists.txt", "cmake/*", "src/*", "include/*", "conanfile.txt", "package.json"

    def build(self):
        cmake = CMake(self)
        cmake.definitions["EXPRESSCPP_USE_CONAN_DEPENDENCIES"] = True
        cmake.configure(source_folder=".")
        cmake.build()

    def requirements(self):
        self.requires.add("boost/1.72.0")
        self.requires.add("fmt/6.1.2")
        self.requires.add("nlohmann_json/3.7.3")

    def package(self):
        self.copy("*.hpp", src=".")
        self.copy("*expresscpp.lib", dst="lib", keep_path=False)
        self.copy("*.dll", dst="bin", keep_path=False)
        self.copy("*.so", dst="lib", keep_path=False)
        self.copy("*.dylib", dst="lib", keep_path=False)
        self.copy("*.a", dst="lib", keep_path=False)

    def package_info(self):
        self.cpp_info.libs = ["expresscpp"]
