{
  description = "Collection of ad-hoc tools to troubleshoot Syslog TLS issues";

  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs?ref=nixos-unstable";
  };

  outputs = { self, nixpkgs }:
  let
    forAllSystems = nixpkgs.lib.genAttrs [ "x86_64-linux" "x86_64-darwin" "aarch64-linux" "aarch64-darwin" ];

    lastModifiedDate = self.lastModifiedDate or self.lastMOdified or "19700101";

    version = builtins.substring 0 8 lastModifiedDate;

    pkgs = forAllSystems (system: import nixpkgs { inherit system; overlays = [ self.overlay ]; });
  in {

    overlay = final: prev: {
      syslog-tls-debug = with final; stdenv.mkDerivation rec {
        pname = "syslog-tls-debug";
        inherit version;

        src = ./.;

        buildPhase = ''
          for f in ld_preload/*.c; do
            filename=''${f%.*}

            gcc -o ''${filename}.o -shared ''${f}
          done

          gcc -o hello hello.c
        '';

        installPhase = ''
          mkdir -p $out/bin
          cp hello $out/bin/hello

          mkdir -p $out/lib
          cp ld_preload/*.o $out/lib
        '';

        nativeBuildInputs = with pkgs; [
          gnutls
        ];
      };
    };

    packages = forAllSystems (system:
      { inherit (pkgs.${system}) syslog-tls-debug; }
    );

    defaultPackage = forAllSystems (system: self.packages.${system}.syslog-tls-debug);
  };
}
