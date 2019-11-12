### 0.3.0 (2019-11-12)

##### Documentation Changes

*   add simple router example ([cbb03ff5](git@gitlab.com:expresscpp/expresscpp/commit/cbb03ff556929c53e5d646dcd667776f7ec1271f))
*  add license badge to readme ([b37912e4](git@gitlab.com:expresscpp/expresscpp/commit/b37912e4a9034daa595871de87122582dcceaf20))

##### New Features

*  create error handler ([f1d3ae14](git@gitlab.com:expresscpp/expresscpp/commit/f1d3ae14aac9668badc218b3d157de2c4af31b76))

##### Refactors

*  move params to request ([0a3a147c](git@gitlab.com:expresscpp/expresscpp/commit/0a3a147cc50b929eb4d913f9f7771baaf48336af))

### 0.2.0 (2019-11-11)

##### Build System / Dependencies

*   enable clang 8 ([0c86e2c4](git@gitlab.com:expresscpp/expresscpp/commit/0c86e2c4282bb37c41a1420e43ae1c2cbd21da70))
*  make it build under windows ([c72d55c9](git@gitlab.com:expresscpp/expresscpp/commit/c72d55c99545817d107f792694684b4b2d24fe32))
*  add conan test_package ([e0aebe1c](git@gitlab.com:expresscpp/expresscpp/commit/e0aebe1c16d93b83b9e79aa3bc66d84fe5f35503))
*  add use instructions and install section to cmake ([d90530a2](git@gitlab.com:expresscpp/expresscpp/commit/d90530a27f092fac67650389981f4a3b0ffc61f9))
*  add automatic version to conan file ([396e6402](git@gitlab.com:expresscpp/expresscpp/commit/396e64026aadfc9a0f9a3a3dd702c62a4f72fedf))

##### Chores

*  remove path from console, only print filename ([70956587](git@gitlab.com:expresscpp/expresscpp/commit/7095658797b23656d59185505834718b9c5d11de))
*   make source location optional ([897c109f](git@gitlab.com:expresscpp/expresscpp/commit/897c109f268f1069b9f79e26999ec6078345198b))
*  add signal handler ([68d8abe5](git@gitlab.com:expresscpp/expresscpp/commit/68d8abe5a7338461856f7b9bc57c82c58b5a9d4a))

##### Continuous Integration

*  enable sanitizers ([e0f7f79c](git@gitlab.com:expresscpp/expresscpp/commit/e0f7f79c4d58fe581456886960ffd55ee61c197d))
*  fix gitlab ci syntax ([4f7894fd](git@gitlab.com:expresscpp/expresscpp/commit/4f7894fdb2069d2f74d00626db28aa07af9c8df0))
*  add ci docker job ([0aa0c458](git@gitlab.com:expresscpp/expresscpp/commit/0aa0c458d2093cbea3de85312fc532d744966fbf))
*  add sanitizers ([9538749d](git@gitlab.com:expresscpp/expresscpp/commit/9538749dccd2b33844e6d96de36fb69c13b5e7ff))
*  export code coverage as html ([70c92ab5](git@gitlab.com:expresscpp/expresscpp/commit/70c92ab5c638e576d54808eca804d3a0180a9cdf))
*  add git to ci image ([fef7934c](git@gitlab.com:expresscpp/expresscpp/commit/fef7934c5a97c41e655ac5e018e10ded526383be))
*  fix type on gitlab file ([9cca4d3b](git@gitlab.com:expresscpp/expresscpp/commit/9cca4d3bef8e5244105b3f96d71733051e54fba5))

##### Documentation Changes

*   add query params example ([e42566c3](git@gitlab.com:expresscpp/expresscpp/commit/e42566c3fee51923fce0029699d97f8031093909))
*  update coverage job ([e383f4a6](git@gitlab.com:expresscpp/expresscpp/commit/e383f4a6246542728988df8843751196c5296746))

##### New Features

*  support query string ([1f116b4d](git@gitlab.com:expresscpp/expresscpp/commit/1f116b4d793a17b008062e75e563fc7712dd9e24))
*  add param handling ([80f4c902](git@gitlab.com:expresscpp/expresscpp/commit/80f4c902f48de92264f427629e6f210e9bd49306))
*   create new fetch method ([3ef5182b](git@gitlab.com:expresscpp/expresscpp/commit/3ef5182b13dcb1e28795ecc41d1ab0c15aa069aa))
*   implement basic path matcher ([df71d9cf](git@gitlab.com:expresscpp/expresscpp/commit/df71d9cf975f4e3c8a855a78860afb4adddcf9b9))

##### Bug Fixes

*  query string handling in fetch ([4308a421](git@gitlab.com:expresscpp/expresscpp/commit/4308a4219cb228a6366897de8839052b6b53c68a))
*  field parsing in request ([af5e70ca](git@gitlab.com:expresscpp/expresscpp/commit/af5e70ca2f9736a2a4c7f279f9c78e30ff01eaa6))

##### Refactors

*  clean up more code ([061db2a8](git@gitlab.com:expresscpp/expresscpp/commit/061db2a85a0f377c5fbf713235b41c3ae1569f85))
*  remove unneeded headers ([b7ab8f20](git@gitlab.com:expresscpp/expresscpp/commit/b7ab8f200ae9a9146fb225216072b2af157680e5))
*   remove getResponse from test utils ([94c043e8](git@gitlab.com:expresscpp/expresscpp/commit/94c043e820bd7e266ec35f78834e06de87deda8a))
*  clean up here and there ([6a81300b](git@gitlab.com:expresscpp/expresscpp/commit/6a81300b0f1020e14b3e1312bbdfc1eb8c8eb46c))
*  change from tree to stack structure ([0436b42a](git@gitlab.com:expresscpp/expresscpp/commit/0436b42a479670fc0556cf4eeb5452e5e2c3aa87))

##### Tests

*  activate auth middleware ([5a9f3b78](git@gitlab.com:expresscpp/expresscpp/commit/5a9f3b78aba4398ae8f0a1d7d60a812e91006a23))
*  add tests for the different http methods ([6a0f07da](git@gitlab.com:expresscpp/expresscpp/commit/6a0f07da0921006fb776f3e41f9b3d137b47513e))
*  enable middleware tests ([f9ef1fbb](git@gitlab.com:expresscpp/expresscpp/commit/f9ef1fbbab498ba4d2c81afb66a4be64eae61a5c))
*  add stack print out test ([e3f76503](git@gitlab.com:expresscpp/expresscpp/commit/e3f76503a19d2c3f9b7468e2a81bbde8367fec79))

### 0.1.0 (2019-10-20)

##### Chores

*  remove qtcreator build stuff ([9a94ee7e](git@gitlab.com:expresscpp/expresscpp/commit/9a94ee7e8b6f3550faadcbae8225b68746f800ac))
*  basic structure ([b84c008b](git@gitlab.com:expresscpp/expresscpp/commit/b84c008b6e2425131413bff5f30fb2802a788fa6))

##### Continuous Integration

*  add change tools ([79559eb9](git@gitlab.com:expresscpp/expresscpp/commit/79559eb9fae0b4d92b89ae5edcbc759b75053817))
*  add first tests ([726e8b2d](git@gitlab.com:expresscpp/expresscpp/commit/726e8b2d98d871cd29e9f19ae087e70b0c4e7d21))
*  add basic linux build job ([3036845d](git@gitlab.com:expresscpp/expresscpp/commit/3036845d8555801031f169b8b7a3d226b4c7cb8a))

##### Documentation Changes

*  add expressjs pendents to expresscpp examples ([e5ab0465](git@gitlab.com:expresscpp/expresscpp/commit/e5ab0465663f788d615da05ad872197efa87a93c))
*  add license ([82afc0b3](git@gitlab.com:expresscpp/expresscpp/commit/82afc0b35a1a35c48e1aa212daea97cf57cad3ed))
*  add logo to readme ([6a29c910](git@gitlab.com:expresscpp/expresscpp/commit/6a29c91091c1e36612564caa81a3c73eec88a064))
*  add link to examples to readme [ci skip] ([528a3734](git@gitlab.com:expresscpp/expresscpp/commit/528a37345cb35342a7155dafdb3d055f8e477a3d))
*  add example for basic static file server [ci skip] ([71259b09](git@gitlab.com:expresscpp/expresscpp/commit/71259b09112c22a2a232129cc12357ee24c071b9))
*  add build and coverage badge ([3b7c9d30](git@gitlab.com:expresscpp/expresscpp/commit/3b7c9d309a6409a73b5c6bfc5157495f580992ca))
*  add design decisions ([5c3ad8c5](git@gitlab.com:expresscpp/expresscpp/commit/5c3ad8c593f503d4b52c7a713139f1403251e3ec))

##### New Features

*   package library as conan library ([e6bca6a6](git@gitlab.com:expresscpp/expresscpp/commit/e6bca6a6c24dbb4d4620c993b40bb33a602dad20))
*  add stack print out for routing ([af3c79f0](git@gitlab.com:expresscpp/expresscpp/commit/af3c79f0f20d8d95a81db6d7318bbb617ee713ec))
*  create initial static file middleware ([3969a028](git@gitlab.com:expresscpp/expresscpp/commit/3969a028381d251bc5c7bd334736431d0358351e))
*  implement basic http methods ([f912a382](git@gitlab.com:expresscpp/expresscpp/commit/f912a382fd846001b10858c940141da20c56cbff))
*  implement basic router - handler map ([271ab022](git@gitlab.com:expresscpp/expresscpp/commit/271ab02203de9862cace7fd0390189b2093b9136))
*  add path and method to main handler ([75e60fe5](git@gitlab.com:expresscpp/expresscpp/commit/75e60fe5fa59beffb473baaa7133a9e8532b6ec3))
*  add first working webserver ([92899954](git@gitlab.com:expresscpp/expresscpp/commit/928999543efd69d9751e3df21d0bd12c7917b664))

