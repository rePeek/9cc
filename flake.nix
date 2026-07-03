{
  inputs.nixpkgs.url = "github:NixOS/nixpkgs/nixpkgs-unstable";

  outputs = { nixpkgs, ... }:
    let
      system = "x86_64-linux";
      pkgs = import nixpkgs { inherit system; };
    in
    {
      devShells.${system}.default = pkgs.mkShell {
        packages = [
          pkgs.gcc
          pkgs.llvmPackages.clang-tools

          pkgs.cmake
          pkgs.ninja
          pkgs.gnumake
          pkgs.pkg-config
          pkgs.git
        ];

        LC_ALL = "C.UTF-8";
        LANG = "C.UTF-8";

        shellHook = ''
          if git rev-parse --is-inside-work-tree >/dev/null 2>&1; then
            git config core.hooksPath .githooks
          fi
        '';
      };
    };
}
