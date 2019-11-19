### 0.10.0 (2019-11-19)

##### Continuous Integration

*  add job using debian packages ([85220998](git@gitlab.com:expresscpp/expresscpp/commit/8522099801e51ea5b249332816777a8b1d169d9e))
*  add basic travis ci ([d1e8e18a](git@gitlab.com:expresscpp/expresscpp/commit/d1e8e18a1602dfee5e2412cdbb8a7acd6df6937c))

##### Documentation Changes

*  fix example in readme ([64ad9138](git@gitlab.com:expresscpp/expresscpp/commit/64ad9138e8a596a6604499ce1c8bf99eea0e2035))
*  improve readme and contributing ([f861efdb](git@gitlab.com:expresscpp/expresscpp/commit/f861efdb7d8a4cfbcf11cc1c4df89061e261d8dc))
*  fix link to some examples ([552c5eca](git@gitlab.com:expresscpp/expresscpp/commit/552c5eca0bdd687a5595b57b9dcc3a225dbcec74))

##### New Features

*  add chain routing ([21b49a24](git@gitlab.com:expresscpp/expresscpp/commit/21b49a24ec65587f204fa75dfaf6014936b8a429))

##### Bug Fixes

*  routing of variadic handlers ([69aa9c8f](git@gitlab.com:expresscpp/expresscpp/commit/69aa9c8f934c7d193eaa79b139a59806b4b7d647))

##### Refactors

*  improve usage of chain routing ([8d4a3113](git@gitlab.com:expresscpp/expresscpp/commit/8d4a3113743753f4ed7cd90bebe7a45adc8a0e05))
*  remove more old stuff ([bdf01cab](git@gitlab.com:expresscpp/expresscpp/commit/bdf01cabd36a0fcf0cca124a683cfda6396c73a8))

##### Tests

*  add favicon tests ([dd5983cb](git@gitlab.com:expresscpp/expresscpp/commit/dd5983cbd2fd23b6166dabbf3664133979eed4e0))

### 0.9.0 (2019-11-16)

##### Chores

*  add git graph extention to recomended ones [ci skip] ([75039a14](git@gitlab.com:expresscpp/expresscpp/commit/75039a14fa44f89dd8a54740f32c8924d3690dbb))

##### New Features

*  add favicon middleware ([60b0a001](git@gitlab.com:expresscpp/expresscpp/commit/60b0a00124da02629afee15174e89923ac84a22d))
*  add favicon middleware ([14ab0b11](git@gitlab.com:expresscpp/expresscpp/commit/14ab0b11f99ba0b1a72f66d5a51a4926a53be118))
*  add favicon middleware ([7a7df1aa](git@gitlab.com:expresscpp/expresscpp/commit/7a7df1aab7ee2ec11d42f8531ddcb7d83d8fda2c))

##### Other Changes

* expresscpp/expresscpp ([82b7fe82](git@gitlab.com:expresscpp/expresscpp/commit/82b7fe82e097b77029d6917b7d5c3a92fd7574b3))
* expresscpp/expresscpp ([6328a579](git@gitlab.com:expresscpp/expresscpp/commit/6328a579ee1d0424f61d43cfda1e61da9ad1e93c))

##### Refactors

*   clean up  more old code ([eb0e4dbd](git@gitlab.com:expresscpp/expresscpp/commit/eb0e4dbd32694dcd9f99f28eea04484cbb5524bd))
*  change to use static file provider as normal middleware ([3c2341b6](git@gitlab.com:expresscpp/expresscpp/commit/3c2341b680b3bdf3c011fa3afd050a8136fe29a9))
*  simplify code and cleanup ([0d813427](git@gitlab.com:expresscpp/expresscpp/commit/0d81342773ba90ff459043fbd4e09da24bafc7dc))

### 0.8.0 (2019-11-16)

##### Bug Fixes

*  static file provider ([007a2670](git@gitlab.com:expresscpp/expresscpp/commit/007a26706738b3829dffb0cc9b650fa16108fde0))

### 0.7.0 (2019-11-15)

##### New Features

*  add run and stop method ([2aea6e86](git@gitlab.com:expresscpp/expresscpp/commit/2aea6e86208d838304b8083545206a4db929b2ec))

##### Refactors

*  make type names shorter ([9ee90ffd](git@gitlab.com:expresscpp/expresscpp/commit/9ee90ffd7a281fea79523b84ae24edb7f725a539))
*  remove old code ([ab806521](git@gitlab.com:expresscpp/expresscpp/commit/ab8065210e4755670a44d13cf23ead64b86b7b4a))
*  improve next handler usage ([8e4482c7](git@gitlab.com:expresscpp/expresscpp/commit/8e4482c7eab584433be47d2375346aa68615d28e))

### 0.6.0 (2019-11-15)

##### Build System / Dependencies

*  use conan in cmake when packaging for conan ([349216d8](git@gitlab.com:expresscpp/expresscpp/commit/349216d84f5feaba86de0f31f96ea45db6f0b34e))

##### Documentation Changes

*  add subrouting example ([f52efaf7](git@gitlab.com:expresscpp/expresscpp/commit/f52efaf76f409d12a35302555ad6044131783377))

##### New Features

*  add nested routing ([49e6eab0](git@gitlab.com:expresscpp/expresscpp/commit/49e6eab05b3f847d07641ba317b7aa28494451b4))
*  make changing the log level possible ([fb1ffc65](git@gitlab.com:expresscpp/expresscpp/commit/fb1ffc6508473a881bac55f77eeb4402ae94aa3c))

##### Bug Fixes

*  tests on windows ([cf7e1e40](git@gitlab.com:expresscpp/expresscpp/commit/cf7e1e408d7e5fab322d99e94ea44122023bba15))

##### Refactors

*  remove dead code ([1d418024](git@gitlab.com:expresscpp/expresscpp/commit/1d4180241aff572042db68591558239a73812e08))
*  remove console header from public headers ([fa639f3b](git@gitlab.com:expresscpp/expresscpp/commit/fa639f3bfe7291ae99c0cbab66ffb976815aa26e))

### 0.5.0 (2019-11-14)

##### Build System / Dependencies

*  fix docker builds ([98a5f498](git@gitlab.com:expresscpp/expresscpp/commit/98a5f498160e67afe2ab6a3fa185aa9792430d4a))

##### New Features

*  enable variadic middlewares ([c70d4155](git@gitlab.com:expresscpp/expresscpp/commit/c70d41553c24f76d9078755ac962c37becaf3640))
*  reaneble static file provider ([27e25297](git@gitlab.com:expresscpp/expresscpp/commit/27e25297170542bfd1411dcf1a7b89dedba1db13))

##### Bug Fixes

*  compilation of conan package ([23923ab7](git@gitlab.com:expresscpp/expresscpp/commit/23923ab763a3c607a3df1c7b0bb641dd97ff73bb))

##### Refactors

*  move code to router ([d300bf95](git@gitlab.com:expresscpp/expresscpp/commit/d300bf95808ee1548f4341f93219730a201a9cde))
*  move code to router ([3fd44f21](git@gitlab.com:expresscpp/expresscpp/commit/3fd44f2142702986ff75e52771e81927bdd60f84))
*  clean up left over comments ([c8a4720e](git@gitlab.com:expresscpp/expresscpp/commit/c8a4720e7c7d9090398a1537c8b4bb4ac6e38fc8))
*  remove unneeded methods ([df983067](git@gitlab.com:expresscpp/expresscpp/commit/df9830677d45815c7118eaf3bc1722f647725bff))

### 0.4.0 (2019-11-13)

##### Build System / Dependencies

*  make it compile under windows ([02e63d4d](git@gitlab.com:expresscpp/expresscpp/commit/02e63d4de6bb4f673c096e2880dd12b243088868))
*  make conan optional in cmake ([83bc45da](git@gitlab.com:expresscpp/expresscpp/commit/83bc45da6dd0deb646a47e6d6b2684c01b23a7b8))

##### Bug Fixes

*  use boost 169 and request parser ([51799b67](git@gitlab.com:expresscpp/expresscpp/commit/51799b672be3c98df345396f1a9bf07754361f29))

##### Tests

*  add big payload tests ([810de0dc](git@gitlab.com:expresscpp/expresscpp/commit/810de0dca1b2d224086df300f47275d6219bbc81))

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

